#pragma once

#include <boost/asio.hpp>

class SessionData {
public:
    SessionData(boost::asio::io_service &io_service) : socket_(io_service) {}
    boost::asio::ip::tcp::socket &socket() { return socket_; }
private:
    boost::asio::ip::tcp::socket socket_;
};
