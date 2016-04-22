#pragma once
#include <vector>
#include <memory>
#include "third_party/chromium/base/basictypes.h"

class User;
// ���������û���

class UserController
{
public:
    UserController();
    ~UserController();

    bool AddUser(const std::string& username, const std::string& password);
    bool FillRoom(uint32 roomid, uint32 count);

    // ִ�ж�Ӧ�����û����Ʋ���,��ʱ��������뷿���֪ͨ�ص�
    void Run();

private:
    std::vector<std::shared_ptr<User> > users_;
};

