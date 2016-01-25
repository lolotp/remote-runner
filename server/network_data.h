#pragma once

#include "packet_parser.hpp"

namespace NetworkData {
    PacketParser packet_parser_;
    /**
     * PacketProcessFunc is a callable of form void(const std::string &packet_content)
     * SocketContinuactionFunc is a callable of form void(void)
     */
    template <typename BufferType, typename PacketProcessFunc, typename SocketContinuationFunc>
    void process_socket_read_result(const boost::system::error_code &ec, 
        BufferType buffer, std::size_t bytes_transferred, 
        PacketProcessFunc process_packet, SocketContinuationFunc continue_socket_activity) {
        if (!ec) {
            std::size_t bufferpos = 0;
            while (bytes_transferred > 0) {
                auto bytes_consumed = packet_parser_.parse_message(buffer, bufferpos, bytes_transferred);
                bufferpos += bytes_consumed;
                bytes_transferred -= bytes_consumed;
                if (packet_parser_.has_parse_error()) {
                    return;
                }
                if (packet_parser_.has_complete_packet()) {
                    process_packet(packet_parser_.packet_content());
                    packet_parser_.reset();
                }
            }
            continue_socket_activity();
        }
    }
};
