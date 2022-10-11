#pragma once
#include <map>
#include <string>
#include <mutex>
#include <functional>

#include "loop.hpp"
#include "poll.hpp"
#include "signal.hpp"

#include "config.h"



enum MESSAGETYPE
{
   MESSAGE_ROUTE = 0,
   MESSAGE_LOGIN_TCP = 1,
   MESSAGE_LOGIN_TCP_RES = 2,
   MESSAGE_LOGIN_UDP = 3,
   MESSAGE_LOGIN_UDP_RES = 4,
   MESSAGE_KEEPALIVE_TCP = 5,
   MESSAGE_KEEPALIVE_TCP_RES = 6,
   MESSAGE_KEEPALIVE_UDP = 7,
   MESSAGE_KEEPALIVE_UDP_RES = 8,
   MESSAGE_CPE_CHANGE_NOTIFY = 18
};

typedef std::function<void(unsigned int , std::string&)> do_send_callback_t;
typedef std::function<void(unsigned int , std::string&)> do_recv_callback_t;


struct cpeNode
{
    unsigned int node_ID;
    unsigned int node_rea1_IP;
    unsigned int node_virtual_IP;
    unsigned int node_subnet_IP_low;
    unsigned int node_subnet_IP_high;
    std::string node_subnet;
    std::string node_mac_addr;
};

class Server
{
public:
	Server(int uid, char* devName, int flag, mzConfig& config_);
	virtual ~Server();

public:
       void start();
       void stop();
       void run();

public:
       void register_send_func_callback(do_send_callback_t send_cb) { m_send_callback = send_cb; }
       void handle_route_data_recv(const char* data, uint16_t len, uint32_t src);
       void handle_session_data_recv(const char* data, uint16_t len);
       void set_gateway_id(uint32_t id) { m_gateway_id = id; }

private:
        void handle_packet_from_dev(std::string& msg);
	void handle_mesh_notify(std::string& name, std::string cpes_info);
	void handle_online_cpes_notify(std::string cpes_info);

private:
        void init_mesh_cpes();
        void init_local_node();
        void init_tap_tun_dev();
        void exec_down_cmd();
        void exec_up_cmd();

private:
        uint32_t getDestIDFromTunPacket(const unsigned char* packet);
        uint32_t getDestIDFromTapPacket(const unsigned char* packet);
        uint32_t getNodeIDFromIP(unsigned int ip, std::string mac = "00:00:00:00:00:00");

private:	
        void add_route_for_nodes();
        void del_route_for_nodes();

private:
        void on_read_tun_tap(int, int);
        void on_signal(int);

private:
        char*       m_devname;
	uint32_t    m_tun_tap_dev_fd;
        uint32_t    m_gateway_id; 
        char        m_tun_tap_dev_name[16];
        std::string m_node_id;

private:
	//uvpp::loop   m_loop;
        //uvpp::Poll   m_poll; //read dev tun fd callback
        //uvpp::Signal m_signal;
	cpeNode      m_node;
        mzConfig&    m_config;

private:
        std::map<uint32_t, uint32_t> m_ip2nodeid_cache;
        std::vector<cpeNode> m_cpes;

private:
       do_send_callback_t m_send_callback;
       do_recv_callback_t m_recv_callback;       
};
