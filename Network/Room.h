#pragma once
#include <string>
#include <memory>

#include "third_party/chromium/base/basictypes.h"

#include "GiftNotifyManager.h"

class CurlWrapper;
class GiftNotifyManager;
class CookiesHelper;
class Room
{
public:
    explicit Room(uint32 roomid);
    ~Room();
    
    bool Enter(const std::string& cookies);
    void SetNormalNotify(NormalNotify normalNotify);
    void SetNotify201(Notify201 notify201);
    // �жϽ������ݵ�����
    bool Exit();

private:
    bool OpenRoom(const std::string& cookies) const;

    bool GetCurrentUserInfo(const std::string& cookies,
        uint32* userid, std::string* nickname, uint32* richlevel);

    bool EnterRoom(const std::string& cookies, uint32* singerid,
        std::string* key, std::string* ext);


    bool ConnectToNotifyServer_(uint32 roomid, uint32 userid,
        const std::string& nickname,
        uint32 richlevel, uint32 ismaster,
        uint32 staruserid,
        const std::string& key,
        const std::string& ext);

    uint32 roomid_;
    uint32 singerid_;
    std::unique_ptr<CurlWrapper> curlWrapper_;
    std::unique_ptr<GiftNotifyManager> giftNotifyManager_;
    std::unique_ptr<CookiesHelper> cookiesHelper_;
};

