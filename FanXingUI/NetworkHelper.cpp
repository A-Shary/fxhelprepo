#include "stdafx.h"
#include "NetworkHelper.h"
#include "Network/User.h"
#include "third_party/chromium/base/strings/sys_string_conversions.h"

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��

#include "Network/MessageNotifyManager.h"
#include "Network/CurlWrapper.h"
#include "Network/EncodeHelper.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"

namespace
{
    RowData EnterRoomUserInfoToRowdata(const EnterRoomUserInfo& enterRoomUserInfo)
    {
        RowData rowdata;
        rowdata.push_back(base::SysUTF8ToWide(enterRoomUserInfo.nickname));
        rowdata.push_back(base::UintToString16(enterRoomUserInfo.richlevel));
        rowdata.push_back(base::UintToString16(enterRoomUserInfo.userid));
        base::Time entertime = base::Time::FromDoubleT(enterRoomUserInfo.unixtime);
        std::wstring time = base::SysUTF8ToWide(MakeFormatTimeString(entertime).c_str());
        rowdata.push_back(time);
        rowdata.push_back(base::UintToString16(enterRoomUserInfo.roomid));
        return rowdata;
    }
};

NetworkHelper::NetworkHelper()
    : authority_(new Authority)
{
    
}


NetworkHelper::~NetworkHelper()
{
    
}

bool NetworkHelper::Initialize()
{
    CurlWrapper::CurlInit();
    user_.reset(new User);

    AuthorityHelper authorityHelper;
    bool result = authorityHelper.Load(authority_.get());
    return result;
}

