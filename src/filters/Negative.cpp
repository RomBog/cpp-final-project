#include "filters/Negative.h"

constexpr int MaxColorValue = 255;

void NegativeFilter::Apply(BMPImage& image) const {
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            auto& pixel = image.At(i, j);
            pixel = Pixel(static_cast<uint8_t>(MaxColorValue - pixel.b), static_cast<uint8_t>(MaxColorValue - pixel.g),
                          static_cast<uint8_t>(MaxColorValue - pixel.r));
        }
    }
}