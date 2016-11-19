
// UserTrackerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#include <memory>
#include <mutex>
#include "UserTrackerHelper.h"

class UserTrackerHelper;

// CUserTrackerDlg �Ի���
class CUserTrackerDlg : public CDialogEx
{
// ����
public:
	CUserTrackerDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CUserTrackerDlg();

// �Ի�������
	enum { IDD = IDD_USERTRACKER_DIALOG };

    enum
    {
        WM_USER_MSG = WM_USER + 1,
    };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

    DECLARE_MESSAGE_MAP()

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedBtnLogin();
    afx_msg void OnBnClickedBtnGetAllRoomData();
    afx_msg void OnBnClickedBtnFindInCache();
    afx_msg void OnBnClickedBtnUpdataFind();
    LRESULT OnNotifyMessage(WPARAM wParam, LPARAM lParam);

private:
    void SetHScroll();
    void Notify(const std::wstring& message);
    CEdit m_edit_target_fanxing_id;
    CListBox m_list_message;
    int list_info_count;

    std::unique_ptr<UserTrackerHelper> tracker_helper_;

    std::mutex messageMutex_;
    std::vector<std::wstring> messageQueen_;
    CEdit m_edit_account;
    CEdit m_edit_password;
};
