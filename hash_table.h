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

// Include once.
#ifndef ABSTRACT_CONTAINER_HASH_TABLE_H_
#define ABSTRACT_CONTAINER_HASH_TABLE_H_

#include "list.h"

namespace abstract_container
{

// Abstract hash table template.
//
// abstractor parameter class must have these public members, or equivalents:
//
// Types:
//
// list -- normally an instantiation of the abstract_container::list type.
//   Must have the members handle, start, push, remove, link, null, purge.
//   Must have a paremeterless constructor that initializes it to the empty
//   state.
// index -- an integral type.
// key -- some copyable type.
//
// Member functions:
//
// index hash_key(key) -- returns the hash value of the given key.
// index hash_elem(handle) -- returns hash value of the key of the list
//   element associated with the given handle.  Each list element placed into
//   the hash table must be associated with a unique handle value.  Each
//   list element place into the hash table must be associated with a unique
//   key value.
// list & bucket(index) -- returns the list to use as a bucket to store
//   each element in the table with the given hash value.
// bool is_key(index, handle) -- returns true if the first parameter is
//   the key of the element whose handle is the second parameter.
//
// Static constants:
//
// static const index num_hash_values -- the maximum number of hash values
//   of keys (with zero being the minimum).
//
template <class abstractor>
class hash_table : protected abstractor
  {
  protected:

    typedef typename abstractor::list list;
    typedef typename abstractor::index index;

  public:

    typedef typename abstractor::key key;
    typedef typename list::handle handle;

    hash_table() = default;

    hash_table(const hash_table &) = delete;

    hash_table & operator = (const hash_table &) = delete;

    // It may be that you will want to search for a key, and if it is
    // not present, allocate a node for the key and insert it.  Hence
    // the encapsulation foul of being able to pass the hash value for
    // a key to some member functions, to avoid unecessary recalculation
    // of the hash value.

    index hash_key(key k) { return(abstractor::hash_key(k)); }

    index hash_elem(handle h) { return(abstractor::hash_elem(h)); }

    void insert(handle h, index hash_value)
      { bucket(hash_value).push(h); }

    void insert(handle h) { insert(h, hash_elem(h)); }

    // Returns null() if no element has key k.
    handle search(key k, index hash_value)
      {
        list &b = bucket(hash_value);
        handle h = b.start();

        while ((h != null()) and !is_key(k, h))
          h = b.link(h);

        return(h);
      }

    handle search(key k) { return(search(k, hash_key(k))); }

    // Returns the handle of the removed element, or null() is no element
    // has key k.
    handle remove_key(key k)
      {
        list &b = bucket(hash_key(k));
        handle h = b.start();
        handle h_last = null();

        while ((h != null()) and !is_key(k, h))
          {
            h_last = h;
            h = b.link(h);
          }

        if (h != null())
          {
            if (h_last == null())
              b.pop();
            else
              b.remove_forward(h_last);
          }

        return(h);
      }

    void remove(handle h) { bucket(hash_elem(h)).remove(h); }

    // Make the hash table empty.
    void purge()
      {
        for (index i = 0; i < num_hash_values; ++i)
          bucket(i).purge();
      }

    static handle null() { return(list::null()); }

    // Note:  removing an element invalidates iterators referencing it,
    // but no others.
    //
    class iter
      {
      public:

	void start_iter(hash_table &ht_)
	  {
	    ht = &ht_;

            hv = index(0) - 1;
            curr_h = hash_table::null();

            advance();
	  }

        iter(hash_table &ht_) { start_iter(ht_); }

        // Returns handle of element currently referenced by iterator, or
        // null() if the iterator is past the last element (if any).
        //
	handle operator * () { return(curr_h); }

	operator bool () { return(curr_h != hash_table::null()); }

	hash_table & table() { return(*ht); }

	void operator ++ () { advance(); }

	void operator ++ (int) { ++(*this); }

      protected:

	// Hash table being iterated over.
	hash_table *ht;

        // Hash value, current bucket.
        index hv;

        // Handle of current element.
        handle curr_h;

        void advance()
          {
            if (curr_h != hash_table::null())
              curr_h = ht->bucket(hv).link(curr_h);

            while (curr_h == hash_table::null())
              {
                if (++hv >= hash_table::num_hash_values)
                  break;

                curr_h = ht->bucket(hv).start();
              }
          }
      };

  protected:

    static const index num_hash_values = abstractor::num_hash_values;

    list & bucket(index hash_value) { return(abstractor::bucket(hash_value)); }

    bool is_key(key k, handle h) { return(abstractor::is_key(k, h)); }
  };

} // end namespace abstract_container

#endif /* Include once */
