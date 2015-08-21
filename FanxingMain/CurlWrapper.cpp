#include "CurlWrapper.h"
#include <assert.h>
#include <iostream>
#include "third_party/libcurl/curl/curl.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/time/time.h"
//#include "third_party/chromium/base/files/file_path.h"

#include "CookiesManager.h"

namespace
{
    const char* fanxingurl = "http://fanxing.kugou.com";
    const char* kugouurl = "http://kugou.com";
    const char* useragent = "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko";
    const char* acceptencode = "gzip, deflate";

    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        std::string data;
        data.assign(ptr, ptr + size*nmemb);
        CurlWrapper* p = static_cast<CurlWrapper*>(userdata);
        p->WriteCallback(data);
        return size*nmemb;
    }

    // ��ȡ13λ�ĵ�ǰʱ���ַ���
    std::string GetNowTimeString()
    {
        return base::Uint64ToString(
            static_cast<uint64>(base::Time::Now().ToDoubleT() * 1000));
    }
}

bool CurlWrapper::WriteCallback(const std::string& data)
{
    std::cout << data <<std::endl;
    //int count = file_.WriteAtCurrentPos(data.c_str(), data.length());
    currentWriteData_ += data;
    return true;
}

CurlWrapper::CurlWrapper()
    :currentWriteData_(""),
    response_of_RoomService_RoomService_enterRoom_(""),
    response_of_Services_UserService_UserService_getMyUserDataInfo_("")
{
    //base::FilePath path(L"d:/response.txt");
    //file_.Initialize(path, base::File::FLAG_OPEN_ALWAYS | base::File::FLAG_WRITE);
    //bool valid = file_.IsValid();
    //file_.Seek(base::File::FROM_BEGIN,0L);
}

CurlWrapper::~CurlWrapper()
{
    //file_.Close();
}

void CurlWrapper::CurlInit()
{
    curl_global_init(CURL_GLOBAL_WIN32);
}

void CurlWrapper::CurlCleanup()
{
    curl_global_cleanup();
}

