#include "stdafx.h"

#include "UserTrackerHelper.h"
#include "Network/User.h"
#include "Network/CurlWrapper.h"
#include "Network/EncodeHelper.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"
#include "third_party/chromium/base/bind.h"

UserTrackerHelper::UserTrackerHelper()
    :curl_wrapper_(new CurlWrapper)
    , worker_thread_(new base::Thread("UserTrackerHelper"))
    , user_(nullptr) //�����ȵ�¼���ò���
{
}


UserTrackerHelper::~UserTrackerHelper()
{
}

bool UserTrackerHelper::Initialize()
{
    CurlWrapper::CurlInit();
    worker_thread_->Start();
    return true;
}

void UserTrackerHelper::Finalize()
{
    worker_thread_->Stop();
    CurlWrapper::CurlCleanup();
}

void UserTrackerHelper::Test()
{
}

void UserTrackerHelper::SetNotifyMessageCallback(
    const base::Callback<void(const std::wstring&)> callback)
{
    message_callback_ = callback;
}

void UserTrackerHelper::CancelCurrentOperation()
{
    cancel_flag_ = true;
}

bool UserTrackerHelper::LoginGetVerifyCode(std::vector<uint8>* picture)
{
    return user_->LoginGetVerifyCode(picture);
}

bool UserTrackerHelper::LoginUser(
    const std::string& user_name, const std::string& password,
    const std::string& verifycode,
    const base::Callback<void(bool, const std::string&)>& callback)
{
    return worker_thread_->message_loop_proxy()->PostTask(FROM_HERE,
        base::Bind(&UserTrackerHelper::DoLoginUser,
        base::Unretained(this), user_name, password, verifycode, callback));
}

void UserTrackerHelper::DoLoginUser(const std::string& user_name, 
    const std::string& password, const std::string& verifycode,
    const base::Callback<void(bool, const std::string&)>& callback)
{
    if (!user_)
        user_.reset(new User);

    std::wstring msg;
    std::string error_msg;
    if (!user_->Login(user_name, password, verifycode, &error_msg))
    {
        msg = L"��¼ʧ��";
        message_callback_.Run(msg);
        callback.Run(false, error_msg);
        return;
    }
    msg = L"��¼�ɹ�";
    message_callback_.Run(msg);
    callback.Run(true, error_msg);
    return;
}

bool UserTrackerHelper::UpdataAllStarRoomUserMap(
    const base::Callback<void(uint32, uint32)>& callback)
{
    if (!user_)
        return false;

    worker_thread_->message_loop_proxy()->PostTask(FROM_HERE,
        base::Bind(&UserTrackerHelper::DoUpdataAllStarRoomUserMap,
        base::Unretained(this), callback));
    return true;
}
void UserTrackerHelper::DoUpdataAllStarRoomUserMap(
    const base::Callback<void(uint32, uint32)>& callback)
{
    std::vector<uint32> roomids;
    std::wstring msg;
    if (!GetAllStarRoomInfos(&roomids))
    {
        msg = L"��ȡ�����б�ʧ��";
        message_callback_.Run(msg);
        return ;
    }
    msg = L"��ȡ�����б�ɹ�";
    message_callback_.Run(msg);
    std::map<uint32, std::map<uint32, EnterRoomUserInfo>> roomid_userid_map;
    if (!GetAllRoomViewers(roomids, &roomid_userid_map, callback))
    {
        msg = L"��ȡ���з�������б�ʧ��";
        message_callback_.Run(msg);
        return ;
    }
    msg = L"��ȡ���з�������б�ɹ�";
    message_callback_.Run(msg);
    roomid_userid_map_ = roomid_userid_map;
    return ;
}

bool UserTrackerHelper::UpdateForFindUser(
    const std::vector<uint32> user_ids,
    const base::Callback<void(uint32, uint32)>& progress_callback,
    const base::Callback<void(uint32, uint32)>& result_callback)
{
    if (!user_)
        return false;

    worker_thread_->message_loop_proxy()->PostTask(FROM_HERE,
        base::Bind(&UserTrackerHelper::DoUpdateForFindUser,
        base::Unretained(this), user_ids, progress_callback, result_callback));
    return true;
}

