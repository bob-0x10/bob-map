#include "integrated.h"
#include "radiotap_header.h"
#include <pcap.h>
#include <chrono>
#include <curl/curl.h>
#include <fstream>
#include <time.h>
#include <map>
#include <unistd.h>

static map<mac_key, data_info> d;
static map<mac_key, data_info>::iterator d_iter;

static char *payload = (char *)malloc(sizeof(char));
extern char* dev;
extern char* server_url;
extern char node_mac[17];
char err[100];


void write_err_log(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char now_time[30];
    sprintf(now_time, "[%d-%02d-%02d %02d:%02d:%02d] ",
            tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);

    fstream dataFile;

    dataFile.open("./error_log.txt", ios::app);
    dataFile << now_time << err << endl;
    dataFile.close();

    memset(err, 0, 100);
}

void send_curl(char *url){
    // libcURL 초기화
    curl_global_init( CURL_GLOBAL_ALL ) ;

    // context객체의 생성
    CURL* ctx = curl_easy_init() ;

    if( NULL == ctx ){
        sprintf(err, "Unable to initialize cURL interface");
        std::cerr << err << std::endl ;
        write_err_log();
    }

    // context 객체를 설정한다.
    // 긁어올 url을 명시하고, url이 URL정보임을 알려준다.
    curl_easy_setopt( ctx , CURLOPT_URL,  url ) ;

    // 웹페이지를 긁어온다.
    printf("\n[response]\n");
    const CURLcode rc = curl_easy_perform( ctx ) ;
    if( CURLE_OK != rc ){
        sprintf(err, "Error from cURL: %s", curl_easy_strerror( rc ));
        std::cerr << err << std::endl ;
        write_err_log();
    }

    // cleanup
    curl_easy_cleanup( ctx ) ;
    curl_global_cleanup() ;

    printf("\n\n");
}

void make_payload(std::mutex& mutex){
    while(true){
        sleep(3);
        mutex.lock();

        if(d.begin() == d.end()){
            sprintf(err, "Error : map container 'd' is empty\n");
            printf("%s\n", err);
            write_err_log();
        }
        else{
            for(d_iter = d.begin(); d_iter != d.end(); d_iter++){
                memset(payload, 0, sizeof(char));

                static int cnt = 0;
                cnt++;
                printf("%3d ======================================================\n", cnt);
                printf("[payload]\n");

                sprintf(payload, "%s/api/upload/packet/data?nodemac=%s&devicemac=",server_url ,node_mac);

                mac_key mac;
                mac = d_iter->first;
                uint8_t *ptr = reinterpret_cast<uint8_t*>(&mac);
                for(int i=0 ; i<6; i++){
                    sprintf(payload + strlen(payload), "%02x:", ptr[i]);
                }

                sprintf(payload + strlen(payload) -1, "&rssi=%d&timestamp=%d" ,(*d_iter).second.pwr, (*d_iter).second.timestamp);

                printf("%s\n",payload);

                send_curl(payload);
            }
        }
        d.clear();
        mutex.unlock();
    }
}

int capture_packet(){
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "pcap_open_live(%s) return nullptr - %s\n", dev, errbuf);
        return -1;
    }

    while (true) {
        struct pcap_pkthdr* header;
        const u_char* packet;
        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 0) continue;
        if (res == -1 || res == -2) {
            sprintf(err, "pcap_next_ex return %d(%s)", res, pcap_geterr(handle));
            printf("%s\n", err);
            write_err_log();
        }

        parse_packet(packet);
    }

    pcap_close(handle);
    return 0;
}

void parse_packet(const u_char* packet){
    //packet header setting
    struct radiotap_header *rh = (struct radiotap_header *)packet;
    struct ieee80211_header *ih = (struct ieee80211_header *)(packet + rh->it_length);
    //uint8_t *wlh = (uint8_t *)ih + IEEE_LEN;               //wireless LAN header

    std::mutex mutex;

    //Catch Probe_request & parsing
    if( (ih->type_subtype == PROBE_REQUEST || ih->type_subtype == NULL_FUNCTION
            || ih->type_subtype == QOS_NULL_FUNCTION || ih->type_subtype == AUTHENTICATION || ih->type_subtype == ACTION)
            && rh->it_antenna_signal > 186){  // it_antenna_signal > -70

        mutex.lock();

        //rssi
        int rssi = 0;
        if (rh->it_antenna_signal<127)
            rssi= rh->it_antenna_signal -1;
        else
            rssi= rh->it_antenna_signal -255 -1;

        //timestamp
        const auto p1 = chrono::system_clock::now();
        int timestamp = chrono::duration_cast<chrono::seconds>(p1.time_since_epoch()).count();


        // Data Save
        // timestamp is first value
        auto d_iter = d.find(ih->add2);
        if(d_iter == d.end()){
            data_info d_info;
            d_info.pwr = rssi;
            d_info.timestamp = timestamp;

            d[ih->add2] = d_info;
        }
        // rssi is maximum value
        else{
            if((*d_iter).second.pwr < rh->it_antenna_signal){
                (*d_iter).second.pwr = rssi;
                (*d_iter).second.timestamp = timestamp;
            }
        }

        mutex.unlock();
    }
}
