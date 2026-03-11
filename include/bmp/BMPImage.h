#pragma once

#include <cstdint>
#include <string>
#include <vector>

struct Pixel {
    uint8_t b;
    uint8_t g;
    uint8_t r;
    Pixel() = default;
    Pixel(uint8_t b, uint8_t g, uint8_t r) : b(b), g(g), r(r) {
    }
};

class BMPImage {
public:
    BMPImage() = default;
    explicit BMPImage(const std::string& path) {
        Load(path);
    }

    void Load(const std::string& path);
    void Save(const std::string& path) const;

    std::size_t Width() const noexcept {
        return width_;
    }
    std::size_t Height() const noexcept {
        return height_;
    }

    std::vector<Pixel>& Data() noexcept {
        return data_;
    }
    const std::vector<Pixel>& Data() const noexcept {
        return data_;
    }
    void ReplaceData(std::vector<Pixel>&& new_data, size_t new_width, size_t new_height) noexcept;

    Pixel& At(size_t i, size_t j);

private:
    std::size_t width_{0};
    std::size_t height_{0};
    std::vector<Pixel> data_;
};