// ��ʹ�û������ݣ��߸��»���߲����û���ֻҪ�ҵ���ֹͣ����������������
void UserTrackerHelper::DoUpdateForFindUser(
    const std::vector<uint32> user_ids,
    const base::Callback<void(uint32, uint32)>& progress_callback,
    const base::Callback<void(uint32, uint32)>& result_callback)
{
    std::wstring msg = L"���²������û���ʼ";
    message_callback_.Run(msg);
    std::vector<uint32> roomids;
    if (!GetAllStarRoomInfos(&roomids))
    {
        msg = L"��ȡ�����б�ʧ��";
        message_callback_.Run(msg);
        return;
    }
    msg = L"��ȡ�����б�ɹ�";
    message_callback_.Run(msg);
    std::map<uint32, std::map<uint32, EnterRoomUserInfo>> roomid_userid_map;
    std::map<uint32, uint32> user_room_map;
    FindUsersWhenUpdateRoomViewerList(roomids, &roomid_userid_map, 
        user_ids, &user_room_map, progress_callback, result_callback);

    for (auto user_room : user_room_map)
    {
        msg = L"�û�[" + base::UintToString16(user_room.first) + L"]";
        msg += L"�ڷ���[" + base::UintToString16(user_room.second) + L"]";
        message_callback_.Run(msg);
    }

    // ɨ�˶��ٸ��¶��ٵ�Ŀǰ��������
    for (auto temp : roomid_userid_map)
    {
        roomid_userid_map_[temp.first] = temp.second;
    }
    msg = L"���·�����[" + base::UintToString16(roomid_userid_map.size()) + L"]";
    message_callback_.Run(msg);
    msg = L"���²������û�����";
    message_callback_.Run(msg);
    return;
}

bool UserTrackerHelper::GetUserLocationByUserId(
    const std::vector<uint32> user_ids,
    const base::Callback<void(uint32, uint32)>& progress_callback,
    const base::Callback<void(uint32, uint32)>& result_callback)
{
    if (!user_)
        return false;

    worker_thread_->message_loop_proxy()->PostTask(FROM_HERE,
        base::Bind(&UserTrackerHelper::DoGetUserLocationByUserId,
        base::Unretained(this), user_ids, progress_callback, result_callback));
    return true;
}

void UserTrackerHelper::DoGetUserLocationByUserId(
    const std::vector<uint32> user_ids,
    const base::Callback<void(uint32, uint32)>& progress_callback,
    const base::Callback<void(uint32, uint32)>& result_callback)
{
    if (roomid_userid_map_.empty())
        return;

    std::wstring msg = L"�ڻ����в����û���ʼ";
    message_callback_.Run(msg);
    std::map<uint32, uint32> user_room_map;
    uint32 all = roomid_userid_map_.size();
    uint32 current = 0;
    for (const auto &room : roomid_userid_map_)
    {
        current++;
        for (const auto& user_id : user_ids)
        {
            auto it = room.second.find(user_id);
            if (it != room.second.end())
            {
                user_room_map[user_id] = room.first;
                // ����©��һ���û��ڶ����������
                msg = L"�û�[" + base::UintToString16(user_id) + L"]";
                msg += L"�ڷ���[" + base::UintToString16(room.first) + L"]";
                message_callback_.Run(msg);
                result_callback.Run(user_id, room.first);
            }
        }
        progress_callback.Run(current, all);
    }

    msg = L"�ڻ����в����û�����";
    message_callback_.Run(msg);
    return;
}

