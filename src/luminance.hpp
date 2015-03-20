#ifndef LUMINANCE_HPP
#define LUMINANCE_HPP

#include <QtGui/QImage>
#include <QtGui/QImageReader>

/*! \struct LuminanceFromImage
    \brief Input image to luminance counter.

    Takes a series of regions and successively
    tranforms an image data for each region
    to luminosity "spectrum" data, which
    may be drawed as histogram.
    This functor is executed in parallel threads.
*/
struct LuminanceFromImage
{
    /*!
     * \brief Functor's overloaded operator().
     * \param inputFile Path to the input file.
     * \param rects Series of regions for thread.
     * \param index Thread index.
     * \return "spectrum" luminosity data.
     */
    HistogramData operator()(const QString& inputFile, const std::vector<QRect>& rects, int index)
    {
        QImage image;
        HistogramData hData(histogramSize, 0);
        auto start = std::chrono::high_resolution_clock::now();

        std::for_each(rects.begin(), rects.end(), [&](const QRect& rect)
        {
            QImageReader reader(inputFile);
            reader.setClipRect(rect);
            if(!reader.read(&image))
            {
                std::cerr << "Thread: " << index << "Image reader failure - " << reader.errorString().toStdString() << std::endl;
                return;
            }

            auto pixels = reinterpret_cast<const QRgb*>(image.constBits());
            auto pixelsSize = image.size().height() * image.size().width();
            for(auto x=0; x<pixelsSize; x++)
            {
                QRgb pixel = pixels[x];
                // Using one of many possible formulas
                int index = static_cast<int>
                            (
                                static_cast<float>(qRed(pixel)) * 0.2126f +
                                static_cast<float>(qGreen(pixel)) * 0.7152f +
                                static_cast<float>(qBlue(pixel)) * 0.0722f
                            );
                hData[index]++;
            };

            return;
        });

        auto stop = std::chrono::high_resolution_clock::now();
        auto totalMks = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
        std::cout << "Thread: " << index << " Total: " << totalMks << " mks" << std::endl;

        return hData;
    }
};

#endif // LUMINANCE_HPP
