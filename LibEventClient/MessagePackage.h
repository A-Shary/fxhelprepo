#pragma once
#include <string>
#include <vector>
#include "MessageDefine.h"

class Package
{
public:
    Package(uint32 sequence_number);
    ~Package();
    void SetHeader(const Header& header);
    void SetData(const std::string& data);

    // ��������������ʱʹ��
    bool HandlePackage(std::vector<uint8>* response_package);

    // �ͻ��˹�������ʱʹ��
    bool GetPackage(std::vector<uint8>* request_package);

private:
    uint32 sequence_number_;
    Header request_header_;
    std::string request_data_;
};