bool UserTrackerHelper::GetAllStarRoomInfos(std::vector<uint32>* roomids)
{
    std::string url1 = "http://visitor.fanxing.kugou.com/VServices/IndexService.IndexService.getLiveList/1-1-1/";
    std::string url2 = "http://visitor.fanxing.kugou.com/VServices/IndexService.IndexService.getLiveList/1-2-1/";
    std::string url3 = "http://visitor.fanxing.kugou.com/VServices/IndexService.IndexService.getLiveList/1-3-1/";
    std::string url4 = "http://visitor.fanxing.kugou.com/VServices/IndexService.IndexService.getLiveList/1-4-1/";

    std::vector<uint32> roomid1;
    std::vector<uint32> roomid2;
    std::vector<uint32> roomid3;
    std::vector<uint32> roomid4;

    std::wstring msg = L"���ڻ�ȡ�����б� ...";
    message_callback_.Run(msg);
    GetTargetStarRoomInfos(url1, &roomid1);
    message_callback_.Run(msg);
    GetTargetStarRoomInfos(url2, &roomid2);
    message_callback_.Run(msg);
    GetTargetStarRoomInfos(url3, &roomid3);
    message_callback_.Run(msg);
    GetTargetStarRoomInfos(url4, &roomid4);

    roomids->insert(roomids->end(), roomid1.begin(), roomid1.end());
    roomids->insert(roomids->end(), roomid2.begin(), roomid2.end());
    roomids->insert(roomids->end(), roomid3.begin(), roomid3.end());
    roomids->insert(roomids->end(), roomid4.begin(), roomid4.end());

    return true;
}

bool UserTrackerHelper::GetTargetStarRoomInfos(const std::string& url, std::vector<uint32>* roomids)
{
    // /VServices/IndexService.IndexService.getLiveList/1-3-1/
    HttpRequest request;
    request.url = url;
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.referer = "http://fanxing.kugou.com/";
    request.cookies = user_->GetCookies();

    HttpResponse response;
    if (!curl_wrapper_->Execute(request, &response))
    {
        return false;
    }

    std::string responsedata(response.content.begin(), response.content.end());
    std::string jsondata = PickJson(responsedata);

    Json::Reader reader;
    Json::Value rootdata(Json::objectValue);
    if (!reader.parse(jsondata, rootdata, false))
    {
        assert(false);
        return false;
    }

    uint32 unixtime = rootdata.get("servertime", 1461378689).asUInt();

    // ��ʱֻ����2016��11��30��ǰ��ʹ�ã���ֹ��й
    if (unixtime > 1480435200)
    {
        return false;
    }

    uint32 status = rootdata.get("status", 0).asUInt();
    if (status != 1)
    {
        assert(false);
        return false;
    }
    Json::Value jvdata(Json::ValueType::objectValue);
    Json::Value data = rootdata.get(std::string("data"), jvdata);
    if (data.isNull() || !data.isArray())
    {
        assert(false);
        return false;
    }

    for (auto roominfo : data)
    {
        uint32 roomId = GetInt32FromJsonValue(roominfo, "roomId");
        roomids->push_back(roomId);
    }
    return true;
}

bool UserTrackerHelper::GetAllRoomViewers(
    const std::vector<uint32>& roomids,
    std::map<uint32, std::map<uint32, EnterRoomUserInfo>>* roomid_user_map,
    const base::Callback<void(uint32, uint32)>& callback)
{
    if (!roomid_user_map)
        return false;

    cancel_flag_ = false;
    std::wstring msg;
    uint32 all = roomids.size();
    uint32 current = 0;

    for (auto roomid : roomids)
    {
        current++;
        if (cancel_flag_)
        {
            msg = L"�û�ȡ����ǰ����";
            message_callback_.Run(msg);
            cancel_flag_ = false;
            return true;
        }

        msg = L"��ʼ��ȡ����[" + base::UintToString16(roomid) + L"] ����";
        message_callback_.Run(msg);
        std::map<uint32, EnterRoomUserInfo> roomid_users;
        if (!GetRoomViewerList(roomid, &roomid_users))
        {
            //assert(false); ������pk����Ҳ����ֽ����ͻ�ȡ�����Աʧ��
            msg = L"��ȡ����[" + base::UintToString16(roomid) + L"] ����ʧ��";
            message_callback_.Run(msg);
            callback.Run(current, all);
            continue;
        }
        msg = L"��ȡ����[" + base::UintToString16(roomid) + L"] ���ڳɹ�";
        message_callback_.Run(msg);
        callback.Run(current, all);
        (*roomid_user_map)[roomid] = roomid_users;
    }

    return !roomid_user_map->empty();
}

