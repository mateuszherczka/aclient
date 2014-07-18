#ifndef KUKASIMULATEDRESPONSE_H
#define KUKASIMULATEDRESPONSE_H


class KukaSimulatedResponse
{
    public:

        KukaSimulatedResponse() {}

        virtual ~KukaSimulatedResponse() {}

        /*
            mesage: a streambuf to fill
            info:   Status Error Mode Tick Id
            frame:  ActPos X Y Z A B C
        */
        void format( boost::asio::streambuf &message, const std::vector<int> &info , const std::vector<double> &frame) {
            // TODO: rewrite this for new data structure - vector< framevector<double> >
            std::ostream to_message_stream(&message);


            to_message_stream   << "<Robot>\r\n";

            to_message_stream   << "<Status>" << info[0] << "</Status>\r\n";
            to_message_stream   << "<Error>" << info[1] << "</Error>\r\n";
            to_message_stream   << "<Mode>" << info[2] << "</Mode>\r\n";
            to_message_stream   << "<Tick>" << info[3] << "</Tick>\r\n";
            to_message_stream   << "<Id>" << info[4] << "</Id>\r\n";

            to_message_stream   << "<ActPos X=\"" << frame[0] << "\" "
                                << "Y=\"" << frame[1] << "\" "
                                << "Z=\"" << frame[2] << "\" "
                                << "A=\"" << frame[3] << "\" "
                                << "B=\"" << frame[4] << "\" "
                                << "C=\"" << frame[5] << "\" />\r\n";

            to_message_stream   << "</Robot>\r\n";
        }

    protected:

    private:

};

#endif // KUKASIMULATEDRESPONSE_H
