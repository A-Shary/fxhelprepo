#include "GiftNotifyManager.h"
#include <assert.h>
#include "TcpClient.h"
#include "EncodeHelper.h"
#include "Thread.h"
#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/time/time.h"
#include "third_party/json/json.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"



namespace
{   
const char* targetip = "42.62.68.50";
const uint16 port843 = 843;
const uint16 port8080 = 8080;
struct cmd201package
{
    uint32 cmd;
    uint32 roomid;
    uint32 userid;
    std::string nickname;
    uint32 richlevel;
    uint32 ismaster;
    uint32 staruserid;
    std::string key;
    uint32 keytime;
    std::string ext;
};
// ������ݰ�Ӧ�ô�http�����Ǳ߷��ع�������
bool GetFirstPackage(const cmd201package& package, 
    std::vector<uint8> *packagedata)
{
    // 10λ��ʱ���
    //uint32 nowtime = static_cast<uint32>(base::Time::Now().ToDoubleT());

    Json::FastWriter writer;
    Json::Value root(Json::objectValue);
    root["cmd"] = package.cmd;
    root["roomid"] = package.roomid;
    root["userid"] = package.userid;
    root["nickname"] = package.nickname;
    root["richlevel"] = package.richlevel;
    root["ismaster"] = package.ismaster;
    root["staruserid"] = package.staruserid;
    root["key"] = package.key;
    root["keytime"] = package.keytime;
    root["ext"] = package.ext;
    std::string data = writer.write(root);
    packagedata->assign(data.begin(), data.end());

    return true;
}
void TcpNotify(void* privatedata, const std::vector<char>& data)
{
    GiftNotifyManager* manager = static_cast<GiftNotifyManager*>(privatedata);
    manager->Notify(data);
    return;
}

DWORD HeartBeatFunc(LPVOID lpParam)
{
    auto p = reinterpret_cast<GiftNotifyManager*>(lpParam);
    p->SendHeartBeat();
    return 0;
}

std::string Packet = "";
int pos = 0;
// �������ѹ���ݰ���ʱ�򣬿��ܷ��ض�����������ݰ�
std::vector<std::string> HandleMixPackage(const std::string& package)
{
    std::vector<std::string> retVec;
    auto it = package.begin();
    auto sentinel = it;
    while (it!=package.end())
    { 
        switch (*it)
        {
        case '{':
            pos++;
            if (pos==1)
            {
                sentinel = it;
            }
            break;
        case '}':
            pos--;
            if (pos == 0)
            {
                Packet += std::string(sentinel, it+1);
                retVec.push_back(Packet);
                Packet = "";
            }
            break;
        default:
            break;
        }
        it++;    
    }

    if(pos!=0) // ���������������һ��������json����Ҫ����һ�����ݰ�
    {
        Packet = std::string(sentinel,it);
    }

    return retVec;
}

};
GiftNotifyManager::GiftNotifyManager()
    :tcpClient_843_(new TcpClient),
    tcpClient_8080_(new TcpClient),
    notify601_(nullptr),
    thread_(new Thread),
    alive(false)
{
    thread_->Init((run_fn)(HeartBeatFunc), this);
}

GiftNotifyManager::~GiftNotifyManager()
{
    thread_->Stop();
}

void GiftNotifyManager::Set601Notify(Notify601 notify601)
{
    notify601_ = notify601;
}

void GiftNotifyManager::SetNormalNotify(NormalNotify normalNotify)
{
    normalNotify_ = normalNotify;
}

void GiftNotifyManager::Notify(const std::vector<char>& data)
{
    // ��Ҫ����ճ������
    std::string datastr(data.begin(), data.end());
    std::vector<std::string> packages = HandleMixPackage(datastr);

    for (auto package : packages)
    {
        try
        {
            alive = true;
            Json::Reader reader;
            Json::Value rootdata(Json::objectValue);
            if (!reader.parse(package, rootdata, false))
            {
                return;
            }

            // ��ʱû�б�Ҫ���status��ֵ
            Json::Value jvCmd(Json::ValueType::intValue);
            int cmd = rootdata.get(std::string("cmd"), jvCmd).asInt();
            if (cmd == 601)
            {
                Json::Value jvContent(Json::ValueType::objectValue);
                Json::Value  content = rootdata.get("content", jvContent);
                if (!content.isNull())
                {
                    Json::Value jvKey(Json::ValueType::stringValue);
                    std::string key = content.get(std::string("token"), jvKey).asString();
                    if (!key.empty())
                    {
                        notify601_(key);
                    }
                }
            }

            // �������ݰ�,ת�������������Ϣ
            std::wstring wstr = base::UTF8ToWide(package);
            normalNotify_(wstr);
        }
        catch (...)
        {
        }
    }
}

bool GiftNotifyManager::Connect843()
{
    // 843�˿ڵ���������ûʲô��
    //tcpClient_843_->SetNotify((NotifyFunction)TcpNotify, this);
    tcpClient_843_->Connect(targetip, port843);
    std::string str = "<policy-file-request/>";
    std::vector<char> data;
    data.assign(str.begin(), str.end());
    data.push_back(0);
    return tcpClient_843_->Send(data);
}

bool GiftNotifyManager::Connect8080(uint32 roomid, uint32 userid, 
    const std::string& nickname, uint32 richlevel, uint32 ismaster, 
    uint32 staruserid, const std::string& key,/* uint64 keytime, */
    const std::string& ext)
{
    std::string decodestr = UrlDecode(ext);// ����ʹ��
    Packet = "";
    pos = 0;
    tcpClient_8080_->SetNotify((NotifyFunction)TcpNotify, this);
    if (!tcpClient_8080_->Connect(targetip, port8080))
    {
        assert(false && L"socket����ʧ��");
        return false;
    }
    uint32 keytime = static_cast<uint32>(base::Time::Now().ToDoubleT());
    std::vector<uint8> data_for_send;
    cmd201package package = { 
        201, roomid, userid, nickname, richlevel, ismaster, staruserid, key, 
        keytime, ext };
    GetFirstPackage(package, &data_for_send);
    std::vector<char> data_8080;
    data_8080.assign(data_for_send.begin(), data_for_send.end());
    data_8080.push_back(0);//���Ǳ��������ֽڵ�
    tcpClient_8080_->Send(data_8080);
    thread_->Start();
    return true;
}

bool GiftNotifyManager::SendHeartBeat()
{
    std::string heartbeat = "HEARTBEAT_REQUEST";
    heartbeat.append("\r\n");
    std::vector<char> heardbeadvec;
    heardbeadvec.assign(heartbeat.begin(), heartbeat.end());
    while (1)
    {
        Sleep(10000);
        if (alive)
        {
            tcpClient_8080_->Send(heardbeadvec);
        }
    }

    return true;
}


