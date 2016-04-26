
// FanXingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <xutility>
#include "FanXing.h"
#include "FanXingDlg.h"
#include "afxdialogex.h"
#include "NetworkHelper.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/utf_string_conversions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace
{
    const wchar_t* columnlist[] = {
        L"�ǳ�",
        L"�Ƹ��ȼ�",
        L"�û�id",
        L"����ʱ��",
        L"�����",
        L"�������"
    };
}

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFanXingDlg �Ի���



CFanXingDlg::CFanXingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFanXingDlg::IDD, pParent)
    , network_(nullptr)
    , infoListCount_(0)
    , listCtrlRowIndex_(0)
    , m_query_key(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); 
}

CFanXingDlg::~CFanXingDlg()
{
    if (network_)
    {
        network_->Finalize();
    }  
}

void CFanXingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, InfoList_);
    DDX_Control(pDX, IDC_LIST_USER_STATUS, m_ListCtrl_Viewers);
    DDX_Text(pDX, IDC_EDIT_QUERY_KEYWORD, m_query_key);
    DDX_Control(pDX, IDC_CHECK_REMEMBER, m_check_remember);
    DDX_Control(pDX, IDC_LIST_USER_STATUS_BLACK, m_ListCtrl_Blacks);
}

BEGIN_MESSAGE_MAP(CFanXingDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CFanXingDlg::OnBnClickedButtonLogin)
    ON_BN_CLICKED(IDC_BUTTON_REWARSTAR, &CFanXingDlg::OnBnClickedButtonRewarstar)
    ON_BN_CLICKED(IDC_BUTTON_REWARDGIFT, &CFanXingDlg::OnBnClickedButtonRewardgift)
    ON_BN_CLICKED(IDC_BUTTON_NAV, &CFanXingDlg::OnBnClickedButtonNav)
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_BTN_GETMSG, &CFanXingDlg::OnBnClickedBtnGetmsg)
    ON_BN_CLICKED(IDC_BTN_ADD, &CFanXingDlg::OnBnClickedBtnAdd)
    ON_MESSAGE(WM_USER_01, &CFanXingDlg::OnNotifyMessage)
    ON_MESSAGE(WM_USER_ADD_ENTER_ROOM_INFO, &CFanXingDlg::OnDisplayDataToViewerList)
    ON_NOTIFY(HDN_ITEMCLICK, 0, &CFanXingDlg::OnHdnItemclickListUserStatus)
    ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CFanXingDlg::OnBnClickedButtonRemove)
    ON_BN_CLICKED(IDC_BTN_MODIFY, &CFanXingDlg::OnBnClickedBtnModify)
    ON_BN_CLICKED(IDC_BTN_QUERY, &CFanXingDlg::OnBnClickedBtnQuery)
    ON_BN_CLICKED(IDC_BTN_SELECT_ALL, &CFanXingDlg::OnBnClickedBtnSelectAll)
    ON_BN_CLICKED(IDC_BTN_SELECT_REVERSE, &CFanXingDlg::OnBnClickedBtnSelectReverse)
    ON_BN_CLICKED(IDC_BTN_KICKOUT_MONTH, &CFanXingDlg::OnBnClickedBtnKickoutMonth)
    ON_BN_CLICKED(IDC_BTN_KICKOUT_HOUR, &CFanXingDlg::OnBnClickedBtnKickoutHour)
    ON_BN_CLICKED(IDC_BTN_SILENT, &CFanXingDlg::OnBnClickedBtnSilent)
    ON_BN_CLICKED(IDC_BTN_UNSILENT, &CFanXingDlg::OnBnClickedBtnUnsilent)
    ON_BN_CLICKED(IDC_BTN_CLEAR, &CFanXingDlg::OnBnClickedBtnClear)
    ON_BN_CLICKED(IDC_BTN_GET_VIEWER_LIST, &CFanXingDlg::OnBnClickedBtnGetViewerList)
    ON_BN_CLICKED(IDC_BTN_KICKOUT_MONTH_BLACK, &CFanXingDlg::OnBnClickedBtnKickoutMonthBlack)
    ON_BN_CLICKED(IDC_BTN_KICKOUT_HOUR_BLACK, &CFanXingDlg::OnBnClickedBtnKickoutHourBlack)
    ON_BN_CLICKED(IDC_BTN_SILENT_BLACK, &CFanXingDlg::OnBnClickedBtnSilentBlack)
    ON_BN_CLICKED(IDC_BTN_UNSILENT_BLACK, &CFanXingDlg::OnBnClickedBtnUnsilentBlack)
    ON_BN_CLICKED(IDC_BTN_SELECT_ALL_BLACK, &CFanXingDlg::OnBnClickedBtnSelectAllBlack)
    ON_BN_CLICKED(IDC_BTN_SELECT_REVERSE_BLACK, &CFanXingDlg::OnBnClickedBtnSelectReverseBlack)
    ON_BN_CLICKED(IDC_BTN_REMOVE_BLACK, &CFanXingDlg::OnBnClickedBtnRemoveBlack)
    ON_BN_CLICKED(IDC_BTN_LOAD_BLACK, &CFanXingDlg::OnBnClickedBtnLoadBlack)
    ON_BN_CLICKED(IDC_BTN_ADD_TO_BLACK, &CFanXingDlg::OnBnClickedBtnAddToBlack)
