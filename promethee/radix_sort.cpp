#include "radix_sort.h"
#include "types.h"

#include <algorithm>
#include <cstring>
#include <vector>

using namespace std;
using radix_sort::PixelData;
using types::ldouble;

// Required number of passes to go through each byte of the type double (64 bits = 8 bytes).
const int NUM_PASSES = sizeof(ldouble);

// The radix (base): 256 because we process 1 byte at a time.
const int RADIX = 256;

// Mask to extract the byte we want to analyze (or sort)
const int BYTE_MASK = 0xFF;

// Mask to extract the MSB and to check if the number is positive or negative
const uint64_t MSB_MASK = 1ULL << 63;

// Number of bits we need to shift to go to the next byte (1 << BITS_PER_BYTE_SHIFT = 8)
const int BITS_PER_BYTE_SHIFT = 3;

// Converts a double (ldouble) to a byte
inline uint64_t doubleToByte(ldouble num) {
    uint64_t byt;
    memcpy(&byt, &num, sizeof(ldouble));

    return (byt & MSB_MASK) ? ~byt : byt ^ MSB_MASK;
}

inline ldouble getTargetData(PixelData &data, bool sort_by_first) {
    return sort_by_first ? data.first : data.second;
}

void radix_sort::radixSort(vector<PixelData> &pixels, bool sort_by_first) {
    int n = (int) pixels.size();
    if (n < 2) {
        return;
    }

    // Temporary buffer to store sorted bytes
    vector<PixelData> temp_buffer(pixels.size());
        
    vector<PixelData> *src = &pixels;
    vector<PixelData> *dest = &temp_buffer;

    for (int pass = 0; pass < NUM_PASSES; pass++) {
        // Compute frequency of each element
        size_t count[RADIX];
        memset(count, 0, sizeof(count)) 
        for (int i = 0; i < n; i++) {
            uint64_t byte_val = doubleToByte(getTargetData((*src)[i], sort_by_first));
            int byte_idx = (byte_val >> (pass << BITS_PER_BYTE_SHIFT)) & BYTE_MASK;
            count[byte_idx]++;
        }

        // Compute offset
        for (int i = 1; i < RADIX; i++) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            uint64_t byte_val = doubleToByte(getTargetData((*src)[i], sort_by_first));
                
            int byte_idx = (byte_val >> (pass << BITS_PER_BYTE_SHIFT)) & BYTE_MASK;
            int pos = --count[byte_idx];
            (*dest)[pos] = (*src)[i];
        }

        swap(src, dest);
    }

    if (src != &pixels) {
        pixels = temp_buffer;
    }
}