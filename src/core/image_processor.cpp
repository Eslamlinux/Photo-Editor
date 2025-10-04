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


bool ImageProcessor::cartoon()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من أن الصورة ملونة
    if (m_image.channels() < 3) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تأثير الكرتون
    cv::Mat gray, edges, color, cartoon;
    
    // تحويل الصورة إلى تدرج الرمادي
    cv::cvtColor(m_image, gray, cv::COLOR_BGR2GRAY);
    
    // تطبيق مرشح ثنائي التباين
    cv::medianBlur(gray, gray, 5);
    
    // كشف الحواف
    cv::adaptiveThreshold(gray, edges, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 9, 9);
    
    // تطبيق مرشح ثنائي التباين على الصورة الملونة
    cv::bilateralFilter(m_image, color, 9, 300, 300);
    
    // دمج الحواف مع الصورة الملونة
    cv::cvtColor(edges, edges, cv::COLOR_GRAY2BGR);
    cv::bitwise_and(color, edges, m_image);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::watercolor()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تأثير الألوان المائية
    cv::Mat result;
    
    // تطبيق مرشح ثنائي التباين
    cv::bilateralFilter(m_image, result, 9, 150, 150);
    
    // تطبيق مرشح التمويه
    cv::GaussianBlur(result, result, cv::Size(5, 5), 0);
    
    // تطبيق مرشح حدة
    cv::Mat sharpened;
    cv::GaussianBlur(result, sharpened, cv::Size(0, 0), 3);
    cv::addWeighted(result, 1.5, sharpened, -0.5, 0, result);
    
    m_image = result;
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}

bool ImageProcessor::mosaic(int blockSize)
{
   // التحقق من وجود صورة
   if (!hasImage()) {
       return false;
   }
   
   // التحقق من صحة حجم الكتلة
   if (blockSize <= 0) {
       return false;
   }
   
   // حفظ الحالة الحالية للتراجع
   saveState();
   
   // تطبيق تأثير الفسيفساء
   int width = m_image.cols;
   int height = m_image.rows;
   
   for (int y = 0; y < height; y += blockSize) {
       for (int x = 0; x < width; x += blockSize) {
           // تحديد منطقة الكتلة
           cv::Rect rect(x, y, std::min(blockSize, width - x), std::min(blockSize, height - y));
           
           // حساب متوسط لون الكتلة
           cv::Scalar color = cv::mean(m_image(rect));
           
           // تعبئة الكتلة باللون المتوسط
           cv::rectangle(m_image, rect, color, -1);
       }
   }
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}
bool ImageProcessor::mosaic(int blockSize)
{
   // التحقق من وجود صورة
   if (!hasImage()) {
       return false;
   }
   
   // التحقق من صحة حجم الكتلة
   if (blockSize <= 0) {
       return false;
   }
   
   // حفظ الحالة الحالية للتراجع
   saveState();
   
   // تطبيق تأثير الفسيفساء
   int width = m_image.cols;
   int height = m_image.rows;
   
   for (int y = 0; y < height; y += blockSize) {
       for (int x = 0; x < width; x += blockSize) {
           // تحديد منطقة الكتلة
           cv::Rect rect(x, y, std::min(blockSize, width - x), std::min(blockSize, height - y));
           
           // حساب متوسط لون الكتلة
           cv::Scalar color = cv::mean(m_image(rect));
           
           // تعبئة الكتلة باللون المتوسط
           cv::rectangle(m_image, rect, color, -1);
       }
   }
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}
bool ImageProcessor::mosaic(int blockSize)
{
   // التحقق من وجود صورة
   if (!hasImage()) {
       return false;
   }
   
   // التحقق من صحة حجم الكتلة
   if (blockSize <= 0) {
       return false;
   }
   
   // حفظ الحالة الحالية للتراجع
   saveState();
   
   // تطبيق تأثير الفسيفساء
   int width = m_image.cols;
   int height = m_image.rows;
   
   for (int y = 0; y < height; y += blockSize) {
       for (int x = 0; x < width; x += blockSize) {
           // تحديد منطقة الكتلة
           cv::Rect rect(x, y, std::min(blockSize, width - x), std::min(blockSize, height - y));
           
           // حساب متوسط لون الكتلة
           cv::Scalar color = cv::mean(m_image(rect));
           
           // تعبئة الكتلة باللون المتوسط
           cv::rectangle(m_image, rect, color, -1);
       }
   }
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}



