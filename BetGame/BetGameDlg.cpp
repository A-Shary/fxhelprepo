
// BetGameDlg.cpp : 实现文件
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


// CBetGameDlg 对话框
namespace
{
    const wchar_t* betcolumnlist[] = {
        L"时间",
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
    DDX_Control(pDX, IDC_BTN_RESULT_1, m_btn_basic[0]);
    DDX_Control(pDX, IDC_BTN_RESULT_2, m_btn_basic[1]);
    DDX_Control(pDX, IDC_BTN_RESULT_7, m_btn_basic[6]);
    DDX_Control(pDX, IDC_BTN_RESULT_8, m_btn_basic[7]);
    DDX_Control(pDX, IDC_BTN_RESULT_3, m_btn_basic[2]);
    DDX_Control(pDX, IDC_BTN_RESULT_4, m_btn_basic[3]);
    DDX_Control(pDX, IDC_BTN_RESULT_5, m_btn_basic[4]);
    DDX_Control(pDX, IDC_BTN_RESULT_6, m_btn_basic[5]);
    DDX_Control(pDX, IDC_STATIC_SPACE1, m_static_space[0]);
    DDX_Control(pDX, IDC_STATIC_SPACE2, m_static_space[1]);
    DDX_Control(pDX, IDC_STATIC_SPACE3, m_static_space[2]);
    DDX_Control(pDX, IDC_STATIC_SPACE4, m_static_space[3]);
    DDX_Control(pDX, IDC_STATIC_SPACE5, m_static_space[4]);
    DDX_Control(pDX, IDC_STATIC_SPACE6, m_static_space[5]);
    DDX_Control(pDX, IDC_STATIC_SPACE7, m_static_space[6]);
    DDX_Control(pDX, IDC_STATIC_SPACE8, m_static_space[7]);
    DDX_Control(pDX, IDC_STATIC_COUNT1, m_static_count[0]);
    DDX_Control(pDX, IDC_STATIC_COUNT2, m_static_count[1]);
    DDX_Control(pDX, IDC_STATIC_COUNT3, m_static_count[2]);
    DDX_Control(pDX, IDC_STATIC_COUNT4, m_static_count[3]);
    DDX_Control(pDX, IDC_STATIC_COUNT5, m_static_count[4]);
    DDX_Control(pDX, IDC_STATIC_COUNT6, m_static_count[5]);
    DDX_Control(pDX, IDC_STATIC_COUNT7, m_static_count[6]);
    DDX_Control(pDX, IDC_STATIC_COUNT8, m_static_count[7]);
    DDX_Control(pDX, IDC_EDIT_DISPLAY, m_edit_display_result);
}

BEGIN_MESSAGE_MAP(CBetGameDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_NAV, &CBetGameDlg::OnBnClickedButtonEnterRoom)
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CBetGameDlg::OnBnClickedButtonLogin)
    ON_MESSAGE(WM_USER_TIPS, &CBetGameDlg::OnTipsMessage)
    ON_MESSAGE(WM_USER_BET_RESULT, &CBetGameDlg::OnDisplayBetResult)
    ON_MESSAGE(WM_USER_BET_TIME, &CBetGameDlg::OnDisplayBetTime)
    ON_NOTIFY(HDN_BEGINTRACK, 0, &CBetGameDlg::OnHdnBegintrackListBetdata)
END_MESSAGE_MAP()


// CBetGameDlg 消息处理程序

BOOL CBetGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
    DWORD dwStyle = m_list_bet_data.GetExtendedStyle();
    //dwStyle |= LVS_EX_CHECKBOXES;
    dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
    dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）

    m_list_bet_data.SetExtendedStyle(dwStyle); //设置扩展风格
    int nColumnCount = m_list_bet_data.GetHeaderCtrl()->GetItemCount();
    for (int i = nColumnCount - 1; i >= 0; i--)
        m_list_bet_data.DeleteColumn(i);
    int index = 0;
    RECT rect;
    m_list_bet_data.GetWindowRect(&rect);
    int time_width = 60;
    int width = (rect.right - rect.left - time_width) / ((sizeof(betcolumnlist) / sizeof(betcolumnlist[0])-1));
    bool first = true;
    for (const auto& it : betcolumnlist)
    {
        if (first)
        {
            first = false;
            m_list_bet_data.InsertColumn(index++, it, LVCFMT_LEFT, time_width);//插入列
        }
        else
        {
            m_list_bet_data.InsertColumn(index++, it, LVCFMT_LEFT, width);//插入列
        }
    }
        

    m_list_msg.InsertString(message_count_++, L"程序启动");

    m_edit_username.SetWindowTextW(L"fanxingtest002");
    m_edit_password.SetWindowTextW(L"1233211234567");
    m_edit_room_id.SetWindowTextW(L"1201793");

    CreateFont();

    COLORREF bk_color = RGB(192, 192, 192);
    int count = sizeof(m_btn_basic) / sizeof(m_btn_basic[0]);
    for (int index = 0; index < count; index++)
    {
        m_btn_basic[index].SetText(base::IntToString16(index+1).c_str());
        m_btn_basic[index].SetBkColor(bk_color);
        m_btn_basic[index].SetFont(&font_bet_basic_, TRUE);

        m_static_space[index].SetWindowTextW(L"0");
        m_static_count[index].SetWindowTextW(L"0");
    }

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBetGameDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
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
    bet_network_->EnterRoom(room_id);
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
        // 保存登录成功配置
    }
}

