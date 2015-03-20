#include <QtGui/QImageReader>

#include <iostream>

#include "input_image.hpp"

InputImage :: InputImage(const QString& path, std::size_t memUsage, int threadsCount) : rects(threadsCount)
{
    QImageReader reader(path);
    if(reader.size().isValid() && reader.imageFormat() == QImage::Format_RGB32)
    {
        // Calc slicing parameters
        const std::size_t maxPixels = memUsage * 1024 * 1024 / 4;   // Assume we work only with 32-bit pixels
        const std::size_t maxChunkSize = maxPixels / threadsCount;
        const std::size_t imageSize = reader.size().height() * reader.size().width();

        auto chunksEval = (imageSize / maxChunkSize);
        int chunksInGroup = chunksEval ? (chunksEval - 1) / (threadsCount * 2) + 1 : 1;
        auto chunksTotal = chunksInGroup * threadsCount * 2;
        auto linesInChunk = reader.size().height() / chunksTotal;
        auto linesRemnant = reader.size().height() % chunksTotal;

        std::cout << "image: " << reader.size().width()  << "x" << reader.size().height() << std::endl;
        std::cout << "imageSize: " << imageSize << std::endl;
        std::cout << "chunksInGroup: " << chunksInGroup << std::endl;
        std::cout << "chunksTotal: " << chunksTotal << std::endl;
        std::cout << "chunksTotal: " << chunksTotal << std::endl;
        std::cout << "linesInChunk: " << linesInChunk << std::endl;
        std::cout << "linesRemnant: " << linesRemnant << std::endl;
        std::cout << "chunkSize: " << linesInChunk * reader.size().width() * 4 << std::endl;
        std::cout << "memSize: " << linesInChunk * reader.size().width() * 4 * threadsCount << std::endl;

        // Slice upper half
        auto lineFrom = 0;
        auto linesAdd = 0;
        for(auto x=0; x<threadsCount; x++)
        {
            for(auto y=0; y<chunksInGroup; y++)
            {
                linesAdd += linesRemnant;
                auto lineTo = lineFrom + linesInChunk - 1 + linesAdd / chunksTotal;
                linesAdd %= chunksTotal;
                rects[x].push_back(QRect(0, lineFrom, reader.size().width(), lineTo - lineFrom + 1));
                std::cout << "Thread: " << x << ", from: " << lineFrom << ", to: " << lineTo << ", total: " << lineTo - lineFrom + 1 << std::endl;
                lineFrom = lineTo + 1;
            }
        }

        // Slice down half vice-versa
        for(auto x=threadsCount-1; x>=0; x--)
        {
            for(auto y=0; y<chunksInGroup; y++)
            {
                linesAdd += linesRemnant;
                auto lineTo = lineFrom + linesInChunk - 1 + linesAdd / chunksTotal;
                linesAdd %= chunksTotal;
                rects[x].push_back(QRect(0, lineFrom, reader.size().width(), lineTo - lineFrom + 1));
                std::cout << "Thread: " << x << ", from: " << lineFrom << ", to: " << lineTo << ", total: " << lineTo - lineFrom + 1 << std::endl;
                lineFrom = lineTo + 1;
            }
        }
    }
}

const std::vector<QRect>& InputImage :: getThreadRects(int index) const
{
    return rects.at(index);
}

bool InputImage :: valid(void) const
{
    return !rects[0].empty();
}
