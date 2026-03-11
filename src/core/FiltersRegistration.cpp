#include "core/BaseFilter.h"
#include "filters/Crop.h"
#include "filters/EdgeDetection.h"
#include "core/FilterDescriptor.h"
#include "core/FilterFactory.h"
#include "filters/Grayscale.h"
#include "filters/GaussianBlur.h"
#include "filters/Negative.h"
#include "filters/PointSurroundingsBlur.h"
#include "filters/PointSurroundingsBlurFast.h"
#include "filters/Sharpening.h"

#include <stdexcept>
#include <memory>

std::unique_ptr<BaseFilter> CreateCrop(const FilterDescriptor& fd) {
    if (fd.args.size() != 2) {
        throw std::runtime_error("Crop expects 2 arguments");
    }
    size_t w = std::stoul(fd.args[0]);
    size_t h = std::stoul(fd.args[1]);
    return std::make_unique<CropFilter>(w, h);
}

std::unique_ptr<BaseFilter> CreateNegative(const FilterDescriptor& fd) {
    if (!fd.args.empty()) {
        throw std::runtime_error("Negative expects no arguments");
    }
    return std::make_unique<NegativeFilter>();
}

std::unique_ptr<BaseFilter> CreateGrayscale(const FilterDescriptor& fd) {
    if (!fd.args.empty()) {
        throw std::runtime_error("Grayscale expects no arguments");
    }
    return std::make_unique<GrayscaleFilter>();
}

std::unique_ptr<BaseFilter> CreateSharpening(const FilterDescriptor& fd) {
    if (!fd.args.empty()) {
        throw std::runtime_error("Sharpening expects no arguments");
    }
    return std::make_unique<SharpeningFilter>();
}

std::unique_ptr<BaseFilter> CreateEdgeDetection(const FilterDescriptor& fd) {
    if (fd.args.size() != 1) {
        throw std::runtime_error("Edge Detection expects 1 arguments");
    }
    double threshold = std::stod(fd.args[0]);
    if (threshold < 0 || threshold > 1) {
        throw std::out_of_range("Inputs out of range");
    }
    return std::make_unique<EdgeDetectionFilter>(threshold);
}

std::unique_ptr<BaseFilter> CreateGaussianBlur(const FilterDescriptor& fd) {
    if (fd.args.size() != 1) {
        throw std::runtime_error("Gaussian Blur expects 1 arguments");
    }
    double sigma = std::stod(fd.args[0]);
    if (sigma <= 0) {
        throw std::out_of_range("Inputs out of range");
    }
    return std::make_unique<GaussianBlurFilter>(sigma);
}

std::unique_ptr<BaseFilter> CreatePointSurroundingsBlurFilter(const FilterDescriptor& fd) {
    if (fd.args.size() != 4) {
        throw std::runtime_error("Point Surroundings Blur expects 4 arguments");
    }
    double focus_x = std::stod(fd.args[0]);
    double focus_y = std::stod(fd.args[1]);
    int32_t safe_radius = std::stoi(fd.args[2]);
    int32_t max_blur = std::stoi(fd.args[3]);
    if (safe_radius < 0 || max_blur < 0 || focus_x < 0 || focus_y < 0 || focus_x > 1 || focus_y > 1) {
        throw std::out_of_range("Inputs out of range");
    }
    return std::make_unique<PointSurroundingsBlurFilter>(focus_x, focus_y, safe_radius, max_blur);
}

std::unique_ptr<BaseFilter> CreatePointSurroundingsBlurFilterFast(const FilterDescriptor& fd) {
    if (fd.args.size() != 4) {
        throw std::runtime_error("Point Surroundings Blur expects 4 arguments");
    }
    double focus_x = std::stod(fd.args[0]);
    double focus_y = std::stod(fd.args[1]);
    int32_t safe_radius = std::stoi(fd.args[2]);
    int32_t max_blur = std::stoi(fd.args[3]);
    if (safe_radius < 0 || max_blur < 0 || focus_x < 0 || focus_y < 0 || focus_x > 1 || focus_y > 1) {
        throw std::out_of_range("Inputs out of range");
    }
    return std::make_unique<PointSurroundingsBlurFastFilter>(focus_x, focus_y, safe_radius, max_blur);
}

void Registration(FilterFactory& factory) {
    factory.Register("gs", CreateGrayscale);
    factory.Register("crop", CreateCrop);
    factory.Register("sharp", CreateSharpening);
    factory.Register("edge", CreateEdgeDetection);
    factory.Register("neg", CreateNegative);
    factory.Register("blur", CreateGaussianBlur);
    factory.Register("pointsurblur", CreatePointSurroundingsBlurFilter);
    factory.Register("pointsurblur+", CreatePointSurroundingsBlurFilterFast);
}