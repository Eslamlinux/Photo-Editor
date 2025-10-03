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

