#pragma once

#include "packet_router.h"

#include <map>

class SimplePacketRouter : public PacketRouter {
public:
    virtual void register_processor(CommandProcessorPtr processor_ptr);
    virtual bool process_packet(const SessionData &session_data, const std::string &packet_content, std::string &reply);
private:
    std::map<uint8_t, CommandProcessorPtr> processors_map;
};
