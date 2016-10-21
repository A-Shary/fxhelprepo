
// BatchLoginDlg.h : ͷ�ļ�
//

#pragma once

#include <memory>
#include <string>
#include <mutex>
#include "afxcmn.h"

#include "Network/EncodeHelper.h"
#include "afxwin.h"

class UserRoomManager;
class TcpManager;
// CBatchLoginDlg �Ի���
class CBatchLoginDlg : public CDialogEx
{
// ����
public:
	CBatchLoginDlg(CWnd* pParent = NULL);	// ��׼���캯��
    virtual ~CBatchLoginDlg();
// �Ի�������
	enum { IDD = IDD_BATCHLOGIN_DIALOG };

    enum
    {
        WM_USER_NOTIFY_MESSAGE = WM_USER + 1,
        WM_USER_USER_LIST_INFO = WM_USER + 2,
        WM_USER_ROOM_LIST_INFO = WM_USER + 3,
    };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedBtnImportUser();
    afx_msg void OnBnClickedBtnLogin();
    afx_msg void OnBnClickedBtnGetUserinfo();// �ֿ���Ϣ�����ʢ��ʹ��
    afx_msg void OnBnClickedBtnGetProxy();
    afx_msg void OnBnClickedBtnBatchEnterRoom();
    //afx_msg void OnBnClickedBtnImportRoom();
    afx_msg void OnBnClickedBtnUpMvBillboard();
    afx_msg void OnBnClickedBtnSaveUserPwdCookie();
    afx_msg void OnLvnItemchangedListRoom(NMHDR *pNMHDR, LRESULT *pResult);
    //afx_msg void OnNMClickListRoom(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnSendAward();
    afx_msg void OnBnClickedBtnSendSingle();
    afx_msg void OnBnClickedBtnLottery();
    afx_msg void OnBnClickedBtnBreak();

    afx_msg void OnBnClickedBtnSelectAll();
    afx_msg void OnBnClickedBtnReverseSelect();
    afx_msg void OnBnClickedBtnDelete();
	DECLARE_MESSAGE_MAP()

private:
    void Notify(const std::wstring& message);
    LRESULT OnNotifyMessage(WPARAM wParam, LPARAM lParam);
    LRESULT OnDisplayDataToUserList(WPARAM wParam, LPARAM lParam);
    LRESULT OnDisplayDataToRoomList(WPARAM wParam, LPARAM lParam);
    bool SendGifts(uint32 gift_id);
    void GetSelectUsers(std::vector<std::wstring>* users);

    std::unique_ptr<UserRoomManager> userRoomManager_;
    std::unique_ptr<TcpManager> tcpManager_;
    CListCtrl m_ListCtrl_Users;
    //CListCtrl m_ListCtrl_Rooms;
    CListCtrl m_list_proxy;
    CEdit m_mv_collection_id;
    CEdit m_mv_id;
    CListBox InfoList_;
    int infoListCount_ = 0;

    std::mutex messageMutex_;
    std::vector<std::wstring> messageQueen_;    
    CEdit m_roomid;
    CEdit m_gift_count;
public:
    CEdit m_nickname;
    afx_msg void OnBnClickedBtnChangeNickname();
};
