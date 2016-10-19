#include "stdafx.h"
#include "UserController.h"
#include "MVBillboard.h"
#include "Network/User.h"
#include "Network/IpProxy.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"

namespace
{
    //const char* serverip = "42.62.68.50";
    const char* serverip = "114.54.2.205";
}
UserController::UserController(TcpManager* tcpManager)
    : mvBillboard_(new MVBillboard)
    , tcpManager_(tcpManager)
{
    assert(tcpManager_);
}

UserController::~UserController()
{
}

bool UserController::AddUser(const std::string& username, 
    const std::string& password, const IpProxy& ipproxy, std::string* errormsg)
{
    if (users_.find(username) != users_.end())
    {
        *errormsg = "�ظ���¼";
        return false;
    }
    std::shared_ptr<User> shared_user(new User);
    if (ipproxy.GetProxyType() != IpProxy::PROXY_TYPE::PROXY_TYPE_NONE)
        shared_user->SetIpProxy(ipproxy);
    
    if (!shared_user->Login(username, password, "", errormsg))
    {
        std::wstring werror = base::UTF8ToWide(*errormsg);
        //assert(false && L"��¼ʧ��");
        return false;
    }
   
    shared_user->SetTcpManager(tcpManager_);
    shared_user->SetRoomServerIp(serverip);
    users_[username] = shared_user;

    return true;
}

bool UserController::AddUserWithCookies(const std::string& username,
    const std::string& cookies, const IpProxy& ipproxy, std::string* errormsg)
{
    if (users_.find(username)!=users_.end())
    {
        *errormsg = "�ظ���¼";
        return false;
    }
    std::shared_ptr<User> shared_user(new User);
    shared_user->SetUsername(username);
    if (ipproxy.GetProxyType() != IpProxy::PROXY_TYPE::PROXY_TYPE_NONE)
        shared_user->SetIpProxy(ipproxy);

    if (!shared_user->LoginWithCookies(cookies, errormsg))
    {
        std::wstring werror = base::UTF8ToWide(*errormsg);
        assert(false && L"��¼ʧ��");
        return false;
    }
    


    shared_user->SetTcpManager(tcpManager_);
    shared_user->SetRoomServerIp(serverip);
    users_[username] = shared_user;

    return true;
}

bool UserController::GetUserLoginInfo(std::vector<UserLoginInfo>* userlogininfo)
{
    for (const auto& it : users_)
    {
        UserLoginInfo logininfo;
        logininfo.accountname = it.second->GetUsername();
        logininfo.passwod = it.second->GetPassword();
        logininfo.cookies = it.second->GetCookies();
        userlogininfo->push_back(logininfo);
    }
    return true;
}

bool UserController::SendGifts(const std::vector<std::string>& accounts,
    uint32 roomid, uint32 gift_id, uint32 gift_count,
    const std::function<void(const std::wstring& msg)>& callback)
{
    for (const auto& account : accounts)
    {
        auto result =users_.find(account);
        if (result == users_.end())
        {
            assert(false && L"�Ҳ�����Ӧ�û�");
            continue;
        }
        std::wstring msg = base::UTF8ToWide(result->first) + L" ���� [" + 
            base::UintToString16(gift_count) + L"] �� giftid = " + 
            base::UintToString16(gift_id) + L" ����";
        if (!result->second->SendGift(roomid, gift_id, gift_count))
        {
            msg += L"ʧ��";
        }
        else
        {
            msg += L"�ɹ�";
        }
        callback(msg);
    }
    return true;
}

bool UserController::FillRoom(uint32 roomid, uint32 count,
    const std::function<void(const std::wstring& msg)>& callback)
{
    for (const auto& it : users_)
    {
        Sleep(1);
        //it.second->EnterRoomFopAlive(roomid);
        // �������, ��Ҫ��ȡ���㹻��Ϣ�����ǲ���Ҫ������Ϣ
        std::wstring msg = base::UTF8ToWide(it.first) + L" ���뷿��";
        if (!it.second->EnterRoomFopOperation(roomid, nullptr))
        {
            msg += L"ʧ��";
        }
        else
        {
            msg += L"�ɹ�";
        }
        callback(msg);
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
        std::wstring message = base::UTF8ToWide(it.second->GetUsername()) + L"���" + base::UTF8ToWide(mvid);
        std::string cookie = it.second->GetCookies();
        
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
