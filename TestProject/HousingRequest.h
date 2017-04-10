#pragma once

#include "third_party/chromium/base/basictypes.h"
#include "Network/EncodeHelper.h"
#include "Network/CurlWrapper.h"
#include "Network/CookiesHelper.h"

struct HouseRecord
{
    std::string house_source_index;
    std::string house_area;
    std::string house_addr;
    std::string house_price;
    std::string house_style; // ����
    std::string house_acreage; // ���
    std::string house_state; // ����
    std::string house_intermediary; // �н���
    std::string house_date; // ����ʱ��
};

class HousingRequest
{
public:
    HousingRequest();
    ~HousingRequest();

    // http://housing.gzcc.gov.cn/search/clf/clfSearch.jsp
    bool GetClfSearch();

    bool Test();

private:
    bool GetByPageNumber(const std::string& page_number, std::list<std::vector<std::string>>* record_list);
    bool ParseClfSearch(const std::string& data, std::list<std::vector<std::string>>* record_list) const;
    bool ParseOneRecode(const std::string& data, std::vector<std::string>* house_record) const;

    // ��ȡ��֤���Ӧ��cookie
    bool GetCountCookie();
    bool GetVerifyCode();

    CurlWrapper curl_;
};

