// NormalWelcomeSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <string>
#include "AntiFlood.h"
#include "NormalWelcomeSettingDlg.h"
#include "afxdialogex.h"

// CNormalWelcomeSetting �Ի���

IMPLEMENT_DYNAMIC(NormalWelcomeSettingDlg, CDialogEx)

NormalWelcomeSettingDlg::NormalWelcomeSettingDlg(
    const std::wstring& welcome, CWnd* pParent /*=NULL*/)
	: CDialogEx(NormalWelcomeSettingDlg::IDD, pParent)
{
    normal_welcome_setting_ = welcome;
}

NormalWelcomeSettingDlg::~NormalWelcomeSettingDlg()
{
}

BOOL NormalWelcomeSettingDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    std::wstring tips = L"���û�ӭ��ʽ˵����[���]������ؼ��֣����������ﱣ����"
        L"\r\n����:"
        L"\r\n�������ˣ���ӭ[���]����ֱ����"
        L"\r\n��С������ֱ�����ʱ�򣬻���ʾ:��ӭС������ֱ����";
    m_edit_welcome_tips.SetWindowTextW(tips.c_str());
    m_edit_normal_welcome_setting.SetWindowTextW(normal_welcome_setting_.c_str());
    m_edit_welcome_tips.EnableWindow(FALSE);
    return TRUE;
}

std::wstring NormalWelcomeSettingDlg::GetNormalWelcome() const
{
    return normal_welcome_setting_;
}

void NormalWelcomeSettingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_WELCOME_TIPS, m_edit_welcome_tips);
    DDX_Control(pDX, IDC_EDIT_WELCOME_PRE, m_edit_normal_welcome_setting);
}


BEGIN_MESSAGE_MAP(NormalWelcomeSettingDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &NormalWelcomeSettingDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &NormalWelcomeSettingDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CNormalWelcomeSetting ��Ϣ�������
void NormalWelcomeSettingDlg::OnBnClickedOk()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString welcome;
    m_edit_normal_welcome_setting.GetWindowTextW(welcome);
    normal_welcome_setting_ = welcome.GetBuffer();
    if (normal_welcome_setting_.find(L"[���]")==std::wstring::npos)
    {
        ::MessageBoxW(NULL, L"������������\"[���]\"", L"���ô���", 0);
        return;
    }
    CDialogEx::OnOK();
}


void NormalWelcomeSettingDlg::OnBnClickedCancel()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CDialogEx::OnCancel();
}
