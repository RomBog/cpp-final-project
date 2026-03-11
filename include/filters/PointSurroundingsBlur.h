#pragma once

#include "core/BaseFilter.h"

class PointSurroundingsBlurFilter : public BaseFilter {
public:
    PointSurroundingsBlurFilter(double focus_x, double focus_y, int32_t focus_radius, int32_t max_blur_radius);
    PointSurroundingsBlurFilter() = default;
    void Apply(BMPImage& image) const override;
    std::unique_ptr<BaseFilter> Clone() const override {
        return std::make_unique<PointSurroundingsBlurFilter>(*this);
    }

private:
    double focus_x_;
    double focus_y_;
    int32_t focus_radius_;
    int32_t max_blur_radius_;
};