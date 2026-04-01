#include <iostream>
#include <string>
#include <filesystem>

#include "Image.h"
#include "imageread.h"
#include "imagewrite.h"
#include "intensity.h"
#include "transforms.h"
#include "imageutils.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[])
{
    // --------------------------------------------
    // Argument check
    // --------------------------------------------
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0]
            << " <absolute_input_image_path> <relative_output_folder>\n";
        return 1;
    }

    std::string inputPath = argv[1];
    std::string outputDir = argv[2];

    std::cout << "Input Image: " << inputPath << "\n";
    std::cout << "Output Dir : " << outputDir << "\n";

    try
    {
        // --------------------------------------------
        // Validate input path
        // --------------------------------------------
        if (!fs::exists(inputPath))
            throw std::runtime_error("Input file does not exist");

        // --------------------------------------------
        // Ensure output directory exists
        // --------------------------------------------
        if (!fs::exists(outputDir))
        {
            std::cout << "Output directory does not exist. Creating...\n";
            fs::create_directories(outputDir);
        }

        // --------------------------------------------
        // Read image
        // --------------------------------------------
        std::string imgType = ImageReader::get_image_type(inputPath);
        std::cout << "Detected type: " << imgType << "\n";

        if (imgType != "PNG")
            throw std::runtime_error("Currently only PNG supported");

        Image img = ImageReader::read_png(inputPath, Mode::GRAY);

        imgutils::print_info(img);

        // --------------------------------------------
        // Apply transforms
        // --------------------------------------------
        std::cout << "\nApplying transforms...\n";

        Image invImg = Transforms::apply_transform(img, Intensity::invert);

        Image logImg = Transforms::apply_transform(
            img,
            [](auto px) { return Intensity::log(px, 30); }
        );

        Image gammaImg = Transforms::apply_transform(
            img,
            [](auto px) { return Intensity::gamma(px, 0.5); }
        );

        Image thsImg = Transforms::apply_transform(
            img,
            [](auto px) { return Intensity::threshold(px, 128, 255); }
        );

        // --------------------------------------------
        // Save outputs
        // --------------------------------------------
        std::cout << "Saving images...\n";

        ImageWriter::write_png((fs::path(outputDir) / "invert.png").string(), invImg);
        ImageWriter::write_png((fs::path(outputDir) / "log.png").string(), logImg);
        ImageWriter::write_png((fs::path(outputDir) / "gamma.png").string(), gammaImg);
        ImageWriter::write_png((fs::path(outputDir) / "threshold.png").string(), thsImg);

        std::cout << "Done! Outputs saved in: " << outputDir << "\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}