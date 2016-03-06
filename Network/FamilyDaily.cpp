#include "FamilyDaily.h"

#include <memory>
#include <assert.h>
#include <iostream>
#include "Network/EncodeHelper.h"
#include "third_party/libcurl/curl/curl.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"
#include "third_party/chromium/base/time/time.h"
#include "third_party/chromium/base/files/file_path.h"
#include "third_party/chromium/base/files/file.h"
#include "third_party/chromium/base/path_service.h"
#include "third_party/chromium/base/rand_util.h"
#include "third_party/json/json.h"


namespace{
    const char* familyurl = "http://family.fanxing.kugou.com";

    // cookies 
    const char* header_connection = "Connection:Keep-Alive";
    const char* header_host = "Host: family.fanxing.kugou.com";
    const char* header_accept = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8";
    const char* header_upgrade = "Upgrade-Insecure-Requests: 1";
    const char* header_useragent = "User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36";
    const char* header_language = "Accept-Language: zh-CN,zh;q=0.8";
    const char* header_content = "Content-Type: application/x-www-form-urlencoded";

    static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        std::string data;
        data.assign(ptr, ptr + size*nmemb);
        FamilyDaily* p = static_cast<FamilyDaily*>(userdata);
        p->WriteResponseDataCallback(data);
        return size*nmemb;
    }

    static size_t header_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
    {
        std::string data;
        data.assign(ptr, ptr + size*nmemb);
        FamilyDaily* p = static_cast<FamilyDaily*>(userdata);
        p->WriteResponseHeaderCallback(data);
        return size*nmemb;
    }
    
    std::string MakeReasonablePath(const std::string& pathfile)
    {
        auto temp = pathfile;
        auto pos = temp.find(':');
        while (pos != std::string::npos)
        {
            temp.erase(pos, 1);
            pos = temp.find(':');
        }
        return std::move(temp);
    }

    std::string MakeFormatDateString(const base::Time time)
    {
        base::Time::Exploded exploded;
        time.LocalExplode(&exploded);
        std::string month = base::IntToString(exploded.month);
        if (month.length()<2)
        {
            month = "0" + month;
        }
        std::string day_of_month = base::IntToString(exploded.day_of_month);
        if (day_of_month.length() < 2)
        {
            day_of_month = "0" + day_of_month;
        }

        std::string timestring = base::IntToString(exploded.year) + "-" +
            month + "-" + day_of_month;
        return std::move(timestring);
    }

    static const char* mark_table_begin = "<table";
    static const char* mark_table_end = "</table>";
    static const char* mark_tr_begin = "<tr>";
    static const char* mark_tr_end = "</tr>";
    static const char* mark_td_begin = "<td>";
    static const char* mark_td_end = "</td>";

    bool GetMarkData(const std::string& pagedata, size_t beginpos,
        const std::string& beginmark, 
        const std::string& endmark,
        std::string* targetdata,
        size_t *afterendmarkpos)
    {
        beginpos = pagedata.find(beginmark, beginpos);
        if (beginpos == std::string::npos)
            return false;
        beginpos += beginmark.size();

        auto endpos = pagedata.find(endmark, beginpos);
        if (endpos == std::string::npos)
            return false;

        *afterendmarkpos = endpos + endmark.size();
        *targetdata = pagedata.substr(beginpos, endpos - beginpos);
        return true;
    }

    bool GetTableData(const std::string& pagedata, std::string* tabledata)
    {
        size_t tableendpos = 0;
        return GetMarkData(pagedata, 0, mark_table_begin, mark_table_end, 
            tabledata, &tableendpos);
    }

    bool GetTrData(const std::string& pagedata, size_t beginpos, 
        std::string* trdata, size_t* trendpos)
    {
        return GetMarkData(pagedata, beginpos, mark_tr_begin, mark_tr_end, trdata, trendpos);
    }

    bool GetTdData(const std::string& pagedata, size_t beginpos, size_t* tdendpos, std::string* tddata)
    {
        return GetMarkData(pagedata, beginpos, mark_td_begin, mark_td_end, tddata, tdendpos);
    }

    bool GetSingerInfoFromTdData(const std::string& tddata,
        size_t beginpos, std::string* singername, 
        uint32* singerid, uint32* roomid)
    {
        std::string strroomid;
        size_t pos = 0;
        if (!GetMarkData(tddata, beginpos, "http://fanxing.kugou.com/", "\"", &strroomid, &pos))
            return false;

        std::string tempsingername;
        beginpos = pos;
        if (!GetMarkData(tddata, beginpos, R"(target="_blank">)", "</a>", &tempsingername, &pos))
            return false;

        std::string strsingerid;
        beginpos = pos;
        if (!GetMarkData(tddata, beginpos, "(", ")", &strsingerid, &pos))
            return false;

        if (!base::StringToUint(strroomid, roomid))
            return false;

        if (!base::StringToUint(strsingerid, singerid))
            return false;

        *singername = tempsingername;
        return true;
    }
}

