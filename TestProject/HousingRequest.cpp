#include "HousingRequest.h"

#include "Network/easy_http_impl.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"

namespace
{
static const char* mark_thead_begin = "<thead";
static const char* mark_thead_end = "</thead>";

static const char* mark_table_begin = "<table";
static const char* mark_table_end = "</table>";
static const char* mark_tr_begin = "<tr";
static const char* mark_tr_end = "</tr>";
static const char* mark_td_begin = "<td ";
static const char* mark_td_end = "</td>";
static const char* mark_a_begin = "<a ";
static const char* mark_a_end = "</a>";
static const char* mark_th_begin = "<th";
static const char* mark_th_end = "</th>";

bool GetMarkData(const std::string& pagedata, size_t beginpos,
    const std::string& beginmark,
    const std::string& endmark,
    std::string* targetdata,
    size_t *afterendmarkpos)
{
    beginpos = pagedata.find(beginmark, beginpos);
    if (beginpos == std::string::npos)
        return false;
    //beginpos += beginmark.size();

    size_t pos = beginpos;
    int pair_count = 0;
    while (pos < pagedata.size())
    {
        switch (pagedata[pos])
        {
        case '<':
            pair_count++;
            break;
        case '>':
            pair_count--;
            break;
        default:
            break;
        }

        // ��ȫԽ����ǩ��
        if (!pair_count)
            break;
        pos++;
    }

    beginpos = pos+1;

    auto endpos = pagedata.find(endmark, beginpos);
    if (endpos == std::string::npos)
        return false;

    *afterendmarkpos = endpos + endmark.size();
    *targetdata = pagedata.substr(beginpos, endpos - beginpos);
    return true;
}

bool GetTheadData(const std::string& pagedata, std::string* theaddata)
{
    size_t theadendpos = 0;
    return GetMarkData(pagedata, 0, mark_thead_begin, mark_thead_end,
        theaddata, &theadendpos);
}

bool GetTableData(const std::string& pagedata, std::string* tabledata)
{
    size_t tableendpos = 0;
    return GetMarkData(pagedata, 0, mark_table_begin, mark_table_end,
        tabledata, &tableendpos);
}

bool GetTrData(const std::string& pagedata, size_t beginpos,
    size_t* trendpos, std::string* trdata)
{
    return GetMarkData(pagedata, beginpos, mark_tr_begin, mark_tr_end, trdata, trendpos);
}

bool GetTdData(const std::string& pagedata, size_t beginpos, size_t* tdendpos, std::string* tddata)
{
    return GetMarkData(pagedata, beginpos, mark_td_begin, mark_td_end, tddata, tdendpos);
}

bool GetThData(const std::string& pagedata, size_t beginpos,
    size_t* thendpos, std::string* thdata)
{
    return GetMarkData(pagedata, beginpos, mark_th_begin, mark_th_end, thdata, thendpos);
}

bool GetAData(const std::string& pagedata, size_t beginpos,
    std::string* trdata, size_t* trendpos)
{
    return GetMarkData(pagedata, beginpos, mark_a_begin, mark_a_end, trdata, trendpos);
}
}

HousingRequest::HousingRequest()
{
    curl_.Initialize();
}

HousingRequest::~HousingRequest()
{
    curl_.Finalize();
}

bool HousingRequest::GetYszResult(std::vector<std::string>* headers,
    std::list<std::vector<std::string>>* record_list)
{
    std::string first_page_content;
    if (!GetFirstPageContent(&first_page_content))
        return false;
    
    uint32 page_count = 0;
    if (!ParsePageCount(first_page_content, &page_count))
        return false;

    std::vector<std::string> temp_header;
    std::list<std::vector<std::string>> temp_record_list;
    if (!ParseYszResult(first_page_content, &temp_header, &temp_record_list))
        return false;
    
    headers->assign(temp_header.begin(), temp_header.end());
    record_list->insert(record_list->end(), temp_record_list.begin(), temp_record_list.end());
    for (uint32 page_no = 1; page_no < page_count; page_no++)
    {
        std::string page_number = base::UintToString(page_no);
        std::string content;
        if (!GetPageContentByNumber(page_number, &content))
            continue;

        std::list<std::vector<std::string>> page_record_list;
        if (!ParseYszResult(content, nullptr, &page_record_list))
            continue;

        record_list->insert(record_list->end(), page_record_list.begin(), page_record_list.end());
    }
    
    return true;
}

