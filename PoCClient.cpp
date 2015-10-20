#include "individual.h"


#include <iostream>
#include <fstream>
#include <string>
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

enum SerializationMode {
    TEXT,
    BINARY
};

std::string zlib_compress(const std::string& input) {
    std::stringstream compressed;
    std::stringstream decompressed;
    decompressed << input;

    boost::iostreams::filtering_streambuf<boost::iostreams::input> out;
    out.push(boost::iostreams::zlib_compressor());
    out.push(decompressed);
    boost::iostreams::copy(out, compressed);

    return compressed.str();
}

std::string serialize_individual(const Individual& indiv,
                                 SerializationMode format) {
    std::string data;
    {
        std::ostringstream archive_stream;
        if (format == TEXT) {
            boost::archive::binary_oarchive archive(archive_stream);
            archive << indiv;

        } else if (format == BINARY) {
            boost::archive::binary_oarchive archive(archive_stream);
            archive << indiv;
        }

        data = archive_stream.str();
    }
    return data;
}

void sendIndividualTo(const std::string& address, const int port,
                      const std::string& individual) {
    try {
        boost::asio::io_service io_service;

        udp::resolver resolver(io_service);
        udp::resolver::query query(udp::v4(), address, std::to_string(port));
        udp::endpoint receiver_endpoint = *resolver.resolve(query);

        udp::socket socket(io_service);

        socket.open(udp::v4());
        socket.send_to(boost::asio::buffer(individual), receiver_endpoint);

        socket.close();

    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

}

void migrate(const Individual& indiv, const std::string& address,
             const int port) {
    auto data = serialize_individual(indiv, BINARY);
    data = zlib_compress(data);
    sendIndividualTo(address, port, data);
}

Individual createTestIndividual(const int nbFares) {
    Fare tmp;

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

    for (int i = 0; i < nbFares; i++) {
        tmpIndiv.listFare.push_back(tmp);
    }

    return tmpIndiv;
}

int main(int argc, const char *argv[]) {
    const int nbFares = 1000;
    const int port = 1337;

    const auto indiv = createTestIndividual(nbFares);
    migrate(indiv, "localhost", port);
    return 0;
}
