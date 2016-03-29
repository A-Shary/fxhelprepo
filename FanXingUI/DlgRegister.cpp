// DlgRegister.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgRegister.h"
#include "atlimage.h"
#include "afxdialogex.h"
#include "NetworkHelper.h"
#include "RegisterHelper.h"
#include "../Network/EncodeHelper.h"
#include "third_party/chromium/base/basictypes.h"

// DlgRegister �Ի���

IMPLEMENT_DYNAMIC(CDlgRegister, CDialogEx)

CDlgRegister::CDlgRegister(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgRegister::IDD, pParent)
{
    registerNetworkHelper_.reset(new NetworkHelper);
    registerNetworkHelper_->Initialize();
    registerHelper_.reset(new RegisterHelper);
}

CDlgRegister::~CDlgRegister()
{
    registerNetworkHelper_->Finalize();
}

void CDlgRegister::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_REGISTER_NAME, m_register_username);
    DDX_Control(pDX, IDC_EDIT_REGISTER_PASSWORD, m_register_password);
    DDX_Control(pDX, IDC_EDIT_VERIFY_CODE, m_register_verifycode);
    DDX_Control(pDX, IDC_STATIC_VERIFYCODE, m_static_verifycode);
}


BEGIN_MESSAGE_MAP(CDlgRegister, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_CHECK_EXIST, &CDlgRegister::OnBnClickedBtnCheckExist)
    ON_BN_CLICKED(IDC_BTN_REGISTER, &CDlgRegister::OnBnClickedBtnRegister)
    ON_BN_CLICKED(IDC_BTN_VERIFY_CODE, &CDlgRegister::OnBnClickedBtnVerifyCode)
    ON_WM_PAINT()
END_MESSAGE_MAP()


// DlgRegister ��Ϣ�������

void CDlgRegister::OnPaint()
{
    if (!image.IsNull())
    {
        int hight = image.GetHeight();
        int width = image.GetWidth();
        CRect rc;
        m_static_verifycode.GetWindowRect(&rc);
        ScreenToClient(rc);
        image.Draw(GetDC()->m_hDC, CRect(rc.left + 20, rc.top + 30, rc.left + width + 20,
            rc.top + hight + 30));
    }
    CDialogEx::OnPaint();
}

void CDlgRegister::OnBnClickedBtnCheckExist()
{
    CString username;
    m_register_username.GetWindowTextW(username);
    registerNetworkHelper_->RegisterCheckUserExist(username.GetBuffer());
}


void CDlgRegister::OnBnClickedBtnRegister()
{
    CString username;
    CString password;
    CString verifycode;

    m_register_username.GetWindowTextW(username);
    m_register_password.GetWindowTextW(password);
    m_register_verifycode.GetWindowTextW(verifycode);

    if (username.IsEmpty() || password.IsEmpty() || verifycode.IsEmpty())
    {
        AfxMessageBox(L"����������ע����Ϣ");
        return;
    }

    if (!registerNetworkHelper_->RegisterCheckUserInfo(username.GetString(),
        password.GetString()))
    {
        AfxMessageBox(L"����û���Ϣʧ��");
        return;
    }

    if (!registerNetworkHelper_->RegisterCheckVerifyCode(verifycode.GetString()))
    {
        AfxMessageBox(L"��֤����ʧ��");
        return;
    }

    if (!registerNetworkHelper_->RegisterUser(username.GetString(),
        password.GetString(), verifycode.GetString()))
    {
        AfxMessageBox(L"ע��ʧ��");
        return;
    }

    registerHelper_->SaveAccountToFile(username.GetString(), password.GetString());
}


void CDlgRegister::OnBnClickedBtnVerifyCode()
{
    std::vector<uint8> picture;
    registerNetworkHelper_->RegisterGetVerifyCode(&picture);
    std::wstring pathname;
    if (!registerHelper_->SaveVerifyCodeImage(picture, &pathname))
    {
        assert(false);
        return;
    }

    if (!image.IsNull())
    {
        image.Destroy();
    }

    image.Load(pathname.c_str());
    int hight = image.GetHeight();
    int width = image.GetWidth();
    CRect rc;
    m_static_verifycode.GetWindowRect(&rc);
    ScreenToClient(rc);
    image.Draw(GetDC()->m_hDC, CRect(rc.left + 20, rc.top + 30, rc.left + width +20, 
        rc.top + hight + 30));
}
