#include "Main.hpp"
#include "ModuleManager.hpp"
#include "Config.hpp"
#include "CheckConfig.hpp"

using namespace zia;

int Main::main(const int ac, const std::string *av)
{
    (void)ac; (void)av;

    api::Conf c;

    try {
        Server server(c);
        return (server.run(server.handleRequest));
    } catch (std::exception const &e) {
        std::cerr << "err: " << e.what() << std::endl;
    }
    return 1;
}
