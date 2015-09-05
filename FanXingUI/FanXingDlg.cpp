
// FanXingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FanXing.h"
#include "FanXingDlg.h"
#include "afxdialogex.h"

#include "WebHandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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



CFanXingDlg::CFanXingDlg(std::shared_ptr<NetworkHelper> network, 
    CWnd* pParent /*=NULL*/)
	: CDialogEx(CFanXingDlg::IDD, pParent)
    , network_(network)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME); 
}

void CFanXingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EXPLORER1, web_);
}

BEGIN_MESSAGE_MAP(CFanXingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CFanXingDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON_CLICK, &CFanXingDlg::OnBnClickedButtonClick)
    ON_BN_CLICKED(IDC_BUTTON_REWARSTAR, &CFanXingDlg::OnBnClickedButtonRewarstar)
    ON_BN_CLICKED(IDC_BUTTON_REWARDGIFT, &CFanXingDlg::OnBnClickedButtonRewardgift)
    ON_BN_CLICKED(IDC_BUTTON_NAV, &CFanXingDlg::OnBnClickedButtonNav)
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDC_BTN_GETMSG, &CFanXingDlg::OnBnClickedBtnGetmsg)
    ON_BN_CLICKED(IDC_BTN_TEST, &CFanXingDlg::OnBnClickedBtnTest)
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

    web_.put_Silent(TRUE);// ��ֹ����360�����������ʾ

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    web_.Navigate(L"http://fanxing.kugou.com/1021987", NULL, NULL, NULL, NULL);
    //web_.Navigate(L"http://www.baidu.com/", NULL, NULL, NULL, NULL);

    SetDlgItemText(IDC_EDIT_NAV, L"http://fanxing.kugou.com/1021987");
    SetDlgItemInt(IDC_EDIT_X, 0);
    SetDlgItemInt(IDC_EDIT_Y, 0);
    SetDlgItemText(IDC_EDIT_GIFT, L"��ͨ,����");

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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFanXingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// ��¼����
void CFanXingDlg::OnBnClickedButton1()
{
    CComQIPtr<IDispatch> iDisp(web_.get_Document());
    if (iDisp)
    {
        CComQIPtr<IHTMLDocument2> iDocu;
        HRESULT hr = iDisp->QueryInterface(IID_IHTMLDocument2, (void**)&iDocu);
        if (!FAILED(hr))
        {
            WebHandler handler(iDocu);
            handler.Execute();
        }
    }
}

//��תҳ�湦��
void CFanXingDlg::OnBnClickedButtonNav()
{
    CString strUrl;
    GetDlgItemText(IDC_EDIT_NAV, strUrl);
    VARIANT vtNull = {};
    web_.Navigate(strUrl, &vtNull, &vtNull, &vtNull, &vtNull);
}

//ָ��λ�õ������
void CFanXingDlg::OnBnClickedButtonClick()
{
    CComQIPtr<IDispatch> iDisp(web_.get_Document());
    if (iDisp)
    {
        CComQIPtr<IHTMLDocument2> iDocu;
        HRESULT hr = iDisp->QueryInterface(IID_IHTMLDocument2, (void**)&iDocu);
        if (!FAILED(hr))
        {
            int x = GetDlgItemInt(IDC_EDIT_X);
            int y = GetDlgItemInt(IDC_EDIT_Y);           

            HWND explorerHWND = nullptr;

            HWND hwnd = ::FindWindowEx(web_.m_hWnd, 0, L"Shell DocObject View", NULL);
            if (hwnd)
                explorerHWND = ::FindWindowEx(hwnd, 0, L"Internet Explorer_Server", NULL);
            
            if (explorerHWND)
            {
                WebHandler handler(iDocu);
                handler.ClickXY(explorerHWND, x, y);
            }
        }
    }
}

// �����ǹ���
void CFanXingDlg::OnBnClickedButtonRewarstar()
{
    CComQIPtr<IDispatch> iDisp(web_.get_Document());
    if (iDisp)
    {
        CComQIPtr<IHTMLDocument2> iDocu;
        HRESULT hr = iDisp->QueryInterface(IID_IHTMLDocument2, (void**)&iDocu);
        if (!FAILED(hr))
        {
            WebHandler handler(iDocu);
            handler.RewardStar();
        }
    }
}

// �����﹦��
void CFanXingDlg::OnBnClickedButtonRewardgift()
{
    CComQIPtr<IDispatch> iDisp(web_.get_Document());
    if (iDisp)
    {
        CComQIPtr<IHTMLDocument2> iDocu;
        HRESULT hr = iDisp->QueryInterface(IID_IHTMLDocument2, (void**)&iDocu);
        if (!FAILED(hr))
        {
            CString strGift;
            GetDlgItemText(IDC_EDIT_GIFT, strGift);

            WebHandler handler(iDocu);
            handler.RewardGift((LPCTSTR)strGift);
        }
    }
}



void CFanXingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialogEx::OnLButtonDown(nFlags, point);
}

// ��ȡ������Ϣ
void CFanXingDlg::OnBnClickedBtnGetmsg()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}

// ���������Եĺ���
void CFanXingDlg::OnBnClickedBtnTest()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    CComQIPtr<IDispatch> iDisp(web_.get_Document());
    if (iDisp)
    {
        CComQIPtr<IHTMLDocument2> iDocu;
        HRESULT hr = iDisp->QueryInterface(IID_IHTMLDocument2, (void**)&iDocu);
        if (!FAILED(hr))
        {
            //��������
            WebHandler handler(iDocu);
            handler.GetChatMessage();
        }
    }

}