bool ImageProcessor::oilPainting(int size, int dynRatio)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من صحة المعاملات
    if (size <= 0 || dynRatio <= 0) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تأثير الرسم الزيتي
    cv::Mat result = m_image.clone();
    
    // تحويل الصورة إلى تدرج الرمادي للحصول على قيم الكثافة
    cv::Mat gray;
    if (m_image.channels() == 1) {
        gray = m_image.clone();
    } else {
        cv::cvtColor(m_image, gray, cv::COLOR_BGR2GRAY);
    }
    
    // تطبيق الخوارزمية
    int width = m_image.cols;
    int height = m_image.rows;
    int channels = m_image.channels();
    
    for (int y = size; y < height - size; y++) {
        for (int x = size; x < width - size; x++) {
            // مصفوفة لتخزين تكرار كل كثافة
            std::vector<int> intensityCount(256, 0);
            std::vector<cv::Vec3f> intensitySum(256, cv::Vec3f(0, 0, 0));
            
            // حساب تكرار كل كثافة في المنطقة المحيطة
            for (int ky = -size; ky <= size; ky++) {
                for (int kx = -size; kx <= size; kx++) {
                    int intensity = gray.at<uchar>(y + ky, x + kx);
                    intensityCount[intensity]++;
                    
                    if (channels == 1) {
                        intensitySum[intensity][0] += m_image.at<uchar>(y + ky, x + kx);
                    } else if (channels == 3) {
                        cv::Vec3b pixel = m_image.at<cv::Vec3b>(y + ky, x + kx);
                        intensitySum[intensity][0] += pixel[0];
                        intensitySum[intensity][1] += pixel[1];
                        intensitySum[intensity][2] += pixel[2];
                    } else if (channels == 4) {
                        cv::Vec4b pixel = m_image.at<cv::Vec4b>(y + ky, x + kx);
                        intensitySum[intensity][0] += pixel[0];
                        intensitySum[intensity][1] += pixel[1];
                        intensitySum[intensity][2] += pixel[2];
                    }
                }
            }
            
            // البحث عن الكثافة الأكثر تكرارًا
            int maxCount = 0;
            int maxIntensity = 0;
            for (int i = 0; i < 256; i++) {
                if (intensityCount[i] > maxCount) {
                    maxCount = intensityCount[i];
                    maxIntensity = i;
                }
            }
            
            // تعيين اللون الجديد
            if (channels == 1) {
                result.at<uchar>(y, x) = intensitySum[maxIntensity][0] / maxCount;
            } else if (channels == 3) {
                cv::Vec3b& pixel = result.at<cv::Vec3b>(y, x);
                pixel[0] = intensitySum[maxIntensity][0] / maxCount;
                pixel[1] = intensitySum[maxIntensity][1] / maxCount;
                pixel[2] = intensitySum[maxIntensity][2] / maxCount;
            } else if (channels == 4) {
                cv::Vec4b& pixel = result.at<cv::Vec4b>(y, x);
                pixel[0] = intensitySum[maxIntensity][0] / maxCount;
                pixel[1] = intensitySum[maxIntensity][1] / maxCount;
                pixel[2] = intensitySum[maxIntensity][2] / maxCount;
                // الحفاظ على قناة ألفا
                pixel[3] = m_image.at<cv::Vec4b>(y, x)[3];
            }
        }
    }
    
    m_image = result;
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::pencilSketch(bool colorOutput)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تأثير الرسم بالقلم الرصاص
    cv::Mat gray, inverted, blurred, sketch;
    
    // تحويل الصورة إلى تدرج الرمادي
    if (m_image.channels() == 1) {
        gray = m_image.clone();
    } else {
        cv::cvtColor(m_image, gray, cv::COLOR_BGR2GRAY);
    }
    
    // عكس الصورة
    cv::bitwise_not(gray, inverted);
    
    // تطبيق مرشح التمويه
    cv::GaussianBlur(inverted, blurred, cv::Size(21, 21), 0);
    
    // دمج الصورة الأصلية مع الصورة المموهة
    cv::divide(gray, 255 - blurred, sketch, 256);
    
    if (colorOutput && m_image.channels() >= 3) {
        // إنشاء نسخة ملونة من الرسم
        cv::Mat colorSketch;
        cv::cvtColor(sketch, colorSketch, cv::COLOR_GRAY2BGR);
        
        // استخراج القنوات الملونة من الصورة الأصلية
        std::vector<cv::Mat> channels;
        cv::split(m_image, channels);
        
        // تطبيق تأثير الرسم على كل قناة
        for (int i = 0; i < 3; i++) {
            channels[i] = channels[i].mul(colorSketch, 1.0 / 255.0);
        }
        
        // دمج القنوات
        cv::merge(channels, m_image);
    } else {
        // استخدام الرسم بالأبيض والأسود
        if (m_image.channels() == 3 || m_image.channels() == 4) {
            cv::cvtColor(sketch, m_image, cv::COLOR_GRAY2BGR);
            if (m_image.channels() == 4) {
                // الحفاظ على قناة ألفا
                cv::Mat alpha;
                cv::extractChannel(m_image, alpha, 3);
                cv::insertChannel(alpha, m_image, 3);
            }
        } else {
            m_image = sketch;
        }
    }
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}




