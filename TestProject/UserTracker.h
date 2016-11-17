#pragma once
#include <string>
#include <vector>
#include <memory>

#include "third_party/chromium/base/basictypes.h"
#include "Network/User.h"

class User;
class CurlWrapper;

struct FollowUserInfo
{
    uint32 user_id;
    std::string nickname;
    std::string last_login;
};
class UserTracker
{
public:
    UserTracker();
    ~UserTracker();

    void Test();

    bool LoginUser(const std::string& user_name, const std::string& password);

    bool GetUserLocationByUserId(uint32 user_id, uint32 room_id);

private:
    //GET /UServices/UserService/UserExtService/getFollowList?args=[1,10,0,%22%22,0,3]&_=1478426799924
    //Referer: http://fanxing.kugou.com/index.php?action=userFollowList
    bool GetMyConcernList(std::vector<FollowUserInfo>* follow_user_infos); // ��ȡ��¼�û��Ĺ�ע�б�

    bool GetUserInfoByUserId(); // ��ȡ�û�����ҳ����Ϣ, �����ж����Ƿ�Ϊ����, ���ǿ�ѡ�ĸ�������

    bool GetUserConcernList(); // ��ȡ�û���ע��������Ϣ, ���ǿ�ѡ�ĸ�������

    bool GetAllStarRoomInfos(std::vector<uint32>* roomids); // ��ȫվ��ȡ��������ֱ��������ֱ��������Ϣ��Ϊ��һ����ȡ�����û��б���׼��
    bool GetRoomViwerList(uint32 roomid, std::vector<EnterRoomUserInfo>* enterRoomUserInfoList); // ��ָ�������ȡ�û��б�׷���û�

    std::unique_ptr<User> user_;
    std::unique_ptr<CurlWrapper> curl_wrapper_;
};

