#include "User.h"


User::User()
{
}


User::~User()
{
}

// ���ò���
void User::SetUserName(const std::string& username)
{
    username_ = username;
}
std::string User::GetUserName() const
{
    return username_;
}

void User::SetPassword(const std::string& password)
{
    password_ = password;
}

std::string User::GetPassword() const
{
    return password_;
}

void User::SetCookies(const std::vector<std::string> cookies)
{
    cookies_ = cookies;
}

std::vector<std::string> User::GetCookies() const
{
    return cookies_;
}

//���÷���������Ϣ�ص�����,����Ľ�������Ϊ����Ҫ�������ģ�鴦��
void SetRoomNotify()
{

}

// ������Ϊ
bool User::Login()
{
    return false;
}

bool User::Logout()
{
    return false;
}

bool User::EnterRoom(uint32 roomid)
{
    return false;
}

bool User::ExitRoom()
{
    return false;
}

bool User::Chat(const std::string& message)
{
    return false;
}

bool User::SendStar(uint32 count)
{
    return false;
}

bool User::RetrieveStart()
{
    return false;
}

bool User::SendGift(uint32 giftid)
{
    return false;
}

bool User::KickoutUser(uint32 userid)
{
    return false;
}

bool User::SilencedUser(uint32 userid)
{
    return false;
}
