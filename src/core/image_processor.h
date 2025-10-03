#ifndef PME_CORE_IMAGE_PROCESSOR_H
#define PME_CORE_IMAGE_PROCESSOR_H

#include <string>
#include <vector>
#include <functional>
#include <opencv2/core.hpp>

namespace pme {
namespace core {


#endif // PME_CORE_IMAGE_PROCESSOR_H
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
