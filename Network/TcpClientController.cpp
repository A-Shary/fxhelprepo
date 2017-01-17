#include "stdafx.h"
#include "TcpClientController.h"

#include <signal.h>

#include "event2/event.h"
#include "event2/util.h"
#include "event2/thread.h"

namespace
{   
std::string data = R"(GET /9AoX9ROWdLBGpj3Uk2T1_Q==/1380986614829967.jpg?param=138y138 HTTP/1.1
Host: p1.music.126.net
Connection: keep-alive
Accept: image/webp,image/*,*/*;q=0.8
User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.110 Safari/537.36
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.8

)";

    struct echo_context
    {
        struct event_base *base;
        struct event *event_write;
        struct event *event_read;
    };
}

TcpClientController::TcpClientController()
{
}


TcpClientController::~TcpClientController()
{
}

bool TcpClientController::Initialize()
{
    WORD wVersionRequested;
    WSADATA wsaData;

    wVersionRequested = MAKEWORD(2, 2);

    (void)WSAStartup(wVersionRequested, &wsaData);
    worker_thread_.reset(new std::thread(std::bind(&TcpClientController::WorkerFunction, this)));

    return true;
}

void TcpClientController::Finalize()
{
    exit_flag_ = true;
    event_active(hup_event_, EV_SIGNAL, 1);
    event_base_loopexit(event_base_, NULL);

    if (worker_thread_)
        worker_thread_->join();
}

bool TcpClientController::AddClient(AddClientCallback addcallback, const IpProxy& ipproxy,
    const std::string& ip, uint16 port, ClientCallback callback)
{
    return InnerAddClient(ip, port, addcallback, callback);
}

bool TcpClientController::InnerAddClient(const std::string&ip, uint16 port,
                                 ConnectCallback connect_cb, 
                                 DataReceiveCallback data_cb)
{
    SingleTcpClient* client = new SingleTcpClient;
    SocketHandle handle = client->Connect(event_base_, ip, port, connect_cb, data_cb);
    if (handle <= 0)
        return false;

    tcp_client_map_.erase(handle);
    tcp_client_map_[handle] = client;
    return true;
}

void TcpClientController::RemoveClient(SocketHandle handle)
{
    auto result = tcp_client_map_.find(handle);
    if (result != tcp_client_map_.end())
    {
        result->second->Close();
        delete result->second;
        tcp_client_map_.erase(result);
    }
}

bool TcpClientController::Send(SocketHandle handle, const std::vector<uint8>& data, 
                            SendDataCallback callback)
{
    auto result = tcp_client_map_.find(handle);
    if (result == tcp_client_map_.end())
        return false;

    return result->second->Send(data);
}

void TcpClientController::signal_cb(intptr_t sock, short flags, void * args)
{

}

void TcpClientController::WorkerFunction()
{
    evthread_use_windows_threads();
    event_base_ = event_base_new();
    evthread_make_base_notifiable(event_base_);
    hup_event_ = evsignal_new(event_base_, SIGINT, signal_cb, NULL);
    event_add(hup_event_, NULL);
    event_base_dispatch(event_base_);
    event_base_free(event_base_);
}