END_MESSAGE_MAP()


// CFanXingDlg ��Ϣ�������

BOOL CFanXingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    DWORD dwStyle = m_ListCtrl_Viewers.GetExtendedStyle();
    dwStyle |= LVS_EX_CHECKBOXES;
    dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
    m_ListCtrl_Viewers.SetExtendedStyle(dwStyle); //������չ���

    SetDlgItemText(IDC_EDIT_NAV, L"0");
    SetDlgItemInt(IDC_EDIT_X, 0);
    SetDlgItemInt(IDC_EDIT_Y, 0);

    int nColumnCount = m_ListCtrl_Viewers.GetHeaderCtrl()->GetItemCount();
    for (int i = nColumnCount - 1; i >= 0; i--)
        m_ListCtrl_Viewers.DeleteColumn(i);

    uint32 i = 0;
    for (const auto& it : columnlist)
    {
        m_ListCtrl_Viewers.InsertColumn(i++, it, LVCFMT_LEFT, 100);//������
    }

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFanXingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFanXingDlg::OnPaint()
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

void CFanXingDlg::OnClose()
{
    if (network_)
    {
        network_->RemoveNotify();
    }  
}
//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFanXingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��¼����
void CFanXingDlg::OnBnClickedButtonLogin()
{
    CString username;
    CString password;
    GetDlgItemText(IDC_EDIT_Username, username);
    GetDlgItemText(IDC_EDIT_Password, password);

    // ����ͨ����curl��¼��ʽ
    bool result = LoginByRequest(username.GetBuffer(), password.GetBuffer());
    std::wstring message = std::wstring(L"login ") + (result ? L"success" : L"failed");
    Notify(message);
}

//��תҳ�湦��
void CFanXingDlg::OnBnClickedButtonNav()
{
    // �����ԭ������
    m_ListCtrl_Viewers.DeleteAllItems();

    CString strRoomid;
    GetDlgItemText(IDC_EDIT_NAV, strRoomid);
    base::StringToUint(strRoomid.GetBuffer(), &roomid_);

    network_->SetNotify(
        std::bind(&CFanXingDlg::Notify, this, std::placeholders::_1));

    network_->SetNotify201(
        std::bind(&CFanXingDlg::Notify201, this, std::placeholders::_1));

    network_->EnterRoom(strRoomid.GetBuffer());
}

// �����ǹ���
void CFanXingDlg::OnBnClickedButtonRewarstar()
{
}

// �����﹦��
void CFanXingDlg::OnBnClickedButtonRewardgift()
{
}

void CFanXingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialogEx::OnLButtonDown(nFlags, point);
}

// ��ȡ������Ϣ
void CFanXingDlg::OnBnClickedBtnGetmsg()
{
}

// ���������Եĺ���
void CFanXingDlg::OnBnClickedBtnAdd()
{
    RowData rowdata;
    rowdata.push_back(L"1");
    rowdata.push_back(L"2");
    rowdata.push_back(L"3");
    rowdata.push_back(L"4");
    rowdata.push_back(L"5");
    rowdata.push_back(L"1");

    int nitem = m_ListCtrl_Viewers.InsertItem(0, rowdata[0].c_str());
    m_ListCtrl_Viewers.SetItemData(nitem, listCtrlRowIndex_++);
    for (uint32 j = 1; j < rowdata.size(); ++j)
    {
        m_ListCtrl_Viewers.SetItemText(nitem, j, rowdata[j].c_str());
    }
}

