#pragma once
#include <string>
#include <map>
#include <vector>

// CookiesManager�ṩ��ȡ����fanxing��http������ص�cookies���ݵĹ���
// ��ȡ����ֱ��ʹ�õ�cookies����
class CookiesHelper
{
public:
    CookiesHelper();
    ~CookiesHelper();

    bool SetCookies(const std::string& key, const std::string& cookie);
    std::string GetCookies(const std::vector<std::string>& keys) const;

private:
    std::map<std::string, std::string> cookies_;
};

