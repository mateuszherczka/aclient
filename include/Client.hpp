#ifndef CLIENT_H
#define CLIENT_H

//#include <iostream>
//#include <istream>
//#include <ostream>
//#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

//#include <KukaBuildXMLFrame.hpp>
//#include <KukaParseXMLFrame.hpp>
//#include <KukaBuildXMLExample.hpp>
//#include <KukaParseXMLExample.hpp>

#include <KukaSimulatedResponse.hpp>
#include <ComputerSimulatedCommand.hpp>

using boost::asio::ip::tcp;
using std::exception;
using boost::thread;
using std::cout;
using std::endl;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

typedef std::vector<int> info_vec;
typedef std::vector<double> frame_vec;

class Client
{
    public:
        Client() {
            // TODO: Read in prefs from an XML file
            // TODO: Read in build and parse classes from file
        }
        virtual ~Client() {
            closeConnection();
        }

        void session() {};

        /*
        Connect to default.
        */
        void connectToServer() {
            connectToServer(defaultHost,defaultPort);
        }

        /*
        Specify server and port.
        Blocks until connection ends.
        */
        void connectToServer(std::string &server, std::string &port) {

            try {
                // we need one of these
                boost::asio::io_service io_service;

                // get available connenctions
                tcp::resolver resolver(io_service);
                tcp::resolver::query query(server, port);
                tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

                // Try each endpoint until we successfully establish a connection.
                cout << "Client trying to connect to " << server << " on port " << port << "." << endl;
                socket_ptr sock(new tcp::socket(io_service));
                boost::asio::connect(*sock, endpoint_iterator);

                connected = true;

                cout << "Connected to a server." << endl;

                // spawn

                boost::thread read_thread(&Client::readMessage,this, sock);
                boost::thread write_thread(&Client::writeMessage,this, sock);

                // block until threads return
                //read_thread.join();
                //write_thread.join();
            }
            catch (std::exception &e){
                cout << "Client connection exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };

        void closeConnection() {
            // TODO: clean thread interrupt
            connected = false;
            //sock->close();
        };

        bool isConnected() {

            return connected;
        }

    protected:

    private:

        bool connected = false;
        bool doParse = true;

        std::string defaultHost = "localhost"; //std::string defaultHost = "127.0.0.1";
        std::string defaultPort = "6008";
        std::string endString = "</ExternalData>\r\n";
        std::size_t maxBufferSize = 1024;

        //KukaBuildXMLFrame kukaBuildMessage;
        //KukaParseXMLFrame kukaParseMessage;

        //KukaBuildXMLExample kukaBuildMessage;
        //KukaParseXMLExample kukaParseMessage;

        KukaSimulatedResponse kukaBuildMessage;
        ComputerSimulatedCommand kukaParseMessage;

        void readMessage(socket_ptr sock) {
            // TODO: is lock read necessary (probably not)
            cout << "Client read thread started." << endl;
            try {
                int counter = 0;

                while (sock->is_open() && connected) {

                    boost::asio::streambuf message(maxBufferSize); // TODO: streambuf sould not be created here but reused, how?

                    try {   // if endtag isnt found before buffer max reached

                        boost::system::error_code error;
                        boost::asio::read_until(*sock, message, endString, error);

                        if (error == boost::asio::error::eof) {
                            cout << "Server disconnected." << endl;
                            cout << "Received " << counter << " messages." << endl;
                            connected = false;
                            return;
                        }

                        kukaParseMessage.parse(message);

                        if (counter % 100 == 0) {   // print every nth
                            kukaParseMessage.printValues();
                        }

                        ++counter;
                    }
                    catch (std::exception &e){
                        cout << "Reading (no matching xml endtag): " << e.what() << endl;
                    }   // complain but don't quit
                }

            }
            catch (std::exception &e){
                cout << "Reading exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };  // separate thread

        void writeMessage(socket_ptr sock) {
            // TODO: is lock write necessary (probably not)
            cout << "Client write thread started, waiting a little." << endl;
            boost::this_thread::sleep( boost::posix_time::milliseconds(1000) );

            int counter = 0;    // just to change message a little
            try {
                while (sock->is_open() && connected) {
                    // is there something in the write queue?
                    // if so, write

                    // write every n seconds
                    boost::this_thread::sleep( boost::posix_time::milliseconds(20) );

                    // write xml every second
                    boost::asio::streambuf message;

                    int i = counter;
                    info_vec info = {i,i,i,i,i};

                    double j = counter * 0.001;
                    frame_vec frame = {j,j,j,j,j,j};

                    kukaBuildMessage.format(message,info,frame);

                    boost::asio::write(*sock, message);

                    ++counter;
                }

                cout << "Wrote " << counter << " messages." << endl;
            }
            catch (std::exception &e){
                cout << "Writing exception: " << e.what() << endl;
                closeConnection();
                return;
            }
        };     // separate thread

        /*
        Gets a pointer to buffer inside streambuf.
        */
        const char * streambufToPtr(boost::asio::streambuf &message) {
            const char* bufPtr=boost::asio::buffer_cast<const char*>(message.data());
            return bufPtr;
        }


};

#endif // CLIENT_H


//boost::thread read_thread(boost::bind(readMessage, sock));
//boost::thread write_thread(boost::bind(writeMessage, sock));

/*
kukaBuildMessage.setPosXYZ(counter,counter,counter);

                    if (counter % 4 == 2 ) {
                        kukaBuildMessage.buildBrokenFirstHalf(message);

                        boost::asio::write(*sock, message);
                        cout << "Client wrote only first half of message #" << counter << endl;
                    }
                    else if (counter % 4 == 5 ) {
                        kukaBuildMessage.buildBrokenSecondHalf(message);

                        boost::asio::write(*sock, message);
                        cout << "Client wrote only second half of message #" << counter << endl;
                    }
                    else if (counter % 4 == 5 ) {
                        kukaBuildMessage.buildBrokenMalformed(message);

                        boost::asio::write(*sock, message);
                        cout << "Client wrote malformed message #" << counter << endl;
                    }
                    else {
                        kukaBuildMessage.build(message);

                        boost::asio::write(*sock, message);
                        cout << "Client wrote message #" << counter << endl;
                    }
                    */
