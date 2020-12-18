#pragma once

#include <iostream>
#include <stdint.h>
#include <string.h>
#include <mutex>

using namespace std;

void get_mac_address(char* node_mac, char* dev);

int capture_packet();

void parse_packet(const u_char* packet);

void make_payload(std::mutex& mutex);
