#pragma once

#include <functional>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <Winsock2.h>

#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/timer/timer.h"
#include "third_party/chromium/base/threading/thread.h"

// �ṩTCP���ӹ��ܣ��������ݣ��������ݣ�
// ��Ҫ����һ���߳̽�ȥ�������շ�����
class Thread;

typedef SOCKET TcpHandle;

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


