
// HyLoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HyLogin.h"
#include "HyLoginDlg.h"
#include "afxdialogex.h"
#include "HuyaLogic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHyLoginDlg �Ի���



CHyLoginDlg::CHyLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHyLoginDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHyLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_ACCOUNT, m_edit_account);
    DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edit_password);
    DDX_Control(pDX, IDC_LIST_INFO, m_list_info);
}

BEGIN_MESSAGE_MAP(CHyLoginDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_LOGIN, &CHyLoginDlg::OnBnClickedBtnLogin)
END_MESSAGE_MAP()


// CHyLoginDlg ��Ϣ�������

BOOL CHyLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHyLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CHyLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHyLoginDlg::OnBnClickedBtnLogin()
{
    CString csAccount;
    m_edit_account.GetWindowTextW(csAccount);
    CString csPassword;
    m_edit_password.GetWindowTextW(csPassword);

    HuyaLogic huyaLogic;
    std::wstring errormsg = L"Login success";
    if (huyaLogic.HuyaLogin(csAccount.GetBuffer(), csPassword.GetBuffer(), &errormsg))
    {
        m_list_info.AddString(L"Login success");
    }
    else
    {
        m_list_info.AddString(L"Login failed");
        m_list_info.AddString(errormsg.c_str());
    }
}
