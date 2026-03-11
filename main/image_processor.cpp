#include <iostream>
#include "core/Parser.h"
#include "core/FilterFactory.h"
#include "core/Pipeline.h"
#include "bmp/BMPImage.h"
#include "core/FiltersRegistration.h"

int main(int argc, char* argv[]) {
    try {
        Parser parser(argc, argv);

        FilterFactory filter_factory;
        Pipeline pipeline;

        Registration(filter_factory);

        BMPImage img;
        img.Load(parser.GetInputPath());

        for (auto& fd : parser.GetFilters()) {
            pipeline.Add(filter_factory.Create(fd));
        }
        pipeline.Apply(img);

        img.Save(parser.GetOutputPath());
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Unknown error!" << std::endl;
    }

    return 0;
}
