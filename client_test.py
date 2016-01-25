#!/usr/bin/env python

import socket
import struct


TCP_IP = '127.0.0.1'
TCP_PORT = 8812
BUFFER_SIZE = 1024

def generate_packet_for_string(s):
    return struct.pack('i%ds' % len(s), len(s), s)

message = generate_packet_for_string('\1message 1')
message += generate_packet_for_string('\1message 2')

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))
s.send(message)
s.send(generate_packet_for_string('\1message 3'))
data = s.recv(BUFFER_SIZE)
s.close()

print "received data:", data
