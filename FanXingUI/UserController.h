#pragma once
#include <vector>
#include <memory>

class User;
// ���������û���

class UserController
{
public:
    UserController();
    ~UserController();

    // ִ�ж�Ӧ�����û����Ʋ���
    void Run();

private:
    std::vector<std::shared_ptr<User> > users_;
};

