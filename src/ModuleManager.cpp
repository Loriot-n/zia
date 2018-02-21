#include "ModuleManager.hpp"


namespace zia
{
    ModuleManager::ModuleManager(const std::string &modulesDir)
    {
        this->modulesDir = modulesDir;
        this->getModulesList();
    }

    void ModuleManager::getModulesList()
    {
        this->modulesList.clear();
        this->readDir(this->modulesDir);
    }

    void ModuleManager::readDir(const std::string &dirName)
    {
        DIR *dir = ::opendir(dirName.c_str());
        t_dir read;
        while((read = ::readdir(dir)) != nullptr)
        {
            if (read->d_name[0] != '.')
            {
                if (read->d_type == DT_DIR)
                {
                    this->readDir(dirName + "/" + std::string(read->d_name));
                }
                else
                {
                    std::string file(read->d_name);
                    if (file.find(".so") != std::string::npos)
                    {
                        std::string name = file.substr(0, file.length() - 3);
                        std::cout << name << std::endl;
                        this->modulesList[name] = dirName + "/" + file;
                    }
                }
            }
        }
        ::closedir(dir);
    }

    void ModuleManager::load(const std::string &name)
    {
	using create_t = IModule *(*)();
	DynLib &lib = libs.emplace_back(modulesList[name]);

	lib.load();
	create_t create = lib.resolve<create_t>("create");
        this->modules.emplace_back(create());
    }

    void ModuleManager::process(HttpDuplex &duplex)
    {
        this->modules.sort([](std::unique_ptr<IModule> const &a, std::unique_ptr<IModule> const &b)
        {
            return a->getPriority() > b->getPriority();
        });

        for (auto &&module : this->modules)
        {
            module->exec(duplex);
        }
    }
}
