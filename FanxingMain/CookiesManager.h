#pragma once
#include <string>

// CookiesManager�ṩ��ȡ����fanxing��http������ص�cookies���ݵĹ���
// ��ȡ����ֱ��ʹ�õ�cookies����
class CookiesManager
{
public:
    CookiesManager();
    ~CookiesManager();

    static bool GetCookies(const std::string&url, std::string* cookies);

};

