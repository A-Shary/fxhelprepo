#pragma once
#include <memory>
#include <vector>
#include <string>
#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "third_party/chromium/base/files/file_path.h"
#include "third_party/chromium/base/files/file.h"

class CurlWrapper;
class RegisterHelper
{
public:
    RegisterHelper();
    ~RegisterHelper();

    bool Initialize();
    void Finalize();

    bool SaveVerifyCodeImage(
        const std::vector<uint8>& image, std::wstring* path);
    bool SaveAccountToFile(const std::wstring& username,
        const std::wstring& password);
    bool LoadAccountFromFile(
        std::vector<std::pair<std::wstring, std::wstring>>* accountinfo);

    // ע���º��룬���粿�ֹ���
    bool RegisterGetVerifyCode(std::vector<uint8>* picture);
    bool RegisterCheckUserExist(const std::wstring& username);
    bool RegisterCheckUserInfo(const std::wstring& username, 
        const std::wstring& password);
    bool RegisterUser(const std::wstring& username, const std::wstring& password,
        const std::wstring& verifycode);

private:
    std::unique_ptr<CurlWrapper> curlWrapper_;
};

