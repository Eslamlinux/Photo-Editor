#ifndef PME_CORE_AI_BACKGROUND_REMOVER_H
#define PME_CORE_AI_BACKGROUND_REMOVER_H

#include <string>
#include <vector>
#include <memory>
#include <opencv2/core.hpp>

// تعريف DISABLE_AI_FEATURES إذا لم يكن معرفًا بالفعل
#ifndef DISABLE_AI_FEATURES
// تعريف مسبق لتجنب تضمين رؤوس ONNX Runtime في هذا الملف
namespace Ort {
    class Session;
    class Env;
    class MemoryInfo;
    class Value;
    class RunOptions;
    class AllocatorWithDefaultOptions;
}
#endif

namespace pme {
namespace core {
namespace ai {

#ifndef DISABLE_AI_FEATURES
class BackgroundRemover {
public:
    BackgroundRemover(const std::string& modelPath);
    ~BackgroundRemover();
    
    cv::Mat removeBg(const cv::Mat& inputImage);
    
private:
    void preprocess(const cv::Mat& input, cv::Mat& output);
    void postprocess(const Ort::Value& tensor, cv::Mat& mask, const cv::Size& originalSize);
    void applyMask(const cv::Mat& image, const cv::Mat& mask, cv::Mat& result);
    
    std::unique_ptr<Ort::Env> m_env;
    std::unique_ptr<Ort::Session> m_session;
    std::unique_ptr<Ort::MemoryInfo> m_memoryInfo;
    std::unique_ptr<Ort::AllocatorWithDefaultOptions> m_allocator;
    
    std::string m_inputName;
    std::string m_outputName;
    std::vector<int64_t> m_inputDims;
    std::vector<int64_t> m_outputDims;
    
    bool m_initialized;
};
#endif // !DISABLE_AI_FEATURES

// وظيفة مساعدة لإزالة الخلفية
cv::Mat removeBackground(const cv::Mat& inputImage, const std::string& modelPath = "");

} // namespace ai
} // namespace core
} // namespace pme

#endif // PME_CORE_AI_BACKGROUND_REMOVER_H
