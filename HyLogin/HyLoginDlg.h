
// HyLoginDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CHyLoginDlg �Ի���
class CHyLoginDlg : public CDialogEx
{
// ����
public:
	CHyLoginDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HYLOGIN_DIALOG };

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
    afx_msg void OnBnClickedBtnLogin();
    CEdit m_edit_account;
    CEdit m_edit_password;
    CListBox m_list_info;
};