bool HousingRequest::ParseYszResult(const std::string& data,
    std::vector<std::string>* headers,
    std::list<std::vector<std::string>>* record_list) const
{
    // ����ҳ�涼�������ʽ��titleͼƬ
    auto pos = data.find("/images/title_info_list_");
    if (pos== std::string::npos)
        return false;

    std::string contain_table = data.substr(pos + 1);

    std::string table_data;
    if (!GetTableData(contain_table, &table_data))
        return false;

    std::string header_tr_data;
    size_t tr_end;
    if (!GetTrData(table_data, 0, &tr_end, &header_tr_data))
        return false;

    if (!headers)
        ParseHeader(header_tr_data, headers);

    // �ӱ���ȡ���ݣ�Ŀǰ������ÿҳ15������ҳδ����
    std::string tr_data;
    while (true)
    {
        if (!GetTrData(table_data, tr_end + 1, &tr_end, &tr_data))
            break;

        std::vector<std::string> record;
        ParseOneRecode(tr_data, &record);
        record_list->push_back(record);
    }

    return true;
}

bool HousingRequest::Test()
{
    std::string temp = R"(
	  <td align="center" class="box_tab_style02_td">1</td>
	    <td align="center" class="box_tab_style02_td"><a href="/search/project/project_detail.jsp?changeproInfoTag=1&changeSellFormtag=1&pjID=47140&name=fdcxmxx" target="_blank">�к���ɳܰ԰���Ա�A4-A7����</a></td>
	    <td align="center" class="box_tab_style02_td"><a href="/search/project/project_detail.jsp?changeproInfoTag=1&changeSellFormtag=1&pjID=47140&name=fdcxmxx" target="_blank">�����к��ز����޹�˾</a></td>
	    <td align="left" class="box_tab_style02_td padding_left10px"><a href="/search/project/project_detail.jsp?changeproInfoTag=1&changeSellFormtag=1&pjID=47140&name=fdcxmxx" target="_blank"><img src="/images/eeb60828ae.gif" width="11" height="12" /><img src="/images/cea7249519.gif" width="11" height="12" /><img src="/images/cea7249519.gif" width="11" height="12" /><img src="/images/853f634d1e.gif" width="11" height="12" /><img src="/images/cea7249519.gif" width="11" height="12" /><img src="/images/6a1d935323.gif" width="11" height="12" /><img src="/images/d759e0da63.gif" width="11" height="12" /><img src="/images/bfea3b40f6.gif" width="11" height="12" /><br></a></td>
	    <td align="center" class="box_tab_style02_td"><a href="/search/project/project_detail.jsp?changeproInfoTag=1&changeSellFormtag=1&pjID=47140&name=fdcxmxx" target="_blank">�������1��3��5��9��11�ţ���ɳ��B3735F01�ؿ飩</a></td>
	    <td align="left" class="box_tab_style02_td padding_left10px"><a href="/search/project/project_detail.jsp?changeproInfoTag=1&changeSellFormtag=1&pjID=47140&name=fdcxmxx" target="_blank"><img src="/images/bfea3b40f6.gif" width="11" height="12" /><img src="/images/ccfcfc3238.gif" width="11" height="12" /><img src="/images/bea57ddbff.gif" width="11" height="12" /><br></a></td>
	    <td align="left" class="box_tab_style02_td padding_left10px"><a href="/search/project/project_detail.jsp?changeproInfoTag=1&changeSellFormtag=1&pjID=47140&name=fdcxmxx" target="_blank"><br></a></td>
)";

    std::vector<std::string> record;
    ParseOneRecode(temp, &record);

    std::string header_data = R"(
    	<thead><tr>
	<th width="5%" align="center" class="box_tab_style02_th"><strong>���</strong></th>
    <th width="25%" align="center" class="box_tab_style02_th"><strong><a href="#" onclick="doOrderBy('PROJECT_NAME','desc');return false;">��Ŀ����</a>
    </strong></th>
    <th width="25%" align="center" class="box_tab_style02_th"><strong><a href="#" onclick="doOrderBy('DEVELOPER','desc');return false;">������</a>
    </strong></th>
    <th width="14%" align="center" class="box_tab_style02_th"><strong><a href="#" onclick="doOrderBy('PRESELL_NO','desc');return false;">Ԥ��֤</a>
    </strong></th>
    <th width="15%" align="center" class="box_tab_style02_th"><strong><a href="#" onclick="doOrderBy('PROJECT_ADDRESS','desc');return false;">��Ŀ��ַ</a></strong></th>
    <th width="8%" align="center" class="box_tab_style02_th"><strong><a href="#" onclick="doOrderBy('HOUSE_SOLD_NUM','desc');return false;">סլ��������</a>
    </strong></th>
    <th width="8%" align="center" class="box_tab_style02_th"><strong><a href="#" onclick="doOrderBy('HOUSE_UNSALE_NUM','desc');return false;">סլδ������</a>
    </strong></th>
    </tr>
    </thead>
    )";
    std::vector<std::string> header;
    ParseHeader(header_data, &header);

    return true;
}

