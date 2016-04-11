#pragma once

#include <string>
#include <vector>
#include <memory>
#include <WinSock.h>

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
    
    bool HandleData(const std::vector<char>& data, int len);
private:
    SOCKET socket_;
};

