#pragma once
#include <string>
#include <vector>
#include <memory>
#include <map>

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/threading/thread.h"
#include "Network/MessageNotifyManager.h"

class User;
class CurlWrapper;

struct FollowUserInfo
{
    uint32 user_id;
    std::string nickname;
    std::string last_login;
};

class UserTrackerHelper
{
public:
    UserTrackerHelper();
    ~UserTrackerHelper();

    bool Initialize();// �����߳�
    void Finalize();// �����߳�

    void Test();

    void SetNotifyMessageCallback(const base::Callback<void(const std::wstring&)> callback);

    void CancelCurrentOperation();

    bool LoginUser(const std::string& user_name, const std::string& password);

    // ǿ�Ƹ���ȫվ���������û��б�����
    bool UpdataAllStarRoomUserMap();

    // ��ʹ�û������ݣ��߸��»���߲����û���ֻҪ�ҵ���ֹͣ����������������
    bool UpdateForFindUser(const std::vector<uint32> user_ids);

    // ���Ѿ����ºõ�ȫվ�����в����û���Ϣ
    bool GetUserLocationByUserId(const std::vector<uint32> user_ids);

    

private:

    void DoLoginUser(const std::string& user_name, const std::string& password);

    // ǿ�Ƹ���ȫվ���������û��б�����
    void DoUpdataAllStarRoomUserMap();

    // ��ʹ�û������ݣ��߸��»���߲����û���ֻҪ�ҵ���ֹͣ����������������
    void DoUpdateForFindUser(const std::vector<uint32> user_ids);

    // ���Ѿ����ºõ�ȫվ�����в����û���Ϣ
    void DoGetUserLocationByUserId(const std::vector<uint32> user_ids);

    bool GetAllStarRoomInfos(std::vector<uint32>* roomids); // ��ȫվ��ȡ��������ֱ��������ֱ��������Ϣ��Ϊ��һ����ȡ�����û��б���׼��
    bool GetTargetStarRoomInfos(const std::string& url, std::vector<uint32>* roomids);

    // ǿ�Ƹ��·����û���Ϣ��roomid_user_map�������з��������û���Ϣ
    bool GetAllRoomViewers(const std::vector<uint32>& roomids,
        std::map<uint32, std::map<uint32, EnterRoomUserInfo>>* roomid_user_map);

    // �����û��߸��·����û���Ϣ��ֻҪ�����û����Ͳ������ˣ�roomid_user_mapֻ���ز��ҹ��ķ��������û���Ϣ
    bool FindUsersWhenRoomViewerList(const std::vector<uint32>& roomids,
        std::map<uint32, std::map<uint32, EnterRoomUserInfo>>* roomid_user_map,
        const std::vector<uint32>& user_ids,
        std::map<uint32, uint32>* user_room_map);

    bool GetRoomViewerList(uint32 roomid, std::map<uint32, EnterRoomUserInfo>* user_map); // ��ָ�������ȡ�û��б�׷���û�
    
    //GET /UServices/UserService/UserExtService/getFollowList?args=[1,10,0,%22%22,0,3]&_=1478426799924
    //Referer: http://fanxing.kugou.com/index.php?action=userFollowList
    bool GetMyConcernList(std::vector<FollowUserInfo>* follow_user_infos); // ��ȡ��¼�û��Ĺ�ע�б�

    bool GetUserInfoByUserId(); // ��ȡ�û�����ҳ����Ϣ, �����ж����Ƿ�Ϊ����, ���ǿ�ѡ�ĸ�������

    bool GetUserConcernList(); // ��ȡ�û���ע��������Ϣ, ���ǿ�ѡ�ĸ�������

    scoped_ptr<base::Thread> worker_thread_;
    std::unique_ptr<User> user_;
    std::unique_ptr<CurlWrapper> curl_wrapper_;

    std::map<uint32, std::map<uint32, EnterRoomUserInfo>> roomid_userid_map_;

    base::Callback<void(const std::wstring&)> message_callback_;

    bool cancel_flag_ = false;
};

