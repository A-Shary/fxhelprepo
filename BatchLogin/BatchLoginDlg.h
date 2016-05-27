
// BatchLoginDlg.h : ͷ�ļ�
//

#pragma once

#include <memory>
#include "afxcmn.h"

#include "Network/EncodeHelper.h"

class UserRoomManager;
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
    afx_msg void OnBnClickedBtnGetProxy();
    afx_msg void OnBnClickedBtnBatchEnterRoom();
    afx_msg void OnBnClickedBtnImportRoom();

	DECLARE_MESSAGE_MAP()

private:
    LRESULT OnNotifyMessage(WPARAM wParam, LPARAM lParam);
    LRESULT OnDisplayDataToUserList(WPARAM wParam, LPARAM lParam);
    LRESULT OnDisplayDataToRoomList(WPARAM wParam, LPARAM lParam);

    std::unique_ptr<UserRoomManager> userRoomManager_;
    CListCtrl m_ListCtrl_Users;
    CListCtrl m_ListCtrl_Rooms;
public:
    afx_msg void OnBnClickedBtnLogin();
};