void CBetGameDlg::TipMessageCallback(const std::wstring& message)
{
    std::wstring* param = new std::wstring(message);
    this->PostMessage(WM_USER_TIPS, 0, (LPARAM)param);
}

void CBetGameDlg::DisplayBetResultCallback(const CaculationData& bet_result)
{
    CaculationData* param = new CaculationData(bet_result);
    this->PostMessage(WM_USER_BET_RESULT, 0, (LPARAM)param);
}

void CBetGameDlg::DisplayBetTimeCallback(uint32 time)
{
    this->PostMessage(WM_USER_BET_TIME, 0, (LPARAM)time);
}

LRESULT CBetGameDlg::OnTipsMessage(WPARAM wParam, LPARAM lParam)
{
    std::wstring* param = (std::wstring*)(lParam);

    // 显示在界面
    m_list_msg.InsertString(message_count_++, param->c_str());

    delete param;
    return 0;
}

LRESULT CBetGameDlg::OnDisplayBetResult(WPARAM wParam, LPARAM lParam)
{
    CaculationData * caculation_data = (CaculationData*)(lParam);
    BetResult* bet_result = &caculation_data->bet_result;
    uint32 result = bet_result->display_result;
    uint32 time = bet_result->time;
    base::Time server_time = base::Time::FromDoubleT(time);
    std::wstring str_time = base::UTF8ToWide(MakeFormatTimeString(server_time));

    // 显示在界面
    int itemcount = m_list_bet_data.GetItemCount();
    std::wstring wresult = base::UintToString16(result);
    int nitem = 0;
    nitem = m_list_bet_data.InsertItem(itemcount++, str_time.c_str());
    m_list_bet_data.SetItemText(nitem, result, wresult.c_str());

    int count = sizeof(m_btn_basic) / sizeof(m_btn_basic[0]);
    for (int index = 0; index < count; index++)
    {
        auto distance = caculation_data->distance[index];
        m_static_space[index].SetWindowTextW(base::UintToString16(distance).c_str());
        auto summary = caculation_data->summary[index];
        m_static_count[index].SetWindowTextW(base::UintToString16(summary).c_str());
    }

    delete caculation_data;
    return 0;
}

LRESULT CBetGameDlg::OnDisplayBetTime(WPARAM wParam, LPARAM lParam)
{
    uint32 time = (uint32)(lParam);
    base::Time server_time = base::Time::FromDoubleT(time);
    std::wstring str_time = base::UTF8ToWide(MakeFormatTimeString(server_time));

    // 显示在界面
    m_list_msg.InsertString(message_count_++, str_time.c_str());

    return 0;
}

void CBetGameDlg::OnHdnBegintrackListBetdata(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    // 保证列头不能拖动
    *pResult = TRUE;
}

void CBetGameDlg::CreateFont()
{

    font_bet_basic_.CreateFont(18,                        // nHeight
                               0,                         // nWidth
                               0,                         // nEscapement
                               0,                         // nOrientation
                               FW_BOLD,                   // nWeight
                               FALSE,                     // bItalic
                               FALSE,                     // bUnderline
                               0,                         // cStrikeOut
                               DEFAULT_CHARSET,           // nCharSet
                               OUT_CHARACTER_PRECIS,        // nOutPrecision
                               CLIP_DEFAULT_PRECIS,       // nClipPrecision
                               DEFAULT_QUALITY,           // nQuality
                               DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                               TEXT("黑体"));             // lpszFacename

    font_bet_count_.CreateFont(12,                        // nHeight
                               0,                         // nWidth
                               0,                         // nEscapement
                               0,                         // nOrientation
                               FW_BOLD,                   // nWeight
                               FALSE,                     // bItalic
                               FALSE,                     // bUnderline
                               0,                         // cStrikeOut
                               DEFAULT_CHARSET,           // nCharSet
                               OUT_CHARACTER_PRECIS,        // nOutPrecision
                               CLIP_DEFAULT_PRECIS,       // nClipPrecision
                               DEFAULT_QUALITY,           // nQuality
                               DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                               TEXT("楷体"));             // lpszFacename

    font_bet_space_.CreateFont(12,                        // nHeight
                               0,                         // nWidth
                               0,                         // nEscapement
                               0,                         // nOrientation
                               FW_BOLD,                   // nWeight
                               FALSE,                     // bItalic
                               FALSE,                     // bUnderline
                               0,                         // cStrikeOut
                               DEFAULT_CHARSET,           // nCharSet
                               OUT_CHARACTER_PRECIS,        // nOutPrecision
                               CLIP_DEFAULT_PRECIS,       // nClipPrecision
                               DEFAULT_QUALITY,           // nQuality
                               DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
                               TEXT("楷体"));             // lpszFacename

}