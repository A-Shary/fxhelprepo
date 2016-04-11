
// FanXingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <xutility>
#include "FanXing.h"
#include "FanXingDlg.h"
#include "afxdialogex.h"
#include "NetworkHelper.h"
#include "WebHandler.h"
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
    DDX_Control(pDX, IDC_LIST_USER_STATUS, m_ListCtrl_UserStatus);
    DDX_Text(pDX, IDC_EDIT_QUERY_KEYWORD, m_query_key);
}

BEGIN_MESSAGE_MAP(CFanXingDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CFanXingDlg::OnBnClickedButtonLogin)
    ON_BN_CLICKED(IDC_BUTTON_CLICK, &CFanXingDlg::OnBnClickedButtonClick)
    ON_BN_CLICKED(IDC_BUTTON_REWARSTAR, &CFanXingDlg::OnBnClickedButtonRewarstar)
    ON_BN_CLICKED(IDC_BUTTON_REWARDGIFT, &CFanXingDlg::OnBnClickedButtonRewardgift)
    ON_BN_CLICKED(IDC_BUTTON_NAV, &CFanXingDlg::OnBnClickedButtonNav)
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_BTN_GETMSG, &CFanXingDlg::OnBnClickedBtnGetmsg)
    ON_BN_CLICKED(IDC_BTN_ADD, &CFanXingDlg::OnBnClickedBtnAdd)
    ON_MESSAGE(WM_USER_01, &CFanXingDlg::OnNotifyMessage)
    ON_MESSAGE(WM_USER_ADD_ENTER_ROOM_INFO, &CFanXingDlg::OnDisplayDataToGrid)
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

    DWORD dwStyle = m_ListCtrl_UserStatus.GetExtendedStyle();
    dwStyle |= LVS_EX_CHECKBOXES;
    dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
    m_ListCtrl_UserStatus.SetExtendedStyle(dwStyle); //������չ���

    SetDlgItemText(IDC_EDIT_NAV, L"1053637");
    SetDlgItemInt(IDC_EDIT_X, 0);
    SetDlgItemInt(IDC_EDIT_Y, 0);

    int nColumnCount = m_ListCtrl_UserStatus.GetHeaderCtrl()->GetItemCount();
    for (int i = nColumnCount - 1; i >= 0; i--)
        m_ListCtrl_UserStatus.DeleteColumn(i);

    uint32 i = 0;
    for (const auto& it : columnlist)
    {
        m_ListCtrl_UserStatus.InsertColumn(i++, it, LVCFMT_LEFT, 100);//������
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
    m_ListCtrl_UserStatus.DeleteAllItems();

    CString strRoomid;
    GetDlgItemText(IDC_EDIT_NAV, strRoomid);
    network_->SetNotify(
        std::bind(&CFanXingDlg::Notify, this, std::placeholders::_1));

    network_->SetNotify201(
        std::bind(&CFanXingDlg::Notify201, this, std::placeholders::_1));

    network_->EnterRoom(strRoomid.GetBuffer());
}

