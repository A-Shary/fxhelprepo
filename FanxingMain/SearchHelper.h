#pragma once
#include <string>
#include <vector>
#include <map>
#include "third_party/chromium/base/basictypes.h"


struct SingerInfo
{
    uint32 roomid;
    uint32 userid;
    uint32 clanid;
    uint32 lastday;//���һ���ϲ������ڵ�������һ���°�30����
    uint32 fans;// ��˿����
    uint32 pictures;//���������Ƭ����
    std::string nickname;
    std::string richlevel;
    std::string starlevel;
    std::string roomurl;
    std::string userurl;
    std::string lastactiionString;
};

// ����������Ϣ
struct FamilyInfo
{
    uint32 clanid;
    std::string clanname;
    std::string badgename;//������
    uint32 clanroomid;
    uint64 cointotal;
    uint32 clanlevel;
    uint32 clanleaderuserid;
    std::string clanleadernickname;
    uint32 managercount;
    uint32 subleadercount;
    uint32 usercount;
    uint32 starcount;
    uint64 addtime;//unix time;
    bool iscompany;
};

// ���๦���Ǵ�ȫվ�ҵ��ܳ�ʱ��û���ϵ�����
class SearchHelper
{
public:
    SearchHelper();
    ~SearchHelper();

    static void CurlInit();
    static void CurlCleanup();

    void Run();
    void SetFilter(uint32 lastactive,uint32 pagecount);
    bool GetExpiredFamilySingers(std::map<uint32, FamilyInfo>* familyInfoMap,
        std::vector<SingerInfo>* singerinfo);

    bool GetExpiredNormalSingers(std::vector<SingerInfo>* singerinfo);

    bool GetClanAllSingerInfo(uint32 clanid);
private:

    bool GetUrlData(const std::string& url, std::string* response) const;
    bool GetAllFamilyInfo(std::map<uint32,FamilyInfo>* familyInfoMap);
    bool GetAllSingers(uint32 clanid, std::vector<std::string>* singerIdList);
    bool GetSingsInfos(const std::string& singerId, SingerInfo* singerinfo);
    bool WriteToFile(const std::map<uint32, FamilyInfo>& familyInfoMap,
        const std::vector<SingerInfo>& singerinfos);
    uint32 lastactive_;
    uint32 pagecount_;
};

