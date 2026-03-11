#pragma once

#include "core/BaseFilter.h"
#include <vector>

class GaussianBlurFilter : public BaseFilter {
public:
    explicit GaussianBlurFilter(double sigma);
    void Apply(BMPImage& image) const override;
    std::unique_ptr<BaseFilter> Clone() const override {
        return std::make_unique<GaussianBlurFilter>(*this);
    }

private:
    double sigma_;
    int radius_;
    std::vector<double> kernel_1d_;
};