#pragma once

#include <functional>

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��

#include "third_party/chromium/base/memory/scoped_ptr.h"
#include "third_party/chromium/base/threading/thread.h"

#include "event2/util.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"

class AuthorityNetwork
{
public:
    AuthorityNetwork();
    ~AuthorityNetwork();

    bool Initialize();
    void Finalize();

    void SetNotify(std::function<void(const std::wstring& message)> callback);

    bool Start();
    void Stop();

private:
    void DispatchFunction();
    static void listener_cb(evconnlistener *listener, evutil_socket_t fd,
                     struct sockaddr *sock, int socklen, void *arg);
    static void socket_read_cb(bufferevent *bev, void *arg);
    static void socket_event_cb(bufferevent *bev, short events, void *arg);
    static void signal_cb(intptr_t sock, short flags, void * args);

    struct event_base * event_base_ = 0;
    struct event* hup_event_ = 0;
    bool exit_flag_ = false;
    scoped_ptr<base::Thread> network_thread_;

    std::function<void(const std::wstring&)> notify_callback_;
};

