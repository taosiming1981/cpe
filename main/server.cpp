/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <thread>
#include <bitset>

#include <string.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/if_tun.h>
#include <fcntl.h>
#include <unistd.h>
#include <cassert>

#include <sys/socket.h>   
#include <netinet/in.h>   
#include <arpa/inet.h>

#include "util.hpp"
#include "tun_tap.hpp"
#include "make_unique_define.h"
#include "login.pb.h"
#include "keepalive.pb.h"
#include "user_info.pb.h"
#include "server.h"
//////////////////////////////////////////////////////////////////////////////////////////
int g_signal[] = {1,2,3,4,5,/*6,*/7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,23,25,26,27,28,29,30,31};
unsigned char* buffer;
#define PACK_MTU 1500
//////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
//copy from if_ether.h:
#define ETH_P_IP       0x0800        /* Internet Protocol packet    */
#define ETH_P_ARP      0x0806        /* Address Resolution packet    */
#define ETH_P_RARP     0x8035        /* Reverse Addr Res packet    */
#define ETH_P_IPX      0x8137        /* IPX over DIX            */
#define ETH_P_PPP_DISC 0x8863        /* PPPoE discovery messages     */
#define ETH_P_PPP_SES  0x8864        /* PPPoE session messages    */
///////////////////////////////////////////////////////////////////////////////////////////
#define IP_PROTO_ICMP 1
#define IP_PROTO_IGMP 2
#define IP_PROTO_TCP  6
#define IP_PROTO_UDP  17
#define IP_PROTO_IGRP 88
#define IP_PROTO_OSPF 89
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Server::Server(int id, char* devName, int flag, mzConfig& config_)
:m_devname(set_tun_name(m_tun_tap_dev_name, devName))
,m_tun_tap_dev_fd(tun_create(m_tun_tap_dev_name, flag)) 
,m_gateway_id(0)
,m_node_id(to_string(id))
//,m_loop(true)
//,m_poll(m_loop, m_tun_tap_dev_fd)
//,m_signal(m_loop)
,m_config(config_)
{
    init_mesh_cpes();
    init_local_node();
    exec_down_cmd();
    exec_up_cmd();
/*   
    for(unsigned int i = 0; i < sizeof(g_signal)/sizeof(g_signal[0]); i++){
        uvpp::Signal* pSignal = new uvpp::Signal(m_loop);
        pSignal->start(g_signal[i], std::bind(&Server::on_signal, this, placeholders::_1)); 
    } 
*/
        
    init_tap_tun_dev(); 
    add_route_for_nodes();	

/*    
    int eventFlag = UV_READABLE;
    m_poll.start(eventFlag, std::bind(&Server::on_read_tun_tap, this, placeholders::_1,  placeholders::_2));
*/
    cout << "tun fd:" << m_tun_tap_dev_fd << endl;   
    cout << "init server finish....... for id:" << m_config.userID << " ip:" << m_config.userIP << endl;
}

Server::~Server()
{
}

void Server::start()
{
}

void Server::stop()
{
}

void Server::exec_down_cmd()
{
   if(m_config.downCmd.size() == 0)
        return;

   system(m_config.downCmd.c_str());
}

void Server::exec_up_cmd()
{
    if(m_config.upCmd.size() == 0)
        return;

    system(m_config.upCmd.c_str());
}


