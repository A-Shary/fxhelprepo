#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Room.h"

#undef max
#undef min
#include "third_party/chromium/base/basictypes.h"

enum class LOGIN_STATE
{  
    LOGOUT = 0,
    LOGIN = 1
};

enum class ROOM_STATE
{
    OUT_ROOM = 0,
    IN_ROOM = 1
};

class CurlWrapper;
class CookiesHelper;
class Room;
// ���๦��
// ���ݴ������ݳ�ʼ���û�ʹ��ҳ�������: �û���+����/cookie·����ʽ
// ���ﲻҪ���κ��̱߳��湦��
class User
{
public:
    User();
    User(const std::string& username, const std::string& password);
    ~User();

    // ���ò���
    void SetUsername(const std::string& username);
    std::string GetUsername() const;
    
    void SetPassword(const std::string& password);
    std::string GetPassword() const;

    void SetCookies(const std::vector<std::string> cookies);
    std::vector<std::string> GetCookies() const;

    //���÷���������Ϣ�ص�����,����Ľ�������Ϊ����Ҫ�������ģ�鴦��
    void SetNormalNotify(NormalNotify normalNotify);
    void SetNotify201(Notify201 notify201);

    // ������Ϊ
    bool Login();
    bool Login(const std::string& username,
        const std::string& password);
    bool Logout();

    bool EnterRoom(uint32 roomid);
    bool ExitRoom(uint32 roomid);
    bool ExitRooms();

    bool Chat(const std::string& message);
    bool SendStar(uint32 count);
    bool RetrieveStart();
    bool SendGift(uint32 giftid);

    bool KickoutUser(uint32 roomid, const EnterRoomUserInfo& enterRoomUserInfo);
    bool SilencedUser(uint32 userid);

private:

    bool LoginHttps(const std::string& username,
        const std::string& password);

    bool LoginUServiceGetMyUserDataInfo();

    bool LoginIndexServiceGetUserCenter();
    std::string username_;
    std::string password_;

    // ��¼����ܻ�õ��û���Ϣ
    uint32 userid_;
    uint32 coin_;
    std::string usertoken_;
    std::string userkey_;

    std::unique_ptr<CurlWrapper> curlWrapper_;
    std::unique_ptr<CookiesHelper> cookiesHelper_;
    std::map<uint32, std::shared_ptr<Room>> rooms_;

    NormalNotify normalNotify_;
    Notify201 notify201_;
};

