#pragma once

#include "processors/command_processor.h"
#include "session_data.h"

class PacketRouter {
public:
    virtual void register_processor(CommandProcessorPtr processor_ptr) = 0;
    virtual bool process_packet(const SessionData &session_data, const std::string &packet_content, std::string &reply) = 0;
};

typedef std::shared_ptr<PacketRouter> PacketRouterPtr;
