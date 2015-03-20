#ifndef INPUT_IMAGE_HPP
#define INPUT_IMAGE_HPP

/*! \class InputImage
    \brief Input image slicer and verifier.

    Verifies that file under given path is correct and create
    series of fair-accessed regions that would be given to threads.
*/
class InputImage
{
    protected:
        std::vector<std::vector<QRect>> rects;

    public:
        /*!
         * \brief Constructor.
         */
        InputImage(const QString& path, std::size_t memUsage, int threadsCount);

    public:
        /*!
         * \brief Get series of regions for thread
         * \param index Thread index
         * \return Series of regions for thread.
         */
        const std::vector<QRect>& getThreadRects(int index) const;

        /*!
         * \brief Check if input image has valid data
         * \return Validity state.
         */
        bool valid(void) const;
};

#endif // INPUT_IMAGE_HPP
