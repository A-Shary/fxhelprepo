#include "stdafx.h"
#include "UserController.h"
#include "Network/User.h"

UserController::UserController()
{
}


UserController::~UserController()
{
}

void UserController::Run()
{
    // ��ȡ�����ļ�����

    // �����û�
    std::shared_ptr<User> user(new User);
    user->Login("fanxingtest001", "1233211234567");
    user->EnterRoom(1084594);
    users_.push_back(user);
}
