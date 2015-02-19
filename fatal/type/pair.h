/*
 *  Copyright (c) 2015, Facebook, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant
 *  of patent rights can be found in the PATENTS file in the same directory.
 */

#ifndef FATAL_INCLUDE_fatal_type_pair_h
#define FATAL_INCLUDE_fatal_type_pair_h

#include <fatal/type/tag.h>
#include <fatal/type/transform.h>

#include <type_traits>
#include <utility>

namespace fatal {

/**
 * Type pair for template metaprogramming.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename TFirst, typename TSecond>
struct type_pair {
  using first = TFirst;
  using second = TSecond;

  /**
   * A `type_pair` with `first` and `second` swapped.
   *
   * Example:
   *
   *  using pair = type_pair<int, double>;
   *
   *  // yields `type_pair<double, int>`
   *  using result = pair::invert;
   *
   * @author: Marcelo Juchem <marcelo@fb.com>
   */
  using invert = type_pair<TSecond, TFirst>;

  /**
   * TODO: TEST
   * Applies the types from this `type_pair` to template `T.
   *
   * Optional transforms can be provided, which will be applie
   * to the elements of this pair before applying them to `T`.
   *
   * Example:
   *
   *  using pair = type_pair<int, bool>;
   *
   *  // yields `std::pair<int, bool>`
   *  using result2 = pair::apply<std::pair>;
   *
   *  // yields `std::pair<Foo<int>, bool>`
   *  using result2 = pair::apply<std::pair, Foo>;
   *
   *  // yields `std::pair<Foo<int>, Bar<bool>>`
   *  using result1 = pair::apply<std::pair, Foo, Bar>;
   *
   * @author: Marcelo Juchem <marcelo@fb.com>
   */
  template <
    template <typename...> class T,
    template <typename...> class TFirstTransform = identity_transform,
    template <typename...> class TSecondTransform = identity_transform
  >
  using apply = T<TFirstTransform<first>, TSecondTransform<second>>;

  /**
   * Applies optional transforms to the elements of this pair.
   *
   * If no transforms are provided, this meta-function behaves
   * as an identity function.
   *
   * Example:
   *
   *  using pair = type_pair<int, bool>;
   *
   *  // yields `type_pair<Foo<int>, Bar<bool>>`
   *  using result1 = pair::transform<Foo, Bar>;
   *
   *  // yields `type_pair<Foo<int>, bool>`
   *  using result2 = pair::transform<Foo>;
   *
   *  // yields `type_pair<int, bool>`
   *  using result2 = pair::transform<>;
   *
   * @author: Marcelo Juchem <marcelo@fb.com>
   */
  template <
    template <typename...> class TFirstTransform = identity_transform,
    template <typename...> class TSecondTransform = identity_transform
  >
  using transform = type_pair<TFirstTransform<first>, TSecondTransform<second>>;
};

/**
 * Creates a `type_pair` out of given types. Takes optional transforms that will
 * be independently applied to the `first` and `second` members of the pair.
 *
 * See individual members below for detailed info.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <
  template <typename...> class TFirstTransform = identity_transform,
  template <typename...> class TSecondTransform = identity_transform
>
struct type_pair_from {
  /**
   * Creates a `type_pair` out of a single type by individually applying
   * transforms to it in order to get the `first` and `second` members of
   * the pair.
   *
   * Example:
   *
   *  // yields `type_pair<Foo<int>, Bar<int>>`
   *  using result1 = type_pair_from<Foo, Bar>::type<int>;
   *
   *  // yields `type_pair<Foo<int>, int>`
   *  using result2 = type_pair_from<Foo>::type<int>;
   *
   *  // yields `type_pair<int, int>`
   *  using result3 = type_pair_from<>::type<int>;
   *
   * @author: Marcelo Juchem <marcelo@fb.com>
   */
  template <typename U>
  using type = type_pair<TFirstTransform<U>, TSecondTransform<U>>;
};

/**
 * Specialization of `type_get_traits` so that `type_get` supports `type_pair`.
 *
 * @author: Marcelo Juchem <marcelo@fb.com>
 */
template <typename TFirst, typename TSecond, std::size_t Index>
struct type_get_traits<type_pair<TFirst, TSecond>, Index> {
  static_assert(Index < 2, "index out of bounds");
  using type = typename std::conditional<Index == 0, TFirst, TSecond>::type;
};

// TODO: DOCUMENT AND TEST
template <typename TFirst, typename TSecond, std::size_t Index>
using indexed_type_pair_tag = indexed_type_tag<
  type_pair<TFirst, TSecond>,
  Index
>;

} // namespace fatal {

#endif // FATAL_INCLUDE_fatal_type_pair_h
