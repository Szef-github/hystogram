#include <QtCore/QString>
#include <QtGui/QApplication>
#include <QtGui/QImage>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <future>
#include <thread>
#include <utility>

#include "hist_data.hpp"
#include "hist_image.hpp"
#include "luminance.hpp"
#include "params.hpp"

int main(int argc, char *argv[])
{
    // One should always create QApplication, even if it's not used directly
    QApplication a(argc, argv);
    Q_UNUSED(a);

    // Parse input params
    CmdLineParams params(std::thread::hardware_concurrency());
    int rc = params.update();
    if(rc <= 0)
        return -rc;

    // Create input QImage to get pixel data from
    QImage inputImage(params.inputFile);
    if(inputImage.isNull() || inputImage.format() != QImage::Format_RGB32)
    {
        std::cerr << "Invalid input image or unrecognized format" << std::endl;
        return 1;
    }
    auto pixels = reinterpret_cast<const QRgb*>(inputImage.constBits());
    auto pixelsSize = inputImage.size().height() * inputImage.size().width();
    auto blockSize = pixelsSize / params.threadsCount;

    // Timer start
    auto start = std::chrono::high_resolution_clock::now();

    // Preparing for threading
    typedef std::future<HistogramData> AsuncResult;
    std::list<AsuncResult> results;
    for(auto i=0; i<params.threadsCount; i++)
    {
        // Each thread get its own block of pixels
        auto from = pixels + i * blockSize;
        auto to = (i < params.threadsCount - 1) ? (pixels + (i + 1) * blockSize) : (pixels + pixelsSize);
        // Start threads
        results.push_back(std::forward<AsuncResult> (std::async(std::launch::async, LuminanceFromRGB(), std::vector<QRgb>(from, to))));
    }

    // Obtain individual thread results and accumulate them
    HistogramData accData(histogramSize, 0);
    std::for_each(results.begin(), results.end(), [&accData](AsuncResult& result)
    {
        accData += result.get();
        return;
    });

    // Timer stop
    auto stop = std::chrono::high_resolution_clock::now();
    auto mks = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
    std::cout << "Done by " << params.threadsCount << " threads in " << mks << " mks" << std::endl;

    // Draw output image
    HistogramImage outputImage;
    outputImage.draw(accData);

    // Save output image to a file
    if(!outputImage.save(params.outputFile))
    {
        std::cerr << "Failed to save output file" << std::endl;
        return 1;
    }

    return 0;
}
