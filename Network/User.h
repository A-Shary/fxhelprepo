#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>
#include "Room.h"
#include "Network/IpProxy.h"

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

class TcpManager;
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
    //User(const std::string& username, const std::string& password);
    ~User();

    // ���ò���
    void SetUsername(const std::string& username);
    std::string GetUsername() const;
    
    void SetPassword(const std::string& password);
    std::string GetPassword() const;

    void SetIpProxy(const IpProxy& ipproxy);
    IpProxy GetIpProxy() const;

    void SetCookies(const std::string& cookies);
    std::string GetCookies() const;

    void SetRoomServerIp(const std::string& serverip);

    void SetTcpManager(TcpManager* tcpManager);

    //���÷���������Ϣ�ص�����,����Ľ�������Ϊ����Ҫ�������ģ�鴦��
    void SetNormalNotify(NormalNotify normalNotify);
    void SetNotify201(Notify201 notify201);
    void SetNotify501(Notify501 notify501);
    void SetNotify601(Notify601 notify601);

    // ������Ϊ
    bool Login();
    bool Login(const std::string& username,
               const std::string& password, 
               const std::string& verifycode,
               std::string* errormsg);

    bool LoginWithCookies(const std::string& cookies, 
                          std::string* errormsg);
    bool Logout();
    bool LoginGetVerifyCode(std::vector<uint8>* picture);

    uint32 GetServerTime() const;
    uint32 GetFanxingId() const;
    uint32 GetClanId() const;

    bool EnterRoomFopOperation(uint32 roomid, uint32* singer_clanid);
    bool EnterRoomFopAlive(uint32 roomid);
    bool ExitRoom(uint32 roomid);
    bool ExitRooms();

    void SetRobotApiKey(const std::string& apikey);
    bool SendChatMessage(uint32 roomid, const std::string& message);
    bool SendChatMessageRobot(const RoomChatMessage& roomChatMessage);
    bool RequestRobot(uint32 senderid, const std::string& request, std::string* response);
    bool SendStar(uint32 count);
    bool RetrieveStart();
    bool SendGift(uint32 giftid);

    bool GetGiftList(uint32 roomid, std::string* content);
    bool GetViewerList(uint32 roomid, 
        std::vector<EnterRoomUserInfo>* enterRoomUserInfo);

	bool KickoutUser(KICK_TYPE kicktype, uint32 roomid, 
        const EnterRoomUserInfo& enterRoomUserInfo);
    bool BanChat(uint32 roomid, const EnterRoomUserInfo& enterRoomUserInfo);
    bool UnbanChat(uint32 roomid, const EnterRoomUserInfo& enterRoomUserInfo);
    bool Worship(uint32 roomid, uint32 userid, std::string* errormsg);
    bool Worship_(const std::string& cookies, uint32 roomid, uint32 userid,
        std::string* errormsg);
    
private:
    bool CheckVerifyCode(const std::string& verifycode, std::string* errormsg);
    bool LoginHttps(const std::string& username, const std::string& password, 
        const std::string& verifycode, std::string* errormsg);

    bool LoginUServiceGetMyUserDataInfo(std::string* errormsg);

    bool LoginIndexServiceGetUserCenter(std::string* errormsg);

    TcpManager* tcpManager_;
    std::string username_;
    std::string password_;
    std::string serverip_;
    IpProxy ipproxy_;

    // ͼ�������ʹ�õĽӿ�key
    std::string apikey_;

    // ��¼����ܻ�õ��û���Ϣ
    std::string nickname_ = "";
    uint32 richlevel_ = 0;
    uint32 kugouid_ = 0;
    uint32 fanxingid_ = 0;
    uint32 clanid_ = 0;
    uint32 coin_ = 0;
    std::string usertoken_ = "";
    std::string userkey_ = "";
    uint32 servertime_ = 0xFFFFFFFF; // ���ֵ������Ȩ�޿��Ƶ�ʱ���ж�ʹ��

    std::unique_ptr<CurlWrapper> curlWrapper_ = nullptr;
    std::unique_ptr<CookiesHelper> cookiesHelper_ = nullptr;
    std::map<uint32, std::shared_ptr<Room>> rooms_;

    NormalNotify normalNotify_;
    Notify201 notify201_;
    Notify501 notify501_;
    Notify601 notify601_;
};