bool ImageProcessor::adjustBrightness(int value)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تعديل السطوع
    m_image.convertTo(m_image, -1, 1, value);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}



bool ImageProcessor::adjustContrast(int value)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تحويل قيمة التباين إلى عامل
    double factor = (100.0 + value) / 100.0;
    
    // تطبيق تعديل التباين
    m_image.convertTo(m_image, -1, factor, 0);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}



bool ImageProcessor::adjustSaturation(int value)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من أن الصورة ملونة
    if (m_image.channels() < 3) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تحويل قيمة التشبع إلى عامل
    double factor = (100.0 + value) / 100.0;
    
    // تحويل الصورة إلى فضاء HSV
    cv::Mat hsv;
    cv::cvtColor(m_image, hsv, cv::COLOR_BGR2HSV);
    
    // تقسيم القنوات
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    
    // تعديل قناة التشبع
    channels[1] = channels[1] * factor;
    
    // دمج القنوات
    cv::merge(channels, hsv);
    
    // تحويل الصورة إلى فضاء BGR
    cv::cvtColor(hsv, m_image, cv::COLOR_HSV2BGR);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}



bool ImageProcessor::adjustHue(int value)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من أن الصورة ملونة
    if (m_image.channels() < 3) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تحويل الصورة إلى فضاء HSV
    cv::Mat hsv;
    cv::cvtColor(m_image, hsv, cv::COLOR_BGR2HSV);
    
    // تقسيم القنوات
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    
    // تعديل قناة تدرج اللون
    channels[0].convertTo(channels[0], -1, 1, value);
    
    // دمج القنوات
    cv::merge(channels, hsv);
    
    // تحويل الصورة إلى فضاء BGR
    cv::cvtColor(hsv, m_image, cv::COLOR_HSV2BGR);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}




bool ImageProcessor::adjustGamma(double value)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من صحة قيمة جاما
    if (value <= 0) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // إنشاء جدول البحث
    cv::Mat lookUpTable(1, 256, CV_8U);
    uchar* p = lookUpTable.ptr();
    
    // ملء جدول البحث
    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, 1.0 / value) * 255.0);
    }
    
    // تطبيق تعديل جاما
    cv::LUT(m_image, lookUpTable, m_image);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}

bool ImageProcessor::adjustTemperature(int value)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من أن الصورة ملونة
    if (m_image.channels() < 3) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تقسيم القنوات
    std::vector<cv::Mat> channels;
    cv::split(m_image, channels);
    
    // تعديل درجة حرارة اللون
    if (value > 0) {
        // أكثر دفئًا (أكثر أحمر، أقل أزرق)
        channels[2] = channels[2] * (1 + value / 100.0);
        channels[0] = channels[0] * (1 - value / 200.0);
    } else {
        // أكثر برودة (أكثر أزرق، أقل أحمر)
        channels[0] = channels[0] * (1 - value / 100.0);
        channels[2] = channels[2] * (1 + value / 200.0);
    }
    
    // دمج القنوات
    cv::merge(channels, m_image);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}





bool ImageProcessor::adjustTemperature(int value)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من أن الصورة ملونة
    if (m_image.channels() < 3) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تقسيم القنوات
    std::vector<cv::Mat> channels;
    cv::split(m_image, channels);
    
    // تعديل درجة حرارة اللون
    if (value > 0) {
        // أكثر دفئًا (أكثر أحمر، أقل أزرق)
        channels[2] = channels[2] * (1 + value / 100.0);
        channels[0] = channels[0] * (1 - value / 200.0);
    } else {
        // أكثر برودة (أكثر أزرق، أقل أحمر)
        channels[0] = channels[0] * (1 - value / 100.0);
        channels[2] = channels[2] * (1 + value / 200.0);
    }
    
    // دمج القنوات
    cv::merge(channels, m_image);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}


bool ImageProcessor::adjustShadowsHighlights(int shadows, int highlights)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تحويل الصورة إلى فضاء Lab
    cv::Mat lab;
    cv::cvtColor(m_image, lab, cv::COLOR_BGR2Lab);
    
    // تقسيم القنوات
    std::vector<cv::Mat> channels;
    cv::split(lab, channels);
    
    // تعديل قناة الإضاءة
    cv::Mat& L = channels[0];
    
    // تعديل الظلال
    if (shadows > 0) {
        // زيادة الظلال
        cv::Mat mask;
        cv::threshold(L, mask, 128, 255, cv::THRESH_BINARY_INV);
        L = L + (shadows / 100.0) * mask;
    } else if (shadows < 0) {
        // تقليل الظلال
        cv::Mat mask;
        cv::threshold(L, mask, 128, 255, cv::THRESH_BINARY_INV);
        L = L * (1 + shadows / 100.0) * mask + L * (1 - mask / 255.0);
    }
    
    // تعديل الإضاءات
    if (highlights > 0) {
        // زيادة الإضاءات
        cv::Mat mask;
        cv::threshold(L, mask, 128, 255, cv::THRESH_BINARY);
        L = L + (highlights / 100.0) * mask;
    } else if (highlights < 0) {
        // تقليل الإضاءات
        cv::Mat mask;
        cv::threshold(L, mask, 128, 255, cv::THRESH_BINARY);
        L = L * (1 + highlights / 100.0) * mask + L * (1 - mask / 255.0);
    }
    
    // دمج القنوات
    cv::merge(channels, lab);
    
    // تحويل الصورة إلى فضاء BGR
    cv::cvtColor(lab, m_image, cv::COLOR_Lab2BGR);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}

