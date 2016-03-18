#pragma once
#include "resource.h"
#include "afxwin.h"
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include "NetworkHelper.h"

// CDlgGiftNotify �Ի���

class CDlgGiftNotify : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGiftNotify)

public:
	CDlgGiftNotify(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGiftNotify();

// �Ի�������
	enum { IDD = IDD_DLG_GIFT };
    enum
    {
        WM_USER_ADD_GIFT_INFO = WM_USER + 1,
        WM_USER_UPDATE_TOTAL_COUNT = WM_USER + 2,
    };

    enum class ROOM_TYPE
    {
        ROOM_LEFT = 0,
        ROOM_RIGHT = 1
    };

    enum TIMMER_ENUM
    {
        TIME_SHOW = 0,
        TIME_SKIP = 1,
    };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnBegin();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    LRESULT OnAddGiftInfo(WPARAM wParam, LPARAM lParam);
    LRESULT OnUpdateTotalCount(WPARAM wParam, LPARAM lParam);

    void Notify(const std::wstring& message);
    void Notify601(ROOM_TYPE roomtype,const RoomGiftInfo601& roomgiftinfo,
                   const GiftInfo& giftinfo);

private:
    struct UserGiftAccumulative
    {
        ROOM_TYPE roomtype;
        uint32 userid;
        uint32 giftcoin;
        std::string nickname;
        uint32 accumulative; // ��ʱ�Ȳ�ʹ��
        std::string displaymsg;
    };

    void ClearList();
    // ʱ���ܾ���ʱ��Ҫ�ս�ͳ�ƣ������º������ݡ�
    void StopAccumulative();

    std::unique_ptr<NetworkHelper> networkLeft_;
    std::unique_ptr<NetworkHelper> networkRight_;

    std::mutex messageLock_;
    std::vector<UserGiftAccumulative> messageQueue_;

    bool display_;
    int m_room_left;
    int m_room_right;
    int m_time_all;
    int m_time_left;
    CListBox m_list_left;
    CListBox m_list_right;

    int m_coin_left;
    int m_coin_right;
    CString m_static_time;
};
