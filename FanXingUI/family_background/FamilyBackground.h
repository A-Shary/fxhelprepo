#pragma once
#include <string>
#include <memory>

// ���ฺ���ṩ���к�̨�����Ľӿ�
// ��������¼

class FamilyDaily;
class FamilyBackground
{
public:
    FamilyBackground();
    ~FamilyBackground();

    bool Init();
    bool Login(const std::string& username, const std::string& password);

private:
    std::unique_ptr<FamilyDaily> familyDaily_;
};

