
// UserTrackerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CUserTrackerDlg �Ի���
class CUserTrackerDlg : public CDialogEx
{
// ����
public:
	CUserTrackerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_USERTRACKER_DIALOG };

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
    afx_msg void OnBnClickedBtnGetallroomdata();
    afx_msg void OnBnClickedBtnFindInCache();
    afx_msg void OnBnClickedBtnUpdataFind();

private:
    CEdit m_edit_target_fanxing_id;
    CListBox m_list_message;
};
