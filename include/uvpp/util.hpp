#pragma once
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <chrono>
#include <cstring>

#include <sys/time.h>
#include <sys/resource.h>

using namespace std;
inline string hexdump(vector<uint8_t> const& bytes)
{     
    ostringstream buff;
    int buflen = bytes.size();
    
    int i;
    int j; 
    char c;
 
    for (i = 0, j = 0; i < buflen; i += 16) {
        // Print offset        
        buff << right << setfill('0') << setw(4) << i << "  ";

       // Print bytes in hexadecimal
        buff << hex;     
        for (j = 0; j < 16; j++) {
            if (i + j < buflen) {
                c = bytes[i + j];        
                buff << setfill('0') << setw(2) << (int(c) & 0xFF);
                buff << " ";
            } else {        
                buff << "   ";
            }
        }     
        buff << dec;
        buff << " ";
                    
        // Print printable characters
        for (j = 0; j < 16; j++) {
            if (i + j < buflen) {
                c = bytes[i + j];
                if (isprint(c)) {
                    buff << c;
                } else {      
                     buff << ".";
                }
            }
        }
        buff << endl;
    }
    return buff.str();
}

inline string hexdump(string msg)
{
    std::vector<unsigned char> dumpData(msg.size());
    std::copy(msg.begin(), msg.end(), dumpData.begin());
    return hexdump(dumpData);   
}

inline string hexdump(const char* data, int len)
{
    std::vector<unsigned char> dumpData(len);
    dumpData.insert(dumpData.end(), data, data + len);
    return hexdump(dumpData);
}

inline void hexdump2(const unsigned char *buf, int len) 
{
#if 1
    int i = 0;
    char output[8192]; 
    memset(output, 0, 8192);
    std::cout << std::endl;
    std::cout << "----------------------hexdump------------------------" << std::endl;;
    for(i = 0; i < len; i++) {
        char tmp[16];
        memset(tmp, 0, 16);
        sprintf(tmp, "%02x ", buf[i]);
        strcat(output, tmp);

        if( (i+1) % 16 == 0) {
           strcat(output, "\n");
        }
    }

    if(i%16 != 0) {
        strcat(output, "\n");
    }
    cout << output;
    cout << dec << "---------------------hexdump-------------------------" << std::endl;
#endif
}


inline string hexMacAddr(string macAddr)
{
    string buff = "mac addr error";
    if(macAddr.size() != 6){
        return buff;
    }

    char mac_addr[6];
    memcpy(mac_addr, macAddr.c_str(), 6);
    char mac_addr_str[256];
    sprintf(mac_addr_str, "%02x:%02x:%02x:%02x:%02x:%02x",  (unsigned char) mac_addr[0], (unsigned char) mac_addr[1],
         (unsigned char) mac_addr[2], (unsigned char) mac_addr[3],
         (unsigned char) mac_addr[4], (unsigned char) mac_addr[5]);
    buff.assign(mac_addr_str, strlen(mac_addr_str));
    return buff;
}



inline string ValueToIP(const int& nValue)  
{  
    char strTemp[20];  
    sprintf( strTemp,"%d.%d.%d.%d",  
        (nValue&0xff000000)>>24,  
        (nValue&0x00ff0000)>>16,  
        (nValue&0x0000ff00)>>8,  
        (nValue&0x000000ff) );  
  
    return string(strTemp);  
} 
#if 1 
#define TIME_FORMAT "%Y-%m-%d %H:%M:%S" // e.g. 2021-01-04 01:10:38

inline string getCurrentTime()
{
    time_t now = time(NULL);
    char timestr[20];
    strftime(timestr, 20, TIME_FORMAT, localtime(&now));

    struct timeval curTime;
    gettimeofday(&curTime, NULL);
    int milli = curTime.tv_usec / 1000;

    char currentTime[30] = { 0 };
    snprintf(currentTime, sizeof(currentTime), "%s-%03d ", timestr, milli);
    return currentTime;
}
#endif

inline void getIPandPortFromString(std::string peerName, std::string& peer, int& port)
{
    return;     
}


inline uint64_t getTick()
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}


