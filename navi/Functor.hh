#ifndef _FUNCTOR_H_
# define _FUNCTOR_H_

# include <memory>
# include "TypeList.hh"

namespace navi {

template <typename RES, typename TLIST>
class FunctorImpl;

template <typename ParentFunctor, typename FUN>
class FunctorHandler : public FunctorImpl<typename ParentFunctor::ResultType,
					  typename ParentFunctor::TList> {
  typedef typename ParentFunctor::ResultType ResultType;
  typedef typename ParentFunctor::A1 A1;
  typedef typename ParentFunctor::A2 A2;
  typedef typename ParentFunctor::A3 A3;
  typedef typename ParentFunctor::A4 A4;
  typedef typename ParentFunctor::A5 A5;
  typedef typename ParentFunctor::A6 A6;
public:
  FunctorHandler(FUN const& fun) : _fun(fun) {}
  FunctorHandler* clone() const { return new FunctorHandler(*this); }

  ResultType operator()() const { return _fun(); }
  ResultType operator()(A1 a1) const { return _fun(a1); }
  ResultType operator()(A1 a1, A2 a2) const { return _fun(a1, a2); }
  ResultType operator()(A1 a1, A2 a2, A3 a3) const { return _fun(a1, a2, a3); }
  ResultType operator()(A1 a1, A2 a2, A3 a3, A4 a4) const { return _fun(a1, a2, a3, a4); }
  ResultType operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const {
    return _fun(a1, a2, a3, a4, a5);
  }
  ResultType operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const {
    return _fun(a1, a2, a3, a4, a5, a6);
  }

private:
  FUN const _fun;
};

template <typename ParentFunctor, typename C, typename FUN>
class MemFunctorHandlerConst : public FunctorImpl<typename ParentFunctor::ResultType,
					     typename ParentFunctor::TList> {
  typedef typename ParentFunctor::ResultType ResultType;
  typedef typename ParentFunctor::A1 A1;
  typedef typename ParentFunctor::A2 A2;
  typedef typename ParentFunctor::A3 A3;
  typedef typename ParentFunctor::A4 A4;
  typedef typename ParentFunctor::A5 A5;
  typedef typename ParentFunctor::A6 A6;
public:
  MemFunctorHandlerConst(C const* instance, FUN fun) : _fun(fun), _instance(instance) {}
  MemFunctorHandlerConst* clone() const { return new MemFunctorHandlerConst(*this); }
  ResultType operator()() const { return ((*_instance).*_fun)(); }
  ResultType operator()(A1 a1) const { return ((*_instance).*_fun)(a1); }
  ResultType operator()(A1 a1, A2 a2) const { return ((*_instance).*_fun)(a1, a2); }
  ResultType operator()(A1 a1, A2 a2, A3 a3) const { return ((*_instance).*_fun)(a1, a2, a3); }
  ResultType operator()(A1 a1, A2 a2, A3 a3, A4 a4) const { return ((*_instance).*_fun)(a1, a2, a3, a4); }
  ResultType operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const {
    return ((*_instance).*_fun)(a1, a2, a3, a4, a5);
  }
  ResultType operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const {
    return ((*_instance).*_fun)(a1, a2, a3, a4, a5, a6);
  }

private:
  FUN const _fun;
  C const* _instance;
};

template <typename ParentFunctor, typename C, typename FUN>
class MemFunctorHandler : public FunctorImpl<typename ParentFunctor::ResultType,
					     typename ParentFunctor::TList> {
  typedef typename ParentFunctor::ResultType ResultType;
  typedef typename ParentFunctor::A1 A1;
  typedef typename ParentFunctor::A2 A2;
  typedef typename ParentFunctor::A3 A3;
  typedef typename ParentFunctor::A4 A4;
  typedef typename ParentFunctor::A5 A5;
  typedef typename ParentFunctor::A6 A6;
public:
  MemFunctorHandler(C* instance, FUN fun) : _fun(fun), _instance(instance) {}
  MemFunctorHandler* clone() const { return new MemFunctorHandler(*this); }
  ResultType operator()() const { return ((*_instance).*_fun)(); }
  ResultType operator()(A1 a1) const { return ((*_instance).*_fun)(a1); }
  ResultType operator()(A1 a1, A2 a2) const { return ((*_instance).*_fun)(a1, a2); }
  ResultType operator()(A1 a1, A2 a2, A3 a3) const { return ((*_instance).*_fun)(a1, a2, a3); }
  ResultType operator()(A1 a1, A2 a2, A3 a3, A4 a4) const { return ((*_instance).*_fun)(a1, a2, a3, a4); }
  ResultType operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const {
    return ((*_instance).*_fun)(a1, a2, a3, a4, a5);
  }
  ResultType operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const {
    return ((*_instance).*_fun)(a1, a2, a3, a4, a5, a6);
  }

private:
  FUN const _fun;
  C* _instance;
};


////////////////////////////////////
//////////// FUNCTOR ///////////////
////////////////////////////////////
template <typename RES = void, typename TLIST = NullType>
class Functor {
public:
  typedef RES ResultType;
  typedef TLIST TList;
  typedef FunctorImpl<RES, TLIST> ImplType;
  typedef typename TL::GetTypeAt<TLIST, 0>::type A1;
  typedef typename TL::GetTypeAt<TLIST, 1>::type A2;
  typedef typename TL::GetTypeAt<TLIST, 2>::type A3;
  typedef typename TL::GetTypeAt<TLIST, 3>::type A4;
  typedef typename TL::GetTypeAt<TLIST, 4>::type A5;
  typedef typename TL::GetTypeAt<TLIST, 5>::type A6;

public:
  Functor(std::auto_ptr<ImplType> impl) {
    _impl = impl;
  }

