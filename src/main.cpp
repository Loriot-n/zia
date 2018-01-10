#include "WorkerManager.hpp"

using zia::WorkerManager;
int main(int ac, char **av)
{
    try
    {
        WorkerManager *workerManager =  new WorkerManager();
        //workerManager.reload();
        workerManager->init();
        workerManager->set(std::string("port") , (long long) 5);
        workerManager->run();
    
        //Signaux KILL,..
        delete workerManager;
    }
    catch(std::exception e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return (0);
}