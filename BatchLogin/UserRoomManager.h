#pragma once
#include "UserController.h"
#include "RoomController.h"
#include "third_party/chromium/base/basictypes.h"

// ���߳̿���
class UserRoomManager
{
public:
    UserRoomManager();
    ~UserRoomManager();

    bool LoadUserConfig();
    bool LoadRoomConfig();

    bool AddRoom(uint32 roomid);

private:
    std::unique_ptr<UserController> userController_;
    std::unique_ptr<RoomController> roomController_;
};

