
// AuthorityServerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


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


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CListBox m_list_msg;
};
