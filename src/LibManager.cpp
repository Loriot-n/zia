    #include "LibManager.hpp"

namespace zia
{
    LibManager::LibManager(const std::string &modulesDir)
    {
        this->modulesDir = modulesDir;
        this->getModulesList();
    }

    LibManager &LibManager::getInstance(const Config &serverConfig)
    {

        static LibManager instance(serverConfig.get<std::string>("dirModule"));
        return instance;
    }

    void LibManager::getModulesList()
    {
        this->modulesList.clear();
        this->readDir(this->modulesDir);
    }

    void LibManager::readDir(const std::string &dirName)
    {
        DIR *dir = ::opendir(dirName.c_str());
	if (dir == nullptr)
	  {
	    throw std::runtime_error("Couldn't open modules directory: " + dirName);
	  }
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
}
