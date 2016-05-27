#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <map>
#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "Network/MessageNotifyManager.h"
#include "Network/CurlWrapper.h"

class CurlWrapper;
class CookiesHelper;
class MessageNotifyManager;
class GiftInfoHelper;
class GiftInfo;
class User;

typedef std::function<void(const std::wstring&)> notifyfn;

typedef std::vector<std::wstring> RowData;
typedef std::vector<RowData> GridData;
typedef std::function<void(const RowData&)> notify201;
typedef std::function<void(const RowData&)> notify501;
typedef std::function<void(uint32,const std::wstring&)> notify502;
typedef std::function<void(const RoomGiftInfo601&, const GiftInfo&)> notify601;

class GiftNetworkHelper
{
public:
    GiftNetworkHelper();
    ~GiftNetworkHelper();

    bool Initialize();// �����߳�
    void Finalize();// �����߳�

    void SetNotify(notifyfn fn);
    void RemoveNotify();

    void SetNotify201(notify201 fn);
    void RemoveNotify201();

    void SetNotify501(notify501 fn);
    void RemoveNotify501();

    void SetNotify502(notify502 fn);
    void RemoveNotify502();

    void SetNotify601(notify601 fn);
    void RemoveNotify601();

    // ��������¼���û�ʹ��
    bool EnterRoom(uint32 strroomid, uint32* singerid);
    bool EnterRoom(const std::wstring& strroomid, uint32* singerid);
    bool GetGiftList(uint32 roomid);

    bool ConnectToNotifyServer(uint32 roomid, uint32 userid,
        const std::string& nickname,
        uint32 richlevel, uint32 ismaster,
        uint32 staruserid,
        const std::string& key,
        const std::string& ext);

private:
    void NotifyCallback(const std::wstring& message);
    void NotifyCallback601(uint32 roomid, uint32 singerid, const RoomGiftInfo601& roomgiftinfo);
    void NotifyCallback201(const EnterRoomUserInfo& enterRoomUserInfo);
    void NotifyCallback501(const EnterRoomUserInfo& enterRoomUserInfo);

    bool EnterRoom_(uint32 roomid, uint32* singerid);
    bool ConnectToNotifyServer_(uint32 roomid, uint32 userid,
                               const std::string& nickname,
                               uint32 richlevel, uint32 ismaster,
                               uint32 staruserid,
                               const std::string& key,
                               const std::string& ext);

    std::unique_ptr<CurlWrapper> curlWrapper_ = nullptr;
    std::unique_ptr<CookiesHelper> cookiesHelper_ = nullptr;
    std::unique_ptr<GiftInfoHelper> giftInfoHelper_;
    std::unique_ptr<MessageNotifyManager> messageNotifyManager_;
    std::map<uint32, EnterRoomUserInfo> enterRoomUserInfoMap_;
    notifyfn notify_;
    notify201 notify201_;
    notify201 notify501_;
    notify502 notify502_;
    notify601 notify601_;
    uint32 roomid_ = 0;
};

