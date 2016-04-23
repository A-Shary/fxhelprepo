#include "User.h"
#include "Room.h"
#include "CurlWrapper.h"
#include "CookiesHelper.h"
#include "EncodeHelper.h"

#include "third_party/json/json.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"

User::User()
    :curlWrapper_(new CurlWrapper)
    , cookiesHelper_(new CookiesHelper)
{
}

User::User(const std::string& username,
    const std::string& password)
    : curlWrapper_(new CurlWrapper)
    , cookiesHelper_(new CookiesHelper)
{
    username_ = username;
    password_ = password;
}


User::~User()
{
    Logout();
}

// ���ò���
void User::SetUsername(const std::string& username)
{
    username_ = username;
}

std::string User::GetUsername() const
{
    return username_;
}

void User::SetPassword(const std::string& password)
{
    password_ = password;
}

std::string User::GetPassword() const
{
    return password_;
}

void User::SetCookies(const std::vector<std::string> cookies)
{
    for (const auto& it : cookies)
    {
        cookiesHelper_->SetCookies(it);
    }
}

std::vector<std::string> User::GetCookies() const
{
    assert(false && L"��ʱ��ʵ��");
    return std::vector<std::string>();
}

//���÷���������Ϣ�ص�����,����Ľ�������Ϊ����Ҫ�������ģ�鴦��
void User::SetNormalNotify(NormalNotify normalNotify)
{
    normalNotify_ = normalNotify;
}

void User::SetNotify201(Notify201 notify201)
{
    notify201_ = notify201;
}

bool User::Login()
{
    return Login(username_, password_);
}

// ������Ϊ
bool User::Login(const std::string& username,
    const std::string& password)
{
    if (!LoginHttps(username, password))
    {
        return false;
    }
    
    if (!LoginUServiceGetMyUserDataInfo())
    {
        return false;
    }

    if (!LoginIndexServiceGetUserCenter())
    {
        return false;
    }

    return true;
}

bool User::Logout()
{
    // ��Ҫ�ϵ���������
    for (const auto& room : rooms_)
    {
        room.second->Exit();
    }
    return false;
}

bool User::EnterRoom(uint32 roomid)
{
    std::shared_ptr<Room> room(new Room(roomid));
    std::vector<std::string> keys;
    keys.push_back("_fx_coin");
    keys.push_back("_fx_user");
    keys.push_back("_fxNickName");
    keys.push_back("_fxRichLevel");
    keys.push_back("FANXING_COIN");
    keys.push_back("FANXING");
    keys.push_back("fxClientInfo");
    keys.push_back("KuGoo");
    std::string cookie = cookiesHelper_->GetCookies(keys);
    if (normalNotify_)
    {
        room->SetNormalNotify(normalNotify_);
    }
    if (notify201_)
    {
        room->SetNotify201(notify201_);
    }

    // ��������ظ��ķ��䣬�ȶϵ��ɵ�
    this->ExitRoom(roomid);

    if (!room->Enter(cookie,usertoken_,userid_))
    {
        return false;
    }
    
    rooms_[roomid] = room;
    return true;
}

bool User::ExitRoom(uint32 roomid)
{
    auto it = rooms_.find(roomid);
    if (it != rooms_.end())
    {
        it->second->Exit();
        rooms_.erase(it);
        return true;
    }
    return false;
}

bool User::ExitRooms()
{
    return false;
}

bool User::Chat(const std::string& message)
{
    return false;
}

bool User::SendStar(uint32 count)
{
    return false;
}

bool User::RetrieveStart()
{
    return false;
}

bool User::SendGift(uint32 giftid)
{
    return false;
}

bool User::GetViewerList(uint32 roomid,
    std::vector<EnterRoomUserInfo>* enterRoomUserInfo)
{
    auto room = rooms_.find(roomid);
    if (room == rooms_.end())
    {
        return false;
    }
    std::vector<std::string> keys;
    keys.push_back("KuGoo");
    keys.push_back("_fx_coin");
    keys.push_back("_fxNickName");
    keys.push_back("_fxRichLevel");
    keys.push_back("FANXING_COIN");
    keys.push_back("FANXING");
    keys.push_back("fxClientInfo");
    std::string cookies = cookiesHelper_->GetCookies(keys);
    bool result = room->second->GetViewerList(cookies, enterRoomUserInfo);
    return result;
}

bool User::KickoutUser(uint32 roomid, 
    const EnterRoomUserInfo& enterRoomUserInfo)
{
    auto room = rooms_.find(roomid);
    if (room==rooms_.end())
    {
        return false;
    }

    std::vector<std::string> keys;
    keys.push_back("KuGoo");
    keys.push_back("_fx_coin");
    keys.push_back("_fxNickName");
    keys.push_back("_fxRichLevel");
    keys.push_back("FANXING_COIN");
    keys.push_back("FANXING");
    keys.push_back("fxClientInfo");
    std::string cookies = cookiesHelper_->GetCookies(keys);

    return room->second->KickOutUser(cookies,enterRoomUserInfo);
}

