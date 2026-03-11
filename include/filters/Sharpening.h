#pragma once

#include "core/BaseFilter.h"

class SharpeningFilter : public BaseFilter {
public:
    SharpeningFilter() = default;
    void Apply(BMPImage& image) const override;
    std::unique_ptr<BaseFilter> Clone() const override {
        return std::make_unique<SharpeningFilter>(*this);
    }
};