FamilyDaily::FamilyDaily()
{
}


FamilyDaily::~FamilyDaily()
{
}

void FamilyDaily::CurlInit()
{
    curl_global_init(CURL_GLOBAL_WIN32);
}
void FamilyDaily::CurlCleanup()
{
    curl_global_cleanup();
}

bool FamilyDaily::WriteResponseDataCallback(const std::string& data)
{
    currentResponseData_ += data;
    return true;
}

bool FamilyDaily::WriteResponseHeaderCallback(const std::string& data)
{
    currentResponseHeader_ += data;
    return true;
}

//GET /admin?act=login HTTP/1.1
//Host: family.fanxing.kugou.com
//Connection: keep-alive
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//Upgrade-Insecure-Requests: 1
//User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36
//Accept-Encoding: gzip, deflate, sdch
//Accept-Language: zh-CN,zh;q=0.8
bool FamilyDaily::Init()
{
    base::FilePath dirPath;
    bool result = PathService::Get(base::DIR_EXE, &dirPath);
    assert(result);
    base::FilePath cookiespath = dirPath.Append(L"family.kugou.com.cookies.txt");
    cookiespath_ = base::WideToUTF8(cookiespath.value());
    assert(!cookiespath_.empty());

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if (!curl)
        return false;

    std::string url = std::string(familyurl) + "/admin?act=login";
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    struct curl_slist *headers = 0;
    headers = curl_slist_append(headers, header_host);
    headers = curl_slist_append(headers, header_connection);
    headers = curl_slist_append(headers, header_language);
    headers = curl_slist_append(headers, header_accept);
    headers = curl_slist_append(headers, header_upgrade);
    headers = curl_slist_append(headers, header_useragent);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    
    currentResponseData_.clear();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

    // Ϊ�˻�ȡcookies������
    currentResponseHeader_.clear();
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    if (headers)
    {
        curl_slist_free_all(headers);
    }

    /* Check for errors */
    if (res != CURLE_OK)
    {
        //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return false;
    }

    std::wstring filename = base::UTF8ToWide(MakeReasonablePath(__FUNCTION__) + ".txt");
    base::FilePath logpath = dirPath.Append(filename);
    base::File logfile(logpath,
        base::File::FLAG_CREATE_ALWAYS | base::File::FLAG_WRITE);
    logfile.Write(0, currentResponseData_.c_str(), currentResponseData_.size());

    auto pos = currentResponseHeader_.find("PHPSESSID=");
    if (pos != std::string::npos)
    {
        auto begin = pos;
        auto end = currentResponseHeader_.find(';', begin);
        cookies_ = currentResponseHeader_.substr(begin, end - begin);
    }
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
        //std::cout << "CURLINFO_COOKIELIST get cookie: " << retCookies;
        curl_slist_free_all(curllist);
    }

    // ��ȡ����ҵ����
    long responsecode = 0;
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responsecode);

    if (responsecode == 200)
    {
        return true;
    }
    return false;
}
// http://family.fanxing.kugou.com/admin?act=login
//POST /admin?act=login HTTP/1.1
//Host: family.fanxing.kugou.com
//Connection: keep-alive
//Content-Length: 47
//Cache-Control: max-age=0
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//Origin: http://family.fanxing.kugou.com
//Upgrade-Insecure-Requests: 1
//User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36
//Content-Type: application/x-www-form-urlencoded
//Referer: http://family.fanxing.kugou.com/admin?act=login
//Accept-Encoding: gzip, deflate
//Accept-Language: zh-CN,zh;q=0.8
//Cookie: PHPSESSID=lqbq66gtuv6rof4vb324112m94; f_p=f_569f993bd3f2c5.14993273; _family_login_time=1
bool FamilyDaily::Login(const std::string& username, const std::string& password)
{
    base::FilePath dirPath;
    bool result = PathService::Get(base::DIR_EXE, &dirPath);
    assert(result);

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if (!curl)
        return false;
   
    std::string url = std::string(familyurl) + "/admin?act=login";
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    // �����������ȥ,���޷��ж�Ŀǰ���
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    struct curl_slist *headers = 0;
    headers = curl_slist_append(headers, header_connection);
    headers = curl_slist_append(headers, "Cache-Control: max-age=0");
    headers = curl_slist_append(headers, header_language);
    headers = curl_slist_append(headers, "Origin: http://family.fanxing.kugou.com");
    headers = curl_slist_append(headers, header_accept);
    headers = curl_slist_append(headers, header_upgrade);
    headers = curl_slist_append(headers, header_useragent);
    headers = curl_slist_append(headers, header_content);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    curl_easy_setopt(curl, CURLOPT_REFERER, "http://family.fanxing.kugou.com/admin?act=login");
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookies_.c_str());

    std::string encodeusername = UrlEncode(username);
    std::string encodepassword = UrlEncode(password);
    std::string postFields = "adminName=" + encodeusername + "&password=" + encodepassword;

    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postFields.size());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    
    currentResponseData_.clear();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

    currentResponseHeader_.clear();
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, this);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);

    if (headers)
    {
        curl_slist_free_all(headers);
    }

    /* Check for errors */
    if (res != CURLE_OK)
    {
        //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return false;
    }

    auto pos = currentResponseHeader_.find("f_p=");
    if (pos != std::string::npos)
    {
        auto begin = pos;
        auto end = currentResponseHeader_.find(';', begin);
        std::string cookies = currentResponseHeader_.substr(begin, end - begin);
        cookies_ += ";" + cookies;
    }

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
        //std::cout << "CURLINFO_COOKIELIST get cookie: " << retCookies;
        curl_slist_free_all(curllist);
    }

    std::wstring filename = base::UTF8ToWide(MakeReasonablePath(__FUNCTION__) + ".txt");
    base::FilePath logpath = dirPath.Append(filename);
    base::File logfile(logpath,
        base::File::FLAG_CREATE_ALWAYS | base::File::FLAG_WRITE);
    logfile.Write(0, currentResponseData_.c_str(), currentResponseData_.size());

    // ��ȡ����ҵ����
    long responsecode = 0;
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responsecode);

    /* always cleanup */
    curl_easy_cleanup(curl);

    if (responsecode == 302)
    {
        return true;
    }

    if (responsecode == 200 && currentResponseData_ =="0")
    {
        return true;
    }
    return false;
}

