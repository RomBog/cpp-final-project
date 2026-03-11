#include "bmp/BMPImage.h"
#include <fstream>
#include <stdexcept>

constexpr uint16_t BM = 0x4D42;
constexpr uint16_t BMBitCount = 24;

#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t file_type;
    uint32_t file_size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t start_position_of_pixel_data;
};

struct BMPInfoHeader {
    uint32_t size_of_this_header;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t size_image;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t colors_used;
    uint32_t important_colors;
};

#pragma pack(pop)

static std::size_t RowLength(std::size_t width) {
    return width * 3 + (4 - (width * 3 % 4)) % 4;
}

Pixel& BMPImage::At(size_t i, size_t j) {
    if (i >= height_ || j >= width_) {
        throw std::out_of_range("Pixel coordinates out of range");
    }
    return data_[i * width_ + j];
}

void BMPImage::Load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + path);
    }

    BMPFileHeader file_header{};
    BMPInfoHeader info_header{};

    file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    if (file_header.file_type != BM) {
        throw std::runtime_error("Not a BMP file");
    }
    if (info_header.bit_count != BMBitCount) {
        throw std::runtime_error("Unsupported bit depth");
    }
    if (info_header.height == 0 || info_header.width == 0) {
        throw std::runtime_error("Empty file");
    }
    if (info_header.height < 0) {
        throw std::runtime_error("BMP can't be top-down");
    }

    width_ = static_cast<std::size_t>(info_header.width);
    height_ = static_cast<std::size_t>(info_header.height);

    file.seekg(static_cast<std::streamoff>(file_header.start_position_of_pixel_data), std::ios::beg);

    data_.assign(width_ * height_, Pixel{});
    const std::size_t chunk = RowLength(width_);
    std::vector<uint8_t> row(chunk);

    for (std::size_t i = 0; i < height_; ++i) {
        file.read(reinterpret_cast<char*>(row.data()), static_cast<std::streamsize>(chunk));
        if (!file) {
            throw std::runtime_error("Broken file");
        }
        const uint8_t* p = row.data();
        for (std::size_t j = 0; j < width_; ++j) {
            data_[(height_ - 1 - i) * width_ + j] = Pixel{*p++, *p++, *p++};
        }
    }
}

void BMPImage::Save(const std::string& path) const {
    if (width_ == 0 || height_ == 0) {
        throw std::runtime_error("Empty image");
    }

    std::ofstream file(path, std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + path);
    }

    const std::size_t chunk = RowLength(width_);

    BMPFileHeader file_header{};
    file_header.file_type = BM;
    file_header.start_position_of_pixel_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    file_header.file_size = file_header.start_position_of_pixel_data + static_cast<uint32_t>(chunk) * height_;

    BMPInfoHeader info_header{};
    info_header.size_of_this_header = sizeof(BMPInfoHeader);
    info_header.width = static_cast<int32_t>(width_);
    info_header.height = static_cast<int32_t>(height_);
    info_header.planes = 1;
    info_header.bit_count = BMBitCount;
    info_header.compression = 0;
    info_header.size_image = static_cast<uint32_t>(chunk) * height_;

    file.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
    file.write(reinterpret_cast<const char*>(&info_header), sizeof(info_header));

    std::vector<uint8_t> row(chunk);
    for (std::size_t i = 0; i < height_; ++i) {
        uint8_t* p = row.data();
        for (std::size_t j = 0; j < width_; ++j) {
            const Pixel& now = data_[(height_ - 1 - i) * width_ + j];
            *p++ = now.b;
            *p++ = now.g;
            *p++ = now.r;
        }
        for (std::size_t j = 0; j < (4 - (width_ * 3 % 4)) % 4; ++j) {
            *p++ = 0;
        }

        file.write(reinterpret_cast<const char*>(row.data()), static_cast<std::streamsize>(chunk));
    }
}

void BMPImage::ReplaceData(std::vector<Pixel>&& new_data, std::size_t new_width, std::size_t new_height) noexcept {
    data_ = std::move(new_data);
    width_ = new_width;
    height_ = new_height;
}