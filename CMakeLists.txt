cmake_minimum_required(VERSION 3.10)
project(image_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(
        image_processor
        main/image_processor.cpp

        src/bmp/BMPImage.cpp

        src/core/BaseFilter.cpp
        src/core/FilterFactory.cpp
        src/core/FiltersRegistration.cpp
        src/core/Parser.cpp
        src/core/Pipeline.cpp

        src/filters/Grayscale.cpp
        src/filters/Crop.cpp
        src/filters/Sharpening.cpp
        src/filters/EdgeDetection.cpp
        src/filters/Negative.cpp
        src/filters/PointSurroundingsBlur.cpp
        src/filters/PointSurroundingsBlurFast.cpp
        src/filters/GaussianBlur.cpp
)