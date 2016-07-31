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

#include "bidir_list.h"
#include "bidir_list.h"

void bp() { }

#include <cstdlib>
#include <iostream>

void check(bool expr, int line)
  {
    if (!expr)
      {
        std::cout << "*** fail line " << line << std::endl;
        bp();
        std::exit(1);
      }
  }

#define CHK(EXPR) check((EXPR), __LINE__)

using namespace abstract_container;

p_bidir_list bdl;

const unsigned num_e = 5;

p_bidir_elem e[num_e];

void init()
  {
    bdl.purge();

    for (unsigned i = 0; i < num_e; ++i)
      bdl.make_detached(e + i);
  }

void scan()
  {
    p_bidir_elem *last = nullptr;

    for (unsigned i = 0; i < num_e; ++i)
      if (!bdl.is_detached(e + i))
        {
          CHK(bdl.link(e + i, reverse) == last);
          if (last)
            CHK(bdl.link(last) == (e + i));
          else
            CHK(bdl.start() == (e + i));
          last = e + i;
        }

    CHK(bdl.start(reverse) == last);
    CHK(bdl.empty() == (last == nullptr));
    if (last)
      CHK(bdl.link(last) == nullptr); 
    else
      CHK(bdl.start() == nullptr);
  }

#define SCAN { std::cout << "SCAN line " << __LINE__ << std::endl; scan(); }

int main()
  {
    CHK(bdl.empty());

    init(); SCAN

    bdl.push(e + 2); SCAN
    bdl.insert(e + 2, e + 4); SCAN
    bdl.insert(e + 2, e + 0, reverse); SCAN
    bdl.insert(e + 2, e + 3); SCAN
    bdl.insert(e + 2, e + 1, reverse); SCAN

    bdl.erase(e + 2); bdl.make_detached(e + 2); SCAN
    bdl.erase(e + 0); bdl.make_detached(e + 0); SCAN
    bdl.erase(e + 4); bdl.make_detached(e + 4); SCAN
    bdl.erase(e + 3); bdl.make_detached(e + 3); SCAN
    bdl.erase(e + 1); bdl.make_detached(e + 1); SCAN

    CHK(bdl.empty());

    bdl.push(e + 2); SCAN
    bdl.pop(); bdl.make_detached(e + 2); SCAN
    bdl.push(e + 2, reverse); SCAN
    bdl.pop(reverse); bdl.make_detached(e + 2); SCAN
    bdl.push(e + 2); SCAN
    bdl.push(e + 1); SCAN
    bdl.push(e + 3, reverse); SCAN
    bdl.pop(reverse); bdl.make_detached(e + 3); SCAN
    bdl.pop(); bdl.make_detached(e + 1); SCAN

    bdl.purge();
    CHK(bdl.empty());

    return(0);
  }
