/*
Copyright (c) 2016 Walter William Karas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

// Unit testing for modulus_hash.h .

#include "modulus_hash.h"
#include "modulus_hash.h"

#include <cstdlib>
#include <iostream>
#include <cstdint>

using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::cout;
using std::endl;
using abstract_container::modulus_hash;

struct Tr
  {
    typedef uint8_t modulus_t;
    typedef uint32_t product_t;

    static const unsigned key_segment_bits = 16;
    static const unsigned num_key_segments = 5;
    static const modulus_t modulus = 19;

    typedef const uint16_t *key;

    template<unsigned Key_segment> static uint16_t get_segment(key k)
      { return(k[Key_segment]); }
  };

const unsigned Num_keys = 1000;

const unsigned Min_segs = 3;

uint16_t data[Tr::num_key_segments];

unsigned hist[Tr::modulus];

#define LAST_ONLY 1

int main()
  {
    const uint16_t Gen = 9 * 9 * 9 * 9;
    uint16_t prod = Gen;

    for (unsigned i = 0; i < Num_keys; ++i)
      {
        for (unsigned j = 0; j < Tr::num_key_segments; ++j)
          {
            if ((j == (Tr::num_key_segments - 1)) or !LAST_ONLY)
              data[j] = prod;
            else
              data[j] = 0;
            prod *= Gen;
          }

        ++hist[modulus_hash<Tr>(data)];
      }

    unsigned ttl = 0;
    for (unsigned i = 0; i < Tr::modulus; ++i)
      {
        cout << hist[i] << endl;
        ttl += hist[i];
      }
    cout << ttl << endl;

    cout << "===================" << endl;

    prod = Gen;

    for (unsigned i = 0; i < Tr::modulus; ++i)
      hist[i] = 0;

    unsigned seg_count = Min_segs;

    for (unsigned i = 0; i < Num_keys; ++i)
      {
        for (unsigned j = 0; j < seg_count; ++j)
          {
            if ((j == (seg_count - 1)) or !LAST_ONLY)
              data[j] = prod;
            else
              data[j] = 0;
            prod *= Gen;
          }

        ++hist[modulus_hash<Tr>(data, seg_count)];

        if (seg_count == Tr::num_key_segments)
          seg_count = Min_segs;
        else
          ++seg_count;
      }

    ttl = 0;
    for (unsigned i = 0; i < Tr::modulus; ++i)
      {
        cout << hist[i] << endl;
        ttl += hist[i];
      }
    cout << ttl << endl;

    return(0);
  }
