#include "simple_packet_router.h"

void SimplePacketRouter::register_processor(CommandProcessorPtr processor_ptr) {
    assert(processor_ptr.get() != NULL);
    processors_map[processor_ptr->get_command_code()] = processor_ptr;
}

bool SimplePacketRouter::process_packet(const SessionData& session_data, const std::string &packet_content, std::string &reply) {
    if (packet_content.length() <= 0) {
        return false;
    }
    uint8_t command_code = packet_content[0];
    auto processors_map_it = processors_map.find(command_code);
    if (processors_map_it == processors_map.end()) {
        return false;
    }
    const char* content = (packet_content.length() <= 1) ? NULL : &packet_content[1];
    reply = processors_map_it->second->process(session_data, content, packet_content.length() - 1);
    return true;
};
