#include "User.h"
#include "Room.h"
#include "CurlWrapper.h"
#include "CookiesHelper.h"
#include "EncodeHelper.h"

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
    for (auto& it : rooms_)
    {
        it->Exit();
    }
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
void User::SetRoomNotify()
{

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
    if (!room->Enter(cookie))
    {
        return false;
    }
    rooms_.push_back(room);
    return true;
}

bool User::ExitRoom()
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

bool User::KickoutUser(uint32 userid)
{
    return false;
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
