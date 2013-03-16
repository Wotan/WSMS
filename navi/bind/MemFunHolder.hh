#ifndef _MEMFUNHOLDER_H_
#define _MEMFUNHOLDER_H_

template <class RET, class FUN>
class MemFunHolder {
public:
  MemFunHolder(FUN f) : _f(f) {}

  template <typename A1>
  RET operator()(A1* a1) const {
    return (a1->*_f)();
  }
  template <typename A1>
  RET operator()(A1  a1) const {
    return (a1.*_f)();
  }

  template <typename A1, typename A2>
  RET operator()(A1* a1, A2 const& a2) const {
    return (a1->*_f)(a2);
  }
  template <typename A1, typename A2>
  RET operator()(A1 a1, A2 const& a2) const {
    return (a1.*_f)(a2);
  }

  template <typename A1, typename A2, typename A3>
  RET operator()(A1* a1, A2 const& a2, A3 const& a3) const {
    return (a1->*_f)(a2, a3);
  }
  template <typename A1, typename A2, typename A3>
  RET operator()(A1 a1, A2 const& a2, A3 const& a3) const {
    return (a1.*_f)(a2, a3);
  }

  template <typename A1, typename A2, typename A3, typename A4>
  RET operator()(A1* a1, A2 const& a2, A3 const& a3, A4 const& a4) const {
    return (a1->*_f)(a2, a3, a4);
  }
  template <typename A1, typename A2, typename A3, typename A4>
  RET operator()(A1 a1, A2 const& a2, A3 const& a3, A4 const& a4) const {
    return (a1.*_f)(a2, a3, a4);
  }

  template <typename A1, typename A2, typename A3, typename A4, typename A5>
  RET operator()(A1* a1, A2 const& a2, A3 const& a3, A4 const& a4, A5 const& a5) const {
    return (a1->*_f)(a2, a3, a4, a5);
  }
  template <typename A1, typename A2, typename A3, typename A4, typename A5>
  RET operator()(A1 a1, A2 const& a2, A3 const& a3, A4 const& a4, A5 const& a5) const {
    return (a1.*_f)(a2, a3, a4, a5);
  }

  template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
  RET operator()(A1* a1, A2 const& a2, A3 const& a3, A4 const& a4,
		 A5 const& a5, A6 const& a6) const {
    return (a1->*_f)(a2, a3, a4, a5, a6);
  }
  template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
  RET operator()(A1 a1, A2 const& a2, A3 const& a3, A4 const& a4,
		 A5 const& a5, A6 const& a6) const {
    return (a1.*_f)(a2, a3, a4, a5, a6);
  }


private:
  FUN _f;
};

#endif /* !_MEMFUNHOLDER_H_ */
