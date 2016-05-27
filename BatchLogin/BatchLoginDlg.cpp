
// BatchLoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BatchLogin.h"
#include "BatchLoginDlg.h"
#include "UserRoomManager.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace{
    const wchar_t* usercolumnlist[] = {
        L"�û���",
        L"����",
        L"�û�id",
        L"�ǳ�",
        L"�Ƹ��ȼ�",
        L"������"
    };

    const wchar_t* roomcolumnlist[] = {
        L"�����",
        L"Э������",
        L"��ʾ����",
        L"��Ч��",
    };
}

// CBatchLoginDlg �Ի���

CBatchLoginDlg::CBatchLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBatchLoginDlg::IDD, pParent)
    , userRoomManager_(nullptr)
{
    userRoomManager_.reset(new UserRoomManager);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CBatchLoginDlg::~CBatchLoginDlg()
{

}

void CBatchLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_USERS, m_ListCtrl_Users);
    DDX_Control(pDX, IDC_LIST_ROOM, m_ListCtrl_Rooms);
}

BEGIN_MESSAGE_MAP(CBatchLoginDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_IMPORT_USER, &CBatchLoginDlg::OnBnClickedBtnImportUser)
    ON_BN_CLICKED(IDC_BTN_GET_PROXY, &CBatchLoginDlg::OnBnClickedBtnGetProxy)
    ON_BN_CLICKED(IDC_BTN_BATCH_ENTER_ROOM, &CBatchLoginDlg::OnBnClickedBtnBatchEnterRoom)
    ON_BN_CLICKED(IDC_BTN_IMPORT_ROOM, &CBatchLoginDlg::OnBnClickedBtnImportRoom)
    ON_MESSAGE(WM_USER_NOTIFY_MESSAGE, &CBatchLoginDlg::OnNotifyMessage)
    ON_MESSAGE(WM_USER_USER_LIST_INFO, &CBatchLoginDlg::OnDisplayDataToUserList)
    ON_MESSAGE(WM_USER_ROOM_LIST_INFO, &CBatchLoginDlg::OnDisplayDataToRoomList)

    ON_BN_CLICKED(IDC_BTN_LOGIN, &CBatchLoginDlg::OnBnClickedBtnLogin)
END_MESSAGE_MAP()


// CBatchLoginDlg ��Ϣ�������

BOOL CBatchLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

    DWORD dwStyle = m_ListCtrl_Users.GetExtendedStyle();
    dwStyle |= LVS_EX_CHECKBOXES;
    dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��

    m_ListCtrl_Users.SetExtendedStyle(dwStyle); //������չ���
    int nColumnCount = m_ListCtrl_Users.GetHeaderCtrl()->GetItemCount();
    for (int i = nColumnCount - 1; i >= 0; i--)
        m_ListCtrl_Users.DeleteColumn(i);
    uint32 index = 0;
    for (const auto& it : usercolumnlist)
        m_ListCtrl_Users.InsertColumn(index++, it, LVCFMT_LEFT, 100);//������


    m_ListCtrl_Rooms.SetExtendedStyle(dwStyle); //������չ���
    nColumnCount = m_ListCtrl_Rooms.GetHeaderCtrl()->GetItemCount();
    for (int i = nColumnCount - 1; i >= 0; i--)
        m_ListCtrl_Rooms.DeleteColumn(i);
    index = 0;
    for (const auto& it : roomcolumnlist)
        m_ListCtrl_Rooms.InsertColumn(index++, it, LVCFMT_LEFT, 100);//������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBatchLoginDlg::OnPaint()
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
HCURSOR CBatchLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBatchLoginDlg::OnBnClickedBtnImportUser()
{
    GridData griddata;
    uint32 total = 0;
    if (!userRoomManager_->LoadUserConfig(&griddata, &total))
        return;
    
    assert(griddata.size() == total);

    // ��ʾ���ݵ�����
    int itemcount = m_ListCtrl_Users.GetItemCount();

    for (uint32 i = 0; i < griddata.size(); ++i)
    {
        bool exist = false;
        // ����Ƿ������ͬ�û�id
        for (int index = 0; index < itemcount; index++)
        {
            CString text = m_ListCtrl_Users.GetItemText(index, 0);
            if (griddata[i][0].compare(text.GetBuffer()) == 0) // ��ͬ�û���
            {
                exist = true;
                break;
            }
        }

        if (!exist) // ��������ڣ���Ҫ����������
        {
            int nitem = m_ListCtrl_Users.InsertItem(itemcount + i, griddata[i][0].c_str());
            //m_ListCtrl_UserStatus.SetItemData(nitem, i);
            for (uint32 j = 0; j < griddata[i].size(); ++j)
            {
                m_ListCtrl_Users.SetItemText(nitem, j, griddata[i][j].c_str());
            }
        }
    }
}

void CBatchLoginDlg::OnBnClickedBtnLogin()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CBatchLoginDlg::OnBnClickedBtnImportRoom()
{
    GridData griddata;
    uint32 total = 0;
    if (!userRoomManager_->LoadRoomConfig(&griddata, &total))
        return;

    assert(griddata.size() == total);

    // ��ʾ���ݵ�����
    int itemcount = m_ListCtrl_Rooms.GetItemCount();

    for (uint32 i = 0; i < griddata.size(); ++i)
    {
        bool exist = false;
        // ����Ƿ������ͬ�û�id
        for (int index = 0; index < itemcount; index++)
        {
            CString text = m_ListCtrl_Rooms.GetItemText(index, 0);
            if (griddata[i][0].compare(text.GetBuffer()) == 0) // ��ͬ�����
            {
                exist = true;
                break;
            }
        }

        if (!exist) // ��������ڣ���Ҫ����������
        {
            int nitem = m_ListCtrl_Rooms.InsertItem(itemcount + i, griddata[i][0].c_str());
            //m_ListCtrl_UserStatus.SetItemData(nitem, i);
            for (uint32 j = 0; j < griddata[i].size(); ++j)
            {
                m_ListCtrl_Rooms.SetItemText(nitem, j, griddata[i][j].c_str());
            }
        }
    }
}

void CBatchLoginDlg::OnBnClickedBtnGetProxy()
{

}

void CBatchLoginDlg::OnBnClickedBtnBatchEnterRoom()
{
    userRoomManager_->FillConfigRooms();
}

LRESULT CBatchLoginDlg::OnNotifyMessage(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CBatchLoginDlg::OnDisplayDataToUserList(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

LRESULT CBatchLoginDlg::OnDisplayDataToRoomList(WPARAM wParam, LPARAM lParam)
{
    return 0;
}



