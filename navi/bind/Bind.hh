#ifndef _BIND_H_
#define _BIND_H_

#include "Storage.hh"
#include "TypeTraits.hh"
#include "Caller.hh"
#include "TypeList.hh"
#include "MemFunHolder.hh"
#include "Ref.hh"

namespace navi {

template <typename ReturnType>
Caller <ReturnType, ReturnType(*)(), typename Traits0::Type >
bind(ReturnType(*f)())
{
  typedef TypeList0 ListType;
  ListType l;
  return Caller<ReturnType, ReturnType(*)(), ListType>(f, l);
}

template <typename ReturnType, typename X1, typename Param1>
Caller <ReturnType, ReturnType(*)(X1), typename Traits1<Param1>::Type >
bind(ReturnType(*f)(X1), Param1 p1)
{
  typedef typename Traits1<Param1>::Type ListType;
  ListType l(p1);
  return Caller<ReturnType, ReturnType(*)(X1), ListType>(f, l);
}

template <typename ReturnType, typename X1, typename X2, typename Param1, typename Param2>
Caller <ReturnType, ReturnType(*)(X1, X2), typename Traits2<Param1, Param2>::Type >
bind(ReturnType(*f)(X1, X2), Param1  p1, Param2  p2)
{
  typedef typename Traits2<Param1, Param2>::Type ListType;
  ListType l(p1, p2);
  return Caller<ReturnType, ReturnType(*)(X1, X2), ListType>(f, l);
}

template <typename ReturnType, typename X1, typename X2, typename X3,
	  typename Param1, typename Param2, typename Param3>
Caller <ReturnType, ReturnType(*)(X1, X2, X3), typename Traits3<Param1, Param2, Param3>::Type >
bind(ReturnType(*f)(X1, X2, X3), Param1  p1, Param2  p2, Param3  p3)
{
  typedef typename Traits3<Param1, Param2, Param3>::Type ListType;
  ListType l(p1, p2, p3);
  return Caller<ReturnType, ReturnType(*)(X1, X2, X3), ListType>(f, l);
}

template <typename ReturnType, typename X1, typename X2, typename X3, typename X4,
	  typename Param1, typename Param2, typename Param3, typename Param4>
Caller <ReturnType, ReturnType(*)(X1, X2, X3, X4), typename Traits4<Param1, Param2, Param3, Param4>::Type >
bind(ReturnType(*f)(X1, X2, X3, X4), Param1  p1, Param2  p2, Param3  p3, Param4  p4)
{
  typedef typename Traits4<Param1, Param2, Param3, Param4>::Type ListType;
  ListType l(p1, p2, p3, p4);
  return Caller<ReturnType, ReturnType(*)(X1, X2, X3, X4), ListType>(f, l);
}

template <typename ReturnType, typename X1, typename X2, typename X3, typename X4, typename X5,
	  typename Param1, typename Param2, typename Param3, typename Param4, typename Param5>
Caller <ReturnType, ReturnType(*)(X1, X2, X3, X4, X5),
	typename Traits5<Param1, Param2, Param3, Param4, Param5>::Type >
bind(ReturnType(*f)(X1, X2, X3, X4, X5),
     Param1  p1, Param2  p2, Param3  p3, Param4  p4, Param5  p5)
{
  typedef typename Traits5<Param1, Param2, Param3, Param4, Param5>::Type ListType;
  ListType l(p1, p2, p3, p4, p5);
  return Caller<ReturnType, ReturnType(*)(X1, X2, X3, X4, X5), ListType>(f, l);
}

template <typename ReturnType, typename X1, typename X2, typename X3, typename X4, typename X5, typename X6,
	  typename Param1, typename Param2, typename Param3, typename Param4, typename Param5, typename Param6>
Caller <ReturnType, ReturnType(*)(X1, X2, X3, X4, X5, X6),
	typename Traits6<Param1, Param2, Param3, Param4, Param5, Param6>::Type >
bind(ReturnType(*f)(X1, X2, X3, X4, X5, X6),
     Param1  p1, Param2  p2, Param3  p3, Param4  p4, Param5  p5, Param6  p6)
{
  typedef typename Traits6<Param1, Param2, Param3, Param4, Param5, Param6>::Type ListType;
  ListType l(p1, p2, p3, p4, p5, p6);
  return Caller<ReturnType, ReturnType(*)(X1, X2, X3, X4, X5, X6), ListType>(f, l);
}


////////////////////////////////////////////////////////
///////////////////// BIND MEMFUN //////////////////////
////////////////////////////////////////////////////////

#define BIND_MEMFUN1(QUAL)						\
  template <typename ReturnType, typename Param1, typename C>		\
  Caller <ReturnType, MemFunHolder<ReturnType, ReturnType (C::*)() QUAL>, \
	  typename Traits1<Param1>::Type >				\
  bind(ReturnType (C::*f)() QUAL, Param1  p1)			\
  {									\
    typedef MemFunHolder<ReturnType, ReturnType (C::*)() QUAL> TypeHolder; \
    typedef typename Traits1<Param1>::Type ListType;			\
    ListType l(p1);							\
    return Caller<ReturnType, TypeHolder, ListType>(TypeHolder(f), l);	\
  }									\

BIND_MEMFUN1();
BIND_MEMFUN1(const);
BIND_MEMFUN1(volatile);

#define BIND_MEMFUN2(QUAL)						\
  template <typename ReturnType, typename Param1, typename Param2, typename X1, typename C> \
  Caller <ReturnType, MemFunHolder<ReturnType, ReturnType (C::*)(X1) QUAL>, \
	  typename Traits2<Param1, Param2>::Type >			\
  bind(ReturnType (C::*f)(X1) QUAL, Param1  p1, Param2  p2)	\
  {									\
    typedef MemFunHolder<ReturnType, ReturnType (C::*)(X1) QUAL> TypeHolder; \
    typedef typename Traits2<Param1, Param2>::Type ListType;		\
    ListType l(p1, p2);							\
    return Caller<ReturnType, TypeHolder, ListType>(TypeHolder(f), l);	\
  }									\

BIND_MEMFUN2();
BIND_MEMFUN2(const);
BIND_MEMFUN2(volatile);

#define BIND_MEMFUN3(QUAL)						\
  template <typename ReturnType, typename Param1, typename Param2, typename Param3, \
	    typename X1, typename X2, typename C>			\
  Caller <ReturnType, MemFunHolder<ReturnType, ReturnType (C::*)(X1, X2) QUAL>, \
	  typename Traits3<Param1, Param2, Param3>::Type >		\
  bind(ReturnType (C::*f)(X1, X2) QUAL, Param1  p1, Param2  p2, Param3  p3) \
  {									\
    typedef MemFunHolder<ReturnType, ReturnType (C::*)(X1, X2) QUAL> TypeHolder; \
    typedef typename Traits3<Param1, Param2, Param3>::Type ListType;	\
    ListType l(p1, p2, p3);						\
    return Caller<ReturnType, TypeHolder, ListType>(TypeHolder(f), l);	\
  }									\

BIND_MEMFUN3();
BIND_MEMFUN3(const);
BIND_MEMFUN3(volatile);

#define BIND_MEMFUN4(QUAL)						\
  template <typename ReturnType, typename Param1, typename Param2, typename Param3, typename Param4, \
	    typename X1, typename X2, typename X3, typename C>		\
  Caller <ReturnType, MemFunHolder<ReturnType, ReturnType (C::*)(X1, X2, X3) QUAL>, \
	  typename Traits4<Param1, Param2, Param3, Param4>::Type >	\
  bind(ReturnType (C::*f)(X1, X2, X3) QUAL, Param1  p1, Param2  p2, Param3  p3, \
       Param4  p4)						\
  {									\
    typedef MemFunHolder<ReturnType, ReturnType (C::*)(X1, X2, X3) QUAL> TypeHolder; \
    typedef typename Traits4<Param1, Param2, Param3, Param4>::Type ListType; \
    ListType l(p1, p2, p3, p4);						\
    return Caller<ReturnType, TypeHolder, ListType>(TypeHolder(f), l);	\
  }									\

BIND_MEMFUN4();
BIND_MEMFUN4(const);
BIND_MEMFUN4(volatile);

#define BIND_MEMFUN5(QUAL)						\
  template <typename ReturnType, typename Param1, typename Param2, typename Param3, typename Param4, \
	    typename Param5, typename X1, typename X2, typename X3, typename X4, typename C> \
  Caller <ReturnType, MemFunHolder<ReturnType, ReturnType (C::*)(X1, X2, X3, X4) QUAL>, \
	  typename Traits5<Param1, Param2, Param3, Param4, Param5>::Type > \
  bind(ReturnType (C::*f)(X1, X2, X3, X4) QUAL, Param1  p1, Param2  p2, Param3  p3, \
       Param4  p4, Param5  p5)						\
  {									\
    typedef MemFunHolder<ReturnType, ReturnType (C::*)(X1, X2, X3, X4) QUAL> TypeHolder; \
    typedef typename Traits5<Param1, Param2, Param3, Param4, Param5>::Type ListType; \
    ListType l(p1, p2, p3, p4, p5);					\
    return Caller<ReturnType, TypeHolder, ListType>(TypeHolder(f), l);	\
  }									\

BIND_MEMFUN5();
BIND_MEMFUN5(const);
BIND_MEMFUN5(volatile);

#define BIND_MEMFUN6(QUAL)						\
  template <typename ReturnType, typename Param1, typename Param2, typename Param3, typename Param4, \
	    typename Param5, typename Param6, typename X1, typename X2, typename X3, typename X4, \
	    typename X5, typename C>					\
  Caller <ReturnType, MemFunHolder<ReturnType, ReturnType (C::*)(X1, X2, X3, X4, X5) QUAL>, \
	  typename Traits6<Param1, Param2, Param3, Param4, Param5, Param6>::Type > \
  bind(ReturnType (C::*f)(X1, X2, X3, X4, X5) QUAL, Param1  p1, Param2  p2, Param3  p3, \
       Param4  p4, Param5  p5, Param6  p6)		\
  {									\
    typedef MemFunHolder<ReturnType, ReturnType (C::*)(X1, X2, X3, X4, X5) QUAL> TypeHolder; \
    typedef typename Traits6<Param1, Param2, Param3, Param4, Param5, Param6>::Type ListType; \
    ListType l(p1, p2, p3, p4, p5, p6);					\
    return Caller<ReturnType, TypeHolder, ListType>(TypeHolder(f), l);	\
  }									\

BIND_MEMFUN6();
BIND_MEMFUN6(const);
BIND_MEMFUN6(volatile);

} // !navi

#endif /* !_BIND_H_ */
