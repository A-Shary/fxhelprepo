#pragma once

#include "AuthorityClientBussiness.h"
#include "third_party/chromium/base/callback.h"
#include "third_party/chromium/base/basictypes.h"
#include <vector>
#include <map>
#include "event2/bufferevent.h"

class AuthorityController
{
public:
    AuthorityController();
    ~AuthorityController();

    bool Initialize();
    void Finalize();

    void SetSendDataFunction(
        base::Callback<bool(bufferevent*, const std::vector<uint8>& data)> callback);

    bool Start();
    void Stop();

    bool AddClient(bufferevent *bev, struct sockaddr& sock);
    bool RemoveClient(bufferevent *bev);

    bool HandleMessage(bufferevent *bev, const std::vector<uint8>& data);

    bool AddUser();
    bool RemoveUser();
    bool QueryUser();
    bool ModifyUser();

private:
    bool SendDataToClient(bufferevent* bev, const std::vector<uint8>& data);

    // ���ݿ����

    // �������Ӷ���

    // Ȩ�޼�ض���

    // �����û�����
    std::map<bufferevent*, scoped_ptr<AuthorityClientBussiness>> client_map_;


    base::Callback<bool(bufferevent *, const std::vector<uint8>& data)> send_data_callback_;
};

