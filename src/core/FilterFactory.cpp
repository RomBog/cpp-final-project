#include "core/FilterFactory.h"
#include <stdexcept>

void FilterFactory::Register(const std::string& name, Aura creation_function) {
    creations_[name] = creation_function;
}

std::unique_ptr<BaseFilter> FilterFactory::Create(const FilterDescriptor& fd) const {
    auto it = creations_.find(fd.name);
    if (it == creations_.end()) {
        throw std::runtime_error("unknown filter type: " + fd.name);
    }
    return it->second(fd);
}