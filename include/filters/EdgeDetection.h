#pragma once

#include "core/BaseFilter.h"
class EdgeDetectionFilter : public BaseFilter {
public:
    explicit EdgeDetectionFilter(double sigma);
    void Apply(BMPImage& image) const override;
    std::unique_ptr<BaseFilter> Clone() const override {
        return std::make_unique<EdgeDetectionFilter>(*this);
    }

private:
    double threshold_;
};