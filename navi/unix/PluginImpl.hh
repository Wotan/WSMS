#ifndef _PLUGINIMPL_H_
# define _PLUGINIMPL_H_

# include <dlfcn.h>

class PluginImpl {
public:
  PluginImpl() {}
  virtual ~PluginImpl() { }

  bool open(std::string const& name)
  {
#if defined(MAC_PLATEFORM)
    std::string realName = name + ".dylib";
#elif defined(UNIX_PLATEFORM)
    std::string realName = name + ".so";
#endif
    _handle = dlopen(realName.c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (_handle)
      dlerror();
    return _handle ? true : false;
  }

  void* getSym(std::string const& name)
  {
    return dlsym(_handle, name.c_str());
  }

  char const* getError()
  {
    return dlerror();
  }

  bool close()
  {
    return (dlclose(_handle) == 0);
  }

private:
  void* _handle;
};

#endif /* !_PLUGINIMPL_H_ */
