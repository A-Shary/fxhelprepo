#pragma once
#include <string>

// CookiesManager�ṩ��ȡ����fanxing��http������ص�cookies���ݵĹ���
// ��ȡ����ֱ��ʹ�õ�cookies����
class CookiesManager
{
public:
    CookiesManager();
    ~CookiesManager();

    static bool GetCookies(std::string* cookies);

};

