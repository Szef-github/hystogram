#ifndef HIST_IMAGE_HPP
#define HIST_IMAGE_HPP

/*! \class HistogramImage
    \brief Output histogram drqwing image class.

    Draws a colorful histogram from supplyed data,
    using subclassed QImage properties.
*/
class HistogramImage : public QImage
{
    protected:
        //! Individual bar width in pixels.
        static const std::size_t barWidth = 3;
        //! Maximum bar height in pixels.
        static const std::size_t barHeight = 400;
        //! Left indentation area in pixels.
        static const std::size_t leftIndent = 55;
        //! Right indentation area in pixels.
        static const std::size_t rightIndent = 15;
        //! Top indentation area in pixels.
        static const std::size_t topIndent = 30;
        //! Bottom indentation area in pixels.
        static const std::size_t bottomIndent = 35;

    public:
        /*!
         * \brief Default constructor.
         */
        HistogramImage(void);

    public:
        /*!
         * \brief Draw on QImage from supplyed histogram data
         * \param data Histogram data
         */
        void draw(const HistogramData& data);
};

#endif // HIST_IMAGE_HPP
