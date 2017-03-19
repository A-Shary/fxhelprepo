
// AuthorityServerDlg.h : ͷ�ļ�
//

#pragma once
#include <memory>
#include "AuthorityNetwork.h"
#include "AuthorityController.h"

#include "afxwin.h"

class AuthorityNetwork;
class AuthorityController;

// CAuthorityServerDlg �Ի���
class CAuthorityServerDlg : public CDialogEx
{
// ����
public:
	CAuthorityServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AUTHORITYSERVER_DIALOG };

    enum
    {
        WM_USER_DISPLAY_MESSAGE = WM_USER + 1,
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
    afx_msg void OnBnClickedBtnRunService();
    afx_msg void OnBnClickedBtnStopService();
    afx_msg void OnBnClickedBtnQuery();
    afx_msg void OnClose();

    DECLARE_MESSAGE_MAP()

    void SetHScroll();
    LRESULT OnNotifyMessage(WPARAM wParam, LPARAM lParam);

private:
    void NotifyMessage(const std::wstring& msg);
    int msg_index_ = 0;
    CListBox m_list_msg;
    CEdit m_edit_query;

    std::unique_ptr<AuthorityNetwork> authority_network_;

    std::unique_ptr<AuthorityController> authority_controller_;
};