bool HousingRequest::GetFirstPageContent(std::string* content)
{
    HttpRequest request;
    request.url = "http://housing.gzcc.gov.cn/fyxx/ysz/";
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.headers["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8";
    request.headers["Accept-Language"] = "zh-CN,zh;q=0.8,en;q=0.6";

    HttpResponse response;
    if (!curl_.Execute(request, &response))
        return false;

    content->assign(response.content.begin(), response.content.end());
    return true;
}

bool HousingRequest::GetPageContentByNumber(const std::string& page_number, std::string* content)
{
    HttpRequest request;
    request.url = "http://housing.gzcc.gov.cn/fyxx/ysz/index_" + page_number + ".shtml";
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.headers["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8";
    request.headers["Accept-Language"] = "zh-CN,zh;q=0.8,en;q=0.6";

    HttpResponse response;
    if (!curl_.Execute(request, &response))
        return false;

    content->assign(response.content.begin(), response.content.end());
    return true;
}

bool HousingRequest::ParsePageCount(const std::string& data, uint32* page_count) const
{
    auto pos = data.find("createPageHTML");
    if (pos == std::string::npos)
        return false;

    auto begin_pos = data.find("(", pos);
    if (begin_pos == std::string::npos)
        return false;

    begin_pos++;
    auto end_pos = data.find(",", begin_pos);
    if (end_pos == std::string::npos)
        return false;

    std::string temp_data = data.substr(begin_pos, end_pos - begin_pos);

    if (!base::StringToUint(temp_data, page_count))
        return false;

    return true;
}

bool HousingRequest::ParseOneRecode(
    const std::string& data, std::vector<std::string>* house_record) const
{
    std::vector<std::string> record_data;
    std::string temp_data = data;
    std::string td_data;
    size_t last_td_end = 0;
    size_t current_td_end;
    if (!GetTdData(temp_data, last_td_end, &current_td_end, &td_data))
        return false;

    last_td_end = current_td_end;
    record_data.push_back(td_data);

    while (true)
    {
        std::string tr_data;
        if (!GetTdData(temp_data, last_td_end, &current_td_end, &tr_data))
            break;

        last_td_end = current_td_end;
        size_t a_end;
        std::string a_data;
        if (!GetAData(tr_data, 0, &a_data, &a_end))
            break;

        record_data.push_back(a_data);
    }
    *house_record = record_data;
    return true;
}

bool HousingRequest::ParseHeader(const std::string& data,
    std::vector<std::string>* house_header) const
{
    std::string temp_data = data;
    size_t current_th_end = 0;
    size_t last_th_end = 0;
    std::vector<std::string> record_data;

    std::string th_data;
    if (!GetThData(temp_data, last_th_end, &current_th_end, &th_data))
        return false;

    size_t current_strong_end;
    std::string serial_number;
    if (!GetMarkData(th_data,
        last_th_end, "<strong>", "</strong>", &serial_number,
        &current_strong_end))
        return false;

    record_data.push_back(serial_number);

    last_th_end = current_th_end;
    while (true)
    {
        std::string th_data;
        if (!GetThData(temp_data, last_th_end, &current_th_end, &th_data))
            break;

        last_th_end = current_th_end;
        size_t a_end;
        std::string a_data;
        if (!GetAData(th_data, 0, &a_data, &a_end))
            break;

        record_data.push_back(a_data);
    }
    *house_header = record_data;
    return true;
}
