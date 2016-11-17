
// BetGameDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BetGame.h"
#include "BetGameDlg.h"
#include "afxdialogex.h"
#include "Network/EncodeHelper.h"

#undef max
#undef min

#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"
//#include "third_party/chromium/base/strings/str"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBetGameDlg �Ի���
namespace
{
    const wchar_t* betcolumnlist[] = {
        L"ʱ��",
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
    , message_count_(0)
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
    DDX_Control(pDX, IDC_LIST_MSG, m_list_msg);
}

BEGIN_MESSAGE_MAP(CBetGameDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_NAV, &CBetGameDlg::OnBnClickedButtonEnterRoom)
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CBetGameDlg::OnBnClickedButtonLogin)
    ON_MESSAGE(WM_USER_TIPS, &CBetGameDlg::OnTipsMessage)
    ON_MESSAGE(WM_USER_BET_RESULT, &CBetGameDlg::OnDisplayBetResult)
    ON_MESSAGE(WM_USER_BET_TIME, &CBetGameDlg::OnDisplayBetTime)
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
    //dwStyle |= LVS_EX_CHECKBOXES;
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

    m_list_msg.InsertString(message_count_++, L"��������");

    m_edit_username.SetWindowTextW(L"fanxingtest002");
    m_edit_password.SetWindowTextW(L"1233211234567");
    m_edit_room_id.SetWindowTextW(L"1201793");

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
    uint32 room_id = 0;
    base::StringToUint(base::WideToUTF8(cs_roomid.GetBuffer()), &room_id);
    if (!bet_network_->EnterRoom(room_id))
    {
    }
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
    bet_network_->SetTipMessage(
        base::Bind(&CBetGameDlg::TipMessageCallback,
        base::Unretained(this)));

    bet_network_->SetBetResultNotify(
        base::Bind(&CBetGameDlg::DisplayBetResultCallback,
        base::Unretained(this)));

    bet_network_->SetBetTimeNotify(
        base::Bind(&CBetGameDlg::DisplayBetTimeCallback,
        base::Unretained(this)));

    if (!bet_network_->Login(username, password))
        return;

    if (m_check_remember.GetCheck())
    {
        // �����¼�ɹ�����
    }
}

void CBetGameDlg::TipMessageCallback(const std::wstring& message)
{
    std::wstring* param = new std::wstring(message);
    this->PostMessage(WM_USER_TIPS, 0, (LPARAM)param);
}

void CBetGameDlg::DisplayBetResultCallback(const BetResult& bet_result)
{
    BetResult* param = new BetResult(bet_result);
    this->PostMessage(WM_USER_BET_RESULT, 0, (LPARAM)param);
}

void CBetGameDlg::DisplayBetTimeCallback(uint32 time)
{
    this->PostMessage(WM_USER_BET_TIME, 0, (LPARAM)time);
}

LRESULT CBetGameDlg::OnTipsMessage(WPARAM wParam, LPARAM lParam)
{
    std::wstring* param = (std::wstring*)(lParam);

    // ��ʾ�ڽ���
    m_list_msg.InsertString(message_count_++, param->c_str());

    delete param;
    return 0;
}

LRESULT CBetGameDlg::OnDisplayBetResult(WPARAM wParam, LPARAM lParam)
{
    BetResult* bet_result = (BetResult*)(lParam);
    uint32 result = bet_result->display_result;
    uint32 time = bet_result->time;
    base::Time server_time = base::Time::FromDoubleT(time);
    std::wstring str_time = base::UTF8ToWide(MakeFormatTimeString(server_time));

    // ��ʾ�ڽ���
    int itemcount = m_list_bet_data.GetItemCount();
    std::wstring wresult = base::UintToString16(result);
    int nitem = 0;
    nitem = m_list_bet_data.InsertItem(itemcount++, str_time.c_str());
    m_list_bet_data.SetItemText(nitem, result, wresult.c_str());
    delete bet_result;

    return 0;
}

LRESULT CBetGameDlg::OnDisplayBetTime(WPARAM wParam, LPARAM lParam)
{
    uint32 time = (uint32)(lParam);
    base::Time server_time = base::Time::FromDoubleT(time);
    std::wstring str_time = base::UTF8ToWide(MakeFormatTimeString(server_time));

    // ��ʾ�ڽ���
    m_list_msg.InsertString(message_count_++, str_time.c_str());

    return 0;
}