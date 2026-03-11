#include "core/Pipeline.h"
#include <vector>
#include <memory>

void Pipeline::Add(std::unique_ptr<BaseFilter> filter) {
    filters_.push_back(std::move(filter));
}

void Pipeline::Apply(BMPImage& image) const {
    for (const auto& filter : filters_) {
        filter->Apply(image);
    }
}

Pipeline::Pipeline(const Pipeline& other) {
    for (const auto& filter : other.filters_) {
        filters_.push_back(filter->Clone());
    }
}