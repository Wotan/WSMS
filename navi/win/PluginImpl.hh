#ifndef _PLUGINIMPL_H_
# define _PLUGINIMPL_H_

namespace {

class PluginImpl {
public:
  PluginImpl() {}
  virtual ~PluginImpl() { }
  
  bool open(std::string const& name)
  {
    std::string realName = name + ".dll";
    _handle = LoadLibraryA(realName.c_str());
    if (_handle) {
  int error = GetLastError();
	fprintf(stderr, strerror(error));
	}
    return _handle ? true : false;
  }

  void* getSym(std::string const& name)
  {
	 return GetProcAddress(_handle, name.c_str());
  }

  char const* getError()
  {
	int error;
  
	error = GetLastError();
	if (error != 0)
		return strerror(error);
	else
		return NULL;
  }

  bool close()
  {
	  return FreeLibrary(_handle);
  }

private:
  HMODULE _handle;
};

} // !unamed

#endif /* !_PLUGINIMPL_H_ */
