#pragma once
#include <vector>
#include <string>
#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "third_party/chromium/base/files/file_path.h"

class RegisterHelper
{
public:
    RegisterHelper();
    ~RegisterHelper();

    bool SaveVerifyCodeImage(
        const std::vector<uint8>& image);

    bool GetVerifyCodeImagePath(std::wstring* path) const;

private:
    base::FilePath pathname_;
};