// ���Գɹ�
bool CurlWrapper::LoginRequestWithCookies()
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
    curl_easy_setopt(curl, CURLOPT_REFERER, "www.fanxing.kugou.com");

    curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());
    // �����󷵻���ʱ���õ�cookie��������
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "d:/LoginRequest.txt");

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

    // ��ȡ��������cookies
    struct curl_slist* curllist = 0;
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &curllist);
    if (curllist)
    {
        struct curl_slist* temp = curllist;
        std::string retCookies;
        while (temp)
        {
            retCookies += std::string(temp->data);
            temp = temp->next;
        }
        std::cout << "CURLINFO_COOKIELIST get cookie: " << retCookies;
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

bool CurlWrapper::LoginRequestWithUsernameAndPassword(const std::string& username,
    const std::string& password)
{
    bool ret = false;

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
    headers = curl_slist_append(headers, "Accept - Language: zh - CN");
    headers = curl_slist_append(headers, "Accept : text / html, application / xhtml + xml, */*");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, acceptencode);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent);
    curl_easy_setopt(curl, CURLOPT_REFERER, "www.fanxing.kugou.com");

    //curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());
    // �����󷵻���ʱ���õ�cookie��������
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "d:/LoginRequestWithUsernameAndPassword.txt");

    curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);

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

    // ��ȡ��������cookies
    struct curl_slist* curllist = 0;
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &curllist);
    if (curllist)
    {
        struct curl_slist* temp = curllist;
        std::string retCookies;
        while (temp)
        {
            retCookies += std::string(temp->data);
            temp = temp->next;
        }
        std::cout << "CURLINFO_COOKIELIST get cookie: " << retCookies;
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
// ���Գɹ�
bool CurlWrapper::Services_UserService_UserService_getMyUserDataInfo()
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

    std::string url = R"(http://fanxing.kugou.com/Services.php?act=UserService.UserService&mtd=getMyUserDataInfo&args=[]&_=)";
    url += GetNowTimeString();
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    struct curl_slist *headers = 0;
    headers = curl_slist_append(headers, "Accept: application/json, text/javascript, */*; q=0.01");
    headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
    headers = curl_slist_append(headers, "Connection: Keep-Alive");
    headers = curl_slist_append(headers, "Accept-Language: zh-CN");
    headers = curl_slist_append(headers, "Host: fanxing.kugou.com");

    
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, acceptencode);
    curl_easy_setopt(curl, CURLOPT_REFERER, "http://fanxing.kugou.com/");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent);

    curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());
    // �����󷵻���ʱ���õ�cookie��������
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "d:/Services_UserService_UserService_getMyUserDataInfo.txt");

    currentWriteData_.clear();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
    response_of_Services_UserService_UserService_getMyUserDataInfo_ = currentWriteData_;

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
    
    // ��ȡ��������cookies
    struct curl_slist* curllist = 0;
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &curllist);
    if (curllist)
    {
        struct curl_slist* temp = curllist;
        std::string retCookies;
        while (temp)
        {
            retCookies += std::string(temp->data);
            temp = temp->next;
        }
        std::cout << "CURLINFO_COOKIELIST get cookie: " << retCookies;
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

// ���Գɹ�
bool CurlWrapper::Services_IndexService_IndexService_getUserCenter()
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

    std::string url = R"(http://fanxing.kugou.com/Services.php?act=IndexService.IndexService&mtd=getUserCenter&args=[""]&_=)";
    url += GetNowTimeString();
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    struct curl_slist *headers = 0;
    headers = curl_slist_append(headers, "Accept: application/json, text/javascript, */*; q=0.01");
    headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
    headers = curl_slist_append(headers, "Connection: Keep-Alive");
    headers = curl_slist_append(headers, "Accept-Language: zh-CN");
    headers = curl_slist_append(headers, "Host: fanxing.kugou.com");


    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, acceptencode);
    curl_easy_setopt(curl, CURLOPT_REFERER, "http://fanxing.kugou.com/");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent);

    curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());
    // �����󷵻���ʱ���õ�cookie��������
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "d:/Services_IndexService_IndexService_getUserCenter.txt");

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

    // ��ȡ��������cookies
    struct curl_slist* curllist = 0;
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &curllist);
    if (curllist)
    {
        struct curl_slist* temp = curllist;
        std::string retCookies;
        while (temp)
        {
            retCookies += std::string(temp->data);
            temp = temp->next;
        }
        std::cout << "CURLINFO_COOKIELIST get cookie: " << retCookies;
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

// ���Գɹ�
bool CurlWrapper::EnterRoom(uint32 roomid)
{
    std::string cookies = "";
    std::string temp = "";
    bool ret = false;
    if (!CookiesManager::GetCookies(fanxingurl, &temp))
    {
        return false;
    }
    cookies += temp;
    if (!CookiesManager::GetCookies(kugouurl, &temp))
    {
        return false;
    }
    cookies += ";"+temp;

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if (!curl)
        return false;

    std::string url = std::string(fanxingurl);
    url += "/" + base::IntToString(roomid);
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    struct curl_slist *headers = 0;
    headers = curl_slist_append(headers, "Accept: text/html, application/xhtml+xml, */*");
    //headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
    headers = curl_slist_append(headers, "Connection: Keep-Alive");
    headers = curl_slist_append(headers, "Accept-Language: zh-CN");
    headers = curl_slist_append(headers, "Host: fanxing.kugou.com");


    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, acceptencode);
    curl_easy_setopt(curl, CURLOPT_REFERER, "http://fanxing.kugou.com/");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent);

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

    // ��ȡ��������cookies
    struct curl_slist* curllist = 0;
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &curllist);
    if (curllist)
    {
        struct curl_slist* temp = curllist;
        std::string retCookies;
        while (temp)
        {
            retCookies += std::string(temp->data);
            temp = temp->next;
        }
        std::cout << "CURLINFO_COOKIELIST get cookie: " << retCookies;
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

// ����ͨ��
bool CurlWrapper::Servies_Uservice_UserService_getCurrentUserInfo(uint32 roomid)
{
	std::string cookies = "";
	std::string temp = "";
	bool ret = false;
	if (!CookiesManager::GetCookies(fanxingurl, &temp))
	{
		return false;
	}
	cookies += temp;
	if (!CookiesManager::GetCookies(kugouurl, &temp))
	{
		return false;
	}
	cookies += ";" + temp;

	CURL *curl;
	CURLcode res;
	curl = curl_easy_init();

	if (!curl)
		return false;

    std::string strtime = GetNowTimeString();
	std::string url = std::string(fanxingurl);
	url += "/Services.php?d="+strtime+"&act=UserService.UserService&mtd=getCurrentUserInfo&args=%5B%5D&test=3";
	curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	/* example.com is redirected, so we tell libcurl to follow redirection */
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

	struct curl_slist *headers = 0;
	headers = curl_slist_append(headers, "Accept: text/html, application/xhtml+xml, */*");
	//headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
	headers = curl_slist_append(headers, "Connection: Keep-Alive");
	headers = curl_slist_append(headers, "Accept-Language: zh-CN");
	headers = curl_slist_append(headers, "Host: fanxing.kugou.com");


	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
	curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, acceptencode);
	std::string referer = "http://fanxing.kugou.com/" + base::IntToString(roomid);
	curl_easy_setopt(curl, CURLOPT_REFERER, referer.c_str());
	curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent);

	curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());
	// �����󷵻���ʱ���õ�cookie��������
	curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "d:/Servies_Uservice_UserService_getCurrentUserInfo.txt");

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

	// ��ȡ��������cookies
	struct curl_slist* curllist = 0;
	res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &curllist);
	if (curllist)
	{
		struct curl_slist* temp = curllist;
		std::string retCookies;
		while (temp)
		{
			retCookies += std::string(temp->data);
			temp = temp->next;
		}
		std::cout << "CURLINFO_COOKIELIST get cookie: " << retCookies;
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

// ����ͨ��
bool CurlWrapper::RoomService_RoomService_enterRoom(uint32 roomid)
{
    //GET / Services.php ? act = RoomService.RoomService&mtd = enterRoom&args = ["1017131", "", "", "web"] & _ = 1439814776455 HTTP / 1.1
    std::string cookies = "";
    std::string temp = "";
    bool ret = false;
    if (!CookiesManager::GetCookies(fanxingurl, &temp))
    {
        return false;
    }
    cookies += temp;
    if (!CookiesManager::GetCookies(kugouurl, &temp))
    {
        return false;
    }
    cookies += ";" + temp;

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if (!curl)
        return false;

    std::string strroomid = base::IntToString(static_cast<int>(roomid));
    std::string url = std::string(fanxingurl);
    url += "/Services.php?act=RoomService.RoomService&mtd=enterRoom&args=[%22";
    url += strroomid + "%22,%22%22,%22%22,%22web%22]&_=";
    url += GetNowTimeString();
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    struct curl_slist *headers = 0;
    //headers = curl_slist_append(headers, "Accept: text/html, application/xhtml+xml, */*");
    headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
    headers = curl_slist_append(headers, "Connection: Keep-Alive");
    headers = curl_slist_append(headers, "Accept-Language: zh-CN,en,*");
    headers = curl_slist_append(headers, "Host: fanxing.kugou.com");
    headers = curl_slist_append(headers, "Accept-Charset: GBK,utf-8;q=0.7,*;q=0.3");
    headers = curl_slist_append(headers, "Accept: application/json;text/javascript,*/*;q=0.01");
    headers = curl_slist_append(headers, "Content-Type: application/json;charset=utf-8");

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    // ���ﲻҪ����ѹ�������ݰ�����ý�ѹ�鷳
    //curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, acceptencode);
    std::string referer = "http://fanxing.kugou.com/" + strroomid;
    curl_easy_setopt(curl, CURLOPT_REFERER, referer.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, useragent);

    curl_easy_setopt(curl, CURLOPT_COOKIE, cookies.c_str());
    // �����󷵻���ʱ���õ�cookie��������
    curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "d:/RoomService_RoomService_enterRoom.txt");

    currentWriteData_.clear();
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

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
    response_of_RoomService_RoomService_enterRoom_ = currentWriteData_;

    // ��ȡ��������cookies
    struct curl_slist* curllist = 0;
    res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &curllist);
    if (curllist)
    {
        struct curl_slist* temp = curllist;
        std::string retCookies;
        while (temp)
        {
            retCookies += std::string(temp->data);
            temp = temp->next;
        }
        std::cout << "CURLINFO_COOKIELIST get cookie: " << retCookies;
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
