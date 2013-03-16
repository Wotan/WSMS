#ifndef _TYPETRAITS_H_
#define _TYPETRAITS_H_

# include "TypeList.hh"

namespace navi {

struct Traits0
{
  typedef TypeList0 Type;
};

template <typename P1>
struct Traits1
{
  typedef typename GetType<P1>::Type Type1;
  typedef TypeList1 <Type1> Type;
};

template <typename P1, typename P2>
struct Traits2
{
  typedef typename GetType<P1>::Type Type1;
  typedef typename GetType<P2>::Type Type2;
  typedef TypeList2 <Type1, Type2> Type;
};

template <typename P1, typename P2, typename P3>
struct Traits3
{
  typedef typename GetType<P1>::Type Type1;
  typedef typename GetType<P2>::Type Type2;
  typedef typename GetType<P3>::Type Type3;
  typedef TypeList3 <Type1, Type2, Type3> Type;
};

template <typename P1, typename P2, typename P3, typename P4>
struct Traits4
{
  typedef typename GetType<P1>::Type Type1;
  typedef typename GetType<P2>::Type Type2;
  typedef typename GetType<P3>::Type Type3;
  typedef typename GetType<P4>::Type Type4;
  typedef TypeList4 <Type1, Type2, Type3, Type4> Type;
};

template <typename P1, typename P2, typename P3, typename P4, typename P5>
struct Traits5
{
  typedef typename GetType<P1>::Type Type1;
  typedef typename GetType<P2>::Type Type2;
  typedef typename GetType<P3>::Type Type3;
  typedef typename GetType<P4>::Type Type4;
  typedef typename GetType<P5>::Type Type5;
  typedef TypeList5 <Type1, Type2, Type3, Type4, Type5> Type;
};

template <typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
struct Traits6
{
  typedef typename GetType<P1>::Type Type1;
  typedef typename GetType<P2>::Type Type2;
  typedef typename GetType<P3>::Type Type3;
  typedef typename GetType<P4>::Type Type4;
  typedef typename GetType<P5>::Type Type5;
  typedef typename GetType<P6>::Type Type6;
  typedef TypeList6 <Type1, Type2, Type3, Type4, Type5, Type6> Type;
};

template <typename T>
struct TypeTraits {
};

} // !navi

#endif /* !_TYPETRAITS_H_ */
