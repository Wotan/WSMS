#ifndef _PLACEHOLDER_H_
#define _PLACEHOLDER_H_

namespace navi {

template <int i>
class Arg {
public:
  Arg() {}
  enum { id = i };
};

} // !navi

static navi::Arg<1> _1;
static navi::Arg<2> _2;
static navi::Arg<3> _3;
static navi::Arg<4> _4;
static navi::Arg<5> _5;
static navi::Arg<6> _6;

#endif /* !_PLACEHOLDER_H_ */
