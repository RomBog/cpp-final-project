#pragma once

#include "core/BaseFilter.h"

class CropFilter : public BaseFilter {
public:
    CropFilter(size_t width, size_t height);
    void Apply(BMPImage& image) const override;
    std::unique_ptr<BaseFilter> Clone() const override {
        return std::make_unique<CropFilter>(*this);
    }

private:
    size_t width_;
    size_t height_;
};