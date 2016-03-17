#pragma once
#include <string>
#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/files/file.h"
#include "Network/CookiesManager.h"
#include "Network/GiftNotifyManager.h"

enum class KICK_TYPE
{
    KICK_TYPE_HOUR = 0,
    KICK_TYPE_MONTH = 1,
};

// �ṩ�����ʹ��curl�ӿڵ�ִ����������
class CurlWrapper
{
public:
    CurlWrapper();
    ~CurlWrapper();
    bool Initialize(){ return true; };//û���̹߳���ֱ�ӷ���true
    void Finalize(){ return; };//û���̹߳���ֱ�ӷ���

    static void CurlInit();
    static void CurlCleanup();
    bool WriteCallback(const std::string& data);
    bool WriteResponseHeaderCallback(const std::string& data);

    bool LoginRequestWithCookies();

    // ����ͨ��
    bool LoginRequestWithUsernameAndPassword(const std::string& username, 
                                             const std::string& password);
    bool Services_UserService_UserService_getMyUserDataInfo();
    bool Services_IndexService_IndexService_getUserCenter();
    
    // ��δ��¼״̬�½��뷿�䣬����ȡ����id,������tcp����ʹ��
    bool EnterRoom(uint32 roomid, uint32* singerid);

	// �ڽ��뷿���Ժ󣬻�ȡ�û���Ϣ
	bool Servies_Uservice_UserService_getCurrentUserInfo(uint32 roomid,
        uint32* userid, std::string* nickname, uint32* richlevel);

    // �ؼ����ݻ�ȡ�����������������tcp��������Ҫ����key����ֵ
    bool RoomService_RoomService_enterRoom(uint32 roomid);

    // ismaster�޷���ȡ��
    bool ExtractStarfulInfo_RoomService_enterRoom(uint32* staruserid,
        std::string* key, std::string* ext);

    // ���Ǳҵ���Ҫ����, ʱ����flash�յ�601������֪ͨ���ݣ�601������key_601ֵ
    bool GiftService_GiftService(uint32 roomid,
        const std::string& key_601, std::wstring* responsedata);

    bool KickoutUser(uint32 singerid, KICK_TYPE kicktype, const EnterRoomUserInfo& enterRoomUserInfo);

    // ��ȡ�����б���Ϣ��Ϊ������ϣ����ﲻ��������
    bool GetGiftList(uint32 roomid, std::string* outputstr);
private:

    //bool ExtractStarfulInfo_RoomService_enterRoom_(const std::string& inputstr,
    //    uint32* staruserid,
    //    std::string* key, std::string* ext);

    bool ParseGiftServiceResponse(const std::string& responsedata,
        std::wstring* notifyinfo);

    bool SetCookieFromString(const std::string& key, const std::string& cookiestring);
    std::string currentWriteData_;
    std::string currentResponseHeader_;
    std::string response_of_RoomService_RoomService_enterRoom_;
    std::string response_of_Services_UserService_UserService_getMyUserDataInfo_;
    std::string response_of_GiftService_GiftService_;
    std::string response_of_LoginWithUsernameAndPassword_;
    std::string response_of_EnterRoom_;

    CookiesManager cookiesmanager_;
};


