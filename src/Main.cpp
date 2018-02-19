#include "Main.hpp"

using namespace zia;


int Main::main(const int ac, const std::string *av)
{

    (void)ac; (void)av;

    api::Conf c;

    Server server(c);
    return (server.run(server.handleRequest));

}