#ifndef _CONDITIONVARIABLE_H_
#define _CONDITIONVARIABLE_H_

# if defined(NAVI_UNIX_PLATEFORM) || defined(NAVI_MAC_PLATEFORM)
#  include "unix/ConditionVariableImpl.hh"
# elif defined(NAVI_WIN_PLATEFORM)
# include "win/ConditionVariableImpl.hh"
# else
#  error "Navi mutex: plateform not supported"
# endif

#endif /* !_CONDITIONVARIABLE_H_ */
