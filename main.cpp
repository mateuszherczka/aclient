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

    cout << "Client exiting" << endl;
    return 0;
}