bool ImageProcessor::autoWhiteBalance()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من أن الصورة ملونة
    if (m_image.channels() < 3) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق توازن اللون الأبيض التلقائي باستخدام خوارزمية Gray World
    std::vector<cv::Mat> channels;
    cv::split(m_image, channels);
    
    // حساب متوسط كل قناة
    cv::Scalar means = cv::mean(m_image);
    double avgB = means[0];
    double avgG = means[1];
    double avgR = means[2];
    
    // حساب متوسط القنوات
    double avgGray = (avgB + avgG + avgR) / 3.0;
    
    // حساب عوامل التصحيح
    double factorB = avgGray / avgB;
    double factorG = avgGray / avgG;
    double factorR = avgGray / avgR;
    
    // تطبيق التصحيح
    channels[0] = channels[0] * factorB;
    channels[1] = channels[1] * factorG;
    channels[2] = channels[2] * factorR;
    
    // دمج القنوات
    cv::merge(channels, m_image);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}




bool ImageProcessor::autoContrast()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تعديل التباين التلقائي
    if (m_image.channels() == 1) {
        // تطبيق معادلة المدرج التكراري للصورة الرمادية
        cv::equalizeHist(m_image, m_image);
    } else {
        // تحويل الصورة إلى فضاء YUV
        cv::Mat yuv;
        cv::cvtColor(m_image, yuv, cv::COLOR_BGR2YUV);
        
        // تقسيم القنوات
        std::vector<cv::Mat> channels;
        cv::split(yuv, channels);
        
        // تطبيق معادلة المدرج التكراري على قناة الإضاءة
        cv::equalizeHist(channels[0], channels[0]);
        
        // دمج القنوات
        cv::merge(channels, yuv);
        
        // تحويل الصورة إلى فضاء BGR
        cv::cvtColor(yuv, m_image, cv::COLOR_YUV2BGR);
    }
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}



bool ImageProcessor::autoColorEnhance()
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من أن الصورة ملونة
    if (m_image.channels() < 3) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تحسين اللون التلقائي
    
    // 1. تطبيق توازن اللون الأبيض
    autoWhiteBalance();
    
    // 2. تحويل الصورة إلى فضاء HSV
    cv::Mat hsv;
    cv::cvtColor(m_image, hsv, cv::COLOR_BGR2HSV);
    
    // 3. تقسيم القنوات
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    
    // 4. زيادة التشبع بنسبة 15%
    channels[1] = channels[1] * 1.15;
    
    // 5. دمج القنوات
    cv::merge(channels, hsv);
    
    // 6. تحويل الصورة إلى فضاء BGR
    cv::cvtColor(hsv, m_image, cv::COLOR_HSV2BGR);
    
    // 7. تطبيق تحسين التباين
    cv::Mat yuv;
    cv::cvtColor(m_image, yuv, cv::COLOR_BGR2YUV);
    
    // تقسيم القنوات
    cv::split(yuv, channels);
    
    // تطبيق معادلة المدرج التكراري على قناة الإضاءة
    cv::equalizeHist(channels[0], channels[0]);
    
    // دمج القنوات
    cv::merge(channels, yuv);
    
    // تحويل الصورة إلى فضاء BGR
    cv::cvtColor(yuv, m_image, cv::COLOR_YUV2BGR);
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}




bool ImageProcessor::reduceNoise(int strength)
{
    // التحقق من وجود صورة
    if (!hasImage()) {
        return false;
    }
    
    // التحقق من صحة قوة التقليل
    if (strength <= 0) {
        return false;
    }
    
    // حفظ الحالة الحالية للتراجع
    saveState();
    
    // تطبيق تقليل الضوضاء
    cv::Mat result;
    
    // استخدام مرشح غير محلي متوسط
    cv::fastNlMeansDenoisingColored(m_image, result, strength, strength, 7, 21);
    m_image = result;
    
    // مسح سجل الإعادة
    clearRedoStack();
    
    // إشعار بالتحديث
    notifyUpdate();
    
    return true;
}



