#include "filters/EdgeDetection.h"
#include <algorithm>

constexpr int MaxColorValue = 255;

constexpr int32_t Matrix[3][3] = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};

EdgeDetectionFilter::EdgeDetectionFilter(double sigma) : threshold_(sigma) {
}

void EdgeDetectionFilter::Apply(BMPImage& image) const {
    std::size_t width = image.Width();
    std::size_t height = image.Height();
    std::vector<Pixel> new_data(width * height);

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            int32_t result = 0;

            for (int32_t y = -1; y <= 1; ++y) {
                for (int32_t x = -1; x <= 1; ++x) {
                    int32_t dy = std::clamp(0, static_cast<int32_t>(i) + y, static_cast<int32_t>(height) - 1);
                    int32_t dx = std::clamp(0, static_cast<int32_t>(j) + x, static_cast<int32_t>(width) - 1);

                    const Pixel& now = image.At(dy, dx);
                    const auto gray = static_cast<uint8_t>(0.299 * now.r + 0.587 * now.g + 0.114 * now.b);

                    result += Matrix[y + 1][x + 1] * gray;
                }
            }

            if (static_cast<double>(result) > threshold_ * MaxColorValue) {
                result = MaxColorValue;
            } else {
                result = 0;
            }

            new_data[i * width + j] = Pixel(result, result, result);
        }
    }

    image.ReplaceData(std::move(new_data), width, height);
}