void Server::run()
{
#if 1
    int selectmax = m_tun_tap_dev_fd + 1;
    fd_set readfds;
    char buffer[PACK_MTU];

    while( 1 ) {
        FD_ZERO( &readfds );
        FD_SET( m_tun_tap_dev_fd, &readfds );

        int z = select( selectmax, &readfds, NULL, NULL, NULL );
        if( z < 0 ) {
            perror( "select() failed" );
            close( m_tun_tap_dev_fd );
            return;
        } else {
            if( FD_ISSET(m_tun_tap_dev_fd, &readfds ) ) {
#if 1		   
                on_read_tun_tap(0,0);
#else
                int bufread = read( m_tun_tap_dev_fd, buffer, sizeof(buffer) );
                if( bufread < 0 ) {
                    continue;
                }

                printf( "tundev has %d bytes of data\n", bufread );
#endif
            }
        }
    }
	
#else	
    m_loop.run();
#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//initilize mesh network;
void Server::init_mesh_cpes(/*string filepath*/)
{
    m_cpes.clear();
    for(auto &itor : m_config.nodeInfoList){
        string virtual_ip = itor.virtualIP;
        string subnet_ip_low = itor.subNet;
        int mask_bit = itor.netMask;
        cpeNode tmpNode;

        int ip_count = (1 << (32-mask_bit)) - 1;

        tmpNode.node_ID = itor.nodeID;
        tmpNode.node_virtual_IP = htonl(inet_addr(virtual_ip.c_str()));
        tmpNode.node_subnet = itor.subNet;
        tmpNode.node_subnet_IP_low = htonl(inet_addr(subnet_ip_low.c_str()));
        tmpNode.node_subnet_IP_high = tmpNode.node_subnet_IP_low + ip_count;
        tmpNode.node_mac_addr = itor.macAddr;

        cout << tmpNode.node_ID << " " << virtual_ip  <<  " " << tmpNode.node_virtual_IP 
		<< " " << tmpNode.node_subnet_IP_low  << " " << tmpNode.node_subnet_IP_high 
		<< " "<< ip_count << endl;
        m_cpes.push_back(tmpNode);
    }
}

void Server::init_local_node()
{
    m_node.node_ID = m_config.userID;
    m_node.node_virtual_IP = htonl(inet_addr(m_config.userIP.c_str()));
}

void Server::init_tap_tun_dev()
{
    cout << "init tap/tun dev, set name::" << m_tun_tap_dev_name << endl;
    if(set_if_flags(m_tun_tap_dev_name, IFF_UP) == -1) {
       cout << "interface up tap device error" << std::endl;
       exit(-1);
    }
    cout << "ifconfig " << m_tun_tap_dev_name << " up and mac addr:" << m_config.macAddr 
         << " ip:" <<  ValueToIP(m_node.node_virtual_IP) << endl;
    
    char* ip = (char*)m_config.userIP.c_str();
    int mask = m_config.netMask;

    if(m_config.tapMode == 0){
        std::string local_ip = ip;
        std::string dev = m_tun_tap_dev_name;
        std::string command = "ip addr add " + local_ip + "/" + std::to_string(mask) + " dev " + dev;
        cout << "ioctl error, then use command::" << command << std::endl;
        system(command.c_str());
        return;    
    }

    if(m_config.macAddr.size() != 0 && m_config.macAddr != "00:00:00:00:00:00") {    
        unsigned char mac_addr[6]; // = {0x52, 0x17, 0xa6, 0x8d, 0x1a, 0x22};
        memset(mac_addr, 0, 6);
        std::vector<std::string> byteArray = mzConfig::split(m_config.macAddr, ":");
        if(byteArray.size() != 6)
            exit(-1);
 
        char* str;
        for(uint32_t i = 0; i < byteArray.size(); i++) 
           mac_addr[i] = strtol(byteArray[i].c_str(), &str, 16);
            
        set_mac(m_tun_tap_dev_name, (char*)mac_addr);
    }

    std::string local_ip = ip;
    std::string dev = m_tun_tap_dev_name;
    std::string command = "ip addr add " + local_ip + "/" + std::to_string(mask) + " dev " + dev;
    cout << "ioctl error, then use command::" << command << std::endl;
    system(command.c_str());

    if(m_config.macAddr.size() == 0 || m_config.macAddr == "00:00:00:00:00:00"){
        unsigned char mac[6];
        memset(mac, 0, 6);
        get_mac((char*)m_config.devName.c_str(), (char*)mac, 6);
        std::string macAddr_str;
        macAddr_str.assign((char*)mac, 6); 
        m_config.setMacAddr(hexMacAddr(macAddr_str));
    }
}

void Server::add_route_for_nodes()
{
    for(auto node : m_cpes) {
        if((unsigned int)atoi(m_node_id.c_str()) == node.node_ID)
            continue;
        std::vector<std::string> netVec = mzConfig::split(node.node_subnet, ",");
        //cout << "id: " << node.node_ID << " subnet:" << node.node_subnet << endl; 
        if(netVec.size() == 0)
            continue;

        for(auto net : netVec){
            std::vector<std::string> meta = mzConfig::split(net, "/");
            if(meta.size() != 2)
                continue;
            
            std::string subnet = meta[0];
            uint32_t    netmask = (uint32_t)atoi(meta[1].c_str());
            if(netmask > 32)
                continue;

            //cout << "add route for dev:" << m_devname << " net:" << subnet 
	    //<< " mask:" << netmask << endl;  

            std::bitset<32> mask(0xFFFFFFFF);
            for(uint32_t i = 0; i < (32-netmask); i++)
                mask[i] = 0;

            std::string subnet_mask = ValueToIP(mask.to_ulong()); 
            cout << "add route for dev:" << m_devname << " net:" << subnet 
		    << " mask:" << subnet_mask << endl;
            route_add_net(m_devname, subnet.c_str(), subnet_mask.c_str()); 
        }   
    }
}


void Server::del_route_for_nodes()
{
    for(auto node : m_cpes) {
        if((unsigned int)atoi(m_node_id.c_str()) == node.node_ID)
            continue;

        std::vector<std::string> netVec = mzConfig::split(node.node_subnet, ",");
        //cout << "id: " << node.node_ID << " subnet:" << node.node_subnet << endl;
        if(netVec.size() == 0)
            continue;

        for(auto net : netVec){
            std::vector<std::string> meta = mzConfig::split(net, "/");
            if(meta.size() != 2)
                continue;

            std::string subnet = meta[0];
            uint32_t    netmask = (uint32_t)atoi(meta[1].c_str());
            if(netmask > 32)
                continue;

            //cout << "add route for dev:" << m_devname << " net:" << subnet 
	    //<< " mask:" << netmask << endl;
            
	    std::bitset<32> mask(0xFFFFFFFF);
            for(uint32_t i = 0; i < (32-netmask); i++)
                mask[i] = 0;

            std::string subnet_mask = ValueToIP(mask.to_ulong());
            cout << "del route for dev:" << m_devname << " net:" << subnet 
		    << " mask:" << subnet_mask << endl;
            route_del_net(m_devname, subnet.c_str(), subnet_mask.c_str());
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
void Server::on_read_tun_tap(int status, int events)
{
    int bufread = read(m_tun_tap_dev_fd, buffer+4, PACK_MTU/*m_packet_buff_size*/);
    if( bufread < 0 ) {
        cout << "Failed to tun read :" <<  strerror(errno) << endl;
        close(m_tun_tap_dev_fd);
        exit(-1);
    }

    if(m_config.debug)
        cout << getCurrentTime() << "read tun dev for data len:" << bufread << endl;

    memcpy(buffer, (char*)&bufread, 4); //add packet size head of packet in 4 byte;

    if(bufread < 20){
        cout << "Failed to read tap because read data less than 20, len:" << bufread << endl;
        close(m_tun_tap_dev_fd);
        exit(-1);
    }

    std::string msg;
    msg.assign((const char*)buffer, bufread+4);

    handle_packet_from_dev(msg);
}


void Server::handle_packet_from_dev(std::string& msg)
{
    unsigned int dest = 0;

    if(m_config.onlyAcc){
	dest = m_gateway_id;
    }else if(m_config.tapMode){
        const unsigned char* packet = (const unsigned char*)msg.c_str()+4;
        dest = getDestIDFromTapPacket(packet);
    }else{
        const unsigned char* packet = (const unsigned char*)msg.c_str()+4;
        dest = getDestIDFromTunPacket(packet);	    
    }
   
    m_send_callback(dest, msg);
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int Server::getNodeIDFromIP(unsigned int ip, std::string mac)
{
    unsigned int dest_ip = ip;
    if(m_config.onlyAcc == 1)//only acc mode
        return m_gateway_id;

    if(m_ip2nodeid_cache.find(ip) != m_ip2nodeid_cache.end()){
        int node_id = m_ip2nodeid_cache[ip];
        return node_id;
    }

    for(auto node : m_cpes){
        if(m_config.debug)
            cout << "dest ip:" << ValueToIP(ip) << " node ip:" << ValueToIP(node.node_virtual_IP)
                << " subnet:" << node.node_subnet
                << " mac:" << hexMacAddr(mac) << endl;

        if(dest_ip == node.node_virtual_IP) return node.node_ID;

        std::vector<std::string> netVec = mzConfig::split(node.node_subnet, ",");
        //cout << "id: " << node.node_ID << " subnet:" << node.node_subnet << endl;

        for(auto net : netVec){
            std::vector<std::string> meta = mzConfig::split(net, "/");
            if(meta.size() != 2)
                continue;

            string   subnet = meta[0];
            uint32_t netmask = (uint32_t)atoi(meta[1].c_str());
            if(netmask > 32)
                continue;

            std::bitset<32> mask(0xFFFFFFFF);
            for(uint32_t i = 0; i < (32-netmask); i++)
                mask[i] = 0;

            if((ip & mask.to_ulong()) == htonl(inet_addr(subnet.c_str()))) 
	        return node.node_ID;
        }

        if(m_config.tapMode && mac == node.node_mac_addr) 
	    return node.node_ID;
    }

    if(!m_config.gateWay){
       return m_gateway_id;
    }

    return 0;
}


uint32_t Server::getDestIDFromTunPacket(const unsigned char* packet)
{
    unsigned int destID = 0;
    unsigned int ip_offset = 16;
    unsigned int dest_ip_int = 0;

    memcpy(&dest_ip_int, packet+ip_offset, 4);//ip addr in ip packet offset is 16bytes;

    destID = getNodeIDFromIP(htonl(dest_ip_int));
    if(destID != 0 && destID != m_gateway_id && m_ip2nodeid_cache.find(htonl(dest_ip_int)) == m_ip2nodeid_cache.end())
       m_ip2nodeid_cache[htonl(dest_ip_int)] = destID;

    if(m_config.debug)
        cout << getCurrentTime() << "read work for default dest:" << destID 
		<< " ip:" << ValueToIP(htonl(dest_ip_int)) << endl;

    return destID;
}

uint32_t Server::getDestIDFromTapPacket(const unsigned char* packet)
{
    unsigned int destID = 0;
    unsigned short proto = 0;
    //unsigned int dest = 0;
    unsigned int proto_offset = 12;
    unsigned int ip_offset = 14 + 16; //eth apcket header 14 bytes; and ip addr in ip packet offset is 16bytes;
    unsigned int dest_ip_int = 0;
    unsigned int ip_proto_offset = 12 + 9;
    unsigned char ip_proto = 0;    
    std::string dest_mac_addr;
    unsigned int dest_mac_addr_offset = 0;
 
    dest_mac_addr.assign((const char*)(packet+dest_mac_addr_offset), 6);
    memcpy(&proto, packet+proto_offset, 2);
    
    proto = htons(proto);
    //memcpy(&dest, packet+ip_offset, 4);//ip addr in ip packet offset is 16bytes;
    if(m_config.debug)
       cout << getCurrentTime() << "read packet from tun proto: 0x" << hex << proto << " " << dec << proto << endl;

    if(proto == ETH_P_IP){
        //multicast,broadcast packet
        memcpy(&ip_proto, packet+ip_proto_offset, 1);
        memcpy(&dest_ip_int, packet+ip_offset, 4);

        if(htonl(dest_ip_int) < 0xE0000000){ //ip packet and dest ip less than 224.0.0.1
            destID = getNodeIDFromIP(htonl(dest_ip_int), dest_mac_addr);
            if(destID != 0 && destID != m_gateway_id && m_ip2nodeid_cache.find(htonl(dest_ip_int)) == m_ip2nodeid_cache.end())
                m_ip2nodeid_cache[htonl(dest_ip_int)] = destID;

        }else if((htonl(dest_ip_int) > 0xE0000000) && (htonl(dest_ip_int) < 0xF0000000) ) {
            destID = 0xFFFFFFFF;//!fixMe::may be retuan vetcor
        }else if(htonl(dest_ip_int) > 0xF0000000){
            destID = 0xFFFFFFFF;
        }

        if(m_config.debug){
             int ip_proto_type = ip_proto;
             cout << getCurrentTime() << "read work for ip packet dest:" << destID << " proto:" << ip_proto_type <<  " ip:" << ValueToIP(htonl(dest_ip_int)) << endl;
        }
    }else if(proto == ETH_P_ARP || proto == ETH_P_RARP || proto == ETH_P_IPX || proto == ETH_P_PPP_DISC  || proto == ETH_P_PPP_SES){
        destID = 0xFFFFFFFF;
        if(m_config.debug)
             cout << getCurrentTime() << "read work for not ip packet default dest:" << destID << " ip:" << ValueToIP(htonl(dest_ip_int)) << endl;
    }

    return destID;
}
///////////////////////////////////////////////////////////////////////////////////////////////
void Server::handle_route_data_recv(const char* data, uint16_t len, uint32_t src)
{
    if(m_tun_tap_dev_fd == 0){
        cout << " tun fd error" << endl;
        return;
    }

    if(m_config.debug)
        cout << getCurrentTime() << "handle route data recv from:" << src << " len:" << len  
    	    << " fd:" << m_tun_tap_dev_fd << endl;
    //hexdump2((const unsigned char*)data, len);
    int ret = write(m_tun_tap_dev_fd, data+4, len-4);
    if(ret < 0){
        cout << "write tun dev error:" << strerror(errno) << endl;
        close(m_tun_tap_dev_fd);
        exit(-1);
    }
}


void Server::handle_session_data_recv(const char* data, uint16_t len)
{
    if(len <= 4){
       cout << "[error ]recv session data callback len:" << len << endl;
       return;
    }

    uint16_t pack_len = 0;
    uint16_t pack_uri = 0;
    memcpy(&pack_len, data, 2);
    memcpy(&pack_uri, data+2, 2);

    if(len != pack_len + 4){
	cout << "[error ]recv session data callback len:" << len 
		<< " pack len:" << pack_len << " type:" << pack_uri << endl;
        return;
    }
#if 1
    switch(pack_uri){
	//vender mesh info;
        case MESSAGETYPE::MESSAGE_CPE_CHANGE_NOTIFY:
        {
            std::string buff;
            buff.assign(data + 4, len-4);

            message::CpeNodeChangeNotify notify;
            if(!notify.ParseFromString(buff)){
                cerr << "on cpes change notify parse error!" << endl;
                return;
            }
            
	    std::string peername = "test";
            std::string cpes_info = notify.data();
            handle_mesh_notify(peername, cpes_info);
            cout << "recv session data cmd:cpe change notify" << endl;
        }
        break;

	//gateway node,online cpes info;
	case MESSAGETYPE::MESSAGE_KEEPALIVE_TCP_RES:
        case MESSAGETYPE::MESSAGE_KEEPALIVE_UDP_RES:
	{
            std::string buff;
            buff.assign(data + 4, len-4);

            message::KeepAliveRes pong;
            if(!pong.ParseFromString(buff)){
                cerr << "on udp keepalive res parse error!" << endl;
                return;
	    }

            std::string snap_info = pong.meta();
            if(snap_info.size() == 0)
                return;
           
	    handle_online_cpes_notify(snap_info);
	}
	break;

        default:
	       cout << "[error ]recv session data callback uri not handle len:" << len
                 << " type:" << pack_uri << endl;

        break;
    }

#endif	
}


void Server::handle_mesh_notify(std::string& name, std::string cpes_info)
{
    if(cpes_info.size() == 0)
        return;


    message::NodeInfoList cpes_info_list;
    if(!cpes_info_list.ParseFromString(cpes_info)){
        cout << "handle mesh notinfy cpe info list  parse error!" << endl;
        return;
    }
              
    m_ip2nodeid_cache.clear();
    std::vector<NodeInfo>& nodeInfoList = m_config.nodeInfoList;
    nodeInfoList.clear();

    for(int i = 0; i < cpes_info_list.cpe_list_size(); i++){
        message::NetInfo info = cpes_info_list.cpe_list(i);

        uint32_t mask = 0xFFFFFFFF - (1 << (32 - info.net_mask())) + 1;
        std::string netmask = ValueToIP(mask);
        //route_add_net(m_devname, info.subnet_ip_start().c_str(), netmask.c_str());

        if(info.subnet_ip_start() == "0.0.0.0"){
                     cout << getCurrentTime() << " [cpes-mesh-info] error id:" << info.node_id() << " ip:"
                         << info.virtual_ip() << " subnet:" << info.subnet_ip_start() << "/" << info.net_mask()
                         << " mask:" << netmask << " mac-addr:" << hexMacAddr(info.mac_addr()) << endl;

                    continue;
        }

        cout << getCurrentTime() << " [cpes-mesh-info] id:" << info.node_id() << " ip:"
                     << info.virtual_ip() << " subnet:" << info.subnet_ip_start() << "/" << info.net_mask()
                     << " mask:" << netmask << " mac-addr:" << hexMacAddr(info.mac_addr()) << endl;

                //cout << "mac addr:" << hexdump(info.mac_addr()) << " size:" >> info.mac_addr().size() << endl;

        NodeInfo node_info;
        node_info.nodeID = info.node_id();
        node_info.virtualIP = info.virtual_ip();
        node_info.subNet = info.subnet_ip_start();

        node_info.netMask = info.net_mask();
        node_info.macAddr = info.mac_addr();

        nodeInfoList.push_back(node_info);
    }

    del_route_for_nodes();
    cout << getCurrentTime() << " delete all route info....." << endl;
  
    init_mesh_cpes();
    cout << getCurrentTime() << " init all cpes info......" << endl;
    
    add_route_for_nodes();
    cout << getCurrentTime() << " add all cpes subnet route info..... size:" << nodeInfoList.size()  << endl;
}

void Server::handle_online_cpes_notify(std::string cpes_info)
{
    message::UserListOnEdge edgeListMsg_;
    edgeListMsg_.ParseFromString(cpes_info);

    m_cpes.clear();
    for(int i = 0; i < edgeListMsg_.edge_list_size(); i++){
        message::CustemorPEList edgeInfoMsg_ = edgeListMsg_.edge_list(i);
        string cpeListInfo = "";
        string edge_id = edgeInfoMsg_.edge_id();
        string edge_ip = edgeInfoMsg_.edge_ip();
        uint32_t edge_port = edgeInfoMsg_.edge_port();
        uint32_t edge_group_id = edgeInfoMsg_.edge_group();

        for(int k = 0; k < edgeInfoMsg_.cpe_list_size(); k++)
        {
            message::CustemorInfo cpe_info = edgeInfoMsg_.cpe_list(k);
            std::string cpe_id = cpe_info.cpe_id();
            std::string cpe_ip = cpe_info.cpe_ip();
            cpeListInfo.append(":"+cpe_id+"("+cpe_ip+")");
            //m_cpe2edge_map[cpe_id].insert(edge_id);
            //m_cpe2super_map[cpe_id].insert(super_node_id);

            cpeNode tmpNode;
            tmpNode.node_ID = atoi(cpe_id.c_str());
            tmpNode.node_virtual_IP = htonl(inet_addr(cpe_ip.c_str()));
            tmpNode.node_subnet_IP_low = 0;
            tmpNode.node_subnet_IP_high = 0;
            m_cpes.push_back(tmpNode);
        }

        if(cpeListInfo.size() == 0)
            cpeListInfo = ":null";

        if(1){
            cout << getCurrentTime() << "snap info for super onlines,edge:"  << edge_id << " ip:" << edge_ip << " port:" << edge_port << " group:" << edge_group_id
                 << " cpes num:" << edgeInfoMsg_.cpe_list_size() << " info" << cpeListInfo << endl;
        }
    }    	
}

void Server::on_signal(int signum)
{
   switch(signum){
        case SIGINT://ctrl+c
        case SIGQUIT:
        case SIGILL:
        case SIGTRAP:
        case SIGBUS:
        case SIGFPE:
        case SIGSEGV:
        case SIGTERM:
        case SIGTSTP:
        case SIGALRM:
        case SIGPIPE:
        case SIGXFSZ:
        case SIGXCPU:
        case SIGSYS:
            exec_down_cmd();
            cout << "get exit signal input for sig:" << signum  << endl; 
            std::abort();
        case SIGUSR1:
            m_config.reload();
            del_route_for_nodes();
            break;
        case SIGUSR2:
            {
                std::stringstream mesh_info_ss;
                mesh_info_ss << getCurrentTime() << "print cpe node mesh info, mesh node num:" << m_cpes.size() << endl;
                for(auto node : m_cpes){
                    mesh_info_ss << "[cpes-mesh-info] id:" << node.node_ID << " ip:"  << ValueToIP(node.node_virtual_IP) 
			    << " mac-addr:" << hexMacAddr(node.node_mac_addr) << endl;
                    cout << "[cpes-mesh-info] id:" << node.node_ID << " ip:"  << ValueToIP(node.node_virtual_IP) 
			    << " mac-addr:" << hexMacAddr(node.node_mac_addr) << endl;
                }
                mesh_info_ss << endl;
                std::string logFile = m_config.meshFile;//"/tmp/log/uvpp.log";
                ofstream fout;
                fout.open(logFile);

                fout << mesh_info_ss.str() << endl;
                fout.close();
            }
            break;
        default:
            cout << "get signal input for sig:" << signum << endl;
            break;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
