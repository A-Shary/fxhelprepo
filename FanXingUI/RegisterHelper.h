#pragma once
#include <vector>
#include <string>
#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "third_party/chromium/base/files/file_path.h"
#include "third_party/chromium/base/files/file.h"

class RegisterHelper
{
public:
    RegisterHelper();
    ~RegisterHelper();

    bool SaveVerifyCodeImage(
        const std::vector<uint8>& image, std::wstring* path);

    bool SaveAccountToFile(const std::wstring& username,
        const std::wstring& password);

    bool LoadAccountFromFile(
        std::vector<std::pair<std::wstring, std::wstring>>* accountinfo);
};

