#pragma once

#include <string>
#include <vector>
#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/time/time.h"

struct SingerDailyData
{
    uint32 singerid;
};

struct SingerSummaryData
{
    uint32 singerid;
    uint32 roomid;
    std::string nickname;
    std::string singerlevel;    //�����ȼ�
    uint32 onlinecount;         //��������
    uint32 onlineminute;        //�ۼ�ֱ��ʱ�������ӣ�
    uint32 effectivecount;      //��Чֱ������������1��Сʱ��
    uint32 maxusers;            //ֱ�����������
    double revenue;             // �Ƕ�����
};

class FamilyDaily
{
public:
    FamilyDaily();
    ~FamilyDaily();

    static void CurlInit();
    static void CurlCleanup();

    // ����ҳ��,��ȡ��Ӧ��cookie;
    bool Init();

    bool Login(const std::string& username, const std::string& password);

    bool GetDailyDataBySingerId(uint32 singerid,
                                std::vector<SingerDailyData> dailydata);

    bool GetSummaryData(const base::Time& begintime, const base::Time& endtime,
                        std::vector<SingerSummaryData>* summerydata);

    bool WriteResponseDataCallback(const std::string& data);
    bool WriteResponseHeaderCallback(const std::string& data);

private:
    bool GetSummaryDataByPage(const base::Time& begintime,
        const base::Time& endtime, uint32 pagenumber,
        std::string* pagedata);

    // ��ժȡ�������л�ȡ����������Ϣ�Լ����з�ҳ����
    bool ParseSummaryData(const std::string& pagedata, 
                          std::vector<SingerSummaryData>* summerydata);

    bool ParsePageCount(const std::string& pagedata, uint32* pagenumber);

    std::string cookies_;
    std::string cookiespath_;

    std::string currentResponseData_;
    std::string currentResponseHeader_;
};