void CFanXingDlg::Notify(const std::wstring& message)
{
    // �������ݸ�����
    messageMutex_.lock();
    messageQueen_.push_back(message);
    messageMutex_.unlock();
    this->PostMessage(WM_USER_01, 0, 0);
}

void CFanXingDlg::Notify201(const RowData& rowdata)
{
    // �������ݸ�����
    viewerRowdataMutex_.lock();
    viewerRowdataQueue_.push_back(rowdata);
    viewerRowdataMutex_.unlock();
    this->PostMessage(WM_USER_ADD_ENTER_ROOM_INFO, 0, 0);
}

bool CFanXingDlg::LoginByRequest(const std::wstring& username, const std::wstring& password)
{
    if (network_)
    {
        network_->Finalize();
    }
    network_.reset(new NetworkHelper);
    network_->Initialize();
    network_->SetNotify(
        std::bind(&CFanXingDlg::Notify, this, std::placeholders::_1));

    return network_->Login(username, password);
}

bool CFanXingDlg::GetSelectViewers(std::vector<EnterRoomUserInfo>* enterRoomUserInfos)
{
	if (!enterRoomUserInfos)
	{
		return false;
	}

	int count = m_ListCtrl_Viewers.GetItemCount();
	for (int i = count - 1; i >= 0; --i)
	{
		if (m_ListCtrl_Viewers.GetCheck(i))
		{
			// �����߳��������������
			EnterRoomUserInfo enterRoomUserInfo;
			uint32 roomid = 0;
			base::StringToUint(m_ListCtrl_Viewers.GetItemText(i, 4).GetBuffer(), &roomid);
			enterRoomUserInfo.roomid = roomid_;
			uint32 richlevel = 0;
			base::StringToUint(m_ListCtrl_Viewers.GetItemText(i, 1).GetBuffer(), &richlevel);
			enterRoomUserInfo.richlevel = richlevel;
			enterRoomUserInfo.nickname = base::WideToUTF8(m_ListCtrl_Viewers.GetItemText(i, 0).GetBuffer());
			uint32 userid = 0;
			base::StringToUint(m_ListCtrl_Viewers.GetItemText(i, 2).GetBuffer(), &userid);
			enterRoomUserInfo.userid = userid;
			enterRoomUserInfos->push_back(enterRoomUserInfo);
		}
	}
	return true;
}

bool CFanXingDlg::GetSelectBlacks(std::vector<EnterRoomUserInfo>* enterRoomUserInfos)
{
    if (!enterRoomUserInfos)
    {
        return false;
    }
    int count = m_ListCtrl_Blacks.GetItemCount();
    for (int i = count - 1; i >= 0; --i)
    {
        if (m_ListCtrl_Blacks.GetCheck(i))
        {
            // �����߳��������������
            EnterRoomUserInfo enterRoomUserInfo;
            enterRoomUserInfo.roomid = roomid_;
            enterRoomUserInfo.nickname = base::WideToUTF8(m_ListCtrl_Blacks.GetItemText(i, 0).GetBuffer());
            uint32 userid = 0;
            base::StringToUint(m_ListCtrl_Blacks.GetItemText(i, 1).GetBuffer(), &userid);
            enterRoomUserInfo.userid = userid;
            enterRoomUserInfos->push_back(enterRoomUserInfo);
        }
    }

    return true;
}

