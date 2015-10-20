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

#define UDP_DGRAM_MAX_SIZE 65507


using boost::asio::ip::udp;

namespace island {
class island_udp_server {
  public:
    island_udp_server(boost::asio::io_service& io_service, const int port)
        : socket_(io_service, udp::endpoint(udp::v4(), port)) {
        start_receive();
    }

    void start_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(recv_buffer_), remote_endpoint_,
            boost::bind(&island_udp_server::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));

    }

    void handle_receive(const boost::system::error_code& error,
                        std::size_t bytes_transferred) {
        const std::string serialized_compressed_indiv(recv_buffer_.begin(),
                recv_buffer_.begin() + bytes_transferred);


        const std::string serialized_indiv = zlib_decompress(
                serialized_compressed_indiv);
        Individual indiv;

        if (!error || error == boost::asio::error::message_size) {
            indiv = individual_deserialize(serialized_indiv);
            start_receive();
        }
    }

  private:

    std::string zlib_decompress(const std::string& input) {
        std::stringstream compressed;
        std::stringstream decompressed;

        compressed << input;

        boost::iostreams::filtering_streambuf<boost::iostreams::input> in;
        in.push(boost::iostreams::zlib_decompressor());
        in.push(compressed);

        boost::iostreams::copy(in, decompressed);
        return decompressed.str();
    }

    Individual individual_deserialize(const std::string& serialize_indiv) {
        Individual result;
        {
            std::istringstream archive_stream(serialize_indiv);
            boost::archive::binary_iarchive archive(archive_stream);
            archive >> result;
        }

        return result;
    }

  private:
    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, UDP_DGRAM_MAX_SIZE> recv_buffer_;
};
}
