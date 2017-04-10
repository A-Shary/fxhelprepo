
// HousingGzccFetchDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HousingGzccFetch.h"
#include "HousingGzccFetchDlg.h"
#include "afxdialogex.h"

#include "Network/EncodeHelper.h"
#include "ExcelHelper.h"
#include "HousingRequest.h"

#include "third_party/chromium/base/path_service.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/files/file.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHousingGzccFetchDlg �Ի���



CHousingGzccFetchDlg::CHousingGzccFetchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHousingGzccFetchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHousingGzccFetchDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_PAGE_NUMBERS, m_edit_max_pages);
    DDX_Control(pDX, IDC_LIST_DISPLAY, m_list_display);
}

BEGIN_MESSAGE_MAP(CHousingGzccFetchDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_FETCH_DATA, &CHousingGzccFetchDlg::OnBnClickedBtnFetchData)
    ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_DISPLAY, &CHousingGzccFetchDlg::OnLvnGetdispinfoListSummaryData)
    ON_BN_CLICKED(IDC_BTN_EXPORT, &CHousingGzccFetchDlg::OnBnClickedBtnExport)
END_MESSAGE_MAP()


// CHousingGzccFetchDlg ��Ϣ�������

BOOL CHousingGzccFetchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
    base::FilePath path;
    PathService::Get(base::DIR_EXE, &path);
    exePath_ = path;

    LOG(INFO) << __FUNCTION__;

    DWORD dwStyle = m_list_display.GetExtendedStyle();
    dwStyle |= LVS_REPORT;
    dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
    dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
    dwStyle |= LVS_OWNERDATA;
    dwStyle |= LVS_AUTOARRANGE;
    m_list_display.SetExtendedStyle(dwStyle); //������չ���
    m_edit_max_pages.SetWindowTextW(L"1");

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CHousingGzccFetchDlg::OnPaint()
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
HCURSOR CHousingGzccFetchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHousingGzccFetchDlg::OnBnClickedBtnFetchData()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������

    CString cs_max_pages;
    m_edit_max_pages.GetWindowTextW(cs_max_pages);
    std::string s_max_page = base::WideToUTF8(cs_max_pages.GetBuffer());
    uint32 max_pages = 1;
    if (!s_max_page.empty())
    {
        base::StringToUint(s_max_page, &max_pages);
    }

    HousingRequest house_request;
    std::vector<std::string> headers;
    std::list<std::vector<std::string>> record_list;
    if (!house_request.GetYszResult(&headers, &record_list, max_pages))
        return;
    
    std::vector<std::wstring> columnlist;
    for (const auto& header : headers)
    {
        columnlist.push_back(GBKToWide(header));
    }

    GridData grid_data;
    for (const auto& record : record_list)
    {
        RowData row_data;
        for (const auto& item : record)
            row_data.push_back(GBKToWide(item));

        grid_data.push_back(row_data);
    }
    DisplayDataToGrid(columnlist, grid_data);
    m_griddata = grid_data;
}

void CHousingGzccFetchDlg::OnBnClickedBtnExport()
{
    ExportToExcel();
}


void CHousingGzccFetchDlg::OnLvnGetdispinfoListSummaryData(NMHDR *pNMHDR, LRESULT *pResult)
{
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
    LV_ITEM *pItem = &(pDispInfo)->item;
    if (pItem->mask & LVIF_TEXT)
    {
        //ʹ�������������������Ӧ
        pItem->pszText = (LPWSTR)m_griddata[pItem->iItem][pItem->iSubItem].c_str();
    }

    *pResult = 0;
}

void CHousingGzccFetchDlg::DisplayDataToGrid(
    const std::vector<std::wstring> columnlist,
    const GridData& griddata)
{
    if (griddata.empty())
        return;
    m_griddata = griddata;

    //ɾ��֮ǰ������
    m_list_display.SetItemCountEx(0);
    m_list_display.Invalidate();
    m_list_display.UpdateWindow();

    int nColumnCount = m_list_display.GetHeaderCtrl()->GetItemCount();
    for (int i = nColumnCount - 1; i >= 0; i--)
        m_list_display.DeleteColumn(i);

    uint32 i = 0;
    for (const auto& it : columnlist)
    {
        m_list_display.InsertColumn(i++, it.c_str(), LVCFMT_LEFT, 100);//������
    }

    //�����µ����ݻ�����
    int nItemCount = griddata.size();
    m_list_display.SetItemCountEx(nItemCount);
    m_list_display.Invalidate();
}

bool CHousingGzccFetchDlg::ExportToExcel()
{
    base::FilePath templatepath = exePath_.Append(L"template.xlsx");
    uint64 time64 = base::Time::Now().ToInternalValue();
    std::wstring timestr = base::Uint64ToString16(time64);
    std::wstring filename = L"����_" + timestr + L".xlsx";
    base::FilePath excelpath = exePath_.Append(filename);

    ExcelHelper excelHelper;
    if (!excelHelper.Init(templatepath.value()))
        return false;

    if (!excelHelper.Create(excelpath.value()))
        return false;
    
    if (!excelHelper.Open(excelpath.value()))
        return false;

    if (!excelHelper.Export(m_griddata))
        return false;

    if (!excelHelper.Close())
        return false;

    return true;
}
