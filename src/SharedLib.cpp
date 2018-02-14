#include "SharedLib.hpp"

namespace zia
{
    SharedLib::SharedLib(const std::string &path)
    {
        this->sharedLib = dlopen(path.c_str(), RTLD_LAZY);
        if (this->sharedLib == nullptr)
        {
            std::cerr << dlerror() << std::endl;
            //throw SharedLibException(path, dlerror());
        }
    }

    IModule *SharedLib::load()
    {
        ((Loader)dlsym(this->sharedLib, "load"))();
        return nullptr;
    }

    SharedLib::~SharedLib()
    {
        if (this->sharedLib != nullptr)
        {
            dlclose(this->sharedLib);
        }
    }
}