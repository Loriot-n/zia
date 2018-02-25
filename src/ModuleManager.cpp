#include "ModuleManager.hpp"


namespace zia
{
    ModuleManager::ModuleManager(LibManager &manager) : libManager(manager)
    {}

    void ModuleManager::load(const std::string &name)
      {
	using create_t = IModule *(*)();
	try
	  {
	    std::pair<std::string const, DynLib> &libPair = libManager.at(name);
	    libPair.second.load();
	    create_t create = libPair.second.resolve<create_t>("create");
	    IModule *imodule = create();
	    imodule->config(Config(libPair.first + ".conf").getConf());
	    this->modules.emplace_back(imodule);
	  }
	catch (std::out_of_range const &e)
	  {
	    std::cerr << "Module '" << name << "' not found" << std::endl;
	  }
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
