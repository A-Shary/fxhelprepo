#pragma once
#include <memory>
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
	enum { IDD = IDD_DLG_REGISTER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();

    afx_msg void OnBnClickedBtnCheckExist();
    afx_msg void OnBnClickedBtnRegister();
    afx_msg void OnBnClickedBtnVerifyCode();

private:
    CStatic m_static_verifycode;
    CEdit m_register_username;
    CEdit m_register_password;
    CEdit m_register_verifycode;
    CImage image;
    std::unique_ptr<NetworkHelper> registerNetworkHelper_;
    std::unique_ptr<RegisterHelper> registerHelper_;

};