void NetworkHelper::Finalize()
{
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

void NetworkHelper::SetNotify201(notify201 fn)
{
    notify201_ = fn;
}

void NetworkHelper::RemoveNotify201()
{
    notify201_ = nullptr;
}

void NetworkHelper::SetNotify501(notify501 fn)
{
    notify501_ = fn;
}

void NetworkHelper::RemoveNotify501()
{
    notify501_ = nullptr;
}

void NetworkHelper::SetNotify502(notify502 fn)
{
    notify502_ = fn;
}
void NetworkHelper::RemoveNotify502()
{
    notify502_ = nullptr;
}


void NetworkHelper::SetNotify601(notify601 fn)
{
    notify601_ = fn;
}

void NetworkHelper::RemoveNotify601()
{
    notify601_ = nullptr;
}

bool NetworkHelper::Login(const std::wstring& username, 
    const std::wstring& password)
{
    std::string strusername = base::WideToUTF8(username);
    std::string strpassword = base::WideToUTF8(password);
    return user_->Login(strusername, strpassword);
}
bool NetworkHelper::GetCurrentUserDisplay(std::wstring* display)
{
    uint32 clanid = user_->GetClanId();
    uint32 fanxingid = user_->GetFanxingId();
    uint64 servertime = static_cast<uint64>(user_->GetServerTime());
    if (!fanxingid || !clanid || !servertime)
        return false;

    servertime *= 1000000;
    std::wstring loginInfo = L"��¼��Ϣ: ";
    loginInfo += L"���Ǻ�: " + base::UintToString16(fanxingid);
    loginInfo += L" ����ID: " + base::UintToString16(clanid);
    *display = loginInfo;
    return true;
}

bool NetworkHelper::EnterRoom(const std::wstring& roomid)
{
    std::string strroomid = base::WideToUTF8(roomid);
    uint32 introomid = 0;
    if (!base::StringToUint(strroomid, &introomid))
    {
        return false;
    }
    return EnterRoom(introomid);
}

bool NetworkHelper::EnterRoom(uint32 roomid)
{
    user_->SetNotify201(std::bind(&NetworkHelper::NotifyCallback201, this,
        std::placeholders::_1));
    user_->SetNormalNotify(std::bind(&NetworkHelper::NotifyCallback, this,
        std::placeholders::_1));
    roomid_ = roomid;
    return user_->EnterRoom(roomid);
}

bool NetworkHelper::GetViewerList(uint32 roomid,
    std::vector<RowData>* enterRoomUserInfoRowdata)
{
    std::vector<EnterRoomUserInfo> enterRoomUserInfoList;
    bool result = user_->GetViewerList(roomid, &enterRoomUserInfoList);
    for (const auto& enterRoomUserInfo : enterRoomUserInfoList)
    {
        RowData rowdata = EnterRoomUserInfoToRowdata(enterRoomUserInfo);
        enterRoomUserInfoRowdata->push_back(rowdata);
    }
    return result;
}

// messageNotifyManager_ �̻߳ص�
void NetworkHelper::NotifyCallback(const std::wstring& message)
{
    // �������ݰ�
    LOG(INFO) << __FUNCTION__ << L" " << base::SysWideToMultiByte(message, 936);
    if (notify_)
    {
        notify_(message);
    }
}

bool NetworkHelper::KickoutUsers(KICK_TYPE kicktype, uint32 roomid, const EnterRoomUserInfo& enterRoomUserInfo)
{
	return user_->KickoutUser(kicktype, roomid, enterRoomUserInfo);;
}

bool NetworkHelper::BanChat(uint32 roomid, const EnterRoomUserInfo& enterRoomUserInfo)
{
    return user_->BanChat(roomid, enterRoomUserInfo);
}

bool NetworkHelper::UnbanChat(uint32 roomid, const EnterRoomUserInfo& enterRoomUserInfo)
{
    return user_->UnbanChat(roomid, enterRoomUserInfo);
}

bool NetworkHelper::GetActionPrivilege(std::wstring* message)
{
    if (user_->GetFanxingId() != authority_->userid)
    {
        *message = L"��ǰ�û�δ��Ȩ!";
        return false;
    }

    uint32 servertime = user_->GetServerTime();
    uint64 expiretime = authority_->expiretime - base::Time::UnixEpoch().ToInternalValue();
    expiretime /= 1000000;
    if (servertime > expiretime)
    {
        *message = L"��ǰ�û���Ȩ�ѹ���!";
        return false;
    }

    if (authority_->clanid && user_->GetClanId() != authority_->clanid)
    {
        *message = L"��ǰ��Ȩ����ָ�������Աʹ��!";
        return false;
    }


    if (authority_->roomid && (roomid_ != authority_->roomid))
    {
        *message = L"��ǰ��Ȩ����ָ������ʹ��!";
        return false;
    }

    return true;
}

// messageNotifyManager_ �̻߳ص�
void NetworkHelper::NotifyCallback601(uint32 roomid, uint32 singerid, const RoomGiftInfo601& roomgiftinfo601)
{
    if (!notify601_)
        return;

    // ��������ڱ������͸���������Ϣ�����˵����ص�
    if ((roomid!=roomgiftinfo601.roomid)
        ||(singerid != roomgiftinfo601.receiverid))
    {
        return;
    }

    if (!roomgiftinfo601.token.empty())
    {
        // ԭ�������ҵĶ�����Ŀǰ�������๦��
    }

    //notify601_(roomgiftinfo601, giftinfo);
}

void NetworkHelper::NotifyCallback201(const EnterRoomUserInfo& enterRoomUserInfo)
{
    if (!notify201_)
        return;

    enterRoomUserInfoMap_[enterRoomUserInfo.userid] = enterRoomUserInfo;
    RowData rowdata = EnterRoomUserInfoToRowdata(enterRoomUserInfo);
    notify201_(rowdata);
}

void NetworkHelper::NotifyCallback501(const EnterRoomUserInfo& enterRoomUserInfo)
{
    if (!notify501_)
        return;

    enterRoomUserInfoMap_[enterRoomUserInfo.userid] = enterRoomUserInfo;
    RowData rowdata = EnterRoomUserInfoToRowdata(enterRoomUserInfo);
    notify501_(rowdata);
}

