#include "Main.hpp"
#include "WorkerManager.hpp"

using zia::WorkerManager;

int Main::main(const int ac, const std::string *av)
{
    try
    {
        //Signaux KILL,..
    }
    catch(std::exception & e)
    {
        std::cerr << e.what() << std::endl;
    }
    return (0);
}