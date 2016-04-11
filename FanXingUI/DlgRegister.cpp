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
    , infoListCount_(0)
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
    DDX_Control(pDX, IDC_LIST_REGISTER_INFO, m_register_info_list);
}


BEGIN_MESSAGE_MAP(CDlgRegister, CDialogEx)
    ON_WM_PAINT()
    ON_BN_CLICKED(IDC_BTN_CHECK_EXIST, &CDlgRegister::OnBnClickedBtnCheckExist)
    ON_BN_CLICKED(IDC_BTN_REGISTER, &CDlgRegister::OnBnClickedBtnRegister)
    ON_BN_CLICKED(IDC_BTN_VERIFY_CODE, &CDlgRegister::OnBnClickedBtnVerifyCode)
    ON_MESSAGE(WM_USER_REGISTER_INFO, &CDlgRegister::OnNotifyMessage)
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

LRESULT CDlgRegister::OnNotifyMessage(WPARAM wParam, LPARAM lParam)
{
    std::vector<std::wstring> messages;
    messageMutex_.lock();
    messages.swap(messageQueen_);
    messageMutex_.unlock();

    for (auto str : messages)
    {
        m_register_info_list.InsertString(infoListCount_++, str.c_str());
    }

    m_register_info_list.SetCurSel(infoListCount_ - 1);
    return 0;
}

void CDlgRegister::Notify(const std::wstring& message)
{
    // �������ݸ�����
    messageMutex_.lock();
    messageQueen_.push_back(message);
    messageMutex_.unlock();
    this->PostMessage(WM_USER_REGISTER_INFO, 0, 0);
}

void CDlgRegister::OnBnClickedBtnCheckExist()
{
    CString username;
    m_register_username.GetWindowTextW(username);
    bool result = registerNetworkHelper_->RegisterCheckUserExist(username.GetBuffer());
    if (!result)
    {
        Notify(L"���������û��Ѿ�����!");
        return;
    }
    Notify(L"�û�������");
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
        Notify(L"�����ע����Ϣ������");
        AfxMessageBox(L"����������ע����Ϣ");
        return;
    }

    bool result = registerNetworkHelper_->RegisterCheckUserExist(username.GetBuffer());
    if (!result)
    {
        Notify(L"���������û��Ѿ�����!");
        return;
    }
    Notify(L"�û�������");

    if (!registerNetworkHelper_->RegisterCheckUserInfo(username.GetString(),
        password.GetString()))
    {
        Notify(L"����û���Ϣʧ��");
        return;
    }
    Notify(L"����û���Ϣ�ɹ�");

    if (!registerNetworkHelper_->RegisterCheckVerifyCode(verifycode.GetString()))
    {
        Notify(L"��֤������ʧЧ");
        return;
    }
    Notify(L"��֤����֤�ɹ�");

    if (!registerNetworkHelper_->RegisterUser(username.GetString(),
        password.GetString(), verifycode.GetString()))
    {
        Notify(L"ע��ʧ��");
        return;
    }
    Notify(L"ע��ɹ�");

    if (!registerHelper_->SaveAccountToFile(username.GetString(), 
        password.GetString()))
    {
        Notify(L"����ע����Ϣ���ļ�ʧ��!");
    }
    Notify(L"����ע����Ϣ���ļ��ɹ�!");
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
