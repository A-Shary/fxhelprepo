#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>
#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "Network/MessageNotifyManager.h"
#include "Network/CurlWrapper.h"
#include "AuthorityHelper.h"

class CurlWrapper;
class GiftNotifyManager;
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

enum class HANDLE_TYPE
{
    HANDLE_TYPE_NOTHANDLE = 0,
    HANDLE_TYPE_KICKOUT = 1,
    HANDLE_TYPE_BANCHAT = 2
};

class AntiStrategy 
    : public std::enable_shared_from_this<AntiStrategy>
{
public:
    AntiStrategy();
    ~AntiStrategy();
    HANDLE_TYPE GetUserHandleType(const std::string& nickname) const;
    HANDLE_TYPE GetHandleType() const;
    void SetHandleType(HANDLE_TYPE handletype);

    bool AddNickname(const std::string& vestname);
    bool RemoveNickname(const std::string& vestname);
private:
    std::set<std::string> vestnames_;
    HANDLE_TYPE handletype_ = HANDLE_TYPE::HANDLE_TYPE_NOTHANDLE;
};

class NetworkHelper
{
public:
    NetworkHelper();
    ~NetworkHelper();

    bool Initialize();// �����߳�
    void Finalize();// �����߳�

    void SetAntiStrategy(std::shared_ptr<AntiStrategy> antiStrategy);

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

    bool Login(const std::wstring& username, const std::wstring& password);
    bool GetCurrentUserDisplay(std::wstring* display);
    bool EnterRoom(const std::wstring& roomid);
    bool EnterRoom(uint32 roomid);
    bool GetViewerList(uint32 roomid,
        std::vector<RowData>* enterRoomUserInfoRowdata);

    void SetHandleChatUsers(bool kick);
    // �ж��û��Ƿ��в���Ȩ�ޣ���ʱʵ��Ϊֻ�й����Ա���ܲ�����
    bool GetActionPrivilege(std::wstring* message);

    bool KickoutUsers(KICK_TYPE kicktype, uint32 roomid, 
        const EnterRoomUserInfo& enterRoomUserInfo);
    bool BanChat(uint32 roomid, const EnterRoomUserInfo& enterRoomUserInfo);
    bool UnbanChat(uint32 roomid, const EnterRoomUserInfo& enterRoomUserInfo);
    bool SendChatMessage(uint32 roomid, const std::string& message);

private:
    void NotifyCallback(const std::wstring& message);
    void NotifyCallback601(uint32 roomid, uint32 singerid, const RoomGiftInfo601& roomgiftinfo);
    void NotifyCallback201(const EnterRoomUserInfo& enterRoomUserInfo);
    void NotifyCallback501(const EnterRoomUserInfo& enterRoomUserInfo);
    void TryHandleUser(const EnterRoomUserInfo& enterRoomUserInfo);
    void TryHandle501Msg(const EnterRoomUserInfo& enterRoomUserInfo);

    std::map<uint32, EnterRoomUserInfo> enterRoomUserInfoMap_;
    notifyfn notify_;
    notify201 notify201_;
    notify501 notify501_;
    bool handleall501_ = false;
    notify502 notify502_;
    notify601 notify601_;
    uint32 roomid_ = 0;
    std::unique_ptr<User> user_;
    std::unique_ptr<Authority> authority_;
    std::shared_ptr<AntiStrategy> antiStrategy_;
};
