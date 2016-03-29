#include "CookiesManager.h"

#include <memory>
#include <windows.h>
#include <Iepmapi.h>
#include <Wininet.h>
#include <assert.h>
#include "EncodeHelper.h"

#pragma comment(lib,"Wininet.lib")
#pragma comment(lib,"Iepmapi.lib")

namespace
{
// InternalInternetGetCookie("http://www.fanxing.kugou.com/",cookies);
bool InternalInternetGetCookie(const std::string& url, std::string* cookie)
{
    if (url.empty() || !cookie)
    {
        //assert(false & L"����Ϊ��");
        return false;
    }
    std::string cookiename = "";
    DWORD dwSize = 0;
    DWORD dwError = 0;
        
    if (!InternetGetCookieA(url.c_str(), cookiename.c_str(), nullptr, &dwSize))
    {
        assert(false && L"��ȥ��IE������ı���ģʽ�����ܻ�ȡ��cookies");
        dwError = GetLastError();
        return false;
    }
        
    if (dwSize <= 0)
    {
        return false;
    }

    std::unique_ptr<char[]> cookieptr(new char[dwSize]);
    if (!InternetGetCookieA(url.c_str(), cookiename.c_str(), cookieptr.get(), &dwSize))
    {
        dwError = GetLastError();
        return false;
    }
    cookie->assign(cookieptr.get(), cookieptr.get() + dwSize);

    return true;
}

// �˺���������
bool InternaletProtectedModeCookie(const std::string& url,
    const std::string& cookiename, std::string* cookie)
{
    HRESULT hr = E_FAIL;
    std::wstring wUrl = MbsToWide(url);
    std::wstring wCookieName = MbsToWide(cookiename);
    DWORD dwSize = 0;
    DWORD dwError = 0;
    hr = IEGetProtectedModeCookie(wUrl.c_str(), wCookieName.c_str(), nullptr,
        &dwSize, 0);
    if (hr!=S_OK)
    {
        dwError = GetLastError();
        hr = HRESULT_FROM_WIN32(dwError);
        return false;
    }
    if (dwSize <= 0)
    {
        return false;
    }
    std::unique_ptr<wchar_t[]> wcookieptr(new wchar_t[dwSize]);
    hr = IEGetProtectedModeCookie(wUrl.c_str(), wCookieName.c_str(), 
        wcookieptr.get(), &dwSize, 0);
    if (hr != S_OK)
    {
        dwError = GetLastError();
        return false;
    }
    std::wstring temp(wcookieptr.get(), wcookieptr.get() + dwSize);
    return WideToMBS(temp, cookie);
}
}

CookiesManager::CookiesManager()
{
}


CookiesManager::~CookiesManager()
{
}

bool CookiesManager::SetCookies(const std::string& key, const std::string& cookie)
{
    //if (cookies_.find(key) != cookies_.end())
    //{
    //    return false;
    //}
    //cookies_.insert(std::make_pair(key, cookie));
    cookies_[key] = cookie;
    return true;
}

std::string CookiesManager::GetCookies(const std::vector<std::string>& keys) const
{
    std::string temp;
    for (const auto& key : keys)
    {
        const auto& result = cookies_.find(key);
        if (result!=cookies_.end())
        {
            temp += result->second;
            temp += ";";
        }
    }
    if (temp.empty())
    {
        return temp;
    }

    temp = temp.substr(0, temp.length() - 1);
    return temp;
}