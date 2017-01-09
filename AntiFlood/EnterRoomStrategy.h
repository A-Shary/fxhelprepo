#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <map>

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "third_party/chromium/base/synchronization/lock.h"

struct EnterRoomUserInfo;

struct WelcomeInfo
{
    uint32 fanxing_id;
    std::wstring name;
    std::wstring content;
};

class EnterRoomStrategy
    :public std::enable_shared_from_this < EnterRoomStrategy >
{
public:
    EnterRoomStrategy();
    ~EnterRoomStrategy();

    void SetWelcomeFlag(bool enable);
    void SetWelcomeLevel(uint32 level);
    void SetWelcomeVipV(bool enable);

    void SetSpecialWelcomeContent(const std::map<uint32, WelcomeInfo>& special_welcome);
    void GetSpecialWelcomeContent(std::map<uint32, WelcomeInfo>* special_welcome);

    void SetNormalWelcomeContent(const std::wstring& normal_welcome);
    bool GetNormalWelcomeContent(std::wstring* normal_welcome);

    bool GetEnterWelcome(const EnterRoomUserInfo& enterinfo, std::wstring* chatmessage);

private:
    bool SaveSpecialWelcomeContent(const std::map<uint32, WelcomeInfo>& special_welcome) const;
    bool LoadSpecialWelcomeContent(std::map<uint32, WelcomeInfo>* special_welcome);

    bool SaveNormalWelcomeContent(const std::wstring& normal_welcome) const;
    bool LoadNormalWelcomeContent(std::wstring* normal_welcome);

    bool welcome_flag_ = false;
    uint32 welcome_level_ = 0;
    bool vip_v_ = false;
    base::Lock welcome_lock_;
    std::map<uint32, WelcomeInfo> welcome_info_map_;
    std::wstring normal_welcome_;
    std::wstring pre_ = L"��ӭ ";
    std::wstring post_ = L" ����ֱ����";
};

