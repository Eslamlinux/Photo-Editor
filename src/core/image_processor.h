#ifndef PME_CORE_IMAGE_PROCESSOR_H
#define PME_CORE_IMAGE_PROCESSOR_H

#include <string>
#include <vector>
#include <functional>
#include <opencv2/core.hpp>

namespace pme {
namespace core {

class ImageProcessor {
public:
    ImageProcessor();
    ~ImageProcessor();
    
    // نوع وظيفة التحديث
    using UpdateCallback = std::function<void()>;
    
    // تعيين وظيفة التحديث
    void setUpdateCallback(UpdateCallback callback);
    
    // عمليات الملف
    bool loadImage(const std::string& filePath);
    bool saveImage(const std::string& filePath);
    
    // عمليات التراجع/الإعادة
    bool undo();
    bool redo();
    bool canUndo() const;
    bool canRedo() const;
    
    // عمليات التحويل
    bool crop(int x, int y, int width, int height);
    bool rotate90CW();
    bool rotate90CCW();
    bool rotate180();
    bool flipHorizontal();
    bool flipVertical();
    bool resize(int width, int height);
    
    // عمليات المرشحات
    bool grayscale();
    bool sepia();
    bool negative();
    bool blur(int radius = 5);
    bool sharpen();
    bool edgeDetection();
    bool emboss();
    bool cartoon();
    bool watercolor();
    bool mosaic(int blockSize = 10);
    bool oilPainting(int size = 5, int dynRatio = 1);
    bool pencilSketch(bool colorOutput = false);
    
    // عمليات التعديل
    bool adjustBrightness(int value);
    bool adjustContrast(int value);
    bool adjustSaturation(int value);
    bool adjustHue(int value);
    bool adjustGamma(double value);
    bool adjustTemperature(int value);
    bool adjustShadowsHighlights(int shadows, int highlights);
    bool autoWhiteBalance();
    bool autoContrast();
    bool autoColorEnhance();
    bool reduceNoise(int strength = 3);
    bool sharpenAdaptive(int strength = 50);
    
    // عمليات أخرى
    bool reset();
    bool cropToAspectRatio(double ratio);
    bool addBorder(int size, const cv::Scalar& color = cv::Scalar(255, 255, 255));
    bool addVignette(double strength = 0.5);
    bool addWatermark(const cv::Mat& watermark, double alpha = 0.3, int x = -1, int y = -1);
    bool addText(const std::string& text, int x, int y, double scale = 1.0, 
                 const cv::Scalar& color = cv::Scalar(255, 255, 255), int thickness = 1);
    
    // الوصول إلى الصورة
    bool hasImage() const;
    cv::Mat getImage() const;
    int getWidth() const;
    int getHeight() const;
    int getChannels() const;
    
private:
    // الصورة الحالية
    cv::Mat m_image;
    cv::Mat m_originalImage;
    
    // سجل التراجع/الإعادة
    std::vector<cv::Mat> m_undoStack;
    std::vector<cv::Mat> m_redoStack;
    
    // وظيفة التحديث
    UpdateCallback m_updateCallback;
    
    // حفظ الحالة الحالية للتراجع
    void saveState();
    
    // مسح سجل الإعادة
    void clearRedoStack();
    
    // استدعاء وظيفة التحديث
    void notifyUpdate();
};

} // namespace core
} // namespace pme

#endif // PME_CORE_IMAGE_PROCESSOR_H
