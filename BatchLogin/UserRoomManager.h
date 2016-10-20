#pragma once
#include <string>
#include <map>
#include <functional>
#include "UserController.h"
#include "RoomController.h"
#include "Network/IpProxy.h"
#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/memory/scoped_ptr.h"
#include "third_party/chromium/base/threading/thread.h"

class TcpManager;
// ���߳̿���
class UserRoomManager 
    :public std::enable_shared_from_this<UserRoomManager>
{
public:
    UserRoomManager(TcpManager* tcpManager_);
    ~UserRoomManager();

    static void AddRef() {}
    static void Release() {}

    bool Initialize();
    bool Finalize();

    void SetNotify(std::function<void(std::wstring)> notify);

    bool LoadUserConfig(GridData* userpwd, uint32* total) const;
    bool LoadRoomConfig(GridData* roomgrid, uint32* total) const;
    bool SaveUserLoginConfig();

    bool LoadIpProxy(GridData* proxyinfo);

    bool BatchLogUsers(const std::map<std::wstring, std::wstring>& userAccountPassword);
    bool BatchLogUsersWithCookie(const std::map<std::wstring, std::wstring>& accountCookie);
    
    bool FillRooms(const std::vector<std::wstring>& roomids);

    bool UpMVBillboard(const std::wstring& collectionid, const std::wstring& mvid);

    bool SendGifts(const std::vector<std::wstring>& users, const std::wstring& room_id,
        uint32 gift_id, uint32 gift_count);

    bool RobVotes(const std::vector<std::wstring>& users, const std::wstring& room_id);

    bool GetUserStorageInfos(const std::vector<std::wstring>& users,
        std::vector<UserStorageInfo>* user_storage_infos);

    void SetBreakRequest(bool interrupt);

protected:
    void Notify(const std::wstring& msg);
    void DoSaveUserLoginConfig();
    void DoBatchLogUsers(const std::map<std::wstring, std::wstring>& userAccountPassword);
    void DoBatchLogUsersWithCookie(const std::map<std::wstring, std::wstring>& accountCookie);
    void DoFillRooms(const std::vector<uint32>& roomids);
    void FillSingleRoom(uint32 roomid);

    void DoUpMVBillboard(const std::wstring& collectionid, const std::wstring& mvid);
    void DoSendGifts(const std::vector<std::wstring>& users,
        uint32 roomid, uint32 gift_id, uint32 gift_count);
    void DoRobVotes(const std::vector<std::wstring>& users, uint32 roomid);

private:
    base::Thread workerThread_;
    std::function<void(std::wstring)> notify_ = nullptr;
    std::unique_ptr<UserController> userController_ = nullptr;
    std::unique_ptr<RoomController> roomController_ = nullptr;
    std::map<std::string, IpProxy> ipProxys_;
    bool break_request_ = false;
};

