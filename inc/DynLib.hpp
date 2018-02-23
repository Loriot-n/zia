#pragma once

#include <memory>
#include <stdexcept>
#include <sstream>
#include <unordered_map>

#include <iostream>

#if __linux__
# include <dlfcn.h>
#elif _WIN32
# include <windows.h>
#endif

class DynLib
{
private:
#ifdef __linux__
  static constexpr auto loadSym = dlsym;
  void *handle;
  int flags;

public:
  enum Flags
  {
    LAZY = RTLD_LAZY,
    NOW = RTLD_NOW,
    GLOBAL = RTLD_GLOBAL,
    LOCAL = RTLD_LOCAL,
    NODELETE = RTLD_NODELETE,
    NOLOAD = RTLD_NOLOAD,
    DEEPBIND = RTLD_DEEPBIND,
  };
private:
#elif _WIN32
  static constexpr auto loadSym = GetProcAddress;
  HINSTANCE handle;
#endif

public:
  DynLib(std::string const &);
  DynLib(DynLib &&);
  ~DynLib();

  std::string const name;

  void load();
  bool isLoaded() const;
  static bool hasGoodExtension(std::string const &file);

#ifdef __linux__
  void setFlags(int);
#endif

  template <class T>
  T resolve(char const *symbol)
  {
    if (!handle)
      load();
    T sym(reinterpret_cast<T>(loadSym(handle, symbol)));

    if (!sym)
      throw std::runtime_error(dlerror());
    return sym;
  }
};
