#pragma once
#include <memory>
#include <vector>
#include <string>
#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "third_party/chromium/base/files/file_path.h"
#include "third_party/chromium/base/files/file.h"

class CurlWrapper;
class CookiesHelper;
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
        const std::wstring& password, const std::string& cookies);
    bool LoadAccountFromFile(
        std::vector<std::pair<std::wstring, std::wstring>>* accountinfo);

    std::wstring GetNewName() const;
    std::wstring GetPassword() const;

    // ע���º��룬���粿�ֹ���
    bool RegisterGetVerifyCode(std::vector<uint8>* picture);

    // ���°汾����������������ˣ���Ŀǰˢ����������Ҫ������������֤�û�������
    bool RegisterCheckUserExist(const std::wstring& username);
    bool RegisterCheckPassword(const std::wstring& username, 
        const std::wstring& password);

    bool RegisterUser(const std::wstring& username, const std::wstring& password,
        const std::string& verifycode, std::string* cookies, std::wstring* errormsg);

private:
    std::unique_ptr<CurlWrapper> curlWrapper_;
    std::unique_ptr<CookiesHelper> cookiesHelper_;
    std::unique_ptr<base::File> accountFile_;
    std::vector<std::string> namepost;
};

