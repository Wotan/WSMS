#ifndef _CALLER_H_
#define _CALLER_H_

namespace navi {

template <typename ReturnType, typename Callable, typename PList>
class Caller {
public:
  Caller(Callable c, PList const& list) : _callable(c), _list(list) {}

  ReturnType operator()() {
    return _list(TypeTraits<ReturnType>(), _callable, _list);
  }

  template <typename A1>
  ReturnType operator()(A1  a1) {
    typedef TypeList1<A1> ListType;
    ListType l(a1);
    return _list(TypeTraits<ReturnType>(), _callable, l);
  }

  template <typename A1, typename A2>
  ReturnType operator()(A1 a1, A2  a2) {
    typedef TypeList2<A1 , A2 > ListType;
    ListType l(a1, a2);
    return _list(TypeTraits<ReturnType>(), _callable, l);
  }

  template <typename A1, typename A2, typename A3>
  ReturnType operator()(A1  a1, A2  a2, A3  a3) {
    typedef TypeList3<A1 , A2 , A3 > ListType;
    ListType l(a1, a2, a3);
    return _list(TypeTraits<ReturnType>(), _callable, l);
  }

  template <typename A1, typename A2, typename A3, typename A4>
  ReturnType operator()(A1  a1, A2  a2, A3  a3, A4  a4) {
    typedef TypeList4<A1 , A2 , A3 , A4 > ListType;
    ListType l(a1, a2, a3, a4);
    return _list(TypeTraits<ReturnType>(), _callable, l);
  }

  template <typename A1, typename A2, typename A3, typename A4, typename A5>
  ReturnType operator()(A1  a1, A2  a2, A3  a3, A4  a4,
			A5  a5) {
    typedef TypeList5<A1 , A2 , A3 , A4 , A5 > ListType;
    ListType l(a1, a2, a3, a4, a5);
    return _list(TypeTraits<ReturnType>(), _callable, l);
  }

  template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
  ReturnType operator()(A1  a1, A2  a2, A3  a3, A4  a4,
			A5  a5, A6  a6) {
    typedef TypeList6<A1 , A2 , A3 , A4 , A5 , A6 > ListType;
    ListType l(a1, a2, a3, a4, a5, a6);
    return _list(TypeTraits<ReturnType>(), _callable, l);
  }

private:
  Callable _callable;
  PList _list;
};

} // !navi

#endif /* !_CALLER_H_ */
