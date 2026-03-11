#include "filters/Sharpening.h"
#include <algorithm>

constexpr int MaxColorValue = 255;

constexpr int32_t Matrix[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};

void SharpeningFilter::Apply(BMPImage& image) const {
    std::size_t width = image.Width();
    std::size_t height = image.Height();
    std::vector<Pixel> new_data(width * height);

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            int32_t r = 0;
            int32_t g = 0;
            int32_t b = 0;

            for (int32_t y = -1; y <= 1; ++y) {
                for (int32_t x = -1; x <= 1; ++x) {
                    int32_t dy = std::clamp(0, static_cast<int32_t>(i) + y, static_cast<int32_t>(height - 1));
                    int32_t dx = std::clamp(0, static_cast<int32_t>(j) + x, static_cast<int32_t>(width - 1));

                    const Pixel& now = image.At(dy, dx);

                    r += Matrix[y + 1][x + 1] * now.r;
                    g += Matrix[y + 1][x + 1] * now.g;
                    b += Matrix[y + 1][x + 1] * now.b;
                }
            }

            new_data[i * width + j] = Pixel(static_cast<int8_t>(std::clamp(0, b, MaxColorValue)),
                                            static_cast<int8_t>(std::clamp(0, g, MaxColorValue)),
                                            static_cast<int8_t>(std::clamp(0, r, MaxColorValue)));
        }
    }

    image.ReplaceData(std::move(new_data), width, height);
}
