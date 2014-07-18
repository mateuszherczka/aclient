#include <iostream>
#include <Client.hpp>

using std::cout;
using std::endl;

int main()
{
    std::string ahost = "localhost"; //std::string defaultHost = "127.0.0.1";
    std::string aport = "6008";

    Client aclient;
    aclient.connectToServer(ahost, aport);

    while (aclient.isConnected()) {

        boost::this_thread::sleep( boost::posix_time::milliseconds(100) );
    }

    cout << "Client exiting" << endl;
    return 0;
}