//GET http://family.fanxing.kugou.com/admin?act=dayStarDataList&startDay=2016-03-01&endDay=2016-03-06&starId=116432085 HTTP/1.1
//Host: family.fanxing.kugou.com
//Connection: keep-alive
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//Upgrade-Insecure-Requests: 1
//User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36
//Referer: http://family.fanxing.kugou.com/admin?act=dayStarDataList
//Accept-Encoding: gzip, deflate, sdch
//Accept-Language: zh-CN,zh;q=0.8
//Cookie: PHPSESSID=828t48vhi1u433g06hgtgk1g00; f_p=f_56d1c9af5aea30.63869374; 
//_fx_r_follow=%5B142314756%2C169907997%2C169000775%2C165996638%2C165025636%2C163083252%2C162498721%2C165754417%2C159616776%2C159531158%2C158459161%2C147385523%2C148041628%2C149259681%2C150377024%2C141023689%2C136981235%2C145901830%2C142879819%2C142078347%2C146052549%2C127624914%2C127618396%2C136883471%2C139839374%2C137880598%2C137180088%2C127277529%2C127277337%2C131405036%2C127277804%2C127277690%2C126280898%2C126014190%2C125360917%2C116432085%2C136625872%2C117438839%2C115609206%2C114721789%2C120357891%2C118257208%2C115043602%2C107301439%2C105209005%2C112791202%2C112569182%2C110469751%2C106559459%2C110384212%2C97362569%2C91428332%2C78125705%2C66239107%2C118774682%2C61517977%2C53722204%2C50999842%2C69260016%2C60843845%2C49592188%2C43385401%2C114748626%2C41028958%2C39310466%2C37713273%2C33986382%2C99241150%2C31889265%2C165569900%2C33313091%2C20081096%2C124625434%2C95259291%2C159730972%5D; pgv_pvi=2486706176; pgv_si=s2470812672; 800057627mid=730_44; 800057627slid=slid_396_77%7C; 800057627slid_396_77=1456921767738; 800057627mh=1456921768746; FANXING=%257B%2522kugouId%2522%253A%2522615887139%2522%252C%2522coin%2522%253A%252226815.00%2522%252C%2522atime%2522%253A1457013595%252C%2522isRemember%2522%253A0%252C%2522sign%2522%253A%2522be974c32cd8401a6ea9fab56d1fb213e%2522%257D; KuGoo=KugooID=615887139&KugooPwd=E54BCC78DA033CB1B0402B4E339757C5&NickName=%u0067%u006c%u006f%u0062%u0061%u006c%u0073%u0074%u0061%u0072%u0030%u0030%u0035&Pic=http://imge.kugou.com/kugouicon/165/20100101/20100101192931478054.jpg&RegState=1&RegFrom=&t=430081cafd2e0d8c1f046744235a50c0bfe64e2f7dcbd2e1a74a8c9b489cb072&a_id=1010&ct=1457152785&UserName=%u0067%u006c%u006f%u0062%u0061%u006c%u0073%u0074%u0061%u0072%u0030%u0030%u0035; fxClientInfo=%7B%22userId%22%3A%22105039763%22%2C%22kugouId%22%3A%22615887139%22%2C%22ip%22%3A%22119.131.77.190%22%7D; Hm_lvt_52e69492bce68bf637c6f3a2f099ae08=1456405260; Hm_lpvt_52e69492bce68bf637c6f3a2f099ae08=1457273231; Hm_lvt_e0a7c5eaf6994884c4376a64da96825a=1456405286; Hm_lpvt_e0a7c5eaf6994884c4376a64da96825a=1457273231
bool FamilyDaily::GetDailyDataBySingerId(uint32 singerid,
    const base::Time& begintime, const base::Time& endtime,
    std::vector<SingerDailyData>* singerdailydata)
{
    // ��֧�ֳ���30��Ĳ�ѯ
    base::TimeDelta delta = endtime - begintime;
    auto days = delta.InDays();
    if (days > 30)
        return false;
    
    std::string pagedata;
    if (!GetDailyDataBySingerIdAndPage(singerid, begintime, endtime, 1, &pagedata))
    {
        return false;
    }

    uint32 pagecount = 0;
    if (!ParsePageCount(pagedata, &pagecount))
    {
        return false;
    }
    assert(pagecount);

    std::vector<SingerDailyData> tempSingerDailyData;
    if (!ParseSingerDailyData(pagedata, &tempSingerDailyData))
    {
        return false;
    }

    singerdailydata->assign(tempSingerDailyData.begin(), tempSingerDailyData.end());

    for (uint32 page = 2; page <= pagecount; page++)
    {
        if (!GetDailyDataBySingerIdAndPage(singerid, begintime, endtime, page, &pagedata))
            return false;

        tempSingerDailyData.clear();
        if (!ParseSingerDailyData(pagedata, &tempSingerDailyData))
            return false;
        assert(!tempSingerDailyData.empty());
        singerdailydata->insert(singerdailydata->end(),
            tempSingerDailyData.begin(), tempSingerDailyData.end());
    }
    return true;
}

