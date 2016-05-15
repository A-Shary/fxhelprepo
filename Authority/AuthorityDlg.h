
// AuthorityDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "ATLComTime.h"


// CAuthorityDlg �Ի���
class CAuthorityDlg : public CDialogEx
{
// ����
public:
	CAuthorityDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AUTHORITY_DIALOG };

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
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
    CEdit m_edit_userid;
    CEdit m_edit_roomid;
    CEdit m_edit_clanid;
    afx_msg void OnBnClickedBtnGenerate();
    afx_msg void OnBnClickedBtnView();
    CButton m_chk_kickout;
    CButton m_banchat;
    CButton m_chk_anti_advance;
    COleDateTime m_oleDateTime_End;
};
