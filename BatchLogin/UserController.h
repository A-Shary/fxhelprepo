#pragma once
#include <map>
#include <vector>
#include <memory>
#include <functional>
#undef max
#undef min
#include "third_party/chromium/base/basictypes.h"

class TcpManager;
class User;
class MVBillboard;
class IpProxy;
// ���������û���
struct UserLoginInfo
{
    std::string accountname;
    std::string passwod;
    std::string cookies;
};

class UserController
{
public:
    UserController(TcpManager* tcpManager);
    ~UserController();

    // ���Ѿ�֪��cookie������²���Ҫ�����ȽϷ�ʱ�ĵ�¼������
    bool AddUserWithCookies(const std::string& username, 
        const std::string& cookies, const IpProxy& ipproxy, 
        std::string* errormsg);

    bool AddUser(const std::string& username, const std::string& password, 
        const IpProxy& ipproxy, std::string* errormsg);

    bool GetUserLoginInfo(std::vector<UserLoginInfo>* userlogininfo);
    bool SendGifts(const std::vector<std::string>& accounts,
        uint32 roomid, uint32 gift_id, uint32 gift_count,
        const std::function<void(const std::wstring& msg)>& callback);

    bool FillRoom(uint32 roomid, uint32 count,
        const std::function<void(const std::wstring& msg)>& callback);
    bool UpMVBillboard(const std::string& collectionid, const std::string& mvid,
                       std::function<void(const std::wstring&message)> callback);

    // ִ�ж�Ӧ�����û����Ʋ���,��ʱ��������뷿���֪ͨ�ص�
    void Run();

private:
    TcpManager* tcpManager_;
    std::map<std::string, std::shared_ptr<User> > users_;
    std::unique_ptr<MVBillboard> mvBillboard_;
};