//GET /admin?act=sumStarDataList&startDay=2016-01-01&endDay=2016-01-21 HTTP/1.1
//Host: family.fanxing.kugou.com
//Connection: keep-alive
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8
//Upgrade-Insecure-Requests: 1
//User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36
//Referer: http://family.fanxing.kugou.com/admin?act=sumStarDataList
//Accept-Encoding: gzip, deflate, sdch
//Accept-Language: zh-CN,zh;q=0.8
//Cookie: PHPSESSID=3dfg2i875s2p7hcousceilmag3; f_p=f_56a0f5ea31c3f7.88052429

bool FamilyDaily::GetSummaryData(const base::Time& begintime, const base::Time& endtime,
    std::vector<SingerSummaryData>* summerydata)
{
    std::string pagedata;
    bool result = GetSummaryDataByPage(begintime, endtime, 1, &pagedata);
    assert(result);
    if (!result)
        return false;

    std::vector<SingerSummaryData> pagesummary;
    uint32 pagecount = 0;
    result = ParsePageCount(pagedata, &pagecount);
    assert(result);
    if (!result)
        return false;

    result = ParseSummaryData(pagedata, &pagesummary);
    assert(result);
    if (!result)
        return false;

    assert(pagecount);
    assert(!pagesummary.empty());

    summerydata->assign(pagesummary.begin(), pagesummary.end());

    for (uint32 page = 2; page <= pagecount; page++)
    {
        result = GetSummaryDataByPage(begintime, endtime, page, &pagedata);
        assert(result);
        if (!result)
            return false;

        pagesummary.clear();
        result = ParseSummaryData(pagedata, &pagesummary);
        assert(result);
        if (!result)
            return false;

        assert(!pagesummary.empty());
        summerydata->insert(summerydata->end(), 
            pagesummary.begin(), pagesummary.end());
    }
    return result;
}

