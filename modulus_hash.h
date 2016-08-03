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
#ifndef ABSTRACT_CONTAINER_MODULUS_HASH_H_
#define ABSTRACT_CONTAINER_MODULUS_HASH_H_

// Modulus hash of keys of arbitrary length.

namespace abstract_container
{

template <class traits, unsigned key_segment>
class modulus_hash_coeff
  {
  public:

    typedef typename traits::modulus_t modulus_t;
    typedef typename traits::product_t product_t;

  private:

    static const product_t coeff1 =
      (product_t(1) << traits::key_segment_bits) % traits::modulus;

  public:

    static const modulus_t val =
      (coeff1 * modulus_hash_coeff<traits, key_segment - 1>::val) %
      traits::modulus;
  };

template <class traits>
class modulus_hash_coeff<traits, 0>
  {
  public:

    typedef typename traits::modulus_t modulus_t;
    typedef typename traits::product_t product_t;

    static const modulus_t val = 1;
  };

namespace impl
{

template <class traits, unsigned reverse_key_segment>
class modulus_hash
  {
  public:

    typedef typename traits::modulus_t modulus_t;
    typedef typename traits::product_t product_t;

    static const unsigned key_segment =
      traits::num_key_segments - 1 - reverse_key_segment;

    static modulus_t val(typename traits::key k)
      {
        return(
          ((product_t(traits::template get_segment<key_segment>(k)) *
            modulus_hash_coeff<traits, key_segment>::val) +
           modulus_hash<traits, reverse_key_segment - 1>::val(k)) %
          traits::modulus);
      }
  };

template <class traits>
class modulus_hash<traits, 0>
  {
  public:

    typedef typename traits::modulus_t modulus_t;
    typedef typename traits::product_t product_t;

    static const unsigned key_segment = traits::num_key_segments - 1;

    static modulus_t val(typename traits::key k)
      {
        return(
          (product_t(traits::template get_segment<key_segment>(k)) *
            modulus_hash_coeff<traits, key_segment>::val) %
          traits::modulus);
      }
  };

} // end namespace impl

template<class traits>
typename traits::modulus_t modulus_hash(typename traits::key k)
  {
    return(impl::modulus_hash<traits, traits::num_key_segments - 1>::val(k));
  }

} // end namespace abstract_container

#endif /* Include once */
