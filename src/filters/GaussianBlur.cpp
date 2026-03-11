#include "filters/GaussianBlur.h"
#include <vector>
#include <cmath>
#include <algorithm>

constexpr double KGaussianSigmaMultiplier = 3.0;
constexpr double DGaussian2 = 2.0;
constexpr int IGaussian2 = 2;

GaussianBlurFilter::GaussianBlurFilter(double sigma)
    : sigma_(sigma), radius_(static_cast<int>(std::ceil(KGaussianSigmaMultiplier * sigma_))) {
    kernel_1d_.resize(IGaussian2 * radius_ + 1);

    const double norm = 1.0 / (std::sqrt(DGaussian2 * M_PI) * sigma_);
    double sum = 0.0;

    for (int32_t i = -radius_; i <= radius_; ++i) {
        double value = norm * std::exp(static_cast<double>(-i * i) / (DGaussian2 * sigma_ * sigma_));
        kernel_1d_[i + radius_] = value;
        sum += value;
    }
    for (double &val : kernel_1d_) {
        val /= sum;
    }
}

void GaussianBlurFilter::Apply(BMPImage &image) const {
    const size_t width = image.Width();
    const size_t height = image.Height();

    std::vector<Pixel> result_hor(height * width);

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {

            double r_sum = 0;
            double g_sum = 0;
            double b_sum = 0;

            for (int32_t k = -radius_; k <= radius_; ++k) {
                int jw = std::clamp(static_cast<int32_t>(j) + k, 0, static_cast<int>(width - 1));

                auto &pixel = image.At(i, jw);
                double weight = kernel_1d_[k + radius_];

                r_sum += pixel.r * weight;
                g_sum += pixel.g * weight;
                b_sum += pixel.b * weight;
            }

            result_hor[i * width + j] =
                Pixel(static_cast<uint8_t>(std::round(b_sum)), static_cast<uint8_t>(std::round(g_sum)),
                      static_cast<uint8_t>(std::round(r_sum)));
        }
    }

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {

            double r_sum = 0;
            double g_sum = 0;
            double b_sum = 0;

            for (int32_t k = -radius_; k <= radius_; ++k) {
                int iw = std::clamp(static_cast<int32_t>(i) + k, 0, static_cast<int>(height - 1));

                auto &pixel = result_hor[iw * width + j];
                double weight = kernel_1d_[k + radius_];

                r_sum += pixel.r * weight;
                g_sum += pixel.g * weight;
                b_sum += pixel.b * weight;
            }

            auto &pixel = image.At(i, j);
            pixel = Pixel(static_cast<uint8_t>(std::round(b_sum)), static_cast<uint8_t>(std::round(g_sum)),
                          static_cast<uint8_t>(std::round(r_sum)));
        }
    }
}