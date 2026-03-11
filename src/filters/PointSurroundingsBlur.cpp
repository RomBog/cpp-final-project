#include "filters/PointSurroundingsBlur.h"
#include <algorithm>
#include <cmath>
#include <iostream>

PointSurroundingsBlurFilter::PointSurroundingsBlurFilter(double focus_x, double focus_y, int focus_radius,
                                                         int max_blur_radius)
    : focus_x_(focus_x), focus_y_(focus_y), focus_radius_(focus_radius), max_blur_radius_(max_blur_radius) {
}

void PointSurroundingsBlurFilter::Apply(BMPImage& image) const {
    std::size_t width = image.Width();
    std::size_t height = image.Height();
    std::vector<Pixel> new_data(width * height);

    const double ci = focus_x_ * static_cast<double>(height);
    const double cj = focus_y_ * static_cast<double>(width);

    for (int32_t i = 0; i < height; ++i) {
        for (int32_t j = 0; j < width; ++j) {
            double dist = std::hypot(static_cast<double>(i) - ci, static_cast<double>(j) - cj);

            double t = std::clamp((dist - focus_radius_) / focus_radius_, 0.0, 1.0);
            auto blur_radius = static_cast<int32_t>(t * max_blur_radius_);

            if (blur_radius == 0) {
                new_data[i * width + j] = image.At(i, j);
                continue;
            }

            int32_t r_sum = 0;
            int32_t g_sum = 0;
            int32_t b_sum = 0;
            int32_t count = 0;

            int32_t li = std::max(0, i - blur_radius);
            int32_t ri = std::min(static_cast<int32_t>(height), i + blur_radius + 1);
            int32_t lj = std::max(0, j - blur_radius);
            int32_t rj = std::min(static_cast<int32_t>(width), j + blur_radius + 1);
            for (size_t i1 = li; i1 < ri; ++i1) {
                for (size_t j1 = lj; j1 < rj; ++j1) {
                    if (std::abs(static_cast<int>(i) - static_cast<int>(i1)) +
                            std::abs(static_cast<int>(j) - static_cast<int>(j1)) >
                        blur_radius) {
                        continue;
                    }
                    auto& p = image.At(i1, j1);
                    r_sum += p.r;
                    g_sum += p.g;
                    b_sum += p.b;
                    ++count;
                }
            }

            new_data[i * width + j] = Pixel(static_cast<uint8_t>(b_sum / count), static_cast<uint8_t>(g_sum / count),
                                            static_cast<uint8_t>(r_sum / count));
        }
    }

    image.ReplaceData(std::move(new_data), width, height);
}