bool FamilyDaily::GetSummaryDataByPage(const base::Time& begintime,
    const base::Time& endtime, uint32 pagenumber,
    std::string* pagedata)
{
    std::string beginstring = MakeFormatDateString(begintime);
    std::string endstring = MakeFormatDateString(endtime);

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if (!curl)
        return false;

    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    std::string requesturl = std::string(familyurl) +
        "/admin?act=sumStarDataList&startDay=" +
        beginstring + "&endDay=" + endstring;

    // ��һҳ���ݲ���Ҫ��ҳ��
    if (pagenumber > 1)
    {
        requesturl += "&page=" + base::IntToString(pagenumber);
    }

    curl_easy_setopt(curl, CURLOPT_URL, requesturl.c_str());

    // ����Get��ʽ
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    // �����������ȥ,���޷��ж�Ŀǰ���
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    struct curl_slist *headers = 0;
    headers = curl_slist_append(headers, header_connection);
    //headers = curl_slist_append(headers, "Cache-Control: max-age=0");
    headers = curl_slist_append(headers, header_language);
    //headers = curl_slist_append(headers, "Origin: http://family.fanxing.kugou.com");
    headers = curl_slist_append(headers, header_accept);
    headers = curl_slist_append(headers, header_upgrade);
    headers = curl_slist_append(headers, header_useragent);
    headers = curl_slist_append(headers, header_content);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    //curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    //curl_easy_setopt(curl, CURLOPT_REFERER, "http://family.fanxing.kugou.com/admin?act=sumStarDataList");

    //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiespath_.c_str());
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookies_.c_str());
    currentResponseData_.clear();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    if (headers)
    {
        curl_slist_free_all(headers);
    }
    
    /* Check for errors */
    if (res != CURLE_OK)
    {
        //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return false;
    }
    // ��ȡ����ҵ����
    long responsecode = 0;
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responsecode);

    base::FilePath dirPath;
    bool result = PathService::Get(base::DIR_EXE, &dirPath);
    std::wstring filename = base::UTF8ToWide(MakeReasonablePath(__FUNCTION__) + ".txt");
    base::FilePath logpath = dirPath.Append(filename);
    base::File logfile(logpath,
        base::File::FLAG_CREATE_ALWAYS | base::File::FLAG_WRITE);
    logfile.Write(0, currentResponseData_.c_str(), currentResponseData_.size());

    /* always cleanup */
    curl_easy_cleanup(curl);

    if (responsecode == 302)
    {
        assert(false && L"GetSummaryData must set CURLOPT_FOLLOWLOCATION to follow redirection");
        return true;
    }

    if (responsecode == 200 && !currentResponseData_.empty())
    {
        *pagedata = currentResponseData_;
        return true;
    }
    return false;
}

