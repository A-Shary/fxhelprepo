// AdvertisementDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AntiFlood/resource.h"        // ������
#include "AdvertisementDlg.h"
#include "afxdialogex.h"


// AdvertisementDlg �Ի���

IMPLEMENT_DYNAMIC(AdvertisementDlg, CDialogEx)

AdvertisementDlg::AdvertisementDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(AdvertisementDlg::IDD, pParent)
{

}

AdvertisementDlg::~AdvertisementDlg()
{
}

void AdvertisementDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EXPLORER_AD, explorer_web_);
}

BOOL AdvertisementDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���

    SetWindowText(L"���");

    
    explorer_web_.put_Silent(TRUE);// ��ֹ����360�����������ʾ
    explorer_web_.Navigate(L"https://www.2345.com/?37815", NULL, NULL, NULL, NULL);

    return TRUE;
}

BEGIN_MESSAGE_MAP(AdvertisementDlg, CDialogEx)
END_MESSAGE_MAP()


// AdvertisementDlg ��Ϣ�������
