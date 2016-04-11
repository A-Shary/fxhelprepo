#pragma once
#include <vector>
#include <string>
#undef max // 因为微软这个二比在某些头文件定义了max宏
#undef min // 因为微软这个二比在某些头文件定义了min宏
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

