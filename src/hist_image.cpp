#include <QtCore/QRect>
#include <QtGui/QImage>
#include <QtGui/QFontMetrics>
#include <QtGui/QPainter>

#include <cmath>
#include <iostream>

#include "hist_data.hpp"
#include "hist_image.hpp"

HistogramImage :: HistogramImage(void) : QImage((barWidth + 1) * histogramSize + leftIndent + rightIndent,
                                                barHeight + topIndent + bottomIndent,
                                                QImage::Format_ARGB32)
{
    QImage::fill(Qt::white);
}

void HistogramImage :: draw(const HistogramData& data)
{
    QPainter p(this);

    std::size_t maxCount = *std::max_element(data.begin(), data.end());
    std::size_t normVal = maxCount + topIndent;

    // Calc step
    std::size_t step = 1;
    if(normVal > 5)
    {
        std::size_t firstDigit = normVal / 5;
        std::size_t power = 1;
        while(firstDigit > 9)
        {
            power *= 10;
            firstDigit /= 10;
        }

        switch(firstDigit)
        {
            case 1: step = 2 * power; break;
            case 2:
            case 3:
            case 4: step = 5 * power; break;
            default: step = 10 * power; break;
        }
    }

    // Generate levels
    std::vector<std::size_t> levels(normVal / step, 0);
    std::size_t n(0);
    std::generate(levels.begin(), levels.end(), [&](void){ return step * ++n; });

    // Draw levels
    p.setPen(QColor(0xE0, 0xE0, 0xE0));
    std::for_each(levels.begin(), levels.end(), [&](std::size_t level)
    {
        p.drawLine(leftIndent,
                   topIndent + barHeight - barHeight * level / normVal,
                   width() - rightIndent,
                   topIndent + barHeight - barHeight * level / normVal);
        return;
    });

    // Draw level marks
    QFont f("times", 7);
    QFontMetrics fm(f);

    p.setPen(Qt::black);
    p.setFont(f);
    std::for_each(levels.begin(), levels.end(), [&](std::size_t level)
    {
        QString str = QString :: number(level);
        QRect rect = fm.boundingRect(QString :: number(level));
        rect.moveTo(leftIndent - rect.width() - 3,
                    topIndent + barHeight - barHeight * level / normVal - rect.height() / 2);
        // QFontMetrics eat a pixel for unknown reason
        rect.setLeft(rect.x() - 1);
        p.drawText(rect, Qt::AlignRight, str);
        return;
    });

    // Generate luminas
    std::vector<std::size_t> luminas(6);
    n = 0;
    std::generate(luminas.begin(), luminas.end(), [&](void){ return 50 * n++; });

    // Draw lumina marks
    std::for_each(luminas.begin(), luminas.end(), [&](std::size_t lumina)
    {
        QString str = QString :: number(lumina);
        QRect rect = fm.boundingRect(QString :: number(lumina));
        // QFontMetrics eat a pixel for unknown reason
        rect.setLeft(rect.x() - 1);
        rect.moveTo(leftIndent + (barWidth + 4) / 2 - rect.width() / 2 + lumina * (barWidth + 1),
                    topIndent + barHeight + 5);

        p.drawText(rect, Qt::AlignHCenter, str);
        return;
    });

    // Draw bars
    p.setPen(Qt::red);
    p.setBrush(Qt::red);
    for(std::size_t i=0; i<histogramSize; i++)
    {
        std::size_t barNormHeight = barHeight * data[i] / normVal;
        p.drawRect(leftIndent + 2 + i * (barWidth + 1),
                   topIndent + barHeight - barNormHeight,
                   barWidth - 1,
                   barNormHeight);
    }

    // Draw axes
    p.setPen(Qt::black);
    p.drawLine(leftIndent, barHeight + topIndent, width() - rightIndent, barHeight + topIndent);
    p.drawLine(leftIndent, topIndent, leftIndent, barHeight + topIndent);
}