bool CFanXingDlg::KickOut(
    const std::vector<EnterRoomUserInfo>& enterRoomUserInfos,
    KICK_TYPE kicktype)
{
    for (const auto& enterRoomUserInfo : enterRoomUserInfos)
    {
        std::wstring msg = base::UTF8ToWide(enterRoomUserInfo.nickname);
        if (!network_->KickoutUsers(kicktype,
            enterRoomUserInfo.roomid, enterRoomUserInfo))
        {
            msg += L"�߳�ʧ��!Ȩ�޲������������!";
            InfoList_.InsertString(infoListCount_++, msg.c_str());
        }
        else
        {
            // ��Ҫɾ������Ϣ������־��¼�б���, id = 2 ���û�id				
            msg += L"���߳�һ����";
            InfoList_.InsertString(infoListCount_++, msg.c_str());
        }
    }

    return true;
}
// �����߳�ִ��
LRESULT CFanXingDlg::OnDisplayDataToViewerList(WPARAM wParam, LPARAM lParam)
{
    if (viewerRowdataQueue_.empty())
        return 0;
    
    std::vector<RowData> rowdatas;
    viewerRowdataMutex_.lock();
    rowdatas.swap(viewerRowdataQueue_);
    viewerRowdataMutex_.unlock();

    int itemcount = m_ListCtrl_Viewers.GetItemCount();

    for (uint32 i = 0; i < rowdatas.size(); ++i)
    {
        bool exist = false;
        // ����Ƿ������ͬ�û�id
        for (int index = 0; index < itemcount; index++)
        {
            CString text = m_ListCtrl_Viewers.GetItemText(index, 2);
            if (rowdatas[i][2].compare(text.GetBuffer()) == 0) // ��ͬ�û�id
            {
                // ���½��������ʾ�����������ݶ�ȫ��Ĭ�ϸ��µ�
                CString itemText = m_ListCtrl_Viewers.GetItemText(index, 5);
                std::string temp = base::WideToUTF8(itemText.GetBuffer());
                uint32 entercount = 0;
                base::StringToUint(temp, &entercount);
                entercount++;
                CString strEnterCount = base::UintToString16(entercount).c_str();
                m_ListCtrl_Viewers.SetItemText(index, 5, strEnterCount);

                for (uint32 j = 0; j < rowdatas[i].size(); ++j)
                {
                    m_ListCtrl_Viewers.SetItemText(itemcount + i, j, rowdatas[i][j].c_str());
                }
                exist = true;
                break;
            }
        }

        if (!exist) // ��������ڣ���Ҫ����������
        {
            int nitem = m_ListCtrl_Viewers.InsertItem(itemcount + i, rowdatas[i][0].c_str());
            //m_ListCtrl_UserStatus.SetItemData(nitem, i);
            for (uint32 j = 0; j < rowdatas[i].size(); ++j)
            {
                m_ListCtrl_Viewers.SetItemText(nitem, j, rowdatas[i][j].c_str());
            }
            m_ListCtrl_Viewers.SetItemText(nitem, 5, L"1"); // ��һ�μ�¼����
        }
    }

    return 0;
}

LRESULT CFanXingDlg::OnDisplayDtatToBlackList(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CFanXingDlg::OnNotifyMessage(WPARAM wParam, LPARAM lParam)
{
    std::vector<std::wstring> messages;
    messageMutex_.lock();
    messages.swap(messageQueen_);
    messageMutex_.unlock();

    for (auto str : messages)
    {
        InfoList_.InsertString(infoListCount_++, str.c_str());
    }
    
    InfoList_.SetCurSel(infoListCount_-1);
    return 0;
}

void CFanXingDlg::OnBnClickedButton2()
{
    m_ListCtrl_Viewers.SetCheck(0);
}

// �ȽϷ���
int CFanXingDlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    CListCtrl* ctlList = reinterpret_cast <CListCtrl*> (lParamSort);

    LV_FINDINFO lvi;
    memset(&lvi, 0, sizeof(lvi));
    lvi.flags = LVFI_PARAM;

    lvi.lParam = lParam1;
    int nItem1(ctlList->FindItem(&lvi));

    lvi.lParam = lParam2;
    int nItem2(ctlList->FindItem(&lvi));

    CString s1(ctlList->GetItemText(nItem1, 0));

    CString s2(ctlList->GetItemText(nItem2, 0));

    int nReturn(s1.CompareNoCase(s2));

    return nReturn > 0 ? 0:1;
}

// ����
void CFanXingDlg::OnHdnItemclickListUserStatus(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    
    m_ListCtrl_Viewers.SortItems(CompareFunc, reinterpret_cast <DWORD> (this));

    *pResult = 0;}


void CFanXingDlg::OnBnClickedButtonRemove()
{
    int count = m_ListCtrl_Viewers.GetItemCount();

    // �Ӻ���ǰɾ��
    for (int i = count - 1; i >= 0; --i)
    {
        if (m_ListCtrl_Viewers.GetCheck(i))
        {
            // ��Ҫɾ������Ϣ������־��¼�б���
            CString itemtext = m_ListCtrl_Viewers.GetItemText(i, 2);
            itemtext + L"�����б���ɾ��";
            InfoList_.InsertString(infoListCount_++, itemtext.GetBuffer());

            // ɾ���Ѿ���ѡ�ļ�¼
            m_ListCtrl_Viewers.DeleteItem(i);
        }
    }
}


