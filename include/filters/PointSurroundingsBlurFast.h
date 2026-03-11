#pragma once

#include "core/BaseFilter.h"

class PointSurroundingsBlurFastFilter : public BaseFilter {
public:
    PointSurroundingsBlurFastFilter(double focus_x, double focus_y, int32_t focus_radius, int32_t max_blur_radius);
    PointSurroundingsBlurFastFilter() = default;
    void Apply(BMPImage& image) const override;
    std::unique_ptr<BaseFilter> Clone() const override {
        return std::make_unique<PointSurroundingsBlurFastFilter>(*this);
    }

private:
    double focus_x_;
    double focus_y_;
    int32_t focus_radius_;
    int32_t max_blur_radius_;
};