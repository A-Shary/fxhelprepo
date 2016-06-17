
// RechargeAgentDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "RechargeAgent.h"
#include "RechargeAgentDlg.h"
#include "RechargeAgentHelper.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRechargeAgentDlg �Ի���


CRechargeAgentDlg::CRechargeAgentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRechargeAgentDlg::IDD, pParent)
    , rechargeAgentHelper_(new RechargeAgentHelper)
{
    rechargeAgentHelper_->Initialize();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRechargeAgentDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TAB_MAIN_PAGE, m_tabctrl);
    DDX_Control(pDX, IDC_EDIT_ACCOUNT, m_edit_account);
    DDX_Control(pDX, IDC_EDIT_PASSWORD, m_edit_password);
    DDX_Control(pDX, IDC_CHK_REMEMBER_USER, m_chk_remember);
    DDX_Control(pDX, IDC_STATIC_VERIFYCODE, m_static_verifycode);
    DDX_Control(pDX, IDC_EDIT_VERIFYCODE, m_register_verifycode);
}

BEGIN_MESSAGE_MAP(CRechargeAgentDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN_PAGE, &CRechargeAgentDlg::OnTcnSelchangeTabMainPage)
    ON_BN_CLICKED(IDC_BTN_LOGIN, &CRechargeAgentDlg::OnBnClickedBtnLogin)
    ON_BN_CLICKED(IDC_BTN_REFLASH_VERIFYCODE, &CRechargeAgentDlg::OnBnClickedBtnReflashVerifycode)
END_MESSAGE_MAP()


// CRechargeAgentDlg ��Ϣ�������

BOOL CRechargeAgentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

    //ΪTab Control��������ҳ��
    m_tabctrl.InsertItem(0, _T("��ֵ�������ۼ�¼"));
    m_tabctrl.InsertItem(1, _T("�Ǳ�ת�ʼ�¼"));

    //���������Ի���
    m_dlgsalerecord.Create(IDD_DLG_SALE_RECORD, &m_tabctrl);
    m_dlgtransferrecord.Create(IDD_DLG_TRANSFER_RECORD, &m_tabctrl);
    pDialog.push_back(&m_dlgsalerecord);
    pDialog.push_back(&m_dlgtransferrecord);

    //�趨��Tab����ʾ�ķ�Χ
    CRect rc;
    m_tabctrl.GetClientRect(rc);
    rc.top += 20;
    rc.bottom -= 0;
    rc.left += 0;
    rc.right -= 0;
    m_dlgsalerecord.MoveWindow(&rc);
    m_dlgtransferrecord.MoveWindow(&rc);

    m_dlgsalerecord.ShowWindow(SW_SHOW);
    m_dlgtransferrecord.ShowWindow(SW_HIDE);

    //���浱ǰѡ��
    m_curtabid = 0;

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRechargeAgentDlg::OnPaint()
{
    if (!image.IsNull())
    {
        int hight = image.GetHeight();
        int width = image.GetWidth();
        CRect rc;
        m_static_verifycode.GetWindowRect(&rc);
        ScreenToClient(rc);
        image.Draw(GetDC()->m_hDC, CRect(rc.left + 20, rc.top + 18, rc.left + width + 20,
            rc.top + hight + 18));
        m_register_verifycode.SetFocus();
    }

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
HCURSOR CRechargeAgentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRechargeAgentDlg::OnTcnSelchangeTabMainPage(NMHDR *pNMHDR, LRESULT *pResult)
{
    //�ѵ�ǰ��ҳ����������
    pDialog[m_curtabid]->ShowWindow(SW_HIDE);
    //�õ��µ�ҳ������
    m_curtabid = m_tabctrl.GetCurSel();
    //���µ�ҳ����ʾ����
    pDialog[m_curtabid]->ShowWindow(SW_SHOW);
    *pResult = 0;
}


void CRechargeAgentDlg::OnBnClickedBtnLogin()
{
    CString username;
    CString password;
    CString verifycode;
    m_edit_account.GetWindowTextW(username);
    m_edit_password.GetWindowTextW(password);
    m_register_verifycode.GetWindowTextW(verifycode);

    bool remember = !!m_chk_remember.GetCheck();

    std::wstring errorcode;
    bool result = LoginByRequest(username.GetBuffer(), password.GetBuffer(),
                                 verifycode.GetBuffer(), &errorcode);
    std::wstring message = std::wstring(L"login ") + (result ? L"success" : L"failed");
}


void CRechargeAgentDlg::OnBnClickedBtnReflashVerifycode()
{
    std::wstring errormsg;
    std::vector<uint8> picture;
    if (!rechargeAgentHelper_->GetVerifyCode(&picture))
    {
        errormsg = L"��ȡ��֤��ʧ��";
        return;
    }
    
    std::wstring pathname;
    if (!rechargeAgentHelper_->SaveVerifyCodeImage(picture, &pathname))
    {
        assert(false);
        return;
    }

    if (!image.IsNull())
    {
        image.Destroy();
    }

    image.Load(pathname.c_str());
    int hight = image.GetHeight();
    int width = image.GetWidth();
    CRect rc;
    m_static_verifycode.GetWindowRect(&rc);
    ScreenToClient(rc);
    image.Draw(GetDC()->m_hDC, CRect(rc.left, rc.top, rc.left + width + 20,
        rc.top + hight + 30));
    m_register_verifycode.SetWindowTextW(L"");
}

bool CRechargeAgentDlg::LoginByRequest(
    const std::wstring& account, const std::wstring& password,
    const std::wstring& verifycode, std::wstring* errorcode)
{
    // ����ͨ����curl��¼��ʽ
    bool result = rechargeAgentHelper_->Login(
        base::WideToUTF8(account), base::WideToUTF8(password), 
        base::WideToUTF8(verifycode), errorcode);

    return result;
}

bool CRechargeAgentDlg::GetVerifyCode(std::vector<uint8>* picture)
{

    return false;
}

