#include "stdafx.h"
#include "UserController.h"
#include "MVBillboard.h"
#include "Network/User.h"
#include "Network/IpProxy.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"

namespace
{
    const char* serverip = "42.62.68.50";
}
UserController::UserController()
    : mvBillboard_(new MVBillboard)
{
}

UserController::~UserController()
{
}

bool UserController::AddUser(const std::string& username, 
    const std::string& password, const IpProxy& ipproxy)
{
    std::shared_ptr<User> shared_user(new User(username, password));
    if (ipproxy.GetProxyType() != IpProxy::PROXY_TYPE::PROXY_TYPE_NONE)
        shared_user->SetIpProxy(ipproxy);

    if (!shared_user->Login())
    {
        assert(false && L"��¼ʧ��");
        return false;
    }
   
    shared_user->SetRoomServerIp(serverip);
    users_.push_back(shared_user);
    return true;
}

bool UserController::AddUserWithCookies(const std::string& username,
    const std::string& cookies, const IpProxy& ipproxy)
{
    std::shared_ptr<User> shared_user(new User(username, ""));
    shared_user->SetCookies(cookies);
    if (ipproxy.GetProxyType() != IpProxy::PROXY_TYPE::PROXY_TYPE_NONE)
        shared_user->SetIpProxy(ipproxy);

    shared_user->SetRoomServerIp(serverip);
    users_.push_back(shared_user);
    return true;
}

bool UserController::GetUserLoginInfo(std::vector<UserLoginInfo>* userlogininfo)
{
    for (const auto& it : users_)
    {
        UserLoginInfo logininfo;
        logininfo.accountname = it->GetUsername();
        logininfo.passwod = it->GetPassword();
        logininfo.cookies = it->GetCookies();
        userlogininfo->push_back(logininfo);
    }
    return true;
}

bool UserController::FillRoom(uint32 roomid, uint32 count)
{
    for (const auto& it : users_)
    {
        it->EnterRoom(roomid);
    }
    return true;
}

bool UserController::UpMVBillboard(const std::string& collectionid,
                                   const std::string& mvid,
                                   std::function<void(const std::wstring&message)> callback)
{
    uint32 successCount = 0;
    for (const auto& it : users_)
    {
        std::wstring message = base::UTF8ToWide(it->GetUsername()) + L"���" + base::UTF8ToWide(mvid);
        std::string cookie = it->GetCookies();
        
        if (mvBillboard_->UpMVBillboard(cookie, collectionid, mvid))
        {
            message += L"�ɹ�";
            successCount++;
        }
        else
        {
            message += L"ʧ��";
        }
        if (callback)
        {
            callback(message);
        }
    }
    assert(users_.size() == successCount);
    return successCount > 0;
}

void UserController::Run()
{
}
