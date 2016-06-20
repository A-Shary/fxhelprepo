#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <WinSock.h>

#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/timer/timer.h"
#include "third_party/chromium/base/threading/thread.h"

// �ṩTCP���ӹ��ܣ��������ݣ��������ݣ�
// ��Ҫ����һ���߳̽�ȥ�������շ�����
class Thread;
class SocketWrapper;

typedef void(*NotifyFunction)(void* privatedata, const std::vector<char>& data);
class TcpClient
    //:std::enable_shared_from_this<TcpClient>
{
public:
    TcpClient();
    ~TcpClient();

    bool Initialize();
    void Finalize();

    bool Connect(const std::string& ip, unsigned short port);
    bool Send(const std::vector<char>& data);
    bool Recv(std::vector<char>* buffer);

private:
    SOCKET socket_;
};

typedef SOCKET TcpHandle;

class TcpManager
{
public:

    typedef std::function<void(bool, TcpHandle)> AddClientCallback;
    typedef std::function<void(bool, const std::vector<char>& data)> ClientCallback;

    TcpManager();
    ~TcpManager();

    static void AddRef() {}
    static void Release() {}

    bool Initialize();
    void Finalize();

    bool AddClient(AddClientCallback addcallback, const std::string& ip, unsigned short port, ClientCallback callback);
    void RemoveClient(TcpHandle handle);
    bool Send(TcpHandle handle, const std::vector<char>& data);
private:

    void DoAddClient(AddClientCallback addcallback, const std::string& ip, unsigned short port, ClientCallback callback);
    void DoRemoveClient(TcpHandle handle);
    void DoSend(TcpHandle handle, const std::vector<char>& data);
    void DoRecv();

    std::map<TcpHandle, ClientCallback> callbacks_;
    bool stopflag = false;
    base::Thread baseThread_;
};