bool FamilyDaily::GetDailyDataBySingerIdAndPage(uint32 singerid,
    const base::Time& begintime,
    const base::Time& endtime, uint32 pagenumber,
    std::string* pagedata)
{
    std::string beginstring = MakeFormatDateString(begintime);
    std::string endstring = MakeFormatDateString(endtime);

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();

    if (!curl)
        return false;

    curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);

    std::string requesturl = std::string(familyurl) +
        "/admin?act=dayStarDataList&startDay=" +
        beginstring + "&endDay=" + endstring + "&starId="
        + base::UintToString(singerid);

    // ��һҳ���ݲ���Ҫ��ҳ��
    if (pagenumber > 1)
    {
        requesturl += "&page=" + base::IntToString(pagenumber);
    }

    curl_easy_setopt(curl, CURLOPT_URL, requesturl.c_str());

    // ����Get��ʽ
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    // �����������ȥ,���޷��ж�Ŀǰ���
    /* example.com is redirected, so we tell libcurl to follow redirection */
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);

    struct curl_slist *headers = 0;
    headers = curl_slist_append(headers, header_connection);
    //headers = curl_slist_append(headers, "Cache-Control: max-age=0");
    headers = curl_slist_append(headers, header_language);
    //headers = curl_slist_append(headers, "Origin: http://family.fanxing.kugou.com");
    headers = curl_slist_append(headers, header_accept);
    headers = curl_slist_append(headers, header_upgrade);
    headers = curl_slist_append(headers, header_useragent);
    headers = curl_slist_append(headers, header_content);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    //curl_easy_setopt(curl, CURLOPT_AUTOREFERER, 1L);
    // �ȱ���referer��Ҳ�п�����Ҫ
    //curl_easy_setopt(curl, CURLOPT_REFERER, "http://family.fanxing.kugou.com/admin?act=sumStarDataList");

    //curl_easy_setopt(curl, CURLOPT_COOKIEFILE, cookiespath_.c_str());
    curl_easy_setopt(curl, CURLOPT_COOKIE, cookies_.c_str());
    currentResponseData_.clear();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    if (headers)
    {
        curl_slist_free_all(headers);
    }

    /* Check for errors */
    if (res != CURLE_OK)
    {
        //fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        return false;
    }

    // ��ȡ����ҵ����
    long responsecode = 0;
    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responsecode);

    base::FilePath dirPath;
    bool result = PathService::Get(base::DIR_EXE, &dirPath);
    std::wstring filename = base::UTF8ToWide(MakeReasonablePath(__FUNCTION__) + ".txt");
    base::FilePath logpath = dirPath.Append(filename);
    base::File logfile(logpath,
        base::File::FLAG_CREATE_ALWAYS | base::File::FLAG_WRITE);
    logfile.Write(0, currentResponseData_.c_str(), currentResponseData_.size());

    /* always cleanup */
    curl_easy_cleanup(curl);

    if (responsecode == 302)
    {
        assert(false && L"GetDailyDataBySingerIdAndPage must set CURLOPT_FOLLOWLOCATION to follow redirection");
        return true;
    }

    if (responsecode == 200 && !currentResponseData_.empty())
    {
        *pagedata = currentResponseData_;
        return true;
    }

    return false;
}

