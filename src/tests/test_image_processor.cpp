#include <gtest/gtest.h>
#include "../core/image_processor.h"

TEST(ImageProcessorTest, EmptyImageOnInit) {
    pme::core::ImageProcessor processor;
    EXPECT_TRUE(processor.getImage().empty());
}

TEST(ImageProcessorTest, LoadInvalidImage) {
    pme::core::ImageProcessor processor;
    EXPECT_FALSE(processor.loadImage("nonexistent_file.jpg"));
}

// More tests would be added here
