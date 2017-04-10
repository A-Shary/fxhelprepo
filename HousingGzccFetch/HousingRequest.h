#pragma once

#include <list>
#include "third_party/chromium/base/basictypes.h"
#include "Network/EncodeHelper.h"
#include "Network/CurlWrapper.h"
#include "Network/CookiesHelper.h"

class HousingRequest
{
public:
    HousingRequest();
    ~HousingRequest();

    // http://housing.gzcc.gov.cn/fyxx/ysz/
    // ysz = Ԥ��֤, max_page��Ϊ�˱���һ�������������,ֻ������ǰ��ҳ
    bool GetYszResult(std::vector<std::string>* headers,
        std::list<std::vector<std::string>>* record_list, uint32 max_pages);

    bool Test();

private:
    bool GetFirstPageContent(std::string* content);
    bool GetPageContentByNumber(const std::string& page_number, std::string* content);
    bool ParsePageCount(const std::string& data, uint32* page_count) const;
    bool ParseYszResult(const std::string& data, std::vector<std::string>* headers,
        std::list<std::vector<std::string>>* record_list) const;
    bool ParseOneRecode(const std::string& data, std::vector<std::string>* house_record) const;
    bool ParseHeader(const std::string& data, std::vector<std::string>* house_header) const;

    CurlWrapper curl_;
};

