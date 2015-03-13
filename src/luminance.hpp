#ifndef LUMNANCE_HPP
#define LUMINANCE_HPP

/*! \struct LuminanceFromRGB
    \brief RGB to Luminance functor helper.

    Takes a vector of RGB pixels and tranforms it
    to luminosity "spectrum" data, which may be drawed
    as histogram.
    This functor is executed in parallel threads.
*/
struct LuminanceFromRGB
{
    /*!
     * \brief Functor's overloaded operator().
     * \param rgb constant vector of rgb pixels.
     * \return "spectrum" luminosity data.
     */
    HistogramData operator()(const std::vector<QRgb>& rgb)
    {
        HistogramData hData(histogramSize, 0);
        std::for_each(rgb.begin(), rgb.end(), [&hData](QRgb pixel)
        {
            // Using one of many possible formulas
            int index = static_cast<int>
                        (
                            static_cast<float>(qRed(pixel)) * 0.2126f +
                            static_cast<float>(qGreen(pixel)) * 0.7152f +
                            static_cast<float>(qBlue(pixel)) * 0.0722f
                        );
            hData[index]++;
            return;
        });

        return hData;
    }
};

#endif // LUMINANCE_HPP
