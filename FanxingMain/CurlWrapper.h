#pragma once
#include <string>
#include "third_party/chromium/base/basictypes.h"

// �ṩ�����ʹ��curl�ӿڵ�ִ����������
class CurlWrapper
{
public:
    CurlWrapper();
    ~CurlWrapper();
    static void CurlInit();
    static void CurlCleanup();

    bool LoginRequestWithCookies();
    bool LoginRequestWithUsernameAndPassword(const std::string& username, 
                                             const std::string& password);
    bool Services_UserService_UserService_getMyUserDataInfo();
    bool Services_IndexService_IndexService_getUserCenter();

    bool EnterRoom(uint32 roomid);

	// �ڽ��뷿���Ժ󣬻�ȡ�û���Ϣ
	bool Servies_Uservice_UserService_getCurrentUserInfo(uint32 roomid);

    // �ؼ����ݻ�ȡ�����������������tcp��������Ҫ����key����ֵ
    bool RoomService_RoomService_enterRoom(uint32 roomid);

    bool WriteCallback(const std::string& data);
};

