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

namespace abstract_container
{

#ifndef ABSTRACT_CONTAINER_RAW_DEFINED_
#define ABSTRACT_CONTAINER_RAW_DEFINED_

class raw_t { };
const raw_t raw;

#endif

template <class abstractor>
class circ_que_front;

template <class abstractor>
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
  private:

    typedef typename abstractor::elem_t elem_t;

    static const unsigned align_size =
      ((sizeof(elem_t) + alignof(elem_t) + 1) / alignof(elem_t))
      * alignof(elem_t);

    alignas(elem_t) char data[abstractor::max_num_elems][align_size];

    elem_t & operator [] (unsigned idx)
      { return(reinterpret_cast<elem_t &>(data[idx])); }

    static unsigned size(unsigned f, unsigned ni)
      { return(ni >= f ? (ni - f) : (ni + abstractor::max_size - f)); }

    friend class circ_que_front<abstractor>;
    friend class circ_que_back<abstractor>;

  public:

    circ_que(const circ_que &) = default;

    circ_que & operator = (const circ_que &) = default;

    static const unsigned max_size = abstractor::max_num_elems;

  }; // end circ_que

template <class abstractor>
class circ_que_front
  {
  private:

    typedef circ_que<abstractor> cq_t;

  public:

    typedef typename abstractor::elem_t elem_t;

    circ_que_front(cq_t &cq_) : cq(cq_) { }

    unsigned size() const
      {
        unsigned ni = cq.abstractor::consume_next_in();
        unsigned f = cq.abstractor::consume_front();
        return(cq_t::size(f, ni));
      }

    bool is_empty() const
      {
        return(
          cq.abstractor::consume_next_in() == cq.abstractor::consume_front());
      }

    void wait_while_empty()
      {
        while (is_empty())
          cq.abstractor::wait_while_empty();
      }

    const elem_t & operator () () const
      { return(cq[cq.abstractor::consume_front()]); }

    const elem_t & operator () (unsigned index) const
      {
        return(
          cq[(cq.abstractor::consume_front() + index) % cq_t::max_size]);
      }

    void pop()
      {
        unsigned f = cq.abstractor::consume_front() + 1;

        cq.abstractor::consume_front(f == cq_t::max_size ? 0 : f);

        cq.abstractor::end_full_wait();
      }

  private:

    cq_t &cq;

  }; // end circ_que_front

template <class abstractor>
class circ_que_back
  {
  private:

    typedef circ_que<abstractor> cq_t;

  public:

    typedef typename abstractor::elem_t elem_t;

    circ_que_back(cq_t &cq_) : cq(cq_) { }

    unsigned size() const
      {
        unsigned ni = cq.abstractor::produce_next_in();
        unsigned f = cq.abstractor::produce_front();
        return(cq_t::size(f, ni));
      }

    bool is_full() const { return(size() == cq_t::max_size); }

    void wait_while_full()
      {
        while (is_full())
          cq.abstractor::wait_while_full();
      }

    elem_t & operator () () { return(cq[cq.next_in()]); }

    void push(raw_t)
      {

        unsigned ni = cq.abstractor::produce_next_in() + 1;

        cq.next_in(ni == cq_t::max_size ? 0 : ni);
      }

  private:

    cq_t &cq;

  }; // end circ_que_back

} // end namespace abstract_container

#endif /* Include once */
