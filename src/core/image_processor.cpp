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

bool ImageProcessor::saveImage(const std::string& filePath)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // محاولة حفظ الصورة
    return cv::imwrite(filePath, m_image);
}


bool ImageProcessor::undo()
{
    // التحقق من إمكانية التراجع
    if (!canUndo()) {
        return false;
    }
    
    // حفظ الحالة الحالية للإعادة
    m_redoStack.push_back(m_image.clone());
    
    // استعادة الحالة السابقة
    m_image = m_undoStack.back();
    m_undoStack.pop_back();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::redo()
{
    // التحقق من إمكانية الإعادة
    if (!canRedo()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    m_undoStack.push_back(m_image.clone());
    
    // استعادة الحالة التالية
    m_image = m_redoStack.back();
    m_redoStack.pop_back();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::canUndo() const
{
    return !m_undoStack.empty();
}

bool ImageProcessor::canRedo() const
{
    return !m_redoStack.empty();
}

void ImageProcessor::saveState()
{
    // حفظ الحالة الحالية للتراجع
    m_undoStack.push_back(m_image.clone());
    
    // تحديد حجم سجل التراجع (للحد من استخدام الذاكرة)
    if (m_undoStack.size() > 20) {
        m_undoStack.erase(m_undoStack.begin());
    }
}

void ImageProcessor::clearRedoStack()
{
    m_redoStack.clear();
}

void ImageProcessor::notifyUpdate()
{
    if (m_updateCallback) {
        m_updateCallback();
    }
}

bool ImageProcessor::hasImage() const
{
    return !m_image.empty();
}

cv::Mat ImageProcessor::getImage() const
{
    return m_image;
}


int ImageProcessor::getWidth() const
{
    return hasImage() ? m_image.cols : 0;
}

int ImageProcessor::getHeight() const
{
    return hasImage() ? m_image.rows : 0;
}

int ImageProcessor::getChannels() const
{
    return hasImage() ? m_image.channels() : 0;
}


bool ImageProcessor::crop(int x, int y, int width, int height)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من صحة المنطقة
    if (x < 0 || y < 0 || width <= 0 || height <= 0 ||
        x + width > getWidth() || y + height > getHeight()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // قص الصورة
    cv::Rect roi(x, y, width, height);
    m_image = m_image(roi).clone();
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::rotate90CW()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تدوير الصورة 90 درجة في اتجاه عقارب الساعة
    cv::rotate(m_image, m_image, cv::ROTATE_90_CLOCKWISE);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::rotate90CCW()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تدوير الصورة 90 درجة عكس اتجاه عقارب الساعة
    cv::rotate(m_image, m_image, cv::ROTATE_90_COUNTERCLOCKWISE);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::rotate180()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تدوير الصورة 180 درجة
    cv::rotate(m_image, m_image, cv::ROTATE_180);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::flipHorizontal()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // قلب الصورة أفقيًا
    cv::flip(m_image, m_image, 1);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::flipHorizontal()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // قلب الصورة أفقيًا
    cv::flip(m_image, m_image, 1);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::flipVertical()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // قلب الصورة رأسيًا
    cv::flip(m_image, m_image, 0);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::resize(int width, int height)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من صحة الأبعاد
    if (width <= 0 || height <= 0) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تغيير حجم الصورة
    cv::resize(m_image, m_image, cv::Size(width, height), 0, 0, cv::INTER_LANCZOS4);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::grayscale()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من أن الصورة ليست بالفعل رمادية
    if (m_image.channels() == 1) {
        return true;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تحويل الصورة إلى تدرج الرمادي
    cv::cvtColor(m_image, m_image, cv::COLOR_BGR2GRAY);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}

bool ImageProcessor::sepia()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تحويل الصورة إلى BGR إذا كانت بتنسيق آخر
    cv::Mat bgrImage;
    if (m_image.channels() == 1) {
        cv::cvtColor(m_image, bgrImage, cv::COLOR_GRAY2BGR);
    } else if (m_image.channels() == 4) {
        cv::cvtColor(m_image, bgrImage, cv::COLOR_BGRA2BGR);
    } else {
        bgrImage = m_image.clone();
    }
    
    // تطبيق مصفوفة التحويل للحصول على تأثير السيبيا
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
        0.272, 0.534, 0.131,
        0.349, 0.686, 0.168,
        0.393, 0.769, 0.189);
    
    cv::transform(bgrImage, m_image, kernel);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::negative()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تأثير السلبي
    cv::bitwise_not(m_image, m_image);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}

bool ImageProcessor::blur(int radius)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من صحة نصف القطر
    if (radius <= 0) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تأثير التمويه
    cv::GaussianBlur(m_image, m_image, cv::Size(2 * radius + 1, 2 * radius + 1), 0);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}

bool ImageProcessor::sharpen()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تأثير التحديد
    cv::Mat blurred;
    cv::GaussianBlur(m_image, blurred, cv::Size(0, 0), 3);
    cv::addWeighted(m_image, 1.5, blurred, -0.5, 0, m_image);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::edgeDetection()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تحويل الصورة إلى تدرج الرمادي إذا لزم الأمر
    cv::Mat grayImage;
    if (m_image.channels() == 1) {
        grayImage = m_image.clone();
    } else {
        cv::cvtColor(m_image, grayImage, cv::COLOR_BGR2GRAY);
    }
    
    // تطبيق كشف الحواف
    cv::Mat edges;
    cv::Canny(grayImage, edges, 50, 150);
    
    // تحويل الصورة إلى BGR إذا كانت الصورة الأصلية ملونة
    if (m_image.channels() == 3) {
        cv::cvtColor(edges, m_image, cv::COLOR_GRAY2BGR);
    } else {
        m_image = edges;
    }
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::emboss()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تأثير النقش
    cv::Mat kernel = (cv::Mat_<float>(3, 3) <<
        -2, -1, 0,
        -1, 1, 1,
        0, 1, 2);
    
    cv::Mat result;
    cv::filter2D(m_image, result, -1, kernel, cv::Point(-1, -1), 128);
    m_image = result;
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}

