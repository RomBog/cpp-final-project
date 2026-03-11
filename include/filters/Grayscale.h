#pragma once

#include "core/BaseFilter.h"

class GrayscaleFilter : public BaseFilter {
public:
    GrayscaleFilter() = default;
    void Apply(BMPImage& image) const override;
    std::unique_ptr<BaseFilter> Clone() const override {
        return std::make_unique<GrayscaleFilter>(*this);
    }
};