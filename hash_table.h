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

namespace abstract_container
{

// Abstract hash table template.
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

    inline index hash_key(key k) { return(abstractor::hash_key(k)); }

    inline index hash_elem(handle h) { return(abstractor::hash_elem(h)); }

    inline handle insert(handle h, index hash_value)
      { bucket(hash_value).push(h); }

    handle insert(handle h) { return(insert(h, hash_elem(h))); }

    inline handle search(key k, index hash_value)
      {
        list &b = bucket(hash_value);
        handle h = b.start();

        while ((h != null()) and !is_key(k, h))
          h = b.link(h);

        return(h);
      }

    handle search(key k) { return(search(k, hash_key(k))); }

    inline handle remove_key(key k)
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

    inline void remove(handle h) { bucket(hash_elem(h)).remove(h); }

    inline void purge(void)
      {
        for (index i = 0; i < num_hash_values; ++i)
          bucket(i).purge();
      }

    handle null(void) { return(list::null()); }

#if 0
    class iter
      {
      public:

	void start_iter(hash_table &ht, key k)
	  {
	    // Mask of high bit in an int.
	    const int MASK_HIGH_BIT = (int) ~ ((~ (unsigned) 0) >> 1);

	    // Save the tree that we're going to iterate through in a
	    // member variable.
	    tree_ = &tree;

	    int cmp, target_cmp;
	    handle h = tree_->abs.root;
	    unsigned d = 0;

	    depth = unsigned(~0);

	    if (h == null())
	      // Tree is empty.
	      return;

	    if (st & LESS)
	      // Key can be greater than key of starting node.
	      target_cmp = 1;
	    else if (st & GREATER)
	      // Key can be less than key of starting node.
	      target_cmp = -1;
	    else
	      // Key must be same as key of starting node.
	      target_cmp = 0;

	    for ( ; ; )
	      {
		cmp = cmp_k_n(k, h);
		if (cmp == 0)
		  {
		    if (st & EQUAL)
		      {
			// Equal node was sought and found as starting node.
			depth = d;
			break;
		      }
		    cmp = -target_cmp;
		  }
		else if (target_cmp != 0)
		  if (!((cmp ^ target_cmp) & MASK_HIGH_BIT))
		    // cmp and target_cmp are both negative or both positive.
		    depth = d;
		h = cmp < 0 ? get_lt(h) : get_gt(h);
		if (read_error())
		  {
		    depth = unsigned(~0);
		    break;
		  }
		if (h == null())
		  break;
		branch[d] = cmp > 0;
		path_h[d++] = h;
	      }
	  }

	handle operator * (void)
	  {
	  }

	void operator ++ (void)

	void operator ++ (int) { ++(*this); }

      protected:

	// Hash table being iterated over.
	hash_table *ht_;

      };
#endif

  protected:

    static const index num_hash_values = abstractor::num_hash_values;

    list & bucket(index hash_value) { return(abstractor::bucket(hash_value)); }

    bool is_key(key k, handle h) { return(abstractor::is_key(k, h)); }
  };

} // end namespace abstract_container

#endif /* Include once */
