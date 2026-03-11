#include "filters/Grayscale.h"

void GrayscaleFilter::Apply(BMPImage& image) const {
    for (size_t i = 0; i < image.Height(); ++i) {
        for (size_t j = 0; j < image.Width(); ++j) {
            auto& pixel = image.At(i, j);
            const auto gray = static_cast<uint8_t>(0.299 * pixel.r + 0.587 * pixel.g + 0.114 * pixel.b);
            pixel = {gray, gray, gray};
        }
    }
}
