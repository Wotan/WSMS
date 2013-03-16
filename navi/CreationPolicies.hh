#ifndef _CREATIONPOLICIES_H_
# define _CREATIONPOLICIES_H_

namespace navi {

template <class T>
struct  CreateWithNew {
  static T* create() {
    return new T();
  }
  static void destroy(T *obj) {
    delete obj;
  }
};

} // !navi

#endif /* !_CREATIONPOLICIES_H_ */