// colspan
bool FamilyDaily::ParseSummaryData(const std::string& pagedata,    
    std::vector<SingerSummaryData>* summerydata) const
{
    bool result = false;
    std::string tabledata;
    result = GetTableData(pagedata, &tabledata);
    std::string trdata;
    size_t trendpos = 0;
    size_t beginpos = 0;
    // ��һ�������Ǳ�ͷ
    result = GetTrData(tabledata, beginpos, &trdata, &trendpos);

    std::vector<std::string> trvector;
    while (result)
    {
        beginpos = trendpos;
        result = GetTrData(tabledata, beginpos, &trdata, &trendpos);
        if (result)
            trvector.push_back(trdata);
    }

    // ���һ���Ǳ���·�ҳ������
    if (!trvector.empty())
    {
        trvector.erase(trvector.end()-1);
    }
    
    
    std::vector<SingerSummaryData> singerSummaryDataVector;
    for (const auto& it : trvector)
    {
        std::string trdata = it;
        size_t beginpos = 0;
        size_t endpos = 0;
        std::string tddata;      
        SingerSummaryData singerSummaryData;

        // �����ֶ�
        // <a href="http://fanxing.kugou.com/1060982" target="_blank">İ·shiley</a>(10215159)
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        uint32 singerid = 0;
        uint32 roomid = 0;
        std::string singername;
        if (!GetSingerInfoFromTdData(tddata, beginpos, &singername, &singerid, &roomid))
        {
            assert(false);
            continue;
        }

        singerSummaryData.singerid = singerid;
        singerSummaryData.roomid = roomid;
        singerSummaryData.nickname = singername;

        // ���������ֱ�������ֶΣ���ʹ��
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }

        // �����ȼ�
        beginpos = endpos;
        result = GetTdData(trdata, beginpos, &endpos, &tddata);
        singerSummaryData.singerlevel = tddata;

        // ��������
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        uint32 onlinecount = 0;
        if (!base::StringToUint(tddata, &onlinecount))
        {
            assert(false);
            return false;
        }

        singerSummaryData.onlinecount = onlinecount;

        // �ۼ�ֱ��ʱ�������ӣ�
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        
        uint32 onlineminute = 0;
        if (!base::StringToUint(tddata, &onlineminute))
        {
            assert(false);
            return false;
        }

        singerSummaryData.onlineminute = onlineminute;

        // ��Чֱ������������1��Сʱ��
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        uint32 effectivecount = 0;
        if (!base::StringToUint(tddata, &effectivecount))
        {
            assert(false);
            return false;
        }
        singerSummaryData.effectivecount = effectivecount;

        // ֱ�����������
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        uint32 maxusers = 0;
        if (!base::StringToUint(tddata, &maxusers))
        {
            assert(false);
            return false;
        }
        singerSummaryData.maxusers = maxusers;

        // �Ƕ�����
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        double revenue = 0;
        if (!base::StringToDouble(tddata, &revenue))
        {
            assert(false);
            return false;
        }
        singerSummaryData.revenue = revenue;
        singerSummaryDataVector.push_back(singerSummaryData);
    }

    *summerydata = std::move(singerSummaryDataVector);
    return true;
}

