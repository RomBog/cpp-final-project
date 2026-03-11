#pragma once

#include "BaseFilter.h"
#include <vector>
#include <memory>

class Pipeline {
public:
    void Add(std::unique_ptr<BaseFilter> filter);
    void Apply(BMPImage& image) const;
    Pipeline() = default;

    Pipeline(const Pipeline& other);

private:
    std::vector<std::unique_ptr<BaseFilter>> filters_;
};
