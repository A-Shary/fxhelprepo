#include "stdafx.h"
#include "NetworkHelper.h"
#include "third_party/chromium/base/strings/sys_string_conversions.h"

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��

#include "../Network/GiftNotifyManager.h"
#include "../Network/CurlWrapper.h"
#include "../Network/EncodeHelper.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"

namespace
{
    uint32 userid;
    uint32 roomid;
    std::string nickname;
    uint32 richlevel;
    uint32 ismaster;
    uint32 staruserid;
    std::string key;
    std::string ext;
};

NetworkHelper::NetworkHelper()
    :curlWrapper_(new CurlWrapper)
    , giftNotifyManager_(new GiftNotifyManager)
{
    
}


NetworkHelper::~NetworkHelper()
{
    
}

bool NetworkHelper::Initialize()
{
    CurlWrapper::CurlInit();
    curlWrapper_->Initialize();
    giftNotifyManager_->Initialize();
    return true;
}

void NetworkHelper::Finalize()
{
    giftNotifyManager_->Finalize();
    curlWrapper_->Finalize();
    CurlWrapper::CurlCleanup();
    return;
}

void NetworkHelper::SetNotify(notifyfn fn)
{
    notify_ = fn;
}

void NetworkHelper::RemoveNotify()
{
    notify_ = nullptr;
}

bool NetworkHelper::EnterRoom(const std::wstring& strroomid)
{
    LOG(INFO) << L"EnterRoom " << strroomid;
    base::StringToUint(strroomid, &roomid);

    bool ret = false;

    if (!curlWrapper_->Servies_Uservice_UserService_getCurrentUserInfo(
        roomid, &userid, &nickname, &richlevel))
    {
        return false;
    }

    ret = curlWrapper_->RoomService_RoomService_enterRoom(
        static_cast<uint32>(roomid));
    assert(ret);
    if (!ret)
    {
        return false;
    }

    ret = curlWrapper_->ExtractStarfulInfo_RoomService_enterRoom(&staruserid, &key, &ext);

    if (!ret)
    {
        return false;
    }

    ret = giftNotifyManager_->Connect843();
    assert(ret);
    giftNotifyManager_->Set601Notify(
        std::bind(&NetworkHelper::NotifyCallback601, 
        this, std::placeholders::_1));

    giftNotifyManager_->SetNormalNotify(
        std::bind(&NetworkHelper::NotifyCallback, 
        this, std::placeholders::_1));

    ret = giftNotifyManager_->Connect8080(roomid, userid, nickname, richlevel,
        ismaster, staruserid, key, ext);

    return ret;
}

bool NetworkHelper::Login(const std::wstring& username, 
    const std::wstring& password)
{
    if (!curlWrapper_->LoginRequestWithUsernameAndPassword(WideToUtf8(username), WideToUtf8(password)))
    {
        return false;
    }

    // ��ȡcookie��������
    if (!curlWrapper_->Services_UserService_UserService_getMyUserDataInfo())
    {
        return false;
    }

    // ��ȡcookie��������
    if (!curlWrapper_->Services_IndexService_IndexService_getUserCenter())
    {
        return false;
    }
    return true;
}

// giftNotifyManager_ �̻߳ص�
void NetworkHelper::NotifyCallback(const std::wstring& message)
{
    // �������ݰ�
    LOG(INFO) << __FUNCTION__ << L" " << base::SysWideToMultiByte(message, 936);
    if (notify_)
    {
        notify_(message);
    }
}

// giftNotifyManager_ �̻߳ص�
void NetworkHelper::NotifyCallback601(const std::string& data)
{
    std::wstring responsedata;
    for (int i = 0; i < 20; i++)
    {
        bool ret = curlWrapper_->GiftService_GiftService(
            roomid, data, &responsedata);
        if (notify_)
        {
            notify_(responsedata);
        }
    }
    
    return;
}
