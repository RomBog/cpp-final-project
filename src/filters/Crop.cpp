#include "filters/Crop.h"

CropFilter::CropFilter(size_t width, size_t height) : width_(width), height_(height) {
}

void CropFilter::Apply(BMPImage& image) const {
    size_t new_width = std::min(width_, image.Width());
    size_t new_height = std::min(height_, image.Height());

    std::vector<Pixel> new_data;
    for (size_t i = 0; i < new_height; ++i) {
        for (size_t j = 0; j < new_width; ++j) {
            new_data.push_back(image.At(i, j));
        }
    }
    image.ReplaceData(std::move(new_data), new_width, new_height);
}