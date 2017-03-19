
// AuthorityServerDlg.h : ͷ�ļ�
//

#pragma once
#include <memory>
#include "AuthorityNetwork.h"

#include "afxwin.h"

class AuthorityNetwork;

// CAuthorityServerDlg �Ի���
class CAuthorityServerDlg : public CDialogEx
{
// ����
public:
	CAuthorityServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AUTHORITYSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedBtnRunService();
    afx_msg void OnBnClickedBtnStopService();
    afx_msg void OnBnClickedBtnQuery();

// ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();

    DECLARE_MESSAGE_MAP()

public:
    CListBox m_list_msg;
    CEdit m_edit_query;

    std::unique_ptr<AuthorityNetwork> authority_network_;
};
