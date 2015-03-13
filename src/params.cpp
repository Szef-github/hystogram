#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include <iostream>

#include "params.hpp"

CmdLineParams :: CmdLineParams(int defaultThreadsCount) : threadsCount(defaultThreadsCount)
{
}

void CmdLineParams :: printUsage(void)
{
    std::cerr << "Usage: hystogram [options] <input_file> <output_file>" << std::endl <<
                 std::endl <<
                 "Options:" << std::endl <<
                 "\t -h, --help\t Print this help" << std::endl <<
                 "\t -n N\t\t Set thread count to N [1-32] (default is system dependent)" << std::endl;
}

int CmdLineParams :: update(void)
{
    // Parse parameters one-by-one
    QString appName;
    QString threadsString;
    QString* option = nullptr;
    foreach(QString arg, QCoreApplication :: arguments())
    {
        if(option != nullptr)
        {
            *option = arg;
            option = nullptr;
        }
        else if((arg == "-h") || (arg == "--help"))
        {
            printUsage();
            return 0;
        }
        else if(arg == "-n")
            option = &threadsString;
        else if(appName.isEmpty())
            appName = arg;
        else if(inputFile.isEmpty())
            inputFile = arg;
        else if(outputFile.isEmpty())
            outputFile = arg;
        else
        {
            std::cerr << "Too many options" << std::endl;
            printUsage();
            return -1;
        }
    }

    // Check if threads count is correct
    if(!threadsString.isEmpty())
    {
        bool ok;
        int nThreads = threadsString.toInt(&ok);
        if(!ok || (nThreads < 1) || (nThreads > threadsMax))
        {
            std::cerr << "Invalid thread count (" << threadsString.toLocal8Bit().data() << ")" << std::endl;
            printUsage();
            return -1;
        }

        threadsCount = nThreads;
    }

    // Check if input and output filename supplied and correct
    if(inputFile.isEmpty())
    {
        std::cerr << "No input filename" << std::endl;
        printUsage();
        return -1;
    }

    if(outputFile.isEmpty())
    {
        std::cerr << "No output file" << std::endl;
        printUsage();
        return -1;
    }

    if(!QFile::exists(inputFile))
    {
        std::cerr << "Input file (" << inputFile.toLocal8Bit().data() << ") doesn't exist" << std::endl;
        printUsage();
        return -1;
    }

    return 1;
}

