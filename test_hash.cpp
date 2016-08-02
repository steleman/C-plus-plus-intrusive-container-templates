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

// Unit testing for hash_table.h .

#include "hash_table.h"
#include "hash_table.h"

#include "list.h"

// Put a breakpoint on this function to break after a check fails.
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

struct Elem { int key; Elem *link; };

Elem e[10];

class Abs
  {
  private:

    struct List_abs
      {
        static const bool store_tail = false;
        typedef Elem *handle;
        static handle null() { return(nullptr); }
        static handle link(handle h) { return(h->link); }
        static void link(handle h, handle link_h) { h->link = link_h; }
      };

  protected:

    typedef abstract_container::list<List_abs> list;
    typedef unsigned index;

    static const index num_hash_values = 10;

  private:

    list table[num_hash_values];

  protected:

    typedef int key;

    list & bucket(index hash_value) { return(table[hash_value]); }

    bool is_key(key k, list::handle h) { return(h->key == k); }
  };

struct Ht : public hash_table<Abs>
  {
  };

Ht ht;

#if 0

// Purge test list, mark all elements as detached.
//
void init()
  {
    lst.purge();

    for (unsigned i = 0; i < num_e; ++i)
      lst.make_detached(e + i);
  }

// Check if list structure is sane, and elements of list are in ascending
// order by address.
//
void scan()
  {
    elem_t *last = nullptr;

    for (unsigned i = 0; i < num_e; ++i)
      if (!lst.is_detached(e + i))
        {
          #if BIDIR
          CHK(lst.link(e + i, reverse) == last);
          #endif
          if (last)
            CHK(lst.link(last) == (e + i));
          else
            CHK(lst.start() == (e + i));
          last = e + i;
        }

    CHK(lst.start(reverse) == last);
    CHK(lst.empty() == (last == nullptr));
    if (last)
      CHK(lst.link(last) == nullptr); 
    else
      CHK(lst.start() == nullptr);
  }

#define SCAN { std::cout << "SCAN line " << __LINE__ << std::endl; scan(); }

#endif

int main()
  {

    return(0);
  }