void CFanXingDlg::OnBnClickedBtnModify()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFanXingDlg::OnBnClickedBtnQuery()
{
    UpdateData(TRUE);
    CString key = m_query_key;
    int count = m_ListCtrl_Viewers.GetItemCount();

    for (int i = count - 1; i >= 0; --i)
    {
        CString temp = m_ListCtrl_Viewers.GetItemText(i, 1);
        if (temp.Find(key)>=0)
        {
            m_ListCtrl_Viewers.SetCheck(i, TRUE);
        }
    }
}


void CFanXingDlg::OnBnClickedBtnSelectAll()
{
    int count = m_ListCtrl_Viewers.GetItemCount();

    for (int i = count - 1; i >= 0; --i)
    {
        m_ListCtrl_Viewers.SetCheck(i, 1);
    }
}

void CFanXingDlg::OnBnClickedBtnSelectReverse()
{
    int count = m_ListCtrl_Viewers.GetItemCount();

    for (int i = count - 1; i >= 0; --i)
    {
        if (m_ListCtrl_Viewers.GetCheck(i))
        {
            m_ListCtrl_Viewers.SetCheck(i, FALSE);
        }
        else
        {
            m_ListCtrl_Viewers.SetCheck(i, TRUE);
        }
    }
}


void CFanXingDlg::OnBnClickedBtnKickoutMonth()
{
	std::vector<EnterRoomUserInfo> enterRoomUserInfos;
	GetSelectViewers(&enterRoomUserInfos);
    KickOut(enterRoomUserInfos, KICK_TYPE::KICK_TYPE_MONTH);
}


void CFanXingDlg::OnBnClickedBtnKickoutHour()
{
	std::vector<EnterRoomUserInfo> enterRoomUserInfos;
	GetSelectViewers(&enterRoomUserInfos);
    KickOut(enterRoomUserInfos, KICK_TYPE::KICK_TYPE_HOUR);
}


void CFanXingDlg::OnBnClickedBtnSilent()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFanXingDlg::OnBnClickedBtnUnsilent()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFanXingDlg::OnBnClickedBtnClear()
{
    int count = m_ListCtrl_Viewers.GetItemCount();
    // �Ӻ���ǰɾ��
    for (int i = count - 1; i >= 0; --i)
    {
        m_ListCtrl_Viewers.DeleteItem(i);
    }

    CString itemtext = L"����б�";
    InfoList_.InsertString(infoListCount_++, itemtext.GetBuffer());
}


void CFanXingDlg::OnBnClickedBtnGetViewerList()
{
    std::vector<RowData> enterRoomUserInfoRowdata;
    if (!network_->GetViewerList(roomid_, &enterRoomUserInfoRowdata))
    {
        return;
    }

    viewerRowdataMutex_.lock();
    for (const auto& rowdata : enterRoomUserInfoRowdata)
    {
        viewerRowdataQueue_.push_back(rowdata);
    }   
    viewerRowdataMutex_.unlock();
    this->PostMessage(WM_USER_ADD_ENTER_ROOM_INFO, 0, 0);

}


void CFanXingDlg::OnBnClickedBtnKickoutMonthBlack()
{
    std::vector<EnterRoomUserInfo> enterRoomUserInfos;
    GetSelectBlacks(&enterRoomUserInfos);
    KickOut(enterRoomUserInfos, KICK_TYPE::KICK_TYPE_MONTH);
}


void CFanXingDlg::OnBnClickedBtnKickoutHourBlack()
{
    std::vector<EnterRoomUserInfo> enterRoomUserInfos;
    GetSelectBlacks(&enterRoomUserInfos);
    KickOut(enterRoomUserInfos, KICK_TYPE::KICK_TYPE_HOUR);
}


void CFanXingDlg::OnBnClickedBtnSilentBlack()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFanXingDlg::OnBnClickedBtnUnsilentBlack()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFanXingDlg::OnBnClickedBtnSelectAllBlack()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFanXingDlg::OnBnClickedBtnSelectReverseBlack()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFanXingDlg::OnBnClickedBtnRemoveBlack()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFanXingDlg::OnBnClickedBtnLoadBlack()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CFanXingDlg::OnBnClickedBtnAddToBlack()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}
