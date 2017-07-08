
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
    DDX_Control(pDX, IDC_EDIT_MESSAGE_SEND, m_edit_message_send);
}

BEGIN_MESSAGE_MAP(CLibEventClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_BEGIN, &CLibEventClientDlg::OnBnClickedBtnBegin)
    ON_MESSAGE(WM_USER_DISPLAY_MESSAGE, &CLibEventClientDlg::OnNotifyMessage)
    ON_BN_CLICKED(IDC_BTN_SEND_MSG, &CLibEventClientDlg::OnBnClickedBtnSendMsg)
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
    m_edit_ip.SetWindowTextW(L"127.0.0.1");
    m_edit_port.SetWindowTextW(L"9999");
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

LRESULT CLibEventClientDlg::OnNotifyMessage(WPARAM wParam, LPARAM lParam)
{
    std::wstring* wstr = reinterpret_cast<std::wstring*>(wParam);
    if (!wstr)
        return 0;

    m_list_message.InsertString(message_index_++, wstr->c_str());
    delete wstr;
    m_list_message.SetCurSel(message_index_ - 1);
    SetHScroll();
    return 0;
}

void CLibEventClientDlg::SetHScroll()
{
    CDC* dc = GetDC();

    CString str;
    int index = m_list_message.GetCount() - 1;
    if (index >= 0)
    {
        m_list_message.GetText(index, str);
        SIZE s = dc->GetTextExtent(str);
        long temp = (long)SendDlgItemMessage(IDC_LIST_MESSAGE, LB_GETHORIZONTALEXTENT, 0, 0); //temp�õ��������Ŀ��
        if (s.cx > temp)
        {
            SendDlgItemMessage(IDC_LIST_MESSAGE, LB_SETHORIZONTALEXTENT, (WPARAM)s.cx, 0);
        }
    }

    ReleaseDC(dc);
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
                                     this, ip, utf8_port, std::placeholders::_1,
                                     std::placeholders::_2),
                                     std::bind(&CLibEventClientDlg::DataReceiveNotify, 
                                     this, ip, utf8_port, 
                                     std::placeholders::_1, std::placeholders::_2)
                                     );
    }
}

void CLibEventClientDlg::ConnectNotify(
    const std::string& ip, const std::string& port, bool result, TCPHANDLE handle)
{
    handles_[handle] = std::make_pair(ip, port);
    std::wstring* wstr = new std::wstring();
    *wstr = base::UTF8ToWide(ip + ":" + port) + std::wstring(L"����") + (result ? L"�ɹ�" : L"ʧ��");
    this->PostMessage(WM_USER_DISPLAY_MESSAGE, (WPARAM)(wstr), 0);
}

void CLibEventClientDlg::DataReceiveNotify(
    const std::string& ip, const std::string& port, bool result, std::vector<uint8>& data)
{
    std::wstring* wstr = new std::wstring();
    *wstr = base::UTF8ToWide(ip + ":" + port) +std::wstring(L"��������") + (result ? L"�ɹ�" : L"ʧ��");
    this->PostMessage(WM_USER_DISPLAY_MESSAGE, (WPARAM)(wstr), 0);

    if (data.size() <= 24)
        return;

    std::string display_result(data.begin() + 24, data.end());
    std::wstring* wstr_result = new std::wstring();
    *wstr_result = base::UTF8ToWide(display_result);
    this->PostMessage(WM_USER_DISPLAY_MESSAGE, (WPARAM)(wstr_result), 0);
}

void CLibEventClientDlg::SendCallback(const std::string& ip, const std::string& port,
                                      bool result)
{
    std::wstring* wstr = new std::wstring();
    *wstr = base::UTF8ToWide(ip + ":" + port) + std::wstring(L"��������") + (result ? L"�ɹ�" : L"ʧ��");
    this->PostMessage(WM_USER_DISPLAY_MESSAGE, (WPARAM)(wstr), 0);
}

void CLibEventClientDlg::OnBnClickedBtnSendMsg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CString cs_message_send;
    m_edit_message_send.GetWindowTextW(cs_message_send);
    if (!cs_message_send.GetLength())
        return;
    
    std::string str_msg = base::WideToUTF8(cs_message_send.GetBuffer());
    std::vector<uint8> data(str_msg.begin(), str_msg.end());
    for (const auto& handle : handles_)
    {
        client_controller_.Send(handle.first, data,
                                std::bind(&CLibEventClientDlg::SendCallback,
                                this, handle.second.first, handle.second.second,
                                std::placeholders::_1));
    }
}
