#pragma once

#include "command_processor.h"

class EchoProcessor : public CommandProcessor {
public:
    virtual uint8_t get_command_code() { return 1; }
    virtual std::string process(const SessionData& session_data, const char* content, size_t length) {
        return std::move(std::string(content, length));
    }
};
