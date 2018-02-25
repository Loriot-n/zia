#include <csignal>
#include <atomic>
#include "Main.hpp"
#include "ModuleManager.hpp"
#include "Config.hpp"
#include "CheckConfig.hpp"

using namespace zia;

static std::atomic<int> reloadLibs = 0;

int Main::main(const int ac, const std::string *av)
{
    (void)ac; (void)av;

    try
    {
      std::signal(SIGUSR1, [](int signal) { if (signal == SIGUSR1) { reloadLibs.store(1); std::cout << "setting reload libs to 1" << std::endl; }});

      zia::Config config("./conf/Zia.conf");
      Server server(config);
      LibManager &libManager = LibManager::getInstance();
      libManager.loadModulesList(config.get<std::string>("dirModule"));
      server.run([&server, &libManager](api::Net::Raw r, api::NetInfo netInfo) -> void
      {
          std::time_t tt = std::chrono::system_clock::to_time_t(netInfo.time);
          std::cout << "\n" << ctime(&tt) <<  "Request from " << netInfo.ip.str << ":" << netInfo.port << std::endl;

          api::HttpDuplex duplex;
          duplex.info = netInfo;
          duplex.raw_req = r;

	  Config newConfig("./conf/Zia.conf");
	  server.updateConfig(newConfig);
	  std::cout << "reload libs : " << reloadLibs.load() << std::endl;
	  if (reloadLibs.load() != 0)
	    {
	      libManager.loadModulesList(newConfig.get<std::string>("dirModule"));
	      reloadLibs.store(0);
	    }

          ModuleManager m1(libManager);
          if (netInfo.sock->isTLS)
          {
            //m1.load("ssl");
            std::cout << "TLS Connection established" << std::endl;
          }

          m1.load("request");
          m1.load("log");
          //m1.load("httpParser");
          m1.load("file_reader");
          m1.process(duplex);

          ModuleManager m2(libManager);
          m2.load("response");
          m2.process(duplex);

      });
      return (0);
    }
    catch (std::exception const &e)
    {
      std::cerr << "err: " << e.what() << std::endl;
    }
    return 1;
}
