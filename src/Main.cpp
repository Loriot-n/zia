#include "Main.hpp"
#include "WorkerManager.hpp"
#include "ServerSocket.hpp"
#include "ModuleManager.hpp"


using namespace zia;

int Main::main(const int ac, const std::string *av)
{
    try
    {
        zia::ModuleManager *md = new zia::ModuleManager("./modules");
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
