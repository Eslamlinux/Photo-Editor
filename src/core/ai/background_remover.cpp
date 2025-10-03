#include "background_remover.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>

#ifndef DISABLE_AI_FEATURES
// تضمين مكتبة ONNX Runtime فقط إذا كانت ميزات الذكاء الاصطناعي مفعلة
#include <onnxruntime_cxx_api.h>
#endif

namespace pme {
namespace core {
namespace ai {

#ifndef DISABLE_AI_FEATURES
BackgroundRemover::BackgroundRemover(const std::string& modelPath) : m_initialized(false) {
    try {
        // تهيئة بيئة ONNX Runtime
        m_env = std::make_unique<Ort::Env>(ORT_LOGGING_LEVEL_WARNING, "BackgroundRemover");
        
        // إعداد خيارات الجلسة
        Ort::SessionOptions sessionOptions;
        sessionOptions.SetIntraOpNumThreads(1);
        sessionOptions.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);
        
        // تحميل النموذج
        m_session = std::make_unique<Ort::Session>(*m_env, modelPath.c_str(), sessionOptions);
        
        // إعداد معلومات الذاكرة
        m_memoryInfo = std::make_unique<Ort::MemoryInfo>(Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault));
        
        // إعداد المخصص
        m_allocator = std::make_unique<Ort::AllocatorWithDefaultOptions>();
        
        // الحصول على أسماء المدخلات والمخرجات
        m_inputName = m_session->GetInputName(0, *m_allocator);
 
        // الحصول على أبعاد المدخلات والمخرجات
        Ort::TypeInfo inputTypeInfo = m_session->GetInputTypeInfo(0);
        auto inputTensorInfo = inputTypeInfo.GetTensorTypeAndShapeInfo();
        m_inputDims = inputTensorInfo.GetShape();
        
        Ort::TypeInfo outputTypeInfo = m_session->GetOutputTypeInfo(0);
        auto outputTensorInfo = outputTypeInfo.GetTensorTypeAndShapeInfo();
        m_outputDims = outputTensorInfo.GetShape();
        
        m_initialized = true;
    }
    catch (const Ort::Exception& e) {
        std::cerr << "ONNX Runtime error: " << e.what() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error initializing BackgroundRemover: " << e.what() << std::endl;
    }
}

BackgroundRemover::~BackgroundRemover() {
    // المخصصات ستتم تنظيفها تلقائيًا بواسطة unique_ptr
}


cv::Mat BackgroundRemover::removeBg(const cv::Mat& inputImage) {
    if (!m_initialized) {
        std::cerr << "BackgroundRemover not initialized properly" << std::endl;
        return inputImage.clone();
    }
    
    try {
        // معالجة مسبقة للصورة
        cv::Mat processedInput;
        preprocess(inputImage, processedInput);
        
        // تحويل البيانات إلى تنسور
        size_t inputTensorSize = processedInput.total() * processedInput.elemSize();
        std::vector<float> inputTensorValues(inputTensorSize / sizeof(float));
        std::memcpy(inputTensorValues.data(), processedInput.data, inputTensorSize);
        
        // إنشاء تنسور المدخلات
        auto inputTensor = Ort::Value::CreateTensor<float>(
            *m_memoryInfo,
            inputTensorValues.data(),
            inputTensorValues.size(),
            m_inputDims.data(),
            m_inputDims.size()
        );
        
        // تشغيل النموذج


        const char* inputNames[] = {m_inputName.c_str()};
        const char* outputNames[] = {m_outputName.c_str()};
        
        auto outputTensors = m_session->Run(
            Ort::RunOptions{nullptr},
            inputNames,
            &inputTensor,
            1,
            outputNames,
            1
        );
        
        // معالجة لاحقة للنتيجة
        cv::Mat mask;
        postprocess(outputTensors[0], mask, inputImage.size());
        
        // تطبيق القناع على الصورة
        cv::Mat result;
        applyMask(inputImage, mask, result);
        
        return result;
    }
    catch (const Ort::Exception& e) {
        std::cerr << "ONNX Runtime error: " << e.what() << std::endl;
        return inputImage.clone();
    }
    catch (const std::exception& e) {
        std::cerr << "Error in removeBg: " << e.what() << std::endl;
        return inputImage.clone();
    }
}


void BackgroundRemover::preprocess(const cv::Mat& input, cv::Mat& output) {
    // تغيير حجم الصورة إلى الأبعاد المطلوبة للنموذج
    int targetHeight = m_inputDims[2];
    int targetWidth = m_inputDims[3];
    
    // تحويل الصورة إلى RGB إذا كانت بتنسيق آخر
    cv::Mat rgbImage;
    if (input.channels() == 4) {
        cv::cvtColor(input, rgbImage, cv::COLOR_BGRA2RGB);
    } else if (input.channels() == 3) {
        cv::cvtColor(input, rgbImage, cv::COLOR_BGR2RGB);
    } else if (input.channels() == 1) {
        cv::cvtColor(input, rgbImage, cv::COLOR_GRAY2RGB);
    } else {
        rgbImage = input.clone();
    }
    
    // تغيير حجم الصورة
    cv::Mat resizedImage;
    cv::resize(rgbImage, resizedImage, cv::Size(targetWidth, targetHeight));
    
    // تطبيع البيانات
    resizedImage.convertTo(resizedImage, CV_32F, 1.0 / 255.0);
    
    // تحويل الصورة إلى تنسور NCHW (Batch, Channels, Height, Width)
    // تقسيم القنوات
    std::vector<cv::Mat> channels(3);
    cv::split(resizedImage, channels);
    
    // إنشاء تنسور المخرجات
    output = cv::Mat(1, 3 * targetHeight * targetWidth, CV_32F);
    
    // نسخ البيانات إلى التنسور
    float* outputData = (float*)output.data;
    
    // القناة الأولى (R)
    for (int h = 0; h < targetHeight; h++) {
        for (int w = 0; w < targetWidth; w++) {
            outputData[0 * targetHeight * targetWidth + h * targetWidth + w] = channels[0].at<float>(h, w);
        }
    }
    
    // القناة الثانية (G)
    for (int h = 0; h < targetHeight; h++) {
        for (int w = 0; w < targetWidth; w++) {
            outputData[1 * targetHeight * targetWidth + h * targetWidth + w] = channels[1].at<float>(h, w);
        }
    }
    
    // القناة الثالثة (B)
    for (int h = 0; h < targetHeight; h++) {
        for (int w = 0; w < targetWidth; w++) {
            outputData[2 * targetHeight * targetWidth + h * targetWidth + w] = channels[2].at<float>(h, w);
        }
    }

