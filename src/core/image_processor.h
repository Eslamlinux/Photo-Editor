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

