
// FamilyDataCenterUIDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "FamilyDataCenterUI/FamilyDataCenterUI.h"
#include "FamilyDataCenterUI/FamilyDataCenterUIDlg.h"
#include "FamilyDataCenterUI/FamilyDataController.h"
#include "FamilyDataCenterUI/FamilyDataModle.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
#include "third_party/chromium/base/time/time.h"

namespace
{
    bool OleDateTimeToBaseTime(const COleDateTime& oletime, base::Time* basetime)
    {
        if (!basetime)
            return false;

        SYSTEMTIME systemtime;
        if (!oletime.GetAsSystemTime(systemtime))
            return false;
        
        FILETIME filetime;
        if (!SystemTimeToFileTime(&systemtime, &filetime))
            return false;
        
        *basetime = base::Time::FromFileTime(filetime);
        return true;
    }
}

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


// CFamilyDataCenterUIDlg �Ի���



CFamilyDataCenterUIDlg::CFamilyDataCenterUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFamilyDataCenterUIDlg::IDD, pParent)
    , familyDataController_(nullptr)
    , familyDataModle_(nullptr)
    , m_oleDateTime_Begin(COleDateTime::GetCurrentTime())
    , m_oleDateTime_End(COleDateTime::GetCurrentTime())
    , m_username(_T(""))
    , m_password(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFamilyDataCenterUIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_SUMMARY_DATA, m_ListCtrl_SummaryData);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_BEGIN, m_oleDateTime_Begin);
    DDX_DateTimeCtrl(pDX, IDC_DATETIMEPICKER_END, m_oleDateTime_End);
    DDX_Text(pDX, IDC_EDIT_USERNAME, m_username);
    DDX_Text(pDX, IDC_EDIT_PASSWORD, m_password);
}

BEGIN_MESSAGE_MAP(CFamilyDataCenterUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_GET_FAMILY_DATA, &CFamilyDataCenterUIDlg::OnBnClickedGetFamilyData)
    ON_BN_CLICKED(IDC_BTN_EXPORT_TO_EXCEL, &CFamilyDataCenterUIDlg::OnBnClickedBtnExportToExcel)
    ON_BN_CLICKED(IDC_BTN_LOGIN, &CFamilyDataCenterUIDlg::OnBnClickedBtnLogin)
END_MESSAGE_MAP()


// CFamilyDataCenterUIDlg ��Ϣ�������

BOOL CFamilyDataCenterUIDlg::OnInitDialog()
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    DWORD dwStyle = m_ListCtrl_SummaryData.GetExtendedStyle();
    dwStyle |= LVS_REPORT;
    dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
    m_ListCtrl_SummaryData.SetExtendedStyle(dwStyle); //������չ���

    std::vector<std::wstring> columnlist = {
        L"����",
        L"�����ȼ�",
        L"��������",
        L"�ۼ�ֱ��",
        L"��Чֱ��",
        L"ֱ�����������",
        L"�Ƕ�����"
    };

    uint32 i = 0;
    for (const auto& it : columnlist)
    {
        m_ListCtrl_SummaryData.InsertColumn(i++, it.c_str(), LVCFMT_LEFT, 140);//������
    }

    familyDataController_.reset(new FamilyDataController);
    familyDataModle_.reset(new FamilyDataModle);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFamilyDataCenterUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFamilyDataCenterUIDlg::OnPaint()
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
HCURSOR CFamilyDataCenterUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CFamilyDataCenterUIDlg::OnBnClickedGetFamilyData()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    base::Time beginTime;
    base::Time endTime;
    OleDateTimeToBaseTime(m_oleDateTime_Begin, &beginTime);
    OleDateTimeToBaseTime(m_oleDateTime_End, &endTime);
    GridData griddata;
    if (!familyDataController_->GetSingerFamilyData(beginTime, endTime, &griddata))
    {
        // ������־��Ϣ
        return;
    }
    DisplayDataToGrid(griddata);
}

void CFamilyDataCenterUIDlg::DisplayDataToGrid(const GridData& griddata)
{  
    // ����Ҫ������ʾ�ڽ�����������ݹ���

    UpdateData(FALSE);
    return;
}

void CFamilyDataCenterUIDlg::OnBnClickedBtnExportToExcel()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    familyDataController_->ExportToExcel();
    //familyDataController_->ExportToTxt();
}


void CFamilyDataCenterUIDlg::OnBnClickedBtnLogin()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    familyDataController_->Login(m_username.GetString(), m_password.GetString());
}
