#include "Room.h"
#include "CurlWrapper.h"
#include "FanXingUI/GiftInfoHelper.h"
#include "GiftNotifyManager.h"
#include "CookiesHelper.h"
#include "EncodeHelper.h"

#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/json/json.h"

Room::Room(uint32 roomid)
    :roomid_(roomid)
    , curlWrapper_(new CurlWrapper)
    , giftNotifyManager_(new GiftNotifyManager)
    , cookiesHelper_(new CookiesHelper)
{
    giftNotifyManager_->Initialize();
}


Room::~Room()
{
    giftNotifyManager_->Finalize();
}

bool Room::Enter(const std::string& cookies, const std::string& usertoken, uint32 userid)
{
    //uint32 userid = 0;
    //std::string nickname = "";
    //uint32 richlevel = 0;
    //uint32 ismaster = 0;
    //uint32 singerid = 0;
    //std::string key = "";
    //std::string ext = "";

    if (!OpenRoom(cookies))
    {
        return false;
    }

    //if (!GetCurrentUserInfo(cookies, &userid, &nickname, &richlevel))
    //{
    //    return false;
    //}

    if (!EnterRoom(cookies, userid, usertoken))
    {
        return false;
    }
    
    if (!ConnectToNotifyServer_(roomid_, userid, usertoken))
    {
        return false;
    }
    return true;
}

bool Room::Exit()
{
    giftNotifyManager_->Finalize();
    return true;
}

bool Room::GetViewerList(const std::string& cookies,
    std::vector<EnterRoomUserInfo>* enterRoomUserInfoList)
{
    std::string url = "http://visitor.fanxing.kugou.com";
    url += "/VServices/RoomService.RoomService.getViewerList/";
    url += base::UintToString(roomid_) +"-" + base::UintToString(singerid_);
    HttpRequest request;
    request.url = url;
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.referer = std::string("http://fanxing.kugou.com/") +
        base::UintToString(roomid_);
    request.cookies = cookies;

    HttpResponse response;
    if (!curlWrapper_->Execute(request, &response))
    {
        return false;
    }

    if (response.content.empty())
    {
        assert(false);
        return false;
    }

    std::string responsedata(response.content.begin(), response.content.end());
    auto beginpos = responsedata.find("(");
    auto endpos = responsedata.rfind(")");
    if (beginpos == std::string::npos || endpos == std::string::npos)
    {
        assert(false);
        return false;
    }

    responsedata = responsedata.substr(beginpos + 1, endpos - beginpos - 1);

    Json::Reader reader;
    Json::Value rootdata(Json::objectValue);
    if (!reader.parse(responsedata, rootdata, false))
    {
        assert(false);
        return false;
    }

    uint32 unixtime = rootdata.get("servertime", 1461378689).asUInt();
    uint32 status = rootdata.get("status", 0).asUInt();
    if (status != 1)
    {
        assert(false);
        return false;
    }
    Json::Value jvdata(Json::ValueType::objectValue);
    Json::Value data = rootdata.get(std::string("data"), jvdata);
    if (data.isNull())
    {
        assert(false);
        return false;
    }
    
    Json::Value jvlist(Json::ValueType::objectValue);
    Json::Value list = data.get(std::string("list"), jvlist);
    if (!list.isArray())
    {
        assert(false);
        return false;
    }

    for (const auto& item : list)
    {
        EnterRoomUserInfo enterRoomUserInfo;
        enterRoomUserInfo.nickname = item.get("nickname", "").asString();
		enterRoomUserInfo.richlevel = GetInt32FromJsonValue(item, "richlevel");
		enterRoomUserInfo.userid = GetInt32FromJsonValue(item, "userid");
        enterRoomUserInfo.unixtime = unixtime;
        enterRoomUserInfoList->push_back(enterRoomUserInfo);
    }

    return true;
}

bool Room::KickOutUser(const std::string&cookies,
    const EnterRoomUserInfo& enterRoomUserInfo)
{
    std::string strroomid = base::IntToString(static_cast<int>(enterRoomUserInfo.roomid));
    std::string url = std::string("http://fanxing.kugou.com");
    url += "/Services.php?act=RoomService.RoomManageService&mtd=kickOut&d=";
    url += GetNowTimeString();
    url += R"(&args=)";
    std::string jsonstr;
    jsonstr += std::string(R"([")");
    jsonstr += base::UintToString(singerid_);
    jsonstr += R"(",")";
    jsonstr += base::UintToString(enterRoomUserInfo.userid);
    jsonstr += R"(",")";
    jsonstr += base::UintToString(enterRoomUserInfo.roomid);
    jsonstr += R"(",3600,")";
    jsonstr += enterRoomUserInfo.nickname;
    jsonstr += R"(",0])"; // ͳһ����һСʱ����
    jsonstr = UrlEncode(jsonstr);

    url += jsonstr;

    HttpRequest request;
    request.url = url;
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.referer = std::string("http://fanxing.kugou.com/") +
        base::UintToString(roomid_);
    request.cookies = cookies;

    HttpResponse response;
    if (!curlWrapper_->Execute(request, &response))
    {
        return false;
    }

    return true;
}

void Room::SetNormalNotify(NormalNotify normalNotify)
{
    giftNotifyManager_->SetNormalNotify(normalNotify);
}

void Room::SetNotify201(Notify201 notify201)
{
    giftNotifyManager_->SetNotify201(notify201);
}

