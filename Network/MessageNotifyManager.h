#pragma once
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include <condition_variable>
#include <functional>
#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/timer/timer.h"
#include "third_party/chromium/base/threading/thread.h"
#include "Network/IpProxy.h"
#include "Network/TcpClient.h"
#include "Network/BetData.h"

class TcpClient;
class TcpManager;

// 201��Ϣ�����������ȥ�����ݰ�
struct EnterRoomUserInfo 
{
    std::string nickname = "";
    uint32 richlevel = 0;
    uint32 roomid = 0;
    uint32 unixtime = 0;
    uint32 userid = 0;
    bool vip_v = false; // vipData��v�ֶ��� 1Ϊ��ͨvip, 2Ϊ�׽�vip�����Կ��������ܵġ�
    bool visable = true; // �Ƿ�����Ĭ���ǿɼ���������������򲻿ɼ�
    bool isAdmin = false;
    bool isUserGuard = false;
    bool isGoldFans = false;
};

struct RoomChatMessage
{
    uint32 roomid = 0;
    uint32 senderid = 0;
    uint32 richlevel = 0;
    std::string sendername = "";
    uint32 receiverid = 0;
    std::string receivername = "";
    bool issecrect = false;
    std::string chatmessage = "";
};



struct RoomGiftInfo601
{
    uint32 time;
    uint32 roomid;
    uint32 senderid;
    std::string sendername;
    uint32 receiverid;
    std::string receivername;
    uint32 giftid;
    std::string giftname;
    uint32 gitfnumber;
    std::string tips;
    uint32 happyobj;
    uint32 happytype;
    std::string token;
};

typedef std::function<void(const RoomGiftInfo601& roomgiftinfo)> Notify601;
typedef std::function<void(const BetResult& bet_result)> Notify620;
typedef std::function<void(const EnterRoomUserInfo& enterRoomUserInfo)> Notify201;
typedef std::function<void(const EnterRoomUserInfo& enterRoomUserInfo,
    const RoomChatMessage& roomChatMessage)> Notify501;

typedef std::function<void(const std::wstring& data)> NormalNotify;
class MessageNotifyManager 
    : public std::enable_shared_from_this <MessageNotifyManager>
{
public:
    MessageNotifyManager();
    ~MessageNotifyManager();

    static void AddRef() {}
    static void Release() {}

    bool Initialize();
    void Finalize();
    void SetTcpManager(TcpManager* tcpManager);
    void SetServerIp(const std::string& serverip);
    void SetIpProxy(const IpProxy& ipproxy);

    // �ɶ�̬�ı�Ĺ��ܣ���Ҫת�������߳�ȥ���
    void SetNotify201(Notify201 notify201);
    void SetNotify501(Notify501 notify201);
    void SetNotify601(Notify601 notify601);
    void SetNotify620(Notify620 notify_620);
    void SetNormalNotify(NormalNotify normalNotify);

    // ʹ���µĸ�Ч�Ķ�·tcp����ַ�ģʽ
    bool NewConnect843(uint32 roomid, uint32 userid,
        const std::string& usertoken, 
        const base::Callback<void()>& conn_break_callback);
    bool NewConnect8080(uint32 roomid, uint32 userid, const std::string& usertoken);
    bool NewSendChatMessage(const std::string& nickname, uint32 richlevel,
                         const std::string& message);

    bool NewSendChatMessageRobot(const RoomChatMessage& roomChatMessage);

private:

    void DoSetNotify201(Notify201 notify201);
    void DoSetNotify501(Notify501 notify201);
    void DoSetNotify601(Notify601 notify601);
    void DoSetNotify620(Notify620 notify_620);
    void DoSetNormalNotify(NormalNotify normalNotify);

    void Notify(const std::vector<char>& data);
    std::vector<std::string> HandleMixPackage(const std::string& package);

    // ʹ���µĸ�Ч�Ķ�·tcp����ַ�ģʽ
    static void NewConnect843Callback(std::weak_ptr<MessageNotifyManager> weakptr,
        bool result, TcpHandle handle);
    static void NewConnect8080Callback(std::weak_ptr<MessageNotifyManager> weakptr, 
        uint32 roomid, uint32 userid,
        const std::string& usertoken,
        bool result, TcpHandle handle);
    static void NewData843Callback(std::weak_ptr<MessageNotifyManager> weakptr, 
        uint32 roomid, uint32 userid, const std::string& usertoken, bool result, 
        const std::vector<uint8>& data);
    static void NewData8080Callback(std::weak_ptr<MessageNotifyManager> weakptr, 
        bool result, const std::vector<uint8>& data);
    static void NewSendDataCallback(std::weak_ptr<MessageNotifyManager> weakptr, 
        TcpHandle handle, bool result);

    void DoNewConnect843Callback(bool result, TcpHandle handle);
    void DoNewConnect8080Callback(uint32 roomid, uint32 userid,
                             const std::string& usertoken,
                             bool result, TcpHandle handle);
    void DoNewData843Callback(uint32 roomid, uint32 userid,
        const std::string& usertoken, bool result, const std::vector<uint8>& data);
    void DoNewData8080Callback(bool result, const std::vector<uint8>& data);

    void DoNewSendHeartBeat(TcpHandle handle);
    void DoNewSendChatMessage(const std::vector<char>& msg); // ������Ҫ���ϼ��ʱ��
    void DoNewSendDataCallback(TcpHandle handle, bool result);

    base::Thread baseThread_;
    base::RepeatingTimer<MessageNotifyManager> repeatingTimer_;
    std::string serverip_ = "192.168.0.1";
    IpProxy ipProxy_;
    std::string Packet_ = "";
    int position_ = 0;

    std::unique_ptr<TcpClient> tcpClient_8080_;
    std::unique_ptr<TcpClient> tcpClient_843_;

    TcpHandle tcphandle_8080_ = -1;
    TcpHandle tcphandle_843_ = -1;

    Notify201 notify201_;
    Notify501 notify501_;
    Notify601 notify601_;
    Notify620 notify_620_;

    NormalNotify normalNotify_;

    TcpManager* tcpManager_;
    base::Callback<void()> conn_break_callback_; // �����������
    bool connected_;// δ����ʱ������ʧ�ܺ󣬶���Ӧ�ô���ص����߳�����

    base::RepeatingTimer<MessageNotifyManager> newRepeatingTimer_;
    base::TimeDelta chat_message_space_;
    base::Time last_chat_time_;

    // �����������
    std::vector<BetShowData> bet_show_datas_;
};

