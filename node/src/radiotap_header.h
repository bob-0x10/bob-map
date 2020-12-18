#include <stdint.h>

#pragma once

#define IEEE_LEN 24

//https://www.semfionetworks.com/uploads/2/9/8/3/29831147/wireshark_802.11_filters_-_reference_sheet.pdf
#define AUTHENTICATION 0xb0
#define ACTION 0xd0
#define NULL_FUNCTION 0x48
#define PROBE_REQUEST 0x40
#define QOS_DATA 0x88
#define QOS_NULL_FUNCTION 0xc8


struct mac_key{     //BSSID
    uint64_t mac:48;
} __attribute__((__packed__));

bool operator<(mac_key const& m1, mac_key const& m2){
    return m1.mac < m2.mac;
}


//radiotap_header is change depending on the adapter device

//Adapter model name : AWUS036NH(ALFA), PAU09(PANDA)
//length : 18
struct radiotap_header {
        uint8_t        it_version;          /* set to 0 */
        uint8_t        it_pad;
        uint16_t       it_length;           /* entire length */
        uint32_t       it_present_flags;    /* fields present */
        uint8_t        it_flags;
        uint8_t        it_data_Rate;
        uint16_t       it_channel_frequency;
        uint16_t       it_channel_flags;
        uint8_t        it_antenna_signal;
        uint8_t        it_antenna;
        uint16_t       it_RX_flags;
};

//Adapter model : iptime
//length : 24
struct radiotap_header_IPTIME {
        uint8_t        it_version;          /* set to 0 */
        uint8_t        it_pad;
        uint16_t       it_length;           /* entire length */
        uint64_t       it_present_flags;    /* fields present */
        uint8_t        it_flags;
        uint8_t        it_data_Rate;
        uint16_t       it_channel_frequency;
        uint16_t       it_channel_flags;
        uint8_t        it_antenna_signal;
        uint8_t        it_empty;            /* empty */
        uint16_t       it_RX_flags;
        uint8_t        it_antenna_signal2;
        uint8_t        it_antenna;
};

struct ieee80211_header {
        uint8_t        type_subtype;
        uint8_t        flags;
        uint16_t       duration;
        /*
         Beacon Frame, Probe Request, Probe Response, Authentication, Deauthentication, Action
            add1 = Receiver, Destination
            add2 = Transmitter, Source
            add3 = BSSID

         Data
            add1 = Rec, Des, STA
            add2 = Trans, BSSID
            add3 = Source

         Qos Null function, Qos Data, Null function
            add1 = Receiver, BSSID
            add2 = Transmitter, Source, STA
            add3 = Destination
        */
        mac_key add1;
        mac_key add2;
        mac_key add3;
        uint16_t       fragment_sequence;
};
 
#pragma pack(push, 1)

struct data_info {
    int pwr=0;              // rssi is maximum value
    int timestamp=0;        // timestamp is first value
};

#pragma pack(pop)
