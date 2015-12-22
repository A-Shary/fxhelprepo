
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



CFanXingDlg::CFanXingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFanXingDlg::IDD, pParent)
    , network_(nullptr)
    , count(0)
{
    //network_->Initialize();
    //network_->SetNotify(
    //    std::bind(&CFanXingDlg::Notify, this, std::placeholders::_1));

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
    DDX_Control(pDX, IDC_EXPLORER1, web_);
    DDX_Control(pDX, IDC_LIST1, InfoList_);
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
    ON_MESSAGE(WM_USER_01, &CFanXingDlg::OnNotifyMessage)
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

    web_.Navigate(L"http://fanxing.kugou.com", NULL, NULL, NULL, NULL);

    SetDlgItemText(IDC_EDIT_NAV, L"1014619");
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
void CFanXingDlg::OnBnClickedButton1()
{
    CString username;
    CString password;
    GetDlgItemText(IDC_EDIT_Username, username);
    GetDlgItemText(IDC_EDIT_Password, password);

    bool loginResult = LoginByWebAction(username, password);

    // ����ͨ����curl��¼��ʽ
    // bool loginResult = LoginByRequest(username.GetBuffer(), password.GetBuffer());

}

//��תҳ�湦��
void CFanXingDlg::OnBnClickedButtonNav()
{
    CString strRoomid;
    GetDlgItemText(IDC_EDIT_NAV, strRoomid);
    CString strUrl = L"http://fanxing.kugou.com/" + strRoomid;
    VARIANT vtNull = {};
    web_.Navigate(strUrl, &vtNull, &vtNull, &vtNull, &vtNull);
    LOG(INFO) << L"Navigate To " << strUrl;
    // ��ȡ������Ϣ����������

    if (network_)
    {
        network_->Finalize();
    }    
    network_.reset(new NetworkHelper);
    network_->Initialize();
    network_->SetNotify(
        std::bind(&CFanXingDlg::Notify, this, std::placeholders::_1));

    network_->EnterRoom(strRoomid.GetBuffer());
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

void CFanXingDlg::Notify(const std::wstring& message)
{
    // �������ݸ�����
    messageMutex_.lock();
    messageQueen_.push_back(message);
    messageMutex_.unlock();
    this->PostMessage(WM_USER_01, 0, 0);
}

bool CFanXingDlg::LoginByWebAction(const CString& username, 
                                   const CString& password)
{
    CComQIPtr<IDispatch> iDisp(web_.get_Document());
    if (iDisp)
    {
        CComQIPtr<IHTMLDocument2> iDocu;
        HRESULT hr = iDisp->QueryInterface(IID_IHTMLDocument2, (void**)&iDocu);
        if (!FAILED(hr))
        {
            WebHandler handler(iDocu);
            //handler.Execute();
            handler.Login(username, password);
            return true;
        }
    }
    return false;
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

LRESULT CFanXingDlg::OnNotifyMessage(WPARAM wParam, LPARAM lParam)
{
    std::vector<std::wstring> messages;
    messageMutex_.lock();
    messages.swap(messageQueen_);
    messageMutex_.unlock();

    for (auto str : messages)
    {
        InfoList_.InsertString(count++, str.c_str());
    }
    
    InfoList_.SetCurSel(count-1);
    return 0;
}
