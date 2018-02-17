#include "Main.hpp"
#include "WorkerManager.hpp"
#include "ServerSocket.hpp"
#include "ModuleManager.hpp"


using namespace zia;

int Main::main(const int ac, const std::string *av)
{
    try
    {
        zia::ModuleManager md("./modules");
	md.load("php-cgi");
	HttpDuplex dup;
	md.process(dup);
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
