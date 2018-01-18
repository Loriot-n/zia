#include "Main.hpp"

int main(int argc, char **argv)
{
    int i = -1;
    std::string *av = new std::string[argc];
    
    while (++i < argc)
    {
        av[i] = std::string(argv[i]);
    }

    return Main::main(argc, av);
}