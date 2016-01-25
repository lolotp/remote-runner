#pragma once

#include "packet_parser.hpp"
#include "packet_router.h"
#include "session_data.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>

class Session : public std::enable_shared_from_this<Session>,
   private boost::noncopyable { 
public:
    Session(boost::asio::io_service&);
    virtual ~Session();
    void start();
    void handle_read(const boost::system::error_code& ec, std::size_t bytes_transferred);
    void handle_write(const boost::system::error_code& ec);
    boost::asio::ip::tcp::socket &socket() { return session_data_.socket(); }
    void set_packet_router(PacketRouterPtr packet_router_ptr);
private:
    boost::array<char, 8192> buffer_;
    PacketParser packet_parser_;
    SessionData session_data_;
    PacketRouterPtr packet_router_ptr_;
};

typedef std::shared_ptr<Session> SessionPtr;