bool Room::OpenRoom(const std::string& cookies)
{
    HttpRequest request;
    request.url = std::string("http://fanxing.kugou.com/") +
        base::UintToString(roomid_);
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.referer = "http://fanxing.kugou.com/";
    request.cookies = cookies;

    HttpResponse response;
    if (!curlWrapper_->Execute(request, &response))
    {
        return false;
    }

    std::string content;
    content.assign(response.content.begin(), response.content.end());

    for (const auto& it : response.cookies)
    {
        assert(false && L"����Ӧ���ǲ�������cookie��");
        cookiesHelper_->SetCookies(it);
    }

    // �򿪷���Ĺ��ܲ���Ҫ����������ҳ������
    if (content.empty())
    {
        return false;
    }
    std::string isClanRoomMark = "isClanRoom";
    std::string starId = "starId";
    auto isClanRoomPos = content.find(isClanRoomMark);
    if (isClanRoomPos == std::string::npos)
    {
        return false;
    }
    auto starPos = content.find(starId, isClanRoomPos + isClanRoomMark.length());

    auto beginPos = content.find("\"", starPos);
    beginPos += 1;
    auto endPos = content.find("\"", beginPos);

    std::string singerid = content.substr(beginPos, endPos - beginPos);
    base::StringToUint(singerid, &singerid_);
    return true;
}

// ��Ҫ��д
bool Room::EnterRoom(const std::string& cookies, uint32 userid, const std::string& usertoken)
{
    HttpRequest request;
    request.url = "http://fanxing.kugou.com/UServices/RoomService/RoomService/tryEnter";
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.referer = "http://fanxing.kugou.com/" + base::IntToString(roomid_);
    request.queries["args"] = "[%22" + 
        base::IntToString(static_cast<int>(roomid_))+
        "%22,%22%22,%22%22,%22web%22]";
    request.cookies = cookies;

    HttpResponse response;
    if (!curlWrapper_->Execute(request, &response))
    {
        return false;
    }

    std::string content;
    content.assign(response.content.begin(), response.content.end());

    for (const auto& it : response.cookies)
    {
        cookiesHelper_->SetCookies(it);
    }

    if (content.empty())
    {
        return false;
    }

    const std::string& data = content;
    //����json����
    Json::Reader reader;
    Json::Value rootdata(Json::objectValue);
    if (!reader.parse(data, rootdata, false))
    {
        return false;
    }

	uint32 status = GetInt32FromJsonValue(rootdata, "status");
    if (status != 1)
    {
        return false;
    }
    return true;
}

//bool Room::GetCurrentUserInfo(const std::string& cookies,
//    uint32* userid, std::string* nickname, uint32* richlevel)
//{
//    HttpRequest request;
//    request.url = std::string("http://fanxing.kugou.com/Services.php");
//    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
//    request.referer = "http://fanxing.kugou.com/" + base::IntToString(roomid_);;
//    request.queries["d"] = GetNowTimeString();
//    request.queries["act"] = "UserService.UserService";
//    request.queries["mtd"] = "getCurrentUserInfo";
//    request.queries["args"] = "%5B%5D";
//    request.queries["test"] = "3";
//    request.cookies = cookies;
//
//    HttpResponse response;
//    if (!curlWrapper_->Execute(request, &response))
//    {
//        return false;
//    }
//
//    std::string contents(response.content.begin(), response.content.end());
//
//    if (contents.empty())
//    {
//        return false;
//    }
//
//    const std::string& data = contents;
//    //����json����
//    Json::Reader reader;
//    Json::Value rootdata(Json::objectValue);
//    if (!reader.parse(data, rootdata, false))
//    {
//        return false;
//    }
//
//    // �б�Ҫ���status��ֵ
//    uint32 status = rootdata.get(std::string("status"), 0).asInt();
//    if (status == 0)
//    {
//        return false;
//    }
//
//    Json::Value dataObject(Json::objectValue);
//    dataObject = rootdata.get(std::string("data"), dataObject);
//    if (dataObject.empty())
//    {
//        return false;
//    }
//
//    Json::Value fxUserInfoObject(Json::objectValue);
//    fxUserInfoObject = dataObject.get("fxUserInfo", fxUserInfoObject);
//    if (fxUserInfoObject.empty())
//    {
//        return false;
//    }
//
//    std::string struserid = fxUserInfoObject["userId"].asString();
//    if (!base::StringToUint(struserid, userid))
//    {
//        return false;
//    }
//
//    *nickname = fxUserInfoObject["nickName"].asString();
//    std::string strrichLevel = fxUserInfoObject["richLevel"].asString();
//    if (!base::StringToUint(strrichLevel, richlevel))
//    {
//        return false;
//    }
//    return true;
//}

bool Room::ConnectToNotifyServer_(uint32 roomid, uint32 userid,
    const std::string& usertoken)
{
    bool ret = false;
    ret = giftNotifyManager_->Connect843();
    assert(ret);

    // ���û��汾����Ҫ������Ϣ�ص�
    //giftNotifyManager_->SetNotify201(
    //    std::bind(&NetworkHelper::NotifyCallback201,
    //    this, std::placeholders::_1));

    //giftNotifyManager_->SetNotify601(
    //    std::bind(&NetworkHelper::NotifyCallback601,
    //    this, roomid, staruserid, std::placeholders::_1));

    //giftNotifyManager_->SetNormalNotify(
    //    std::bind(&NetworkHelper::NotifyCallback,
    //    this, std::placeholders::_1));

    ret = giftNotifyManager_->Connect8080(roomid, userid, usertoken);
    return ret;
}
