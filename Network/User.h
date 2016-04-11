#pragma once
#include <string>
#include <vector>
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

// ���๦��
// ���ݴ������ݳ�ʼ���û�ʹ��ҳ�������: �û���+����/cookie·����ʽ
// ���ﲻҪ���κ��̱߳��湦��
class User
{
public:
    User();
    ~User();

    // ���ò���
    void SetUserName(const std::string& username);
    std::string GetUserName() const;
    
    void SetPassword(const std::string& password);
    std::string GetPassword() const;

    void SetCookies(const std::vector<std::string> cookies);
    std::vector<std::string> GetCookies() const;

    //���÷���������Ϣ�ص�����,����Ľ�������Ϊ����Ҫ�������ģ�鴦��
    void SetRoomNotify();   

    // ������Ϊ
    bool Login();
    bool Logout();

    bool EnterRoom(uint32 roomid);
    bool ExitRoom();

    bool Chat(const std::string& message);
    bool SendStar(uint32 count);
    bool RetrieveStart();
    bool SendGift(uint32 giftid);

    bool KickoutUser(uint32 userid);
    bool SilencedUser(uint32 userid);

private:
    std::string username_;
    std::string password_;
    std::vector<std::string> cookies_;

    // ��¼����ܻ�õ��û���Ϣ
    uint32 userid_;
    uint32 coin_;
    std::string usertoken_;
    std::string userkey_;

    // 
};

