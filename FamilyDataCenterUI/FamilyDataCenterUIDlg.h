
// FamilyDataCenterUIDlg.h : ͷ�ļ�
//

#pragma once

#include "afxcmn.h"
#include <memory>

#include "FamilyDataCenterUI/FamilyDataController.h"
#include "FamilyDataCenterUI/FamilyDataModle.h"
#include "ATLComTime.h"

// CFamilyDataCenterUIDlg �Ի���
class CFamilyDataCenterUIDlg : public CDialogEx
{
// ����
public:
	CFamilyDataCenterUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_FAMILYDATACENTERUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_ListCtrl_SummaryData;
    afx_msg void OnBnClickedGetFamilyData();

private:
    std::unique_ptr<FamilyDataController> familyDataController_;
    std::unique_ptr<FamilyDataModle> familyDataModle_;
public:
    afx_msg void OnBnClickedBtnExportToExcel();
    COleDateTime m_oleDateTime_Begin;
    COleDateTime m_oleDateTime_End;
    afx_msg void OnBnClickedBtnLogin();
    CString m_username;
    CString m_password;
};