bool User::SilencedUser(uint32 userid)
{
    return false;
}

bool User::LoginHttps(const std::string& username,
    const std::string& password)
{
    const char* loginuserurl = "https://login-user.kugou.com/v1/login/";
    HttpRequest request;
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.url = loginuserurl;
    request.referer = "http://www.fanxing.kugou.com";
    request.cookies = "";
    auto& queries = request.queries;
    queries["appid"] = "1010";
    queries["username"] = UrlEncode(username);
    queries["pwd"] = MakeMd5FromString(password);;
    queries["code"] = "";
    queries["clienttime"] = base::UintToString(
        static_cast<uint32>(base::Time::Now().ToDoubleT()));
    queries["expire_day"] = "3";
    queries["autologin"] = "false";
    queries["redirect_uri"] = "";
    queries["state"] = "";
    queries["callback"] = "loginSuccessCallback";
    queries["login_ver"] = "1";

    HttpResponse response;
    if (!curlWrapper_->Execute(request, &response))
    {
        return false;
    }
 //   loginSuccessCallback({
	//"pic" : "http:\/\/imge.kugou.com\/kugouicon\/165\/20100101\/20100101192931478054.jpg",
	//"nickname" : "fanxingtest001",
	//"username" : "fanxingtest001",
	//"token" : "20c7dd5dbfdea2e25846e666f820d64ae1ad3afdb8a0a7652a447fcc392d1e46",
	//"userid" : 641607819
 //   })

    std::string responsedata;
    responsedata.assign(response.content.begin(), response.content.end());
    if (responsedata.empty())
        return  false;
    std::string header = "loginSuccessCallback(";
    responsedata = responsedata.substr(header.length(), 
        responsedata.length() - header.length() - 2);
    //std::string beginmark = R"("token":")";
    //auto beginpos = responsedata.find(beginmark);
    //if (beginpos == std::string::npos)
    //    return false;
    //beginpos += beginmark.size();
    //auto endpos = responsedata.find("\"",beginpos);
    //usertoken_ = responsedata.substr(beginpos, endpos - beginpos);

    Json::Reader reader;
    Json::Value logindata(Json::objectValue);
    if (!reader.parse(responsedata, logindata, false))
    {
        return false;
    }

    // ��ʱû�б�Ҫ���status��ֵ
    Json::Value jvCmd(Json::ValueType::intValue);
    usertoken_ = logindata.get("token", "").asString();
    userid_ = logindata.get("userid",0).asUInt();

    for (const auto& it : response.cookies)
    {
        cookiesHelper_->SetCookies(it);
    }

    // response.content�����ﲻ�ô���
    return true;
}

bool User::LoginUServiceGetMyUserDataInfo()
{
    const char* GetMyUserDataInfoUrl = "http://fanxing.kugou.com/UServices/UserService/UserService/getMyUserDataInfo";
    HttpRequest request;
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.url = GetMyUserDataInfoUrl;
    request.referer = "http://www.fanxing.kugou.com";
    request.cookies = cookiesHelper_->GetCookies("KuGoo");;
    auto& queries = request.queries;
    queries["args"] = "[]";
    queries["_"] = GetNowTimeString();

    HttpResponse response;
    if (!curlWrapper_->Execute(request, &response))
    {
        return false;
    }

    // ������Ϊ��ȡ��cookies
    for (const auto& it : response.cookies)
    {
        cookiesHelper_->SetCookies(it);
    }

    // response.content��ʵҲ����һЩ�û���Ϣ���������ﲻ�ô���
    return true;
}

bool User::LoginIndexServiceGetUserCenter()
{
    const char* GetMyUserDataInfoUrl = "http://fanxing.kugou.com/Services/IndexService/IndexService/getUserCenter";
    HttpRequest request;
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.url = GetMyUserDataInfoUrl;
    request.referer = "http://www.fanxing.kugou.com";
    request.cookies = cookiesHelper_->GetCookies("KuGoo");;
    auto& queries = request.queries;
    queries["args"] = "[%22%22]";
    queries["_"] = GetNowTimeString();

    HttpResponse response;
    if (!curlWrapper_->Execute(request, &response))
    {
        return false;
    }

    // ������Ϊ��ȡ��cookies
    for (const auto& it : response.cookies)
    {
        cookiesHelper_->SetCookies(it);
    }

    // response.content�����ﲻ�ô���
    return true;
}
