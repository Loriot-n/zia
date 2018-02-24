#include "ModuleManager.hpp"


namespace zia
{
    ModuleManager::ModuleManager(LibManager &manager) : libManager(manager)
    {}

    void ModuleManager::load(const std::string &name)
    {
        using create_t = IModule *(*)();
        DynLib &lib = this->libManager.modulesList.at(name).second;
        std::string const configFile = this->libManager.modulesList.at(name).first + ".conf";
        lib.load();
        create_t create = lib.resolve<create_t>("create");
        IModule *imodule = create();
        imodule->config(Config(configFile).getConf());
        this->modules.emplace_back(imodule);
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
