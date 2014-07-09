#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio;
using std;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

class Client
{
    public:
        Client() {

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
        void connectToServer(const char *server, unsigned short port) {

            try {
                // we need one of these
                asio::io_service io_service;

                //tcp::acceptor a(io_service, tcp::endpoint(tcp::v4(), port));

                // get available connenctions
                tcp::resolver resolver(io_service);
                tcp::resolver::query query(server, port);
                tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

                // Try each endpoint until we successfully establish a connection.
                cout << "Client trying to connect to " << server << " on port " << port << "." << endl;
                socket_ptr socket(new tcp::socket(io_service));
                asio::connect(*socket, endpoint_iterator);

                connected = true;

                // spawn
                boost::thread read_thread(readMessage, sock);
                boost::thread write_thread(writeMessage, sock);

                // block until threads return
                read_thread.join();
                write_thread.join();


            }
            catch (exception &e){
                cout << "Client connection exception: " << e.what() << endl;
                socket.close();
                connected = false;
                return;
            }
        };

        void closeConnection() {
            // TODO: clean thread interrupt
            connected = false;
            socket.close();
        };

    protected:
    private:

        bool connected = false;

        std::string defaultHost = "localhost"; //std::string defaultHost = "127.0.0.1";
        std::string defaultPort = "10001";

        KukaBuildXMLFrame kukaBuildMessage;


        void readMessage(socket_ptr socket) {
            // TODO: is lock read necessary (probably not)
            try {
                while (*socket.is_open() && connected) {
                    boost::asio::streambuf message; // TODO: streambuf sould not be created here but reused, how?
                    boost::asio::read_until(socket, response, "</Rob>\r\n");
                }
            }
            catch (exception &e){
                cout << "Reading exception: " << e.what() << endl;
                socket.close();
                connected = false;
                return;
            }
        };  // separate thread

        void writeMessage(socket_ptr socket) {
            // TODO: is lock write necessary (probably not)
            int counter = 0;    // just to change message a little
            try {
                while (*socket.is_open() && connected) {
                    // is there something in the write queue?
                    // if so, write

                    boost::this_thread::sleep( boost::posix_time::seconds(1) );

                    // write xml every second
                    boost::asio::streambuf message;
                    kukaFrame.build(message,1+counter,2+counter,3+counter,4+counter,5+counter,6+counter);
                    boost::asio::write(*socket, message);

                    ++counter;
                }
            }
            catch (exception &e){
                cout << "Writing exception: " << e.what() << endl;
                socket.close();
                connected = false;
                return;
            }
        };     // separate thread




};

#endif // CLIENT_H


//boost::thread read_thread(boost::bind(readMessage, sock));
//boost::thread write_thread(boost::bind(writeMessage, sock));
