#ifndef _PLUGIN_H_
# define _PLUGIN_H_

# include "Exception.hh"
# include "TemplateMacro.hh"

# if defined(NAVI_UNIX_PLATEFORM) || defined(NAVI_MAC_PLATEFORM)
#  include "unix/PluginImpl.hh" /* On dlopen plateform dont forget to link with libdl.so */
# elif defined(NAVI_WIN_PLATEFORM)
# include "windows/PluginImpl.hh"
# else
#  error "Navi thread: plateform not supported"
# endif


namespace navi {

class Plugin {
public:
  DEFINE_EXCEPTION(Error);

  Plugin() : _loaded(false) {}
  virtual ~Plugin()
  {
    if (_loaded)
      close();
  }

  virtual void open(std::string const& fileName)
  {
    if (_loaded)
      close();
    if (_impl.open(fileName)) {
      _loaded = true;
    } else {
      _loaded = false;
      throw Error(_impl.getError());
    }
  }

  virtual void close()
  {
    if (_loaded) {
      _impl.close();
      _loaded = false;
    }
  }

# define PLUGIN_CALL_SYM_TPL(n)						        \
  template <class RET NAVI_COMMA_IFN0(n) NAVI_TPL_DEF(n)>		        \
  RET callSym(std::string const& name NAVI_COMMA_IFN0(n) NAVI_TPL_FUN_DEF(n)) { \
    if (!_loaded) throw Error("Plugin not loaded");				\
    return getSym<RET NAVI_COMMA_IFN0(n) NAVI_TPL_TYPE_DEF(n)>			\
      (name)(NAVI_TPL_FUN_CALL(n));						\
  }
# define PLUGIN_GET_SYM_TPL(n)					\
  template <class RET NAVI_COMMA_IFN0(n) NAVI_TPL_DEF(n)>	\
  RET (*getSym(std::string const& name))(NAVI_TPL_FUN_DEF(n))	\
  {								\
    typedef RET (*Fun)(NAVI_TPL_TYPE_DEF(n));			\
    Fun sym = reinterpret_cast<Fun>(_impl.getSym(name));	\
    if (!sym) throw Error(_impl.getError());			\
    return sym;							\
  }

  PLUGIN_CALL_SYM_TPL(0)
  PLUGIN_CALL_SYM_TPL(1)
  PLUGIN_CALL_SYM_TPL(2)
  PLUGIN_CALL_SYM_TPL(3)
  PLUGIN_CALL_SYM_TPL(4)
  PLUGIN_CALL_SYM_TPL(5)
  PLUGIN_GET_SYM_TPL(0)
  PLUGIN_GET_SYM_TPL(1)
  PLUGIN_GET_SYM_TPL(2)
  PLUGIN_GET_SYM_TPL(3)
  PLUGIN_GET_SYM_TPL(4)
  PLUGIN_GET_SYM_TPL(5)

private:
  bool _loaded;
  PluginImpl _impl;
};

} // !navi

#endif /* !_PLUGIN_H_ */
