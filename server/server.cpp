#include "server.h"
#include "session.h"
#include "simple_packet_router.h"
#include "processors/echo_processor.h"

#include <cstdio>

using boost::asio::ip::tcp;

TcpServer::TcpServer(int port, unsigned int num_threads) : port_(port),
  num_threads_(num_threads),
  endpoint_(tcp::v4(), port_),
  session_io_services_(new boost::asio::io_service[num_threads]),
  acceptor_(acceptor_io_service_, endpoint_),
  cur_io_service_idx_(0),
  packet_router_ptr_(new SimplePacketRouter()) {

    assert(num_threads > 0);
    start_accept();

    init_router();
}

void TcpServer::init_router() {
    CommandProcessorPtr processor_ptr(new EchoProcessor());
    packet_router_ptr_->register_processor(processor_ptr);
}

void TcpServer::start_accept() {
    auto ptr_session = std::make_shared<Session>(session_io_services_[cur_io_service_idx_]);
    ptr_session->set_packet_router(packet_router_ptr_);
    cur_io_service_idx_ = (cur_io_service_idx_ + 1) % num_threads_;
    acceptor_.async_accept(ptr_session->socket(),
        [this, ptr_session](boost::system::error_code ec)
        {
            if (!ec) {
                ptr_session->start();
            }
            start_accept();
        });
}

TcpServer::~TcpServer() {
    if (!acceptor_io_service_.stopped()) {
        stop();
    }
}

void TcpServer::start() {
    for (unsigned i = 0; i < num_threads_; i++) {
        boost::asio::io_service &io_service = session_io_services_[i];
        threads_.push_back(std::thread([&io_service](){ 
            boost::asio::io_service::work work(io_service);
            io_service.run(); 
        }));
    }
    threads_.push_back(std::thread([this] { acceptor_io_service_.run(); }));
}

void TcpServer::stop() {
    acceptor_io_service_.stop();
    for (unsigned i = 0; i < num_threads_; i++) {
        session_io_services_[i].stop();
    }
    for (std::vector<std::thread>::reverse_iterator it = threads_.rbegin(); it != threads_.rend(); it++) {
        it->join();
    }
    
}
