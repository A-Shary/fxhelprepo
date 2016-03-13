#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <map>
#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "../Network/GiftNotifyManager.h"

class CurlWrapper;
class GiftNotifyManager;
typedef std::function<void(const std::wstring&)> notifyfn;

typedef std::vector<std::wstring> RowData;
typedef std::vector<RowData> GridData;
typedef std::function<void(const RowData&)> notify201;

class NetworkHelper
{
public:
    NetworkHelper();
    ~NetworkHelper();

    bool Initialize();// �����߳�
    void Finalize();// �����߳�

    void SetNotify(notifyfn fn);
    void RemoveNotify();

    void SetNotify201(notify201 fn);
    void RemoveNotify201();

    bool EnterRoom(const std::wstring& strroomid);
    bool Login(const std::wstring& username, const std::wstring& password);

    bool KickoutUsers(uint32 singerid, const EnterRoomUserInfo& enterRoomUserInfo);

private:
    void NotifyCallback(const std::wstring& message);
    void NotifyCallback601(const std::string& data);
    void NotifyCallback201(const EnterRoomUserInfo& enterRoomUserInfo);

    std::unique_ptr<CurlWrapper> curlWrapper_;
    std::unique_ptr<GiftNotifyManager> giftNotifyManager_;
    std::map<uint32, EnterRoomUserInfo> enterRoomUserInfoMap_;
    notifyfn notify_;
    notify201 notify201_;
};

