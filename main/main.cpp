#include <thread>
#include <iostream>
#include <fstream>
#include <string>

#include <unistd.h>
#include <linux/if_tun.h>
#include <arpa/inet.h>

#include "util.hpp"
#include "server.h"
#include "config.h"
#include "FancyMsgChanLibInterfaceDll.h"

using namespace std;
Server* tun_ = nullptr;
#if 0
void redirect_file(mzConfig& config_)
{
    ofstream outf(config_.logFile.c_str());
    if(!outf.is_open()){
        cout << "open log file error for path:" << config_.logFile << endl;
    }else{
        cout.rdbuf(outf.rdbuf());// redirect to out.txt
    }

    std::string statFilePath = "output.log";
    if(config_.statistics){
        statFilePath = config_.statFile;
    }

    ofstream errf(statFilePath.c_str());
    if(!outf.is_open()){
        cout << "open stat file error for path:" << config_.statFile << endl;
    }else{
        cout << "open stat file success for path:" << config_.statFile << endl;
        cerr.rdbuf(errf.rdbuf());// redirect to out.txt
    }
}
#endif

void RecvMsgCallback(const char* data_, int len, int src)
{
    //cout << "recv data call back from:" << src << " len:" << len << endl;
    if(tun_)
	tun_->handle_data_recv(data_, len, src);
}

void ConnTunnelCallback(uint32_t ip, bool connected)
{
    cout << "connect tunnel ip:" << ValueToIP(ip) 
	    << " connected:" << (connected ? "success" : "failed") << endl;
}

int  start(FancyMsgChanLibInterfaceDll& dll, Server& svr, mzConfig& config, std::string path)
{
    uint32_t node_id_ = config.userID;
    //FancyMsgChanLibInterfaceDll dll;
    std::cout << " begin load lib  from path:" << path << std::endl;
    if(!dll.LoadLibFromPath(path))
    {
        cout << "load libary so error !!" << endl;
        return 0;
    }
    if(!dll.GetAllFunctions())
    {
        cout << "get function error!!!" << endl;
        return 0;
    }

    const char* version = dll.m_FancyJingMsgVersion();
    cout << "version:" << version << endl;
    uint32_t virtual_ip_ = htonl(inet_addr(config.userIP.c_str()));
    uint16_t mode_ = atoi(config.userIP.c_str());

    dll.m_FancyJingMsgInit(node_id_, virtual_ip_, mode_);
    
    for(auto &itor : config.tunnelList)
    {
      std::string local_ip = itor.local_ip;
      std::string server_ip = itor.server_ip;
      int port = atoi(itor.server_port.c_str());
      int up_bw = atoi(itor.server_port.c_str());
      int down_bw = atoi(itor.server_port.c_str());
      int priority = atoi(itor.priority.c_str());

      dll.m_FancyJingMsgAddConnAddr(local_ip.c_str(), server_ip.c_str(), port, up_bw, down_bw, priority);
    }

    FancyMsgChanLibInterfaceDll::FuncFancyjingCallback callback = &RecvMsgCallback;
    dll.m_FancyJingMsgRegisterRecvCallback(*callback);

    FancyMsgChanLibInterfaceDll::FuncFancyjingConnCallback conn_callback = &ConnTunnelCallback;
    dll.m_FancyJingMsgRegisterConnCallback(*conn_callback);
    dll.m_FancyJingMsgRun();
    return 0;
}

int main(int argc, const char * argv[])
{
    FancyMsgChanLibInterfaceDll dll;
    std::string dll_path = "../../libs/libFancyJingMsgChannel.so";

    std::string path = "mingzhui.cfg";
    if(argc >= 2)
        path = argv[1];
    
    mzConfig config;
    int ret = config.loadConfig(path.c_str());
    if(ret != 0){
        cout << "load config file error" << endl;
        exit(0);
    }

    uint32_t gateway_id = 100000;
    if(config.gateWay != 0)
        gateway_id = config.gateWay;

    std::cout << " tun dev name:" << config.devName << std::endl;
    char* devName = new char[128];
    strcpy(devName, "tunUDP");
    if(config.devName.size())
        strcpy(devName, config.devName.c_str());

    //redirect_file(config);
    uint32_t user_id = config.userID;
    int flag = 0;
    if(config.tapMode == 0)
        flag = IFF_TUN | IFF_NO_PI;
    else
        flag = IFF_TAP | IFF_NO_PI;

    auto send_cb = [&dll](uint32_t dest, std::string& data){
         // cout << "send data to dest:" << dest << " len:" << data.length() << endl;
          //hexdump2((const unsigned char*)data.c_str(), data.length());   
	  dll.m_FancyJingMsgSendToPeer(dest, data.c_str(), data.length());
    };
    
    tun_ = new Server(user_id, devName, flag, config);
    start(dll, *tun_, config, dll_path);
    tun_->register_send_func_callback(send_cb);
    tun_->set_gateway_id(gateway_id);
    tun_->run();
    return ret;

}