// ��EnterRoom����
//{
//    "servertime" : "1439814776",
//        "callback" : [],
//        "data" : {
//        "fxUserInfo" : {
//            "userId" : "40919199",
//                "userName" : "sail2010@163.com",
//                "kugouId" : "454395944",
//                "nickName" : "\u9ea6\u514b\u54e5\u54e5",
//                "userLogo" : "http:\/\/s3.fx.kgimg.com\/fxusercmdavata\/system.gif",
//                "coin" : "160.00",
//                "coinSpend" : "8470.00",
//                "coinTotal" : "10.00",
//                "bean" : "5.00",
//                "beanTotal" : "5.00",
//                "payTotal" : "0",
//                "czTotal" : "10000",
//                "starLevel" : "0",
//                "richLevel" : "2",
//                "roomId" : "0",
//                "status" : "0",
//                "starIcon" : "http:\/\/s3.fx.kgimg.com\/fxstatic\/images\/starLevel\/pc\/0.png",
//                "richIcon" : "http:\/\/s3.fx.kgimg.com\/fxstatic\/images\/richLevel\/pc\/2.gif",
//                "nickNameSpecial" : 1,
//                "clanId" : "0",
//                "userLogoM" : "http:\/\/s3.fx.kgimg.com\/fxusercmdavata\/system.gif",
//                "starCurValue" : "0",
//                "starCurIcon" : "http:\/\/s3.fx.kgimg.com\/fxstatic\/images\/starLevel\/pc\/0.png",
//                "starNextValue" : "2000",
//                "starNextIcon" : "http:\/\/s3.fx.kgimg.com\/fxstatic\/images\/starLevel\/pc\/1.png",
//                "starValue" : "10.00",
//                "richCurValue" : "5000",
//                "richCurIcon" : "http:\/\/s3.fx.kgimg.com\/fxstatic\/images\/richLevel\/pc\/2.gif",
//                "richNextValue" : "15000",
//                "richNextIcon" : "http:\/\/s3.fx.kgimg.com\/fxstatic\/images\/richLevel\/pc\/3.gif",
//                "richValue" : "8470.00",
//                "isSign" : false
//        },
//        "hasNewMsg" : 0,
//        "ip" : "58.254.4.121",
//        "isOpenNoviceRecharge" : 0,
//        "starData" : {
//                "userId" : "17895233",
//                    "nickName" : "ACE_VIva\u675c\u809a\u809a",
//                    "starLevel" : "34",
//                    "fansCount" : "762193",
//                    "userLogo" : "http:\/\/s3.fx.kgimg.com\/fxuserlogo\/85\/20150305\/20150305192745580927.jpg",
//                    "photoCount" : "59",
//                    "messageCount" : "928",
//                    "coinTotal" : "553070133.00",
//                    "liveTimes" : "20:00-02:00",
//                    "starIcon" : "http:\/\/s3.fx.kgimg.com\/fxstatic\/images\/starLevel\/pc\/34.png",
//                    "kugouId" : "359154768",
//                    "endTime" : "",
//                    "startTime" : "20:10",
//                    "curBean" : "553070133.00",
//                    "upNeedBean" : 26929867,
//                    "medalList" : [{
//                    "medalId" : "193",
//                        "medalName" : "\u5916\u661f\u603b\u7edf",
//                        "addTime" : "1438691218",
//                        "limitTime" : "86400",
//                        "startTime" : "1438833600",
//                        "endTime" : "1441252800",
//                        "medalStatus" : "1",
//                        "metalValue" : "0",
//                        "medalLevel" : "6",
//                        "conditionType" : "0",
//                        "conditionValueType" : "0",
//                        "relationType" : "0",
//                        "userTypeId" : "2",
//                        "addType" : "0",
//                        "affectTime" : "86400",
//                        "sumTotal" : "0",
//                        "isFinishLoop" : "1",
//                        "extContent" : "",
//                        "content" : "\u94f6\u6cb3\u7cfb\u6f2b\u6e38\u6d3b\u52a8\u671f\u95f4\uff0c\u4e00\u5929\u5185\u6536\u5230\u671b\u8fdc\u955c20\u4e2a\u3001\u5317\u6597\u661f\u7cfb1\u4e2a\uff082015\u5e748\u67086\u65e5\u4e2d\u534812:00\u81f39\u67083\u65e5\u4e2d\u534812:00\uff09",
//                        "finishedWhiteImageUrl" : "\/fxmetal\/20150804\/20150804202658664767.png",
//                        "finishedGrayImageUrl" : "",
//                        "finishedTransImageUrl" : "",
//                        "finishingWhiteImageUrl" : "\/fxmetal\/20150804\/20150804202816734985.png",
//                        "finishingGrayImageUrl" : "",
//                        "finishingTransImageUrl" : "",
//                        "finishlessWhiteImageUrl" : "\/fxmetal\/20150804\/20150804202816927307.png",
//                        "finishlessGrayImageUrl" : "",
//                        "finishlessTransImageUrl" : "",
//                        "timeType" : "0",
//                        "weekTime" : "a:0:{}",
//                        "elsestartTime" : "-28800",
//                        "elseendTime" : "-28800",
//                        "medallight" : "1"
//                }, {
//                    "medalId" : "85",
//                    "medalName" : "\u5bb6\u65cf\u5f62\u8c61\u5927\u4f7f",
//                    "addTime" : "1387617736",
//                    "limitTime" : "86400",
//                    "startTime" : "1389870000",
//                    "endTime" : "1393430100",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "4",
//                    "conditionType" : "0",
//                    "conditionValueType" : "0",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "1209600",
//                    "sumTotal" : "0",
//                    "isFinishLoop" : "1",
//                    "extContent" : "",
//                    "content" : "\u4e3e\u529e\u5bb6\u65cf\u6d3b\u52a8\u7684\u5bb6\u65cf\uff0c\u6240\u6709\u5bb6\u65cf\u4e3b\u64ad\u53ef\u4ee5\u83b7\u5f97",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140827\/20140827155425649811.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140827\/20140827155425858425.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140827\/20140827155425545390.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "0",
//                    "weekTime" : "",
//                    "elsestartTime" : "0",
//                    "elseendTime" : "0",
//                    "medallight" : "1"
//                }, {
//                    "medalId" : "24",
//                    "medalName" : "\u7f57\u66fc\u8482\u514b",
//                    "addTime" : "1366791333",
//                    "limitTime" : "86400",
//                    "startTime" : "0",
//                    "endTime" : "0",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "3",
//                    "conditionType" : "0",
//                    "conditionValueType" : "1",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "604800",
//                    "sumTotal" : "500000",
//                    "isFinishLoop" : "1",
//                    "extContent" : "",
//                    "content" : "\u5728\u4e00\u5929\u5185\u6536\u5230\u6d6a\u6f2b\u7c7b\u793c\u7269\u8fbe\u5230500000\u661f\u5e01",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150900448694.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150900420792.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150900207260.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "1",
//                    "weekTime" : "a:7:{i:0;i:0;i:1;i:1;i:2;i:2;i:3;i:3;i:4;i:4;i:5;i:5;i:6;i:6;}",
//                    "elsestartTime" : "-28800",
//                    "elseendTime" : "57599",
//                    "medallight" : "1"
//                }, {
//                    "medalId" : "28",
//                    "medalName" : "\u821e\u6797\u76df\u4e3b",
//                    "addTime" : "1366792412",
//                    "limitTime" : "86400",
//                    "startTime" : "0",
//                    "endTime" : "0",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "3",
//                    "conditionType" : "0",
//                    "conditionValueType" : "1",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "604800",
//                    "sumTotal" : "500000",
//                    "isFinishLoop" : "1",
//                    "extContent" : "",
//                    "content" : "\u5728\u4e00\u5929\u5185\u6536\u5230\u4f34\u821e\u7c7b\u793c\u7269\u8fbe\u5230500000\u661f\u5e01",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150938528791.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150938950518.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150938294103.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "1",
//                    "weekTime" : "a:7:{i:0;i:0;i:1;i:1;i:2;i:2;i:3;i:3;i:4;i:4;i:5;i:5;i:6;i:6;}",
//                    "elsestartTime" : "-28800",
//                    "elseendTime" : "57599",
//                    "medallight" : "1"
//                }, {
//                    "medalId" : "32",
//                    "medalName" : "\u6c7d\u8f66\u4eba",
//                    "addTime" : "1367581199",
//                    "limitTime" : "86400",
//                    "startTime" : "0",
//                    "endTime" : "0",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "3",
//                    "conditionType" : "0",
//                    "conditionValueType" : "1",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "604800",
//                    "sumTotal" : "500000",
//                    "isFinishLoop" : "1",
//                    "extContent" : "",
//                    "content" : "\u5728\u4e00\u5929\u5185\u6536\u5230\u8c6a\u534e\u7c7b\u793c\u7269\u8fbe\u5230500000\u661f\u5e01",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140827\/20140827151015161205.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140827\/20140827151015455703.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140827\/20140827151015480000.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "1",
//                    "weekTime" : "a:7:{i:0;i:0;i:1;i:1;i:2;i:2;i:3;i:3;i:4;i:4;i:5;i:5;i:6;i:6;}",
//                    "elsestartTime" : "-28800",
//                    "elseendTime" : "57599",
//                    "medallight" : "1"
//                }, {
//                    "medalId" : "9",
//                    "medalName" : "\u5305\u79df\u5a46",
//                    "addTime" : "1366726444",
//                    "limitTime" : "86400",
//                    "startTime" : "0",
//                    "endTime" : "0",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "2",
//                    "conditionType" : "0",
//                    "conditionValueType" : "0",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "259200",
//                    "sumTotal" : "0",
//                    "isFinishLoop" : "1",
//                    "extContent" : "",
//                    "content" : "\u5728\u4e00\u5929\u5185\u6536\u52304\u5ea7\u7231\u6566\u9601\u522b\u5885\u548c2\u5ea7\u8428\u83f2\u62c9\u675c\u62c9\u5c9b",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150754562447.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150754481170.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150754514383.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "1",
//                    "weekTime" : "a:7:{i:0;i:0;i:1;i:1;i:2;i:2;i:3;i:3;i:4;i:4;i:5;i:5;i:6;i:6;}",
//                    "elsestartTime" : "-28800",
//                    "elseendTime" : "57599",
//                    "medallight" : "1"
//                }, {
//                    "medalId" : "55",
//                    "medalName" : "\u597d\u8fd0\u8fde\u8fde",
//                    "addTime" : "1373386583",
//                    "limitTime" : "86400",
//                    "startTime" : "1410883200",
//                    "endTime" : "1410969599",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "2",
//                    "conditionType" : "0",
//                    "conditionValueType" : "1",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "604800",
//                    "sumTotal" : "30000",
//                    "isFinishLoop" : "1",
//                    "extContent" : "",
//                    "content" : "\u5728\u6bcf\u5468\u4e09\u7684\u201c\u5e78\u8fd0\u661f\u671f\u4e09\u201d\u6d3b\u52a8\u5f53\u5929\u6536\u5230\u5e78\u8fd0\u7c7b\u793c\u7269\u6ee130000\u661f\u5e01",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140827\/20140827151819420000.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140827\/20140827151819651852.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140827\/20140827151819733852.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "1",
//                    "weekTime" : "a:1:{i:0;s:1:\"3\";}",
//                    "elsestartTime" : "-28800",
//                    "elseendTime" : "57599",
//                    "medallight" : "1"
//                }, {
//                    "medalId" : "22",
//                    "medalName" : "\u6d41\u884c\u5973\u738b",
//                    "addTime" : "1366791113",
//                    "limitTime" : "86400",
//                    "startTime" : "0",
//                    "endTime" : "0",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "1",
//                    "conditionType" : "0",
//                    "conditionValueType" : "1",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "259200",
//                    "sumTotal" : "50000",
//                    "isFinishLoop" : "1",
//                    "extContent" : "",
//                    "content" : "\u5728\u4e00\u5929\u5185\u6536\u5230\u65f6\u5c1a\u7c7b\u793c\u7269\u8fbe\u523050000\u661f\u5e01",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150834972361.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150834330363.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150834688300.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "1",
//                    "weekTime" : "a:7:{i:0;s:1:\"0\";i:1;s:1:\"1\";i:2;s:1:\"2\";i:3;s:1:\"3\";i:4;s:1:\"4\";i:5;s:1:\"5\";i:6;s:1:\"6\";}",
//                    "elsestartTime" : "-28800",
//                    "elseendTime" : "57599",
//                    "medallight" : "1"
//                }, {
//                    "medalId" : "56",
//                    "medalName" : "\u98ce\u751f\u6c34\u8d77",
//                    "addTime" : "1373386682",
//                    "limitTime" : "86400",
//                    "startTime" : "1372649334",
//                    "endTime" : "1514735999",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "6",
//                    "conditionType" : "0",
//                    "conditionValueType" : "0",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "604800",
//                    "sumTotal" : "0",
//                    "isFinishLoop" : "1",
//                    "extContent" : "",
//                    "content" : "\u6536\u5230\u201d\u5e78\u8fd0\u661f\u671f\u4e09\u201d\u6d3b\u52a8\u5927\u5956 - \u5e78\u8fd0\u5929\u4f7f1\u4e2a",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140827\/20140827151837893164.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140827\/20140827151837819860.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140827\/20140827151837985338.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "0",
//                    "weekTime" : "a:0:{}",
//                    "elsestartTime" : "-28800",
//                    "elseendTime" : "-28800",
//                    "medallight" : "0"
//                }, {
//                    "medalId" : "134",
//                    "medalName" : "\u6700\u5f3a\u97f3",
//                    "addTime" : "1409729458",
//                    "limitTime" : "86400",
//                    "startTime" : "1409817302",
//                    "endTime" : "1443599702",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "6",
//                    "conditionType" : "0",
//                    "conditionValueType" : "0",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "31536000",
//                    "sumTotal" : "0",
//                    "isFinishLoop" : "1",
//                    "extContent" : "\u901a\u8fc7\u6700\u5f3a\u97f3\u8003\u6838\u7684\u6b4c\u624b\u53ef\u70b9\u4eae\u52cb\u7ae0",
//                    "content" : "\u6700\u5f3a\u97f3",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140903\/20140903153055118457.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140903\/20140903153055591592.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140903\/20140903153055667564.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "0",
//                    "weekTime" : "a:0:{}",
//                    "elsestartTime" : "-28800",
//                    "elseendTime" : "-28800",
//                    "medallight" : "0"
//                }, {
//                    "medalId" : "15",
//                    "medalName" : "\u8d22\u6e90\u6eda\u6eda",
//                    "addTime" : "1366727198",
//                    "limitTime" : "86400",
//                    "startTime" : "0",
//                    "endTime" : "0",
//                    "medalStatus" : "1",
//                    "metalValue" : "0",
//                    "medalLevel" : "3",
//                    "conditionType" : "0",
//                    "conditionValueType" : "1",
//                    "relationType" : "0",
//                    "userTypeId" : "2",
//                    "addType" : "0",
//                    "affectTime" : "604800",
//                    "sumTotal" : "500000",
//                    "isFinishLoop" : "1",
//                    "extContent" : "",
//                    "content" : "\u5728\u4e00\u5929\u5185\u6536\u5230\u8d22\u795e\u6216\u666e\u5929\u540c\u5e86\u8fbe\u5230500000\u661f\u5e01",
//                    "finishedWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150814200884.png",
//                    "finishedGrayImageUrl" : "",
//                    "finishedTransImageUrl" : "",
//                    "finishingWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150814887204.png",
//                    "finishingGrayImageUrl" : "",
//                    "finishingTransImageUrl" : "",
//                    "finishlessWhiteImageUrl" : "\/fxmetal\/20140827\/20140827150814655089.png",
//                    "finishlessGrayImageUrl" : "",
//                    "finishlessTransImageUrl" : "",
//                    "timeType" : "1",
//                    "weekTime" : "a:7:{i:0;i:0;i:1;i:1;i:2;i:2;i:3;i:3;i:4;i:4;i:5;i:5;i:6;i:6;}",
//                    "elsestartTime" : "-28800",
//                    "elseendTime" : "57599",
//                    "medallight" : "0"
//                }
//                    ],
//                    "liveTimesStart" : "20:00",
//                    "liveTimesEnd" : "02:00",
//                    "songCount" : 14,
//                    "companyName" : "",
//                    "isArt" : 0,
//                    "artGoodsCount" : 0
//            },
//            "liveData" : {
//                        "userRole" : 0,
//                            "liveName" : "fx_hifi_17895233",
//                            "liveType" : "live",
//                            "HDModel" : "hp",
//                            "recordName" : "",
//                            "recInfo" : "[{\"roomId\":\"1041235\",\"roomName\":\"\\u5f00\\u64ad\\u4e86\",\"imgPath\":\"http:\\\/\\\/p3.fx.kgimg.com\\\/v2\\\/fxroomcover\\\/T1y9ZbBTE_1RCvBVdK.jpg\",\"auditStatus\":\"0\",\"banStatus\":\"0\",\"status\":\"1\",\"type\":\"1\",\"stream\":\"fx_hifi_79716542\",\"category\":\"2\",\"userId\":\"79716542\",\"startTime\":\"7\\u79d2\\u524d\",\"endTime\":\"1439814701\",\"pauseTime\":\"1439814632\",\"enterLimitNum\":\"0\",\"stTime\":\"1439814739\",\"isHifi\":1,\"viewerNum\":921,\"nickName\":\"Ms-\\u6768\\u94b0\\u83b9\",\"starLevel\":\"10\",\"starIcon\":\"http:\\\/\\\/s3.fx.kgimg.com\\\/fxstatic\\\/images\\\/starLevel\\\/pc\\\/10.png\",\"signTime\":\"1423050523\",\"isTalentPlayer\":0,\"isStarDream\":0,\"rightbuttomIcon\":\"\",\"coinSum\":0,\"starCount\":0,\"isArt\":0,\"companyName\":\"\",\"isExclusive\":0,\"starTagsList\":[],\"isInterviewRoom\":0},{\"roomId\":\"1031569\",\"roomName\":\"\\u5f00\\u64ad\\u4e86\",\"imgPath\":\"http:\\\/\\\/p3.fx.kgimg.com\\\/v2\\\/fxroomcover\\\/T1KMDbBXZv1RCvBVdK.jpg\",\"auditStatus\":\"0\",\"banStatus\":\"0\",\"status\":\"1\",\"type\":\"1\",\"stream\":\"fx_hifi_30688131\",\"category\":\"4\",\"userId\":\"30688131\",\"startTime\":\"13\\u79d2\\u524d\",\"endTime\":\"1439807639\",\"pauseTime\":\"1439807639\",\"enterLimitNum\":\"0\",\"stTime\":\"1439814733\",\"isHifi\":1,\"viewerNum\":12,\"nickName\":\"YT-\\u82b1\\u5f71\",\"starLevel\":\"15\",\"starIcon\":\"http:\\\/\\\/s3.fx.kgimg.com\\\/fxstatic\\\/images\\\/starLevel\\\/pc\\\/15.png\",\"signTime\":\"1409142879\",\"isTalentPlayer\":0,\"isStarDream\":0,\"rightbuttomIcon\":\"\",\"coinSum\":0,\"starCount\":22,\"isArt\":0,\"companyName\":\"\",\"isExclusive\":0,\"starTagsList\":[{\"starUserId\":\"30688131\",\"starTagsId\":\"18\",\"starTagsName\":\"\\u5e7d\\u9ed8\\u641e\\u7b11\",\"addTime\":\"1410056140\"}],\"isInterviewRoom\":0},{\"roomId\":\"1046345\",\"roomName\":\"\\u5f00\\u64ad\\u4e86\",\"imgPath\":\"http:\\\/\\\/p3.fx.kgimg.com\\\/v2\\\/fxroomcover\\\/T14MxbBQhg1RCvBVdK.jpg\",\"auditStatus\":\"0\",\"banStatus\":\"0\",\"status\":\"1\",\"type\":\"1\",\"stream\":\"fx_flash_94184108\",\"category\":\"2\",\"userId\":\"94184108\",\"startTime\":\"14\\u79d2\\u524d\",\"endTime\":\"1439746541\",\"pauseTime\":\"1439746539\",\"enterLimitNum\":\"0\",\"stTime\":\"1439814732\",\"isHifi\":0,\"viewerNum\":2,\"nickName\":\"GSA\\u4e36\\u5927\\u6735\",\"starLevel\":\"7\",\"starIcon\":\"http:\\\/\\\/s3.fx.kgimg.com\\\/fxstatic\\\/images\\\/starLevel\\\/pc\\\/7.png\",\"signTime\":\"1429169283\",\"isTalentPlayer\":0,\"isStarDream\":0,\"rightbuttomIcon\":\"\",\"coinSum\":20,\"starCount\":0,\"isArt\":0,\"companyName\":\"\",\"isExclusive\":0,\"starTagsList\":[],\"isInterviewRoom\":0}]",
//                            "roomId" : "1017131",
//                            "getStarNum" : "8715624",
//                            "liveClient" : "[\"rtmp:\\\/\\\/edge.live.fanxing.com\\\/live\",\"rtmp:\\\/\\\/edge.live.fanxing.com:80\\\/live\"]",
//                            "publicMesg" : "\u5145\u503c\u70b9\u8fd9\u91cc[\u7fa431062066 [\u9a6c\u7532\u683c\u5f0fVIva\u67d0\u67d0] \u65b0\u6d6a\u5fae\u535a\u5c0f\u809a\u809aviva",
//                            "privateMesg" : "\u4f60\u7684\u52aa\u529b\uff0c\u522b\u4eba\u4e0d\u4e00\u5b9a\u653e\u5728\u773c\u91cc\uff0c\u4f60\u4e0d\u52aa\u529b\uff0c\u522b\u4eba\u4e00\u5b9a\u653e\u5728\u5fc3\u91cc\u3002",
//                            "publicMesgUrl" : "http:\/\/fanxing.kugou.com\/index.php?action=rechargeIndex&agentUserId=23754679",
//                            "background" : "#14152a url(http:\/\/s3.fanxing.com\/fxroombg\/20120830143902969459.jpg) no-repeat center top",
//                            "status" : "1",
//                            "imgPath" : "http:\/\/p3.fx.kgimg.com\/v2\/fxroomcover\/T1O9hbBvVj1RCvBVdK.jpg"
//                    },
//                    "vipLevel" : 0,
//                    "now" : "1439814776",
//                    "patrol" : 0,
//                    "giftStar" : {
//                            "userId" : "40919199",
//                                "starCount" : "10",
//                                "lastTime" : "1439814776",
//                                "getStarNum" : "0",
//                                "sendCount" : "294"
//                        },
//                        "focus" : 0,
//                        "debris" : 0,
//                        "isGoldFans" : 0,
//                        "isOpenSubscribe" : 1,
//                        "weekAwardIsShow" : 0,
//                        "isHasSubscribe" : 0,
//                        "signInNum" : 0,
//                        "specialPermit" : "[]",
//                        "mobilePhone" : "",
//                        "publicTalkLimit" : "{\"roomId\":\"1017131\",\"limitType\":0,\"limitValue\":0,\"isError\":0}",
//                        "socketConfig" : {
//                                "sokt" : "[{\"ip\":\"chat1.live.fanxing.com\",\"connet\":\"8080\",\"policy\":\"843\"},{\"ip\":\"chat2.live.fanxing.com\",\"connet\":\"8080\",\"policy\":\"843\"}]",
//                                    "enter" : "78ea361aa7cb33331c3c8f11cd29cdda",
//                                    "visiId" : 0,
//                                    "time" : "1439814776",
//                                    "ext" : "%7B%22ui%22%3A0%2C%22isSpRoom%22%3A0%2C%22stli%22%3A%7B%22st%22%3A%222%22%2C%22sl%22%3A0%7D%2C%22userGuard%22%3A%5B%5D%2C%22starCard%22%3A0%2C%22external%22%3A0%2C%22exMemo%22%3A%22%22%2C%22worship%22%3A3%2C%22z%22%3A0%2C%22isGoldFans%22%3A0%2C%22token%22%3A%22c920ae50679115ada1ef5f89b350829466b035f515c4ac4693f965e5321b41b3%22%2C%22kugouId%22%3A454395944%7D"
//                            },
//                            "isDayRecord" : 0,
//                            "punishScore" : "",
//                            "micSound" : {
//                                    "micIsSilence" : 0,
//                                        "starIsSilence" : 0
//                                },
//                                "micRoomConfig" : {
//                                        "roomId" : "1017131",
//                                            "isOpenMic" : "0",
//                                            "micTime" : "0",
//                                            "title" : "111"
//                                    },
//                                    "micStreamName" : "fx_mic_1017131",
//                                    "connectMicStreamName" : "fx_mic1_1017131"
//    },
//    "status" : 1,
//    "errorcode" : ""
//}

// ��201tcp������Ҫ�õ�����
//{
//    "cmd" : 201,
//        "roomid" : 1017131,
//        "userid" : "40919199",
//        "nickname" : "............",
//        "richlevel" : 2,
//        "ismaster" : 0,
//        "staruserid" : "17895233",
//        "key" : "78ea361aa7cb33331c3c8f11cd29cdda",
//        "keytime" : 1439814773,
//        "ext" : "
//    {
//        " ui " : 0,
//            " isSpRoom " : 0,
//            " stli " : {
//            " st " : " 2 ",
//                " sl " : 0
//        },
//        " userGuard " : [],
//        " starCard " : 0,
//        " external " : 0,
//        " exMemo " : " ",
//        " worship " : 3,
//        " z " : 0,
//        " isGoldFans " : 0,
//        " token " : " c920ae50679115ada1ef5f89b350829466b035f515c4ac4693f965e5321b41b3 ",
//        " kugouId " : 454395944
//    }"
//}
