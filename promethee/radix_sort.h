// radix_sort.h

#ifndef RADIX_SORT_H
#define RADIX_SORT_H

#include "types.h"

#include <utility>
#include <vector>

namespace radix_sort {

    using types::ldouble;

    // Represents a pixel: .first is the intensity value, .second
    // is the original index or position. 
    using PixelData = std::pair<ldouble, ldouble>;

    // This function uses an optimized Radix Sort to sort the image pixels
    // according to their values. If `sort_by_first` is true, then it sorts
    // by the first value, otherwise it sorts by the second value of the PixelData.
    void radixSort(std::vector<PixelData> &pixels, bool sort_by_first = true);

}   // namespace radix_sort

#endif