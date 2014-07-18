#ifndef COMPUTERSIMULATEDCOMMAND_H
#define COMPUTERSIMULATEDCOMMAND_H

#include <vector>
#include <XMLParser.hpp>


using boost::lexical_cast;
using boost::bad_lexical_cast;

class ComputerSimulatedCommand : public XMLParser
{
    public:

        // ---------------------------------------------------------------------------
        // Constructor / Destructor
        // ---------------------------------------------------------------------------

        ComputerSimulatedCommand() : info(4,0), frame(6,0) {}

        virtual ~ComputerSimulatedCommand() {}

        // ---------------------------------------------------------------------------
        // Public Methods
        // ---------------------------------------------------------------------------

        // parses and stores internally
        void parse(boost::asio::streambuf &message) {

            // try to parse
            errorState = doc.Parse(streambufToPtr(message));

            if (errorState != 0) {  // something wrong with xml
                malformedXMLError("ComputerSimulatedCommand XMLDocument" );
                return;
            }

            // doc could parse message, try to get data
            valid = true;
            XMLHandle docHandle( &doc );
            XMLText *xmltext;

            // Mode
            xmltext = docHandle.FirstChildElement( "ExternalData" )
                        .FirstChildElement( "Mode" )
                        .FirstChild()   // this is a text, not an element
                        .ToText();

            if( xmltext ) {
                try {
                    info[0] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Mode");
                }
            }
            else { nodeNotFound("Mode"); }

            // Tick
            xmltext = docHandle.FirstChildElement( "ExternalData" )
                        .FirstChildElement( "Tick" )
                        .FirstChild()   // this is a text, not an element
                        .ToText();

            if( xmltext ) {
                try {
                    info[1] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Tick");
                }
            }
            else { nodeNotFound("Tick"); }

            // Id
            xmltext = docHandle.FirstChildElement( "ExternalData" )
                        .FirstChildElement( "Id" )
                        .FirstChild()   // this is a text, not an element
                        .ToText();

            if( xmltext ) {
                try {
                    info[2] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Id");
                }
            }
            else { nodeNotFound("Id"); }

            // Run
            xmltext = docHandle.FirstChildElement( "ExternalData" )
                        .FirstChildElement( "Run" )
                        .FirstChild()   // this is a text, not an element
                        .ToText();

            if( xmltext ) {
                try {
                    info[3] = boost::lexical_cast<int>(xmltext->Value());
                }
                catch (const boost::bad_lexical_cast &e) {
                    badCast("Run");
                }
            }
            else { nodeNotFound("Run"); }

            // ActPos
            XMLElement *xframe = docHandle.FirstChildElement( "ExternalData" ).
                        FirstChildElement( "XFrame" ).
                        ToElement();

            if ( xframe ) {    // check for nulpointer

                int xmlerr = 0;

                xmlerr += xframe->QueryDoubleAttribute( "XPos",&frame[0] );    // errorcode 0 if ok

                xmlerr += xframe->QueryDoubleAttribute( "YPos",&frame[1] );

                xmlerr += xframe->QueryDoubleAttribute( "ZPos",&frame[2] );

                xmlerr += xframe->QueryDoubleAttribute( "ARot",&frame[3] );    // errorcode 0 if ok

                xmlerr += xframe->QueryDoubleAttribute( "BRot",&frame[4] );

                xmlerr += xframe->QueryDoubleAttribute( "CRot",&frame[5] );

                if (xmlerr != 0) {
                    malformedXMLError("XFrame Attributes");
                }
            }
            else {  nodeNotFound("XFrame"); }
        }

        /*
            Status Error Mode Tick Id
        */
        std::vector<int> getInfo() {
            return info;
        }

        /*
            X Y Z A B C
        */
        std::vector<double> getFrame() {
            return frame;
        }

        void printValues() {

            cout << "Info: ";
            for (const auto &val : info) {
                cout << val << " ";
            }
            cout << endl;

            cout << "Frame: ";
            for (const auto &val : frame) {
                cout << val << " ";
            }
            cout << endl;
        }

    protected:

    private:

        // ---------------------------------------------------------------------------
        // Private Data
        // ---------------------------------------------------------------------------

        std::vector<int> info;
        std::vector<double> frame;

};



#endif // COMPUTERSIMULATEDCOMMAND_H
