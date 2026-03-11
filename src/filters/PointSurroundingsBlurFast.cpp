#include "filters/PointSurroundingsBlurFast.h"
#include <algorithm>
#include <cmath>
#include <cassert>
#include <vector>

PointSurroundingsBlurFastFilter::PointSurroundingsBlurFastFilter(double focus_x, double focus_y, int focus_radius,
                                                                 int max_blur_radius)
    : focus_x_(focus_x), focus_y_(focus_y), focus_radius_(focus_radius), max_blur_radius_(max_blur_radius) {
}

void PointSurroundingsBlurFastFilter::Apply(BMPImage& image) const {
    std::size_t width = image.Width();
    std::size_t height = image.Height();

    auto field_size = static_cast<int32_t>(width + height - 1);
    auto offset = static_cast<int32_t>(width - 1);

    std::vector<std::vector<int32_t>> sum_r(field_size, std::vector<int32_t>(field_size));
    std::vector<std::vector<int32_t>> sum_g(field_size, std::vector<int32_t>(field_size));
    std::vector<std::vector<int32_t>> sum_b(field_size, std::vector<int32_t>(field_size));
    std::vector<std::vector<int32_t>> sum_cnt(field_size, std::vector<int32_t>(field_size));

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            const Pixel& now = image.At(i, j);
            sum_r[i + j][i - j + offset] = now.r;
            sum_g[i + j][i - j + offset] = now.g;
            sum_b[i + j][i - j + offset] = now.b;
            sum_cnt[i + j][i - j + offset] = 1;
        }
    }
    for (size_t i = 0; i < field_size; ++i) {
        for (size_t j = 0; j < field_size; ++j) {
            if (i > 0) {
                sum_r[i][j] += sum_r[i - 1][j];
                sum_g[i][j] += sum_g[i - 1][j];
                sum_b[i][j] += sum_b[i - 1][j];
                sum_cnt[i][j] += sum_cnt[i - 1][j];
            }
            if (j > 0) {
                sum_r[i][j] += sum_r[i][j - 1];
                sum_g[i][j] += sum_g[i][j - 1];
                sum_b[i][j] += sum_b[i][j - 1];
                sum_cnt[i][j] += sum_cnt[i][j - 1];
            }
            if (i > 0 && j > 0) {
                sum_r[i][j] -= sum_r[i - 1][j - 1];
                sum_g[i][j] -= sum_g[i - 1][j - 1];
                sum_b[i][j] -= sum_b[i - 1][j - 1];
                sum_cnt[i][j] -= sum_cnt[i - 1][j - 1];
            }
        }
    }

    std::vector<Pixel> new_data(width * height);

    const double ci = focus_x_ * static_cast<double>(height);
    const double cj = focus_y_ * static_cast<double>(width);

    for (int32_t i = 0; i < height; ++i) {
        for (int32_t j = 0; j < width; ++j) {
            double dist = std::hypot(static_cast<double>(i) - ci, static_cast<double>(j) - cj);

            double t = std::clamp((dist - focus_radius_) / (focus_radius_), 0.0, 1.0);
            auto blur_radius = static_cast<int32_t>(t * max_blur_radius_);

            if (blur_radius <= 0) {
                new_data[i * width + j] = image.At(i, j);
                continue;
            }

            int32_t u = i + j;
            int32_t v = i - j + offset;

            int32_t u1 = std::max(0, u - blur_radius);
            int32_t v1 = std::max(0, v - blur_radius);
            int32_t u2 = std::min(field_size - 1, u + blur_radius);
            int32_t v2 = std::min(field_size - 1, v + blur_radius);

            auto get_sum = [&](const std::vector<std::vector<int32_t>>& a) {
                return a[u2][v2] - (u1 ? a[u1 - 1][v2] : 0) - (v1 ? a[u2][v1 - 1] : 0) +
                       (u1 && v1 ? a[u1 - 1][v1 - 1] : 0);
            };

            int32_t count = get_sum(sum_cnt);

            new_data[i * width + j] =
                Pixel(static_cast<uint8_t>(get_sum(sum_b) / count), static_cast<uint8_t>(get_sum(sum_g) / count),
                      static_cast<uint8_t>(get_sum(sum_r) / count));
        }
    }

    image.ReplaceData(std::move(new_data), width, height);
}