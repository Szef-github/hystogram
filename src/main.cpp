#include <QtCore/QString>
#include <QtGui/QApplication>
#include <QtGui/QImageReader>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <future>
#include <thread>
#include <utility>

#include "input_image.hpp"
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
    CmdLineParams params(1024, std::thread::hardware_concurrency());
    int rc = params.update();
    if(rc <= 0)
        return -rc;

    // Create input image object to get region series from
    InputImage inputImage(params.inputFile, params.maxMem, params.threadsCount);
    if(!inputImage.valid())
    {
        std::cerr << "Invalid input image or unrecognized format" << std::endl;
        return 1;
    }

    // Start threads
    typedef std::future<HistogramData> AsyncResult;
    std::list<AsyncResult> results;
    for(auto x=0; x<params.threadsCount; x++)
        results.emplace_back(std::async(std::launch::async, LuminanceFromImage(),
                                        std::cref(params.inputFile),
                                        inputImage.getThreadRects(x), x));

    // Obtain individual thread results and accumulate them
    HistogramData accData(histogramSize, 0);
    std::for_each(results.begin(), results.end(), [&accData](AsyncResult& result)
    {
        accData += result.get();
        return;
    });

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
