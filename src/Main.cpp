#include "Main.hpp"
#include "socket/Reactor.hpp"
#include "socket/Listener.hpp"
#include <thread>
#include <functional>
#include <vector>
#include "api/Net.hpp"

using namespace zia;

void test(api::Net::Raw r, api::NetInfo netInfo) {
    (void)r; (void)netInfo;
    std::cout << "in Callback" << std::endl;

}

int Main::main(const int ac, const std::string *av)
{

    (void)ac; (void)av;

    std::cout << "running..." << std::endl;

    try {
        api::Conf c;
        // Create the socket
        ReactorPtr listenerReactor(new Reactor(256));
        ReactorPtr connectionReactor(new Reactor(256));

        ListenerPtr listener = Listener::create(listenerReactor, connectionReactor, 30000, 8081);
        listener->open();

        api::Net::Raw r;
        api::NetInfo info;
        api::Net::Callback cb = test;

        std::thread listenerThread([&]() {
            listenerReactor->handleEvents(cb);
        });

        std::vector<std::thread> workers;

        for (int i = 0 ; i < 4 ; i++) {

            workers.push_back(std::thread([&]() {
                connectionReactor->handleEvents(cb);
            }));
        }

        listenerThread.join();

        std::for_each(workers.begin(), workers.end(),
            [](std::thread &t) {
                t.join();
            }
        );

    } catch ( SocketException& e ) {
        std::cout << "Exception was caught: " << e.what() << "\nExiting.\n";
    }
    return 0;
}