  template <typename FUN>
  Functor(FUN const& fun)
    : _impl(new FunctorHandler<Functor, FUN>(fun)) {}

  template <typename C, typename FUN>
  Functor(C* c, FUN const& fun)
    : _impl(new MemFunctorHandler<Functor, C, FUN>(c, fun)) {}

  template <typename C, typename FUN>
  Functor(C const* c, FUN const& fun)
    : _impl(new MemFunctorHandlerConst<Functor, C, FUN>(c, fun)) {}

  Functor(Functor const& f) :
    _impl(f._impl->clone()) {
  }

public:
  RES operator()() const { return (*_impl)(); }
  RES operator()(A1 a1) const { return (*_impl)(a1); }
  RES operator()(A1 a1, A2 a2) const { return (*_impl)(a1, a2); }
  RES operator()(A1 a1, A2 a2, A3 a3) const { return (*_impl)(a1, a2, a3); }
  RES operator()(A1 a1, A2 a2, A3 a3, A4 a4) const { return (*_impl)(a1, a2, a3, a4); }
  RES operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) const {
    return (*_impl)(a1, a2, a3, a4, a5);
  }
  RES operator()(A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) const {
    return (*_impl)(a1, a2, a3, a4, a5, a6);
  }

private:
  std::auto_ptr<ImplType> _impl;
};

template <typename T>
class FunctorImplBase {
public:
  virtual T* clone() const = 0;
};

template <typename RES>
class FunctorImpl<RES, NullType> :
    public FunctorImplBase<FunctorImpl<RES, NullType> > {
public:
  virtual ~FunctorImpl() {}
  virtual RES operator()() const = 0;
};

template <typename RES, typename A1>
class FunctorImpl<RES, TYPELIST_1(A1)> :
    public FunctorImplBase<FunctorImpl<RES, TYPELIST_1(A1)> > {
public:
  virtual ~FunctorImpl() {}
  virtual RES operator()(A1) const = 0;
};

template <typename RES, typename A1, typename A2>
class FunctorImpl<RES, TYPELIST_2(A1, A2)> :
    public FunctorImplBase<FunctorImpl<RES, TYPELIST_2(A1, A2)> > {
public:
  virtual ~FunctorImpl() {}
  virtual RES operator()(A1, A2) const = 0;
};

template <typename RES, typename A1, typename A2, typename A3>
class FunctorImpl<RES, TYPELIST_3(A1, A2, A3)> :
    public FunctorImplBase<FunctorImpl<RES, TYPELIST_3(A1, A2, A3)> > {
public:
  virtual ~FunctorImpl() {}
  virtual RES operator()(A1, A2, A3) const = 0;
};

template <typename RES, typename A1, typename A2, typename A3, typename A4>
class FunctorImpl<RES, TYPELIST_4(A1, A2, A3, A4)> :
    public FunctorImplBase<FunctorImpl<RES, TYPELIST_4(A1, A2, A3, A4)> > {
public:
  virtual ~FunctorImpl() {}
  virtual RES operator()(A1, A2, A3, A4) const = 0;
};

template <typename RES, typename A1, typename A2,
	  typename A3, typename A4, typename A5>
class FunctorImpl<RES, TYPELIST_5(A1, A2, A3, A4, A5)> :
    public FunctorImplBase<FunctorImpl<RES, TYPELIST_5(A1, A2, A3, A4, A5)> > {
public:
  virtual ~FunctorImpl() {}
  virtual RES operator()(A1, A2, A3, A4, A5) const = 0;
};

template <typename RES, typename A1, typename A2,
	  typename A3, typename A4, typename A5, typename A6>
class FunctorImpl<RES, TYPELIST_6(A1, A2, A3, A4, A5, A6)> :
    public FunctorImplBase<FunctorImpl<RES, TYPELIST_6(A1, A2, A3, A4, A5, A6)> > {
public:
  virtual ~FunctorImpl() {}
  virtual RES operator()(A1, A2, A3, A4, A5, A6) const = 0;
};

} // !navi

#endif /* !_FUNCTOR_H_ */
