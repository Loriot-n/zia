#include "Main.hpp"
#include "WorkerManager.hpp"
#include "Config.hpp"


using zia::WorkerManager;

int Main::main(const int ac, const std::string *av)
{
    try
    {
      zia::Config d("oui.json");
      zia::api::ConfObject const &object = d.getConf();
      long long const &oui = std::get<long long>(object.at("oui").v);
      std::cout << oui << std::endl;
      d.set("oui", 4ll);
      std::cout << oui << std::endl;
      return 0;
        WorkerManager *workerManager =  new WorkerManager();
        //workerManager.reload();
        workerManager->init();
        workerManager->set(std::string("port") , (long long) 5);
        workerManager->run();

        //Signaux KILL,..
        delete workerManager;
    }
    catch (std::runtime_error const &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}
