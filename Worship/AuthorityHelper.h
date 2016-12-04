#pragma once
#include <string>
#include "third_party/chromium/base/basictypes.h"

struct WorshipAuthority
{
    uint32 user_id = 0;
    uint64 expiretime = 0;
    std::string tracker_host = "";// ��֤δ��Ȩ�����û������ʹ��Ŀ�������
};

// �ṩ��Ȩ�ļ����ĸ�ʽ��д����
class AuthorityHelper
{
public:
    AuthorityHelper();
    ~AuthorityHelper();

    bool LoadUserTrackerAuthority(WorshipAuthority* authority);
    bool GetTrackerAuthorityDisplayInfo(const WorshipAuthority& authority,
        std::wstring* display);
};

