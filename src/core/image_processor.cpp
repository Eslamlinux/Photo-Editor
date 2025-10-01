#include "core/image_processor.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/photo.hpp>
#include <iostream>
#include <cmath>

namespace pme {
namespace core {

ImageProcessor::ImageProcessor() : m_updateCallback(nullptr)
{
}

ImageProcessor::~ImageProcessor()
{
}

void ImageProcessor::setUpdateCallback(UpdateCallback callback)
{
    m_updateCallback = callback;
}

bool ImageProcessor::loadImage(const std::string& filePath)
{
    // محاولة تحميل الصورة
    cv::Mat image = cv::imread(filePath, cv::IMREAD_UNCHANGED);
    
    // التحقق من نجاح التحميل
    if (image.empty()) {
        return false;
    }
    
    // تعيين الصورة الحالية
    m_image = image;
    m_originalImage = image.clone();
    
    // مسح سجلات التراجع/الإعادة
    m_undoStack.clear();
    m_redoStack.clear();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}
