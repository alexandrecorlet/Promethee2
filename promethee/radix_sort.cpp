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

namespace radix_sort {

    // Converts a double (ldouble) to a byte
    inline uint64_t doubleToByte(ldouble num) {
        uint64_t byt;
        memcpy(&byt, &num, sizeof(ldouble));

        return (byt & MSB_MASK) ? ~byt : byt ^ MSB_MASK;
    }

    inline ldouble getTargetData(PixelData &data, bool sort_by_first) {
        return sort_by_first ? data.first : data.second;
    }

    void radixSort(vector<PixelData> &pixels, bool sort_by_first) {
        size_t n = (int) pixels.size();
        if (n < 2) {
            return;
        }

        size_t count[NUM_PASSES][RADIX];
        for (int i = 0; i < NUM_PASSES; i++) {
            fill(count[i], count[i] + RADIX, 0);
        }

        // Compute the frequency of each element. This
        // is used to compute the offset later
        for (int i = 0; i < n; i++) {
            uint64_t byte_val = doubleToByte(getTargetData(pixels[i], sort_by_first));
            
            for (int pass = 0; pass < NUM_PASSES; pass++) {
                int byte_idx = (byte_val >> (pass << BITS_PER_BYTE_SHIFT)) & BYTE_MASK;
                count[pass][byte_idx]++;
            }
        }

        // Temporary buffer to store sorted bytes
        vector<PixelData> temp_buffer(pixels.size());
        
        vector<PixelData> *src = &pixels;
        vector<PixelData> *dest = &temp_buffer;

        for (int pass = 0; pass < NUM_PASSES; pass++) {
            // Check if this pass is skipable. A pass is skipable if all
            // elements in the bucket are the same.
            bool can_skip = false;
            for (int i = 0; i < RADIX; i++) {
                if (count[pass][i] == n) {
                    can_skip = true;
                }

                if (count[pass][i] > 0) {
                    break;
                }
            }

            if (can_skip) {
                continue;
            }

            // Compute offset
            for (int i = 1; i < RADIX; i++) {
                count[pass][i] += count[pass][i - 1];
            }

            for (int i = n - 1; i >= 0; i--) {
                uint64_t byte_val = doubleToByte(getTargetData((*src)[i], sort_by_first));
                
                int byte_idx = (byte_val >> (pass << BITS_PER_BYTE_SHIFT)) & BYTE_MASK;
                int pos = --count[pass][byte_idx];
                (*dest)[pos] = (*src)[i];
            }

            swap(src, dest);
        }

        if (src != &pixels) {
            pixels = temp_buffer;
        }
    }

} // namespace radix_sort