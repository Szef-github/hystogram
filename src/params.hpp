#ifndef PARAMS_HPP
#define PARAMS_HPP

/*! \class CmdLineParams
    \brief Command line params parser.

    Takes a command line parameters one by one and
    translates to own members, verifying that they
    are correct.
*/
class CmdLineParams
{
    protected:
        //! Maximum number of parallel threads.
        static constexpr int threadsMax = 32;

    public:
        //! Input file name.
        QString inputFile;
        //! Output file name.
        QString outputFile;
        //! Parallel threads count.
        int threadsCount;

    public:
        /*!
         * \brief Constructor with default value for threads count supplyed.
         * \param defaultThreadsCount default threads count.
         */
        CmdLineParams(int defaultThreadsCount);

    protected:
        void printUsage(void);

    public:
        /*!
         * \brief Update public parameters from command line
         * \return -1 in case of error, 0 in case of normal exit, 1 in case of success
         */
        int update(void);
};

#endif // PARAMS_HPP
