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
