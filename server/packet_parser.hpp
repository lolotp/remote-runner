#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <string>

#define HEADER_LENGTH 4
#define MAX_MESSAGE_LENGTH (4 * (1<<20))

class PacketParser {
public:
    PacketParser() {
        reset();
    }
    virtual ~PacketParser() {}
    void reset() {
        packet_content_.clear();
        parsing_state_ = READ_HEADER;
        num_parsed_bytes_ = 0;
    }
    template<typename BufferType>
    std::size_t parse_message(const BufferType &buffer, std::size_t buffer_pos, std::size_t bytes_left) {
        switch (parsing_state_) {
            case READ_HEADER: {
                int bytes_to_copy = std::min(HEADER_LENGTH, num_parsed_bytes_ + (int)bytes_left) - num_parsed_bytes_;
                memcpy((char*)(&message_length_) + num_parsed_bytes_, &(buffer[buffer_pos]), bytes_to_copy);
                num_parsed_bytes_ += bytes_to_copy;
                if (num_parsed_bytes_ == HEADER_LENGTH) {
                    if (message_length_ > MAX_MESSAGE_LENGTH) {
                        parsing_state_ = INVALID_HEADER;
                    } else {
                        parsing_state_ = READ_BODY;
                        packet_content_.resize(message_length_);
                    }
                }
                return bytes_to_copy;
                break;
            }
            case READ_BODY: {
                int num_content_bytes_parsed = num_parsed_bytes_ - HEADER_LENGTH;
                int bytes_to_copy = std::min(num_content_bytes_parsed + (int)bytes_left, message_length_) - num_content_bytes_parsed;
                memcpy(&(packet_content_[num_content_bytes_parsed]), &(buffer[buffer_pos]), bytes_to_copy);
                num_parsed_bytes_ += bytes_to_copy;
                return bytes_to_copy;
                break;
            }
            default:
                assert(false);
        };
    };
    bool has_complete_packet() const { return num_parsed_bytes_ == message_length_ + HEADER_LENGTH; }
    bool has_parse_error() const { return parsing_state_ == INVALID_HEADER; }
    const std::string &packet_content() const { return packet_content_; }

private:
    enum ParsingState {
        READ_HEADER = 0,
        READ_BODY = 1,
        INVALID_HEADER = 2
    };
    std::string packet_content_;
    ParsingState parsing_state_;
    int message_length_;
    int num_parsed_bytes_;
};
