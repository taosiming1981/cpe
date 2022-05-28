#pragma once
#include <string>
#include <cstdint>
#include <iostream>
#include <cstring>
#include "util.hpp"

#define MEM_POOL false
#define PACK_SIZE 10*1024

const int PACK_HEAD_LEN = 16; //len(int32) + dest(int32) + seq(int4)
const int PACK_BUFFER_SIZE = 1550;

class msg_buf
{
public:
    msg_buf()
    :server("")
    ,msg("")
    ,seq(0)
    ,dest(0)
    ,src(0)
    ,bRouteData(false){
       create_tick = getTick();
       bTcpTransport = false;
    }

    virtual  ~msg_buf()
    {
    }

#ifdef __DEBUG_MSG_
    msg_buf(const msg_buf& data)
    {
       server = data.server;
       msg = data.msg;
       seq = data.seq;
       std::cout << " *** this:" << this << " old msg data str addr:" << (void*)data.msg.c_str() << " new addr:" << (void*) msg.c_str() << std::endl;
    }

    msg_buf& operator =(const msg_buf& data)
    {
       if(this == &data)
          return *this;
       server = data.server;
       msg = data.msg;
       seq = data.seq;
       std::cout << " #### old msg data str addr:" << (void*)data.msg.c_str() << " new addr:" << (void*) msg.c_str() << std::endl;     
       return *this; 
    }

    msg_buf(msg_buf && m)
    {
       std::cout << "$$$$$$$$$$" << std::endl;
    }
   
   
    msg_buf& operator =(msg_buf&& data)
    {
       std::cout << "@@@@@@@@@@" << std::endl;  
       return *this;
    }

    void debug() const
    {
      std::cout << "debug buf, this:" << this << " and msg.str addr:" << (void*)(msg.c_str()) << std::endl;
    }
#endif

public:
    inline void set_tcp_transport() {
           bTcpTransport = true;
       }
    inline bool is_tcp_transport() const {
           return bTcpTransport;
       }

    inline void set_peer(std::string p){
           server = p;
       }

    inline std::string get_peer() {
           return server;
       }
  
    inline void set_dest(uint32_t d){ 
           dest = d;
       }
    
    inline uint32_t get_dest() const {
           return dest;
       }
    inline void set_src(uint32_t s){
           src = s;
       }

    inline uint32_t get_src() const{
           return src;
       }

    inline uint32_t get_seq() const{
           return seq;
       }

    inline void set_seq(uint32_t q)  {
           seq = q;
       }

    inline bool is_route_data() const {
           return bRouteData;
       }

    inline virtual void append(const char* data, int len){
           msg.append(data, len);
           return;
        }
    
    inline virtual const char* getBuff() const {
            return msg.c_str();
        }

    inline virtual uint32_t getSize() const { 
            return msg.size();
        }  

    inline virtual const char* getBody() const {
            return nullptr;
        }

    inline virtual uint32_t getBodySize() const {
            return 0;
        }

    inline virtual void serialize(){  
        }

    inline virtual void unSerialize(){
            cout << "unSeirilazie() ......" << endl;
       }
       

protected:
    std::string server;//edge svr
    std::string msg;
    uint32_t seq;
    uint32_t dest;
    uint32_t src;
    uint64_t create_tick;
    bool bRouteData;  //messge to edge is session data; messag to other cpe is route data
    bool bTcpTransport;
};


class send_msg_buf : public msg_buf
{
public:
    send_msg_buf(std::string server_, uint32_t dest_, uint32_t src_, uint32_t seq_, bool bMemPool_ = MEM_POOL)
    :msg_buf()
    ,body_buff(nullptr)
    ,body_buff_len(0)
    {
       server = server_;
       dest = dest_;
       src = src_;
       seq = seq_;
       bMemPool = bMemPool_;
       bRouteData = true;

       bEncrypt = false;

       init_body_buffer();
    }


    virtual  ~send_msg_buf()
    {
       free_body_buffer();
    }

public:
    inline void init_body_buffer(){
    }

    inline void free_body_buffer(){
        body_buff = nullptr;
        body_buff_len = 0;
        capacity = 0;
    }

    inline void serialize_msg_head(){
        memset(pack_head, 0, PACK_HEAD_LEN);
        int packLen = getHeadSize() + getBodySize() - 4;
        memcpy(pack_head + 0, &packLen, 4);
        memcpy(pack_head + 4, (const char*)&dest,  4);
        memcpy(pack_head + 8, (const char*)&src,   4);
        memcpy(pack_head + 12, (const char*)&seq,  4);
        
        msg.append(pack_head, getHeadSize());
        
    }

    inline void serialize_msg_body(){
        //assert(g_aes_encryptor != nullptr);
        msg.append(body_msg);
    }

    inline void encrypt_body(){
/*        if(!bMemPool){
            body_msg = g_aes_encryptor->EncryptString(body_msg);
         }else{
            string body = g_aes_encryptor->EncryptString((const unsigned char*)body_buff, body_buff_len);
            memcpy(body_buff, body.c_str(),  body.size());
            body_buff_len = body.size();
        }
*/	
    }

public:
    inline virtual void appendBody(const char* data, int len){
           body_msg.append(data, len);
           return;
    }


    inline virtual const char* getBody() const {
         return body_msg.c_str();
    }


    inline virtual uint32_t getBodySize() const {
         return body_msg.size();
    }

    inline uint32_t getHeadSize() const {
         return PACK_HEAD_LEN;
    }

    inline virtual void serialize(){
        if(bEncrypt){
            encrypt_body();
        }

        serialize_msg_head();
        serialize_msg_body();
    }

private:
    char pack_head[PACK_HEAD_LEN];
    uint32_t capacity;

    std::string body_msg;
    char* body_buff;
    uint32_t body_buff_len;

    bool bMemPool;
    bool bEncrypt;
};

class recv_msg_buf : public msg_buf
{
public:
    recv_msg_buf(bool bMemPool_ = MEM_POOL)
    :msg_buf()
    ,body_buff(nullptr)
    ,body_buff_len(0)
    {
       bMemPool = bMemPool_;
       bRouteData = true;
       
       bEncrypt = false;

       init_body_buffer();
    }


    virtual  ~recv_msg_buf()
    {
       free_body_buffer();
    }

private:
    inline void init_body_buffer(){
    }

    inline void free_body_buffer(){
        body_buff = nullptr;
        body_buff_len = 0;
        capacity = 0;
    }

   inline void unSerialize_msg_head(){
        memcpy(pack_head,  msg.c_str(), PACK_HEAD_LEN);
        int packLen = 0;
        memcpy(&packLen, pack_head + 0,  4);
        memcpy(&dest,    pack_head + 4,  4);
        memcpy(&src,     pack_head + 8,  4);
        memcpy(&seq,     pack_head + 12, 4);
    }

    inline void unSerialize_msg_body(){
        if(bEncrypt){
            decrypt_body();
            return;          
        }
           
            body_msg.append(msg.c_str()+getHeadSize(), getSize()-getHeadSize());
    }

    inline void decrypt_body(){
/*        string body = msg.substr(getHeadSize());

        if(!bMemPool){
            body_msg.append(g_aes_encryptor->DecryptString(body));   
        }else{     
            string body_dec = g_aes_encryptor->DecryptString(body); 
            memcpy(body_buff, body_dec.c_str(), body_dec.length());
            body_buff_len = body_dec.length();
        }
*/	
    }

public:
    inline virtual void append(const char* data, int len){
       msg.append(data, len);
    }

    inline virtual const char* getBody() const {
         return body_msg.c_str();

    inline virtual uint32_t getBodySize() const {
         return body_msg.size();
    }

    inline uint32_t getHeadSize() const {
         return PACK_HEAD_LEN;
    }

    inline virtual void unSerialize(){
        unSerialize_msg_head();
        unSerialize_msg_body();
    }

private:
    char pack_head[PACK_HEAD_LEN];
    uint32_t capacity;

    std::string body_msg;
    char* body_buff;
    uint32_t body_buff_len;

    bool bMemPool;
    bool bEncrypt;
};

