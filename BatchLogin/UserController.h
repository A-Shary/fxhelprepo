#pragma once
#include <vector>
#include <memory>
#undef max
#undef min
#include "third_party/chromium/base/basictypes.h"

class User;
class MVBillboard;
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
    UserController();
    ~UserController();

    // ���Ѿ�֪��cookie������²���Ҫ�����ȽϷ�ʱ�ĵ�¼������
    bool AddUserWithCookies(const std::string& username, const std::string& cookies);

    bool AddUser(const std::string& username, const std::string& password);

    bool GetUserLoginInfo(std::vector<UserLoginInfo>* userlogininfo);
    bool FillRoom(uint32 roomid, uint32 count);
    bool UpMVBillboard(const std::string& collectionid, const std::string& mvid);

    // ִ�ж�Ӧ�����û����Ʋ���,��ʱ��������뷿���֪ͨ�ص�
    void Run();

private:
    std::vector<std::shared_ptr<User> > users_;
    std::unique_ptr<MVBillboard> mvBillboard_;
};

