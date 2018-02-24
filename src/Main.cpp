#include "Main.hpp"
#include "ModuleManager.hpp"
#include "Config.hpp"
#include "CheckConfig.hpp"

using namespace zia;

int Main::main(const int ac, const std::string *av)
{
    (void)ac; (void)av;

    zia::Config config("./conf/Zia.conf");

    try 
    {
      Server server(config);
      server.run([config](api::Net::Raw r, api::NetInfo netInfo) -> void
      {
          std::time_t tt = std::chrono::system_clock::to_time_t(netInfo.time);
          std::cout << "\n" << ctime(&tt) <<  "Request from " << netInfo.ip.str << ":" << netInfo.port << std::endl;

          api::HttpDuplex duplex;
          duplex.info = netInfo;
          duplex.raw_req = r;
          

          ModuleManager m1(LibManager::getInstance(config));
          if (netInfo.sock->isTLS)
          {
            //m1.load("ssl");
            std::cout << "TLS Connection established" << std::endl;			
          }
          //m1.load("httpParser");
          m1.load("file_reader");
          m1.process(duplex);

          ModuleManager m2(LibManager::getInstance(config));
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
