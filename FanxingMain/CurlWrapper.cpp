#include "CurlWrapper.h"
#include <assert.h>
#include "third_party/libcurl/curl/curl.h"

#include "CookiesManager.h"

namespace
{
    const char* fanxingurl = "http://fanxing.kugou.com";
    const char* useragent = "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko";
    const char* acceptencode = "gzip, deflate";
}

CurlWrapper::CurlWrapper()
{
}


CurlWrapper::~CurlWrapper()
{
}

void CurlWrapper::CurlInit()
{
    curl_global_init(CURL_GLOBAL_WIN32);
}

void CurlWrapper::CurlCleanup()
{
    curl_global_cleanup();
}

// ����ͨ��
bool CurlWrapper::LoginRequest()
{
    std::string cookies = "";
    bool ret = false;
    if (!CookiesManager::GetCookies(fanxingurl, &cookies))
    {
        return false;
    }

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if (!curl)
        return false;

    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, fanxingurl);

    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    struct curl_slist *headers = 0;
    headers = curl_slist_append(headers, "Connection: Keep-Alive");
    headers = curl_slist_append(headers,  "Accept - Language: zh - CN");
    headers = curl_slist_append(headers, "Accept : text / html, application / xhtml + xml, */*");
 
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, acceptencode);
    
    curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent);
    //curl_easy_setopt(curl, CURLOPT_REFERER, "www.fanxing.kugou.com");

    curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());
    // �����󷵻���ʱ���õ�cookie��������
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "d:/cookie.txt");

    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return false;
    }
    // ��ȡ����ҵ����
    long responsecode = 0;
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responsecode);

    // ��ȡ������ʱ��
    long servertime = 0;
    res = curl_easy_getinfo(curl, CURLINFO_FILETIME, &servertime);

    char* redirecturl = 0;
    res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &redirecturl);
    if (redirecturl)
    {
        assert(false);
        std::string redirect = redirecturl;
    }

    long headersize = 0;
    res = curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &headersize);

    long requestsize = 0;
    res = curl_easy_getinfo(curl, CURLINFO_REQUEST_SIZE, &requestsize);

    char* localip = 0;
    res = curl_easy_getinfo(curl, CURLINFO_LOCAL_IP, &localip);

    long localport = 0;
    res = curl_easy_getinfo(curl, CURLINFO_LOCAL_PORT, &localport);

    // ��ȡ��������cookies
    struct curl_slist* curllist = 0;
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &curllist);
    if (curllist)
    {
        assert(false&&"��ȡ��cookie");
        curl_slist_free_all(curllist);
    }
    /* always cleanup */
    curl_easy_cleanup(curl);

    if (responsecode == 200)
    {
        return true;
    }
    return false;
}

bool CurlWrapper::Services_UserService_UserService_getMyUserDataInfo()
{
    return true;
}

bool CurlWrapper::Services_IndexService_IndexService_getUserCenter()
{
    return true;
}

bool CurlWrapper::EnterRoom(uint32 roomid)
{
    return false;
}