bool UserTrackerHelper::FindUsersWhenUpdateRoomViewerList(
    const std::vector<uint32>& roomids,
    std::map<uint32, std::map<uint32, EnterRoomUserInfo>>* roomid_user_map,
    const std::vector<uint32>& user_ids,
    std::map<uint32, uint32>* user_room_map,
    const base::Callback<void(uint32, uint32)>& progress_callback,
    const base::Callback<void(uint32, uint32)>& result_callback)
{
    cancel_flag_ = false;
    std::wstring msg;
    std::vector<uint32> temp_user_ids = user_ids;
    uint32 all = roomids.size();
    uint32 current = 0;
    for (auto roomid : roomids)
    {
        current++;
        if (cancel_flag_)
        {
            msg = L"�û�ȡ����ǰ����";
            message_callback_.Run(msg);
            cancel_flag_ = false;
            return true;
        }

        msg = L"��ȡ����[" + base::UintToString16(roomid) + L"] ���ڿ�ʼ";
        message_callback_.Run(msg);
        std::map<uint32, EnterRoomUserInfo> roomid_users;
        if (!GetRoomViewerList(roomid, &roomid_users))
        {
            //assert(false); ������pk����Ҳ����ֽ����ͻ�ȡ�����Աʧ��
            msg = L"��ȡ����[" + base::UintToString16(roomid) + L"] ����ʧ��";
            message_callback_.Run(msg);
            progress_callback.Run(current, all);
            continue;
        }
        msg = L"��ȡ����[" + base::UintToString16(roomid) + L"] ���ڳɹ�";
        message_callback_.Run(msg);
        progress_callback.Run(current, all);

        (*roomid_user_map)[roomid] = roomid_users;

        for (auto user_it = temp_user_ids.begin();
            user_it != temp_user_ids.end(); user_it++)
        {
            if (roomid_users.end() != roomid_users.find(*user_it))
            {
                (*user_room_map)[*user_it] = roomid;
                msg = L"�ڷ���[" + base::UintToString16(roomid) + 
                    L"] �ҵ�Ŀ�� [" + base::UintToString16(*user_it) + L"]";
                message_callback_.Run(msg);
                result_callback.Run(*user_it, roomid);
                temp_user_ids.erase(user_it);
                break;
            }
        }

        if (temp_user_ids.empty())
        {
            return true; // ȫ���ҵ���
        } 
    }
    return false; // û���ҵ�ȫ��
}

bool UserTrackerHelper::GetRoomViewerList(uint32 roomid, std::map<uint32, EnterRoomUserInfo>* user_map)
{
    std::vector<EnterRoomUserInfo> enterRoomUserInfoList;
    if (!user_->OpenRoomAndGetViewerList(roomid, &enterRoomUserInfoList))
    {
        return false;
    }
    
    for (auto user_info : enterRoomUserInfoList)
    {
        (*user_map)[user_info.userid] = user_info;
    }
    return true;
}

//GET /UServices/UserService/UserExtService/getFollowList?args=[1,10,0,%22%22,0,3]&_=1478426799924
//Referer: http://fanxing.kugou.com/index.php?action=userFollowList
bool UserTrackerHelper::GetMyConcernList(std::vector<FollowUserInfo>* follow_user_infos)
{
    HttpRequest request;
    request.url = "http://fanxing.kugou.com/UServices/UserService/UserExtService/getFollowList";
    request.queries["args"] = "[1,10,0,%22%22,0,3]";
    request.queries["_"] = GetNowTimeString();
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.referer = "http://fanxing.kugou.com/index.php?action=userFollowList";
    request.cookies = user_->GetCookies();

    HttpResponse response;
    if (!curl_wrapper_->Execute(request, &response))
    {
        return false;
    }

    return true;
}

bool UserTrackerHelper::GetUserInfoByUserId()
{
    return false;
}


bool UserTrackerHelper::GetUserConcernList()
{
    return false;
}




