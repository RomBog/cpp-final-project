#pragma once

#include "BaseFilter.h"
#include "FilterDescriptor.h"
#include <string>
#include <map>
#include <memory>

using Aura = std::unique_ptr<BaseFilter> (*)(const FilterDescriptor&);

class FilterFactory {
public:
    void Register(const std::string& name, Aura creation_function);
    std::unique_ptr<BaseFilter> Create(const FilterDescriptor& fd) const;

private:
    std::map<std::string, Aura> creations_;
};
