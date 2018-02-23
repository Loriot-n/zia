#include "ModuleManager.hpp"


namespace zia
{
    ModuleManager::ModuleManager(const std::string &modulesDir)
    {
        this->modulesDir = modulesDir;
        this->getModulesList();
    }

    ModuleManager &ModuleManager::getInstance()
    {

        static ModuleManager instance("./modules");
        return instance;
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
                    std::string const file(read->d_name);
                    if (file.find(".so") != std::string::npos)
                    {
                        std::string const name = file.substr(0, file.length() - 3);
                        std::cout << name << std::endl;
			std::string const filepath = dirName + "/" + name;
                        this->modulesList.emplace(std::make_pair(name,
								 std::make_pair(filepath,
								 DynLib(filepath + ".so"))));
                    }
                }
            }
        }
        ::closedir(dir);
    }

    void ModuleManager::load(const std::string &name)
    {
	using create_t = IModule *(*)();
	DynLib &lib = modulesList.at(name).second;

	lib.load();
	create_t create = lib.resolve<create_t>("create");
        this->modules.emplace_back(create());
	Config config("conf/Zia.conf");
	modules.back()->config(config.getConf());
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
        this->modules.clear();
    }
}
