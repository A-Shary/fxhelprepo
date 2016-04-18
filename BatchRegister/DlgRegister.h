#pragma once
#include <memory>
#include <vector>
#include <mutex>
#include "resource.h"
#include "afxwin.h"

class NetworkHelper;
class RegisterHelper;
// DlgRegister �Ի���

class CDlgRegister : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRegister)

public:
	CDlgRegister(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgRegister();

// �Ի�������
	enum { IDD = IDD_DLG_REGISTER ,
        WM_USER_REGISTER_INFO = WM_USER + 1,
    };

    
    afx_msg void OnPaint();
    afx_msg LRESULT OnHotKey(WPARAM wp, LPARAM lp);
    afx_msg void OnBnClickedBtnCheckExist();
    afx_msg void OnBnClickedBtnRegister();
    afx_msg void OnBnClickedBtnVerifyCode();
protected:
    virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

    LRESULT OnNotifyMessage(WPARAM wParam, LPARAM lParam);
    void Notify(const std::wstring& message);
private:
    CStatic m_static_verifycode;
    CEdit m_register_username;
    CEdit m_register_password;
    CEdit m_register_verifycode;
    CImage image;
    std::unique_ptr<RegisterHelper> registerHelper_;

    int infoListCount_;
    std::mutex messageMutex_;
    std::vector<std::wstring> messageQueen_;
    CListBox m_register_info_list;
};
