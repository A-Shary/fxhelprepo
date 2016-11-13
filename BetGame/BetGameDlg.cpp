
// BetGameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BetGame.h"
#include "BetGameDlg.h"
#include "afxdialogex.h"

#undef max
#undef min

#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBetGameDlg �Ի���
namespace
{
    const wchar_t* betcolumnlist[] = {
        L"1",
        L"2",
        L"3",
        L"4",
        L"5",
        L"6",
        L"7",
        L"8",
    };
}

CBetGameDlg::CBetGameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBetGameDlg::IDD, pParent)
    , bet_network_(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBetGameDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_BETDATA, m_list_bet_data);
    DDX_Control(pDX, IDC_EDIT_Username, m_edit_username);
    DDX_Control(pDX, IDC_EDIT_Password, m_edit_password);
    DDX_Control(pDX, IDC_EDIT_NAV, m_edit_room_id);
    DDX_Control(pDX, IDC_CHECK_REMEMBER, m_check_remember);
}

BEGIN_MESSAGE_MAP(CBetGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_NAV, &CBetGameDlg::OnBnClickedButtonEnterRoom)
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CBetGameDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// CBetGameDlg ��Ϣ�������

BOOL CBetGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    DWORD dwStyle = m_list_bet_data.GetExtendedStyle();
    dwStyle |= LVS_EX_CHECKBOXES;
    dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��

    m_list_bet_data.SetExtendedStyle(dwStyle); //������չ���
    int nColumnCount = m_list_bet_data.GetHeaderCtrl()->GetItemCount();
    for (int i = nColumnCount - 1; i >= 0; i--)
        m_list_bet_data.DeleteColumn(i);
    int index = 0;
    RECT rect;
    m_list_bet_data.GetWindowRect(&rect);
    int width = (rect.right - rect.left) / (sizeof(betcolumnlist) / sizeof(betcolumnlist[0]));
    for (const auto& it : betcolumnlist)
        m_list_bet_data.InsertColumn(index++, it, LVCFMT_LEFT, width);//������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBetGameDlg::OnPaint()
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
HCURSOR CBetGameDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBetGameDlg::OnBnClickedButtonEnterRoom()
{
    CString cs_roomid;
    m_edit_room_id.GetWindowTextW(cs_roomid);
    

}

void CBetGameDlg::OnBnClickedButtonLogin()
{
    CString cs_username;
    CString cs_password;
    m_edit_username.GetWindowTextW(cs_username);
    m_edit_password.GetWindowTextW(cs_password);

    std::string username = base::WideToUTF8(cs_username.GetBuffer());
    std::string password = base::WideToUTF8(cs_password.GetBuffer());

    bet_network_.reset(new BetNetworkHelper);
    
    bet_network_->Initialize();

    if (!bet_network_->Login(username, password))
        return;

    if (m_check_remember.GetCheck())
    {
        // �����¼�ɹ�����
    }

}