#include "session.h"

#include <boost/bind.hpp>

using boost::asio::ip::tcp;

namespace {
    template <typename BufferType>
    unsigned int parseMessageSize(BufferType buffer) {
    };
};

Session::Session(boost::asio::io_service &io_service) : session_data_(io_service) {
}

Session::~Session() {
    printf("session closed\n");
}

void Session::start() {
    session_data_.socket().async_read_some(boost::asio::buffer(buffer_),
        boost::bind(&Session::handle_read, shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
}

void Session::handle_read(const boost::system::error_code& ec,
    std::size_t bytes_transferred) {
    if (!ec) {
        std::size_t buffer_pos = 0;
        while (bytes_transferred > 0) {
            auto bytes_consumed = packet_parser_.parse_message(buffer_, buffer_pos, bytes_transferred);
            buffer_pos += bytes_consumed;
            bytes_transferred -= bytes_consumed;
            if (packet_parser_.has_parse_error()) {
                return;
            }
            if (packet_parser_.has_complete_packet()) {
                printf("%s\n", packet_parser_.packet_content().c_str());
                if (packet_router_ptr_.get()) {
                    std::string reply;
                    bool ok = packet_router_ptr_->process_packet(session_data_, packet_parser_.packet_content(), reply);
                    if (!ok) {
                        return;
                    }
                    int reply_length = reply.length();
                    reply = std::string((const char*)(&reply_length), sizeof reply_length) + reply;
                    boost::asio::async_write(session_data_.socket(),
                        boost::asio::buffer(reply),
                        boost::bind(&Session::handle_write, shared_from_this(),
                                        boost::asio::placeholders::error));
                }
                packet_parser_.reset();
            }
        }
        session_data_.socket().async_read_some(boost::asio::buffer(buffer_),
            boost::bind(&Session::handle_read, shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
}

void Session::handle_write(const boost::system::error_code& ec) {
    if (ec) {
        session_data_.socket().close();
    }
}

void Session::set_packet_router(PacketRouterPtr packet_router_ptr) {
    packet_router_ptr_ = packet_router_ptr;
}
