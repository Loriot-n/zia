#include <iostream>
#include "DynLib.hpp"

DynLib::DynLib(std::string const &name)
  : handle(nullptr),
#ifdef __linux__
    flags(RTLD_LAZY),
#endif
    name(name)
{
}

DynLib::DynLib(DynLib &&o)
  : handle(o.handle),
#ifdef __linux__
    flags(o.flags),
#endif
    name(o.name)
{
  o.handle = nullptr;
  o.handle = nullptr;
}

DynLib::~DynLib()
{
  if (handle)
    {
      std::cout << "Closing " << name << std::endl;
      dlclose(handle);
    }
}

void DynLib::load()
{
  std::cout << "Loading '" << name << "'" << std::endl;
#ifdef __linux__
  handle = dlopen(name.c_str(), flags);
#elif _WIN32
  handle = LoadLibrary(name.c_str());
#endif
  if (!handle)
    throw std::runtime_error("couldn't open dynamic library " + name);
}

#ifdef __linux__
void DynLib::setFlags(int flags)
{
  this->flags = flags;
}
#endif

bool DynLib::isLoaded() const
{
  return handle;
}

bool DynLib::hasGoodExtension(std::string const &file)
{
  return file.substr(file.find_last_of(".") + 1)
#ifdef __linux__
        == "so";
#elif _WIN32
        == "dll";
#endif
}