//ָ��λ�õ������
void CFanXingDlg::OnBnClickedButtonClick()
{

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

    int nitem = m_ListCtrl_UserStatus.InsertItem(0, rowdata[0].c_str());
    m_ListCtrl_UserStatus.SetItemData(nitem, listCtrlRowIndex_++);
    for (uint32 j = 1; j < rowdata.size(); ++j)
    {
        m_ListCtrl_UserStatus.SetItemText(nitem, j, rowdata[j].c_str());
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
    rowdataMutex_.lock();
    rowdataQueue_.push_back(rowdata);
    rowdataMutex_.unlock();
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

// �����߳�ִ��
LRESULT CFanXingDlg::OnDisplayDataToGrid(WPARAM wParam, LPARAM lParam)
{
    if (rowdataQueue_.empty())
        return 0;
    
    std::vector<RowData> rowdatas;
    rowdataMutex_.lock();
    rowdatas.swap(rowdataQueue_);
    rowdataMutex_.unlock();

    int itemcount = m_ListCtrl_UserStatus.GetItemCount();

    for (uint32 i = 0; i < rowdatas.size(); ++i)
    {
        bool exist = false;
        // ����Ƿ������ͬ�û�id
        for (int index = 0; index < itemcount; index++)
        {
            CString text = m_ListCtrl_UserStatus.GetItemText(index, 3);
            if (rowdatas[i][3].compare(text.GetBuffer()) == 0) // ��ͬ�û�id
            {
                // ���½��������ʾ�����������ݶ�ȫ��Ĭ�ϸ��µ�
                CString itemText = m_ListCtrl_UserStatus.GetItemText(index, 6);
                std::string temp = base::WideToUTF8(itemText.GetBuffer());
                uint32 entercount = 0;
                base::StringToUint(temp, &entercount);
                entercount++;
                CString strEnterCount = base::UintToString16(entercount).c_str();
                m_ListCtrl_UserStatus.SetItemText(index, 6, strEnterCount);

                for (uint32 j = 0; j < rowdatas[i].size(); ++j)
                {
                    m_ListCtrl_UserStatus.SetItemText(itemcount + i, j, rowdatas[i][j].c_str());
                }
                exist = true;
                break;
            }
        }

        if (!exist) // ��������ڣ���Ҫ����������
        {
            int nitem = m_ListCtrl_UserStatus.InsertItem(itemcount + i, rowdatas[i][0].c_str());
            m_ListCtrl_UserStatus.SetItemData(nitem, i);
            for (uint32 j = 1; j < rowdatas[i].size(); ++j)
            {
                m_ListCtrl_UserStatus.SetItemText(itemcount + nitem, j, rowdatas[i][j].c_str());
            }
            m_ListCtrl_UserStatus.SetItemText(nitem, 6, L"1"); // ��һ�μ�¼����
        }
    }

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


    m_ListCtrl_UserStatus.SetCheck(0);
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
    
    m_ListCtrl_UserStatus.SortItems(CompareFunc, reinterpret_cast <DWORD> (this));

    *pResult = 0;}


void CFanXingDlg::OnBnClickedButtonRemove()
{
    int count = m_ListCtrl_UserStatus.GetItemCount();

    // �Ӻ���ǰɾ��
    for (int i = count - 1; i >= 0; --i)
    {
        if (m_ListCtrl_UserStatus.GetCheck(i))
        {
            // ��Ҫɾ������Ϣ������־��¼�б���
            CString itemtext = m_ListCtrl_UserStatus.GetItemText(i, 2);
            itemtext + L"�����б���ɾ��";
            InfoList_.InsertString(infoListCount_++, itemtext.GetBuffer());

            // ɾ���Ѿ���ѡ�ļ�¼
            m_ListCtrl_UserStatus.DeleteItem(i);
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
    int count = m_ListCtrl_UserStatus.GetItemCount();

    for (int i = count - 1; i >= 0; --i)
    {
        CString temp = m_ListCtrl_UserStatus.GetItemText(i, 3);
        if (temp.Find(key)>=0)
        {
            m_ListCtrl_UserStatus.SetCheck(i, TRUE);
        }
    }
}


void CFanXingDlg::OnBnClickedBtnSelectAll()
{
    int count = m_ListCtrl_UserStatus.GetItemCount();

    for (int i = count - 1; i >= 0; --i)
    {
        m_ListCtrl_UserStatus.SetCheck(i, 1);
    }
}


void CFanXingDlg::OnBnClickedBtnSelectReverse()
{
    int count = m_ListCtrl_UserStatus.GetItemCount();

    for (int i = count - 1; i >= 0; --i)
    {
        if (m_ListCtrl_UserStatus.GetCheck(i))
        {
            m_ListCtrl_UserStatus.SetCheck(i,FALSE);
        }
        else
        {
            m_ListCtrl_UserStatus.SetCheck(i, TRUE);
        }
    }
}


void CFanXingDlg::OnBnClickedBtnKickoutMonth()
{
    int count = m_ListCtrl_UserStatus.GetItemCount();

    // �Ӻ���ǰɾ��
    for (int i = count - 1; i >= 0; --i)
    {
        if (m_ListCtrl_UserStatus.GetCheck(i))
        {
            // �����߳��������������

            // ��Ҫɾ������Ϣ������־��¼�б���
            CString itemtext = m_ListCtrl_UserStatus.GetItemText(i, 2);
            itemtext += L"���߳�һ���£������б���ɾ��";
            InfoList_.InsertString(infoListCount_++, itemtext.GetBuffer());

            // ɾ���Ѿ���ѡ�ļ�¼
            m_ListCtrl_UserStatus.DeleteItem(i);
        }
    }
}


void CFanXingDlg::OnBnClickedBtnKickoutHour()
{
    EnterRoomUserInfo enterRoomUserInfo;
    enterRoomUserInfo.roomid = 1053637;
    enterRoomUserInfo.richlevel = 1;
    enterRoomUserInfo.nickname = "fanxingtest111";
    enterRoomUserInfo.userid = 120831944;
    network_->KickoutUsers(110468466, enterRoomUserInfo);
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
    int count = m_ListCtrl_UserStatus.GetItemCount();
    // �Ӻ���ǰɾ��
    for (int i = count - 1; i >= 0; --i)
    {
        m_ListCtrl_UserStatus.DeleteItem(i);
    }

    CString itemtext = L"����б�";
    InfoList_.InsertString(infoListCount_++, itemtext.GetBuffer());
}
