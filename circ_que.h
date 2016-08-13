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
#ifndef ABSTRACT_CONTAINER_CIRC_QUE_H_
#define ABSTRACT_CONTAINER_CIRC_QUE_H_

#include <new>
#include <utility>

namespace abstract_container
{

#ifndef ABSTRACT_CONTAINER_RAW_DEFINED_
#define ABSTRACT_CONTAINER_RAW_DEFINED_

class raw_t { };
const raw_t raw;

#endif

template <class cq_t>
class circ_que_front;

template <class cq_t>
class circ_que_back;

/*
Circular queue container class template

The 'abstractor' template parameter class must have the following public
or protected members, or behave as though it does:

type elem_t -- 
type front_t -- 
type back_t -- 
static const man_num_elems

bool wait_while_empty()
bool wait_while_full()
bool
*/
template <class abstractor>
class circ_que : protected abstractor
  {
  public:

    static const unsigned max_size = abstractor::max_num_elems;

  private:

    static const unsigned dimension = max_size + 1;

    typedef circ_que cq_t;

    typedef typename abstractor::elem_t elem_t;

    static const unsigned align_size =
      ((sizeof(elem_t) + alignof(elem_t) + 1) / alignof(elem_t))
      * alignof(elem_t);

    alignas(elem_t) char data[dimension][align_size];

    elem_t & operator [] (unsigned idx)
      { return(reinterpret_cast<elem_t &>(data[idx])); }

    static unsigned size(unsigned f, unsigned ni)
      { return(ni >= f ? (ni - f) : (ni + dimension - f)); }

    friend class circ_que_front<cq_t>;
    friend class circ_que_back<cq_t>;

  public:

    circ_que() = default;

    circ_que(const circ_que &) = default;

    circ_que & operator = (const circ_que &) = default;

  }; // end circ_que

template <class cq_t>
class circ_que_front
  {
  public:

    typedef typename cq_t::elem_t elem_t;

    circ_que_front(cq_t &cq_) : cq(cq_) { }

    unsigned size() const
      {
        unsigned ni = cq.consume_next_in();
        unsigned f = cq.consume_front();
        return(cq_t::size(f, ni));
      }

    bool is_empty() const
      {
        return(
          cq.consume_next_in() == cq.consume_front());
      }

    void wait_while_empty()
      {
        while (is_empty())
          cq.wait_while_empty();
      }

    const elem_t & operator () () const
      { return(cq[cq.consume_front()]); }

    const elem_t & operator () (unsigned index) const
      { return(cq[(cq.consume_front() + index) % cq_t::dimension]); }

    void pop(raw_t) { pop_(cq.consume_front()); }

    void pop()
      {
        unsigned f = cq.consume_front();

        cq[f].~elem_t();

        pop_(f);
      }

  private:

    void pop_(unsigned f)
      {
        ++f;

        cq.consume_front(f == cq_t::dimension ? 0 : f);

        cq.end_full_wait();
      }

    cq_t &cq;

  }; // end circ_que_front

template <class cq_t>
class circ_que_back
  {
  public:

    typedef typename cq_t::elem_t elem_t;

    circ_que_back(cq_t &cq_) : cq(cq_) { }

    unsigned size() const
      {
        unsigned ni = cq.produce_next_in();
        unsigned f = cq.produce_front();
        return(cq_t::size(f, ni));
      }

    bool is_full() const { return(size() == cq_t::max_size); }

    void wait_while_full()
      {
        while (is_full())
          cq.wait_while_full();
      }

    elem_t & operator () () { return(cq[cq.produce_next_in()]); }

    void push(raw_t) { push_(cq.produce_next_in()); }

    template<typename ... args_t>
    void push(args_t && ... args)
      {
        unsigned ni = cq.produce_next_in();

        new(&cq[ni]) elem_t(std::forward<args_t>(args)...);

        push_(ni);
      }

  private:

    void push_(unsigned ni)
      {
        ++ni;

        cq.produce_next_in(ni == cq_t::dimension ? 0 : ni);

        cq.end_empty_wait();
      }

    cq_t &cq;

  }; // end circ_que_back

template <
  typename elem_t_, unsigned max_num_elems_, typename index_t = unsigned>
class circ_que_abs_basic
  {
  protected:

    typedef elem_t_ elem_t;

    static const index_t max_num_elems = max_num_elems_;

    index_t front, next_in;

    circ_que_abs_basic() : front(0), next_in(0) { }

    unsigned produce_front() const { return(front); }
    unsigned consume_front() const { return(front); }

    void consume_front(unsigned f) { front = f; }

    unsigned produce_next_in() const { return(next_in); }
    unsigned consume_next_in() const { return(next_in); }

    void produce_next_in(unsigned ni) { next_in = ni; }

    void end_full_wait() { }
    void end_empty_wait() { }
  };

template <typename elem_t, unsigned max_num_elems, typename index_t = unsigned>
using basic_circ_que =
  circ_que<circ_que_abs_basic<elem_t, max_num_elems, index_t> >;

// Note:  This is not an intrusive, non-copying container.  I just
// put it here rather than putting it in it's own small repo.

} // end namespace abstract_container

#endif /* Include once */
