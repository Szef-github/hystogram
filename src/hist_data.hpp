#ifndef HIST_DATA_HPP
#define HIST_DATA_HPP

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <vector>

/*! \file hist_data.hpp
    \brief Histogramm data module
*/

/*! \var std::size_t histogramSize
    \brief Constant for histogram size.
*/
constexpr std::size_t histogramSize = 256;

/*! \var typedef std::vector<std::size_t> HistogramData
    \brief Using typedefed structure for convinience.

    HistogramData is actually a vector of counters, that count every pixel
    having appropriate luminosity. Should be always have size=histogramSize
*/
typedef std::vector<std::size_t> HistogramData;

/*! \fn template <typename T> std::vector<T>& operator+=(std::vector<T>& h1, const std::vector<T>& h2)
    \brief Overloads addition with assignement operator.
    \param h1 First vector and assignee.
    \param h2 Second vector.

    This overload operator performs per-element += operation over two vectors,
    i.e. h1[i] += h2[i]
*/
template <typename T>
std::vector<T>& operator+=(std::vector<T>& h1, const std::vector<T>& h2)
{
    assert(h1.size() == h2.size());
    std::transform(h1.begin(), h1.end(), h2.begin(), h1.begin(), std::plus<T>());
    return h1;
}

#endif // HIST_DATA_HPP
