
// IpProxyAchieverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "IpProxyAchiever.h"
#include "YoudailiHelper.h"
#include "Network/CurlWrapper.h"

#include "afxdialogex.h"
#include "IpProxyAchieverDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIpProxyAchieverDlg �Ի���



CIpProxyAchieverDlg::CIpProxyAchieverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIpProxyAchieverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    CurlWrapper::CurlInit();
}

void CIpProxyAchieverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIpProxyAchieverDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_GET_PROXY, &CIpProxyAchieverDlg::OnBnClickedBtnGetProxy)
END_MESSAGE_MAP()


// CIpProxyAchieverDlg ��Ϣ�������

BOOL CIpProxyAchieverDlg::OnInitDialog()
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

void CIpProxyAchieverDlg::OnPaint()
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
HCURSOR CIpProxyAchieverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIpProxyAchieverDlg::OnBnClickedBtnGetProxy()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    YoudailiHelper youdaili;
    std::vector<IpProxy> ipproxys;
    if (!youdaili.GetAllProxyInfo(&ipproxys))
    {
        MessageBox(L"��ȡ����ʧ��", L"����", 0);
        return;
    }
    
    youdaili.SaveIpProxy(ipproxys);
}
