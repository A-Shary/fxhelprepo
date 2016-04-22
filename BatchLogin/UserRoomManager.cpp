#include "stdafx.h"
#include <memory>
#include <string>
#include <map>
#include <set>

#include "Network/EncodeHelper.h"
#include "UserRoomManager.h"

#undef max
#undef min
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"
#include "third_party/chromium/base/path_service.h"
#include "third_party/chromium/base/files/file_util.h"

namespace
{
    const uint32 roomusercount = 200;
}

UserRoomManager::UserRoomManager()
    :userController_(new UserController)
    , roomController_(new RoomController)
{
}


UserRoomManager::~UserRoomManager()
{
}

bool UserRoomManager::LoadUserConfig()
{
    // ���ļ�
    base::FilePath path;
    PathService::Get(base::DIR_EXE, &path);
    path = path.Append(L"BatchLogin.User.cfg");
    base::File userfile(path, base::File::FLAG_OPEN|base::File::FLAG_READ);
    if (!userfile.IsValid())
    {
        assert(false && L"��ȡ�����ļ�ʧ��");
        LOG(ERROR) << L"��ȡ�����ļ�BatchLogin.User.cfgʧ��";
        return false;
    }

    const uint32 memlen = 1024;
    std::string data;
    char str[memlen] = { 0 };
    userfile.Seek(base::File::FROM_BEGIN, 0);
    int read = userfile.ReadAtCurrentPos(str, memlen);
    DWORD err = GetLastError();
    while (read > 0)
    {
        data.insert(data.end(), str, str + read);
        if (read < memlen)//������
            break;
        read = userfile.ReadAtCurrentPos(str, memlen);
    }

    assert(!data.empty());

    std::vector<std::string> userinfos = SplitString(data, "\r\n");

    for (const auto& it : userinfos)
    {
        std::vector<std::string> userinfo = SplitString(it, "\t");
        if (userinfo.size()!=2) // �û���������
        {
            assert(false && L"account info error!");
            continue;
        }
        std::string username = userinfo[0];
        std::string password = userinfo[1];
        RemoveSpace(&username);
        RemoveSpace(&password);
        userController_->AddUser(username, password);
    }
    
    return false;
}

bool UserRoomManager::LoadRoomConfig()
{
    base::FilePath path;
    PathService::Get(base::DIR_EXE, &path);
    path = path.Append(L"BatchLogin.Room.cfg");
    base::File userfile(path, base::File::FLAG_OPEN|base::File::FLAG_READ);
    if (!userfile.IsValid())
    {
        assert(false && L"��ȡ�����ļ�ʧ��");
        LOG(ERROR) << L"��ȡ�����ļ�BatchLogin.Room.cfgʧ��";
        return false;
    }
    const uint32 memlen = 1024;
    std::string data;
    char str[memlen] = { 0 };
    userfile.Seek(base::File::FROM_BEGIN, 0);
    int read = userfile.ReadAtCurrentPos(str, memlen);
    DWORD err = GetLastError();
    while (read > 0)
    {
        data.insert(data.end(), str, str + read);
        if (read < memlen)//������
            break;
        read = userfile.ReadAtCurrentPos(str, memlen);
    }

    assert(!data.empty());

    std::vector<std::string> rooms = SplitString(data, "\r\n");

    for (const auto& it : rooms)
    {
        std::string roomstr = it;
        RemoveSpace(&roomstr);
        uint32 roomid = 0;
        if (!base::StringToUint(roomstr, &roomid))
        {
            assert(false && L"�����ת������");
            LOG(ERROR) << L"roomid change error! " << it;
            continue;
        }
        roomController_->AddRoom(roomid);           
    }
    return true;
}

bool UserRoomManager::FillSingleRoom(uint32 roomid)
{
    if (!userController_->FillRoom(roomid, roomusercount))
        return false;

    return true;
}

bool UserRoomManager::FillConfigRooms()
{
    std::vector<uint32> roomids = roomController_->GetRooms();
    for (const auto& roomid : roomids)
    {
        if (!userController_->FillRoom(roomid, roomusercount))
        {
            assert(false && L"���뷿��ʧ��");
            LOG(ERROR) << L"FillRoom failed! " << base::UintToString(roomid);
        }
        continue;
    }
    return false;
}