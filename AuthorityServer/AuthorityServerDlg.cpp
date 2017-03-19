
// AuthorityServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AuthorityServer.h"
#include "AuthorityServerDlg.h"

#include "AuthorityController.h"

#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAuthorityServerDlg �Ի���



CAuthorityServerDlg::CAuthorityServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAuthorityServerDlg::IDD, pParent)
    , authority_network_(new AuthorityNetwork)
    , authority_controller_(new AuthorityController)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAuthorityServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
    DDX_Control(pDX, IDC_EDIT_QUERY, m_edit_query);
}

BEGIN_MESSAGE_MAP(CAuthorityServerDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_RUN_SERVICE, &CAuthorityServerDlg::OnBnClickedBtnRunService)
    ON_BN_CLICKED(IDC_BTN_STOP_SERVICE, &CAuthorityServerDlg::OnBnClickedBtnStopService)
    ON_BN_CLICKED(IDC_BTN_QUERY, &CAuthorityServerDlg::OnBnClickedBtnQuery)
    ON_MESSAGE(WM_USER_DISPLAY_MESSAGE, &CAuthorityServerDlg::OnNotifyMessage)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAuthorityServerDlg ��Ϣ�������

BOOL CAuthorityServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��


    authority_controller_->Initialize();
    authority_network_->Initialize(authority_controller_.get());
    authority_network_->SetNotify(
        std::bind(&CAuthorityServerDlg::NotifyMessage, this,
        std::placeholders::_1));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAuthorityServerDlg::NotifyMessage(const std::wstring& msg)
{
    std::wstring* wstr(new std::wstring(msg));
    this->PostMessage(WM_USER_DISPLAY_MESSAGE, (WPARAM)(wstr), 0);
}


void CAuthorityServerDlg::SetHScroll()
{
    CDC* dc = GetDC();

    CString str;
    int index = m_list_msg.GetCount() - 1;
    if (index >= 0)
    {
        m_list_msg.GetText(index, str);
        SIZE s = dc->GetTextExtent(str);
        long temp = (long)SendDlgItemMessage(IDC_LIST_MSG, LB_GETHORIZONTALEXTENT, 0, 0); //temp�õ��������Ŀ��
        if (s.cx > temp)
        {
            SendDlgItemMessage(IDC_LIST_MSG, LB_SETHORIZONTALEXTENT, (WPARAM)s.cx, 0);
        }
    }

    ReleaseDC(dc);
}

LRESULT CAuthorityServerDlg::OnNotifyMessage(WPARAM wParam, LPARAM lParam)
{
    scoped_ptr<std::wstring> wstr(reinterpret_cast<std::wstring*>(wParam));
    if (!wstr)
        return 0;

    m_list_msg.InsertString(msg_index_++, wstr->c_str());
    m_list_msg.SetCurSel(msg_index_ - 1);
    SetHScroll();
    return 0;
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAuthorityServerDlg::OnPaint()
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

void CAuthorityServerDlg::OnClose()
{
    authority_network_->Finalize();
    authority_controller_->Finalize();
    CDialogEx::OnClose();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAuthorityServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAuthorityServerDlg::OnBnClickedBtnRunService()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    authority_network_->Start();
}


void CAuthorityServerDlg::OnBnClickedBtnStopService()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    authority_network_->Stop();
}


void CAuthorityServerDlg::OnBnClickedBtnQuery()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}
