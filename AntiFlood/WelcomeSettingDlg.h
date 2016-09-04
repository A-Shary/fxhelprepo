#pragma once
#include "afxwin.h"


// WelcomeSettingDlg �Ի���

class WelcomeSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WelcomeSettingDlg)

public:
	WelcomeSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WelcomeSettingDlg();
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();

    bool GetSettingInfo(std::wstring* fanxingid, std::wstring* name, 
        std::wstring* content) const;
// �Ի�������
	enum { IDD = IDD_WELCOME_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
    CEdit m_edit_welcome_fanxingid;
    CEdit m_edit_welcome_setting_name;
    CEdit m_edit_welcome_content;

    std::wstring fanxingid_;
    std::wstring name_;
    std::wstring content_;
};
