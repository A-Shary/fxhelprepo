#include "stdafx.h"
#include "AuthorityNetwork.h"
#include "third_party/chromium/base/bind.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"

#include<stdio.h>  
#include<string.h>  

#include "event2/event.h"
#include <signal.h>
#include "event2/thread.h"



AuthorityNetwork::AuthorityNetwork()
    :network_thread_(nullptr)
    , notify_callback_(nullptr)
{
}


AuthorityNetwork::~AuthorityNetwork()
{
}

bool AuthorityNetwork::Initialize()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD(2, 2);
    (void)WSAStartup(wVersionRequested, &wsaData);

    network_thread_.reset(new base::Thread("network_thread"));
    return true;
}

void AuthorityNetwork::Finalize()
{
    if (network_thread_->IsRunning())
        Stop();
}

void AuthorityNetwork::SetNotify(
    std::function<void(const std::wstring& message)> callback)
{
    notify_callback_ = callback;
}

bool AuthorityNetwork::Start()
{
    network_thread_->Start();
    network_thread_->message_loop_proxy()->PostTask(
        FROM_HERE, base::Bind(&AuthorityNetwork::DispatchFunction,
        base::Unretained(this)));
    notify_callback_(L"����ҵ��");

    return true;
}

void AuthorityNetwork::Stop()
{
    exit_flag_ = true;
    event_active(hup_event_, EV_SIGNAL, 1);
    event_base_loopexit(event_base_, NULL);

    network_thread_->Stop();
    notify_callback_(L"ֹͣҵ��");
}

void AuthorityNetwork::DispatchFunction()
{
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(struct sockaddr_in));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9999);

    evthread_use_windows_threads();
    event_base_ = event_base_new();
    evthread_make_base_notifiable(event_base_);
    hup_event_ = evsignal_new(event_base_, SIGINT, signal_cb, this);
    event_add(hup_event_, NULL);

    notify_callback_(L"��������");
    evconnlistener *listener
        = evconnlistener_new_bind(event_base_, &AuthorityNetwork::listener_cb, this,
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
        10, (struct sockaddr*)&sin,
        sizeof(struct sockaddr_in));

    notify_callback_(L"�����ɷ�����");
    event_base_dispatch(event_base_);

    evconnlistener_free(listener);
    event_base_free(event_base_);
    exit_flag_ = true;
    notify_callback_(L"�˳���������");
    return ;
}

//һ���¿ͻ��������Ϸ�������  
//���˺���������ʱ��libevent�Ѿ�������accept������ͻ��ˡ��ÿͻ��˵�
//�ļ�������Ϊfd
void AuthorityNetwork::listener_cb(evconnlistener *listener, evutil_socket_t fd,
struct sockaddr *sock, int socklen, void *arg)
{
    LOG(INFO)<<"accept a client " << fd;

    AuthorityNetwork* network = reinterpret_cast<AuthorityNetwork*>(arg);
    event_base *base = network->event_base_;
    auto notify_callback = network->notify_callback_;

    std::wstring wstr = L"accept a client";
    notify_callback(wstr);

    // ��¼���Ӷ˵���Ϣ

    //Ϊ����ͻ��˷���һ��bufferevent  
    bufferevent *bev = bufferevent_socket_new(base, fd,
                                              BEV_OPT_CLOSE_ON_FREE);
    
    // ����Ҫ���϶�write_cb�Ĵ���
    bufferevent_setcb(bev, AuthorityNetwork::socket_read_cb, NULL, 
                      AuthorityNetwork::socket_event_cb, (void*)(network));
    bufferevent_enable(bev, EV_READ | EV_PERSIST);
}


void AuthorityNetwork::socket_read_cb(bufferevent *bev, void *arg)
{
    AuthorityNetwork* network = reinterpret_cast<AuthorityNetwork*>(arg);
    event_base *base = network->event_base_;
    auto notify_callback = network->notify_callback_;


    char msg[4096];

    size_t len = bufferevent_read(bev, msg, sizeof(msg) - 1);

    msg[len] = '\0';
    printf("server read the data %s\n", msg);
    std::wstring wstr = L"server read the data:";
    wstr += base::UTF8ToWide(msg);
    notify_callback(wstr);

    // �Ӹ�ҵ���߳�ȥ����ͻ����͹���������


    char reply[] = "I has read your data";
    bufferevent_write(bev, reply, strlen(reply));
    wstr = L"I has read your data";
    notify_callback(wstr);
}


void AuthorityNetwork::socket_event_cb(bufferevent *bev, short events, void *arg)
{
    AuthorityNetwork* network = reinterpret_cast<AuthorityNetwork*>(arg);
    event_base *base = network->event_base_;
    auto notify_callback = network->notify_callback_;

    if (events & BEV_EVENT_EOF)
    {
        printf("connection closed\n");
        notify_callback(L"connection closed");
    }
        
    else if (events & BEV_EVENT_ERROR)
    {
        printf("some other error\n");
        notify_callback(L"some other error");
    }

    //�⽫�Զ�close�׽��ֺ�free��д������  
    bufferevent_free(bev);
}

void AuthorityNetwork::signal_cb(intptr_t sock, short flags, void * args)
{
    AuthorityNetwork* network = reinterpret_cast<AuthorityNetwork*>(args);
    event_base *base = network->event_base_;
    auto notify_callback = network->notify_callback_;

    notify_callback(L"signal_cb call!");
}
