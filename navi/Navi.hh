#ifndef _NAVI_H_
# define _NAVI_H_

# if defined(unix) || defined(__unix__) || defined(__unix)
#  define NAVI_UNIX_PLATEFORM
#  define UNIX_PLATEFORM
# elif defined(_WINDOWS) || defined(__WIN32) || defined(WIN32) || defined(__WIN32__)
#  define NAVI_WIN_PLATEFORM
#  define WIN_PLATEFORM
# elif defined(__APPLE__)
#  define NAVI_MAC_PLATEFORM
#  define MAC_PLATEFORM
# else
#  error "Unknown plateform"
# endif

#endif /* !_NAVI_H_ */
