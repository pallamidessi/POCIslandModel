#include <iostream>
#include <cstdlib>
#include "individual.h"

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/copy.hpp>
using boost::asio::ip::udp;

class udp_server {
  public:
    udp_server(boost::asio::io_service& io_service)
        : socket_(io_service, udp::endpoint(udp::v4(), 1337)) {
        start_receive();
    }

    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&udp_server::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));

    }

    void handle_receive(const boost::system::error_code& error,
                        std::size_t bytes_transferred) {
        std::string serializedIndiv(recv_buffer_.begin(),
                                    recv_buffer_.begin() + bytes_transferred);


        std::stringstream compressed;
        std::stringstream decompressed;
        compressed << serializedIndiv;
        boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
        in.push(boost::iostreams::zlib_decompressor());
        in.push(compressed);
        boost::iostreams::copy(in, decompressed);
        serializedIndiv = decompressed.str();
        Individual tmpIndiv;

        {
            std::istringstream archive_stream(serializedIndiv);
            boost::archive::binary_iarchive archive(archive_stream);
            archive >> tmpIndiv;
        }

        if (!error || error == boost::asio::error::message_size) {
            std::cout << "Got it yep" << std::endl;
            std::cout << tmpIndiv.listFare[723].price << std::endl;
            if (tmpIndiv.listFare[723].crewIdx = -1)

                start_receive();
        }
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 65507> recv_buffer_;
};

int main(int argc, const char *argv[]) {

    try {
        boost::asio::io_service io_service;
        udp_server server(io_service);
        io_service.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
