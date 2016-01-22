#pragma once
#include <string>
#include <memory>

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��

#include "third_party/chromium/base/time/time.h"

// ���ฺ���ṩ���к�̨�����Ľӿ�
// ��������¼

class FamilyDaily;
class FamilyBackground
{
public:
    FamilyBackground();
    ~FamilyBackground();

    void Test();
    bool Init();
    bool Login(const std::string& username, const std::string& password);
    bool GetSummaryData(const base::Time& begintime, const base::Time& endtime);

private:
    std::unique_ptr<FamilyDaily> familyDaily_;
};

