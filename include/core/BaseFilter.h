#pragma once

#include "bmp/BMPImage.h"
#include <memory>

class BaseFilter {
public:
    virtual ~BaseFilter() = default;
    virtual void Apply(BMPImage& image) const = 0;
    virtual std::unique_ptr<BaseFilter> Clone() const = 0;
};
