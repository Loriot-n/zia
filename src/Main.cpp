#include "Main.hpp"
#include "WorkerManager.hpp"
#include "ServerSocket.hpp"

using namespace zia;

int Main::main(const int ac, const std::string *av)
{

    std::cout << "running..." << std::endl;

    try {
        api::Conf c;
        // Create the socket
        ServerSocket server (c);
        while (true) {
            ServerSocket new_sock;
            server.accept (new_sock);
            try {
                while (true) {
                    api::Net::Raw data;
                    new_sock >> data;
                    new_sock << data;
                }
            } catch (const SocketException& e) {
                std::cerr << e.what() << std::endl;
            }
        }
    } catch ( SocketException& e ) {
        std::cout << "Exception was caught:" << e.what() << "\nExiting.\n";
    }
    return 0;
}