#pragma once

#include "handle.hpp"
#include "error.hpp"
#include "net.hpp"
#include <algorithm>
#include <memory>
#include <iostream>
namespace uvpp {
//template<typename HANDLE_T>
class Udp : public handle<uv_udp_t>
{
public:
    Udp(): handle<uv_udp_t>()
    {
        uv_udp_init(uv_default_loop(), get());
    }

    Udp(loop& l): handle<uv_udp_t>()
    {
        uv_udp_init(l.get(), get());
    }

    void reset(loop& l)
    {
      //read_stop();
      handle_reset();
      uv_udp_init(l.get(), get());
    }

   
    ~Udp()
    { 
         std::cout << "delete udp class ....." << std::endl;
         uv_udp_recv_stop(get());
    }

    int getsockname(struct sockaddr* name, int* namelen)
    {
        return uv_udp_getsockname(get(), name, namelen);
    }
   
    int set_ttl(int ttl)
    {
        return uv_udp_set_ttl(get(), ttl);
    }

    void set_dest(const std::string& ip, int port)
    {
        m_dest_addr = to_ip4_addr(ip, port);
    }

    void set_dest(ip4_addr* addr)
    {
        m_dest_addr = *addr;
    }
    // FIXME: refactor with getaddrinfo
    bool bind(const std::string& ip, int port)
    {
        ip4_addr addr = to_ip4_addr(ip, port);
        return uv_udp_bind(get(), reinterpret_cast<sockaddr*>(&addr), 0) == 0;
    }

    bool read_start(std::function<void(const char* buf, ssize_t len, const struct sockaddr* addr)> callback)
    {
        return read_start<0>(callback);
    }

    template<size_t max_alloc_size>
    bool read_start(std::function<void(const char* buf, ssize_t len, const struct sockaddr* addr)> callback)
    {
        //printf("udp read data fun start \n");
        callbacks::store(handle<uv_udp_t>::get()->data, uvpp::internal::uv_cid_udp_read_start, callback);

        auto alloc_cb = [](uv_handle_t*, size_t suggested_size, uv_buf_t* buf)
        {
            assert(buf);
            auto size = std::max(suggested_size, max_alloc_size);
            buf->base = new char[size];
            buf->len = size;
        };
    
        auto read_cb = [](uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags)                                                                                              {
            // handle callback throwing exception: hold data in unique_ptr
            //printf("recv data for udp len %d \n", (int)nread);
#if 1
            std::shared_ptr<char> baseHolder(buf->base, std::default_delete<char[]>());

            if (nread < 0)
            {
                // FIXME error has nread set to -errno, handle failure
                // assert(nread == UV_EOF); ???
                callbacks::invoke<decltype(callback)>(handle->data, uvpp::internal::uv_cid_udp_read_start, nullptr, nread, addr);
            }
            else if (nread >= 0)
            {
                callbacks::invoke<decltype(callback)>(handle->data, uvpp::internal::uv_cid_udp_read_start, buf->base, nread, addr);
            }
#endif
        };

        return uv_udp_recv_start(get(), alloc_cb, read_cb) == 0;

#if 0
        return uv_udp_recv_start(get(),
                             [](uv_handle_t*, size_t suggested_size, uv_buf_t* buf)
        {
            assert(buf);
            auto size = std::max(suggested_size, max_alloc_size);
            buf->base = new char[size];
            buf->len = size;
        },
        [](uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags)
        {
            // handle callback throwing exception: hold data in unique_ptr
            printf("recv data for udp len %d", (int)nread);
            std::shared_ptr<char> baseHolder(buf->base, std::default_delete<char[]>());

            if (nread < 0)
            {
                // FIXME error has nread set to -errno, handle failure
                // assert(nread == UV_EOF); ???
                callbacks::invoke<decltype(callback)>(handle->data, uvpp::internal::uv_cid_udp_read_start, nullptr, nread);
            }
            else if (nread >= 0)
            {
                callbacks::invoke<decltype(callback)>(handle->data, uvpp::internal::uv_cid_udp_read_start, buf->base, nread);
            }
        }) == 0;
#endif
    }

    bool read_stop()
    {
        return uv_udp_recv_stop(get()) == 0;
    }
    
    
    bool write(const char* buf, int len, void* data, std::function<void(void* data, error status)> callback)
    {
        uv_buf_t bufs[] = { uv_buf_t { const_cast<char*>(buf), static_cast<size_t>(len) } };
        callbacks::store(get()->data, uvpp::internal::uv_cid_udp_write, callback);
        uv_udp_send_t* req = new uv_udp_send_t;
        req->data = data;
        return uv_udp_send(req, get(), bufs, 1, (const sockaddr*)&m_dest_addr, [](uv_udp_send_t* req, int status)
        {
            std::unique_ptr<uv_udp_send_t> reqHolder(req);
            callbacks::invoke<decltype(callback)>(req->handle->data, uvpp::internal::uv_cid_udp_write, req->data, error(status));
        }) == 0;
    }

    bool write(const char* buf, int len, CallbackWithResult callback)
    {
        uv_buf_t bufs[] = { uv_buf_t { const_cast<char*>(buf), static_cast<size_t>(len) } };
        callbacks::store(get()->data, uvpp::internal::uv_cid_udp_write, callback);
        return uv_udp_send(new uv_udp_send_t, get(), bufs, 1, (const sockaddr*)&m_dest_addr, [](uv_udp_send_t* req, int status)
        {
            std::unique_ptr<uv_udp_send_t> reqHolder(req);
            callbacks::invoke<decltype(callback)>(req->handle->data, uvpp::internal::uv_cid_udp_write, error(status));
        }) == 0;
    }

    bool write(const std::string& buf, CallbackWithResult callback)
    {
        uv_buf_t bufs[] = { uv_buf_t { const_cast<char*>(buf.c_str()), buf.length()} };
        callbacks::store(get()->data, uvpp::internal::uv_cid_udp_write, callback);
        uv_udp_send_t* udp_s = new uv_udp_send_t;
        udp_s->data = (void*)buf.c_str();
       
        return uv_udp_send(udp_s, get(), bufs, 1, (const sockaddr*)&m_dest_addr, [](uv_udp_send_t* req, int status)
        {
            std::unique_ptr<uv_udp_send_t> reqHolder(req);
            callbacks::invoke<decltype(callback)>(req->handle->data, uvpp::internal::uv_cid_udp_write, error(status));
        }) == 0;
    }

    bool write(const std::vector<char>& buf, CallbackWithResult callback)
    {
        uv_buf_t bufs[] = { uv_buf_t { const_cast<char*>(&buf[0]), buf.size() } };
        callbacks::store(get()->data, uvpp::internal::uv_cid_udp_write, callback);
        return uv_udp_send(new uv_udp_send_t, get(), bufs, 1, (const sockaddr*)&m_dest_addr, [](uv_udp_send_t* req, int status)
        {
            std::unique_ptr<uv_udp_send_t> reqHolder(req);
            callbacks::invoke<decltype(callback)>(req->handle->data, uvpp::internal::uv_cid_udp_write, error(status));
        }) == 0;
    }
 
#if 0 
    bool shutdown(CallbackWithResult callback)
    {
        callbacks::store(get()->data, uvpp::internal::uv_cid_udp_shutdown, callback);
        return uv_shutdown(new uv_shutdown_t, get(), [](uv_shutdown_t* req, int status)
        {
            std::unique_ptr<uv_shutdown_t> reqHolder(req);
            callbacks::invoke<decltype(callback)>(req->handle->data, uvpp::internal::uv_cid_udp_shutdown, error(status));
        }) == 0;
    }
#endif

    private:
      ip4_addr m_dest_addr;
};
}
