
// LibEventClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <functional>
#include "LibEventClient.h"
#include "LibEventClientDlg.h"
#include "afxdialogex.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLibEventClientDlg �Ի���

CLibEventClientDlg::CLibEventClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLibEventClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLibEventClientDlg::~CLibEventClientDlg()
{
    client_controller_.Finalize();
}

void CLibEventClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_TARGET_IP, m_edit_ip);
    DDX_Control(pDX, IDC_EDIT_TARGET_PORT, m_edit_port);
    DDX_Control(pDX, IDC_EDIT_CLIENT_COUNT, m_edit_target_count);
    DDX_Control(pDX, IDC_LIST_MESSAGE, m_list_message);
    DDX_Control(pDX, IDC_LIST_STATUS, m_listctrl_ip_target_status);
}

BEGIN_MESSAGE_MAP(CLibEventClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_BEGIN, &CLibEventClientDlg::OnBnClickedBtnBegin)
END_MESSAGE_MAP()


// CLibEventClientDlg ��Ϣ�������

BOOL CLibEventClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    client_controller_.Initialize();
    m_edit_ip.SetWindowTextW(L"58.63.236.248");
    m_edit_port.SetWindowTextW(L"80");
    m_edit_target_count.SetWindowTextW(L"3");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLibEventClientDlg::OnPaint()
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
HCURSOR CLibEventClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CLibEventClientDlg::OnBnClickedBtnBegin()
{
    CString cs_ip;
    CString cs_port;
    CString cs_target_count;

    m_edit_ip.GetWindowText(cs_ip);
    m_edit_port.GetWindowText(cs_port);
    m_edit_target_count.GetWindowText(cs_target_count);

    std::string utf8_target_count = base::WideToUTF8(cs_target_count.GetBuffer());
    int target_count = 1;
    base::StringToInt(utf8_target_count, &target_count);

    std::string ip = base::WideToUTF8(cs_ip.GetBuffer());

    int port = 0;
    std::string utf8_port = base::WideToUTF8(cs_port.GetBuffer());
    base::StringToInt(utf8_port, &port);

    for (int index = 0; index < target_count; index++)
    {
        client_controller_.AddClient(ip, port,
                                     std::bind(&CLibEventClientDlg::ConnectNotify,
                                     this, std::placeholders::_1),
                                     std::bind(&CLibEventClientDlg::DataReceiveNotify, 
                                     this, std::placeholders::_1, std::placeholders::_2)
                                     );
    }
}

void CLibEventClientDlg::ConnectNotify(bool result)
{

}

void CLibEventClientDlg::DataReceiveNotify(bool result, std::vector<uint8>& data)
{

}


