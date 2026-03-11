#pragma once

#include "core/BaseFilter.h"

class NegativeFilter : public BaseFilter {
public:
    NegativeFilter() = default;
    void Apply(BMPImage& image) const override;
    std::unique_ptr<BaseFilter> Clone() const override {
        return std::make_unique<NegativeFilter>(*this);
    }
};