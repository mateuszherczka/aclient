#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <tinyxml2.h>

using namespace tinyxml2;
using std::cout;
using std::endl;

/*
Base.
*/
class XMLParser
{
    public:

        const int MALFORMED_XML_ERROR = 666;

        XMLParser() {}
        virtual ~XMLParser() {}

        bool isValid() { return valid; }

    protected:

        int errorState = 0;
        XMLDocument doc;        // tinyxml
        bool valid = true; // this is set to false if there is a malformed XML error

        /*
        Gets pointer to buffer inside streambuf.
        */
        const char * streambufToPtr(boost::asio::streambuf &message) {
            const char* bufPtr=boost::asio::buffer_cast<const char*>(message.data());
            return bufPtr;
        }

        void malformedXMLError(const char *e) {
            cout << "XML malformed at: " << e << endl;
            errorState = MALFORMED_XML_ERROR;
            valid = false;
        }

        void nodeNotFound(const char *e) {
            cout << "Node not found, value not changed: " << e << endl;
        }

        void badCast(const char *e) {
            cout << "Bad cast exception, value not changed: " << e << endl;
            valid = false;
        }


    private:

};

#endif // XMLPARSER_H
