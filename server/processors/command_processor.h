#pragma once

#include "../session_data.h"

class CommandProcessor {
public:
    virtual uint8_t get_command_code() = 0;
    virtual std::string process(const SessionData& session_data, const char* content, size_t length) = 0;
};

typedef std::shared_ptr<CommandProcessor> CommandProcessorPtr;
