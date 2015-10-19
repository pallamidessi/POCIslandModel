#include "individual.h"


#include <iostream>
#include <fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/copy.hpp>
using boost::asio::ip::udp;

int main(int argc, const char *argv[]) {
    try {
        boost::asio::io_service io_service;

        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), "localhost", "1337");
        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        udp::socket socket(io_service);
        socket.open(udp::v4());
        Fare tmp;
        Fare tmp2;
        tmp.journeyId = 1;
        tmp.plannedElementIdx = 2;
        tmp.crewIdx = 3;
        tmp.timestamp = 4;
        tmp.typeVehicle = 5;
        tmp.price = 6;
        tmp.realTimestamp = 7;
        tmp.employeeIdx[0] = 8;
        tmp.employeeIdx[1] = 9;
        tmp.crewSize = 10;
        Individual tmpIndiv;
        Individual tmpIndiv2;

        for (int i = 0; i < 1000; i++) {
            tmpIndiv.listFare.push_back(tmp);
        }

        std::string data;
        {
            std::ostringstream archive_stream;
            boost::archive::binary_oarchive archive(archive_stream);
            archive << tmpIndiv;
            data = archive_stream.str();
        }

        std::stringstream compressed;
        std::stringstream decompressed;
        decompressed << data;

        boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
        out.push(boost::iostreams::zlib_compressor());
        out.push(decompressed);
        boost::iostreams::copy(out, compressed);
        data = compressed.str();

        socket.send_to(boost::asio::buffer(data), receiver_endpoint);

        socket.close();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
