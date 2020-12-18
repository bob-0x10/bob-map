#include "integrated.h"
#include <thread>

char* dev;
char* server_url;
char node_mac[17];

void usage() {
    printf("syntax: bob0x10_Core <interface> <server_url>\n");
    printf("sample: bob0x10_Core wlan0 http://127.0.0.1:5000\n");
}

int main(int argc, char* argv[]){
    if (argc != 3) {
        usage();
        return -1;
    }

    cout << "bob-0x10 attendance-node program start" << endl;
    cout << "======================================\n" << endl;

    dev = argv[1];
    server_url = argv[2];

    get_mac_address(node_mac, dev);

    std::mutex mutex;
    thread t1(capture_packet);
    thread t2(make_payload, ref(mutex));

    t1.join();
    t2.join();
}
