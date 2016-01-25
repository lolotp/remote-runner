#pragma once

#include "packet_router.h"

#include <vector>
#include <thread>

#include <boost/asio.hpp>
#include <boost/scoped_array.hpp>

class TcpServer {
public:
    typedef std::shared_ptr<boost::asio::io_service> IoServicePtr;
    TcpServer(int port, unsigned int num_threads = 2);
    virtual ~TcpServer();
    void start();
    void stop();
private:
    void start_accept();
    void init_router();
private:
    int port_;
    unsigned int num_threads_;
    const boost::asio::ip::tcp::endpoint endpoint_;
    boost::asio::io_service acceptor_io_service_;
    boost::scoped_array<boost::asio::io_service> session_io_services_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::vector<boost::asio::ip::tcp::socket> sockets_;
    std::vector<std::thread> threads_;
    int cur_io_service_idx_;

    PacketRouterPtr packet_router_ptr_;
};
