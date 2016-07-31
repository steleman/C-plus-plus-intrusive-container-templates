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
#ifndef ABSTRACT_CONTAINER_BIDIR_LIST_H_
#define ABSTRACT_CONTAINER_BIDIR_LIST_H_

namespace abstract_container
{

constexpr bool forward = true;
constexpr bool reverse = false;

template <class abstractor>
class bidir_list
  {
  public:

    typedef typename abstractor::handle handle;

    handle null() { return(abstractor::null()); }

    bidir_list() : head{ null(), null() } { }

    bidir_list(const bidir_list &) = delete;

    bidir_list & operator = (const bidir_list &) = delete;

    handle link(handle h, bool is_forward = true)
     { return(abstractor::link(h, is_forward)); }

    void make_detached(handle h) { link(h, h, forward); }

    bool is_detached(handle h) { return(link(h, forward) == h); }

    // FUTURE
    // bidir_list(
    //   bidir_list &to_split, handle first_in_list, handle last_in_list)

    handle start(bool is_forward = true) { return(head[is_forward]); }

    void insert(handle in_list, handle to_insert, bool is_forward = true)
      {
        handle ilf = link(in_list, is_forward);
        link(to_insert, ilf, is_forward);
        link(to_insert, in_list, !is_forward);
        if (ilf == null())
          // New head in reverse direction.
          head[!is_forward] = to_insert;
        else
          link(ilf, to_insert, !is_forward);
        link(in_list, to_insert, is_forward);
      }

    #if 0
    // FUTURE
    void insert(handle in_list, bidir_list &to_insert, bool is_forward = true)
      {
        if (to_insert.head[forward] == null())
          return;
        ...
      }
    #endif

    void erase(handle in_list)
      {
        handle f = link(in_list, forward);
        handle r = link(in_list, reverse);

        if (head[forward] == in_list)
          head[forward] = f;

        if (head[reverse] == in_list)
          head[reverse] = r;

        if (f != null())
          link(f, r, reverse);

        if (r != null())
          link(r, f, forward);
      }

    // FUTURE
    // void erase(handle first_in_list, handle last_in_list))

    void push(handle to_push, bool is_forward = true)
      {
        link(to_push, null(), !is_forward);
        link(to_push, head[is_forward], is_forward);

        if (head[is_forward] != null())
          link(head[is_forward], to_push, !is_forward);
        else
          head[!is_forward] = to_push;

        head[is_forward] = to_push;
      }

    // FUTURE
    // void push(bidir_list &to_push, bool is_forward = true)

    handle pop(bool is_forward = true)
      {
        handle p = head[is_forward];

        head[is_forward] = link(p, is_forward);

        if (head[is_forward] == null())
          head[!is_forward] = null();
        else
          link(head[is_forward], null(), !is_forward);

        return(p);
      }

    bool empty() { return(head[forward] == null()); }

    void purge() { head[forward] = null(); head[reverse] = null(); }

  private:

    handle head[2];

    void link(handle h, handle link_h, bool is_forward = true)
     { abstractor::link(h, link_h, is_forward); }

  }; // end bidir_list

namespace impl
{

struct p_bidir_list_abs;

}

class p_bidir_elem
  {
  public:

    const p_bidir_elem * link(bool is_forward = true) const
      { return(link_[is_forward]); }

  private:

    p_bidir_elem *link_[2];

    friend struct impl::p_bidir_list_abs;
  };

namespace impl
{

struct p_bidir_list_abs
  {
    typedef p_bidir_elem *handle;

    static handle null() { return(nullptr); }

    static handle link(handle h, bool is_forward)
      { return(h->link_[is_forward]); }

    static void link(handle h, handle link_h, bool is_forward)
      { h->link_[is_forward] = link_h; }
  };

}

typedef bidir_list<impl::p_bidir_list_abs> p_bidir_list;

} // end namespace abstract_container

#endif /* Include once */