bool FamilyDaily::ParseSingerDailyData(const std::string& pagedata,
    std::vector<SingerDailyData>* singerdailydatavector) const
{
    bool result = false;
    std::string tabledata;
    result = GetTableData(pagedata, &tabledata);
    std::string trdata;
    size_t trendpos = 0;
    size_t beginpos = 0;
    // ��һ�������Ǳ�ͷ
    result = GetTrData(tabledata, beginpos, &trdata, &trendpos);

    std::vector<std::string> trvector;
    while (result)
    {
        beginpos = trendpos;
        result = GetTrData(tabledata, beginpos, &trdata, &trendpos);
        if (result)
            trvector.push_back(trdata);
    }

    // ���һ���Ǳ���·�ҳ������
    if (!trvector.empty())
    {
        trvector.erase(trvector.end() - 1);
    }

    std::vector<SingerDailyData> tempvector;
    for (const auto& it : trvector)
    {
        std::string trdata = it;
        size_t beginpos = 0;
        size_t endpos = 0;
        std::string tddata;
        SingerDailyData singerdailydata;

        // ����
        beginpos = endpos;
        result = GetTdData(trdata, beginpos, &endpos, &tddata);
        singerdailydata.date = tddata;

        // ��������
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        uint32 onlinecount = 0;
        if (!base::StringToUint(tddata, &onlinecount))
        {
            assert(false);
            return false;
        }
        singerdailydata.onlinecount = onlinecount;

        // �ۼ�ֱ��ʱ�������ӣ�
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }

        uint32 onlineminute = 0;
        if (!base::StringToUint(tddata, &onlineminute))
        {
            assert(false);
            return false;
        }
        singerdailydata.onlineminute = onlineminute;

        // ��Чֱ������������1��Сʱ��
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        uint32 effectivecount = 0;
        if (!base::StringToUint(tddata, &effectivecount))
        {
            assert(false);
            return false;
        }
        singerdailydata.effectivecount = effectivecount;

        // ֱ�����������
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        uint32 maxusers = 0;
        if (!base::StringToUint(tddata, &maxusers))
        {
            assert(false);
            return false;
        }
        singerdailydata.maxusers = maxusers;

        // �Ƕ�����
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        double revenue = 0;
        if (!base::StringToDouble(tddata, &revenue))
        {
            assert(false);
            return false;
        }
        singerdailydata.revenue = revenue;

        // �����ۼƿ۷�
        beginpos = endpos;
        if (!GetTdData(trdata, beginpos, &endpos, &tddata))
        {
            assert(false);
            continue;
        }
        uint32 blame = 0;
        if (!base::StringToUint(tddata, &blame))
        {
            assert(false);
            return false;
        }
        singerdailydata.blame = blame;
        tempvector.push_back(singerdailydata);
    }

    *singerdailydatavector = std::move(tempvector);
    return true;
}


bool FamilyDaily::ParsePageCount(const std::string& pagedata, uint32* pagenumber)
{
    static const char* colspan = R"(<tr><td colspan=")";
    static const char* doublequote = R"(")";
    std::string str = colspan;
    auto pos = pagedata.find(str);
    if (pos == pagedata.npos)
        return false;

    pos += strlen(colspan);
    auto endpos = pagedata.find(doublequote, pos);
    std::string count = pagedata.substr(pos, endpos - pos);
    uint32 countperpage = 0;
    if (!base::StringToUint(count, &countperpage))
        return false;

    if (!countperpage)
        return false;

    std::string totalbegin = base::WideToUTF8(L"<span>�ܹ�");
    std::string totalend = base::WideToUTF8(L"����¼");

    pos = pagedata.find(totalbegin);
    if (pos == std::string::npos)
        return false;

    pos += totalbegin.size();
    endpos = pagedata.find(totalend);
    std::string strtotalcount = pagedata.substr(pos, endpos - pos);
    uint32 totalcount = 0;
    if (!base::StringToUint(strtotalcount, &totalcount))
        return false;

    *pagenumber = totalcount / countperpage;
    *pagenumber += (totalcount % countperpage) > 0 ? 1 : 0;
    return true;
}
