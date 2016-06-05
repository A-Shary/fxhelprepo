
// IpProxyAchieverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include <regex>
#include "IpProxyAchiever.h"
#include "IpProxyAchieverDlg.h"
#include "afxdialogex.h"
#undef min
#undef max
#include "Network/CurlWrapper.h"
#include "Network/EncodeHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CIpProxyAchieverDlg �Ի���



CIpProxyAchieverDlg::CIpProxyAchieverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIpProxyAchieverDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    CurlWrapper::CurlInit();
    curlWrapper_.reset(new CurlWrapper);
}

void CIpProxyAchieverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIpProxyAchieverDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_GET_PROXY, &CIpProxyAchieverDlg::OnBnClickedBtnGetProxy)
END_MESSAGE_MAP()


// CIpProxyAchieverDlg ��Ϣ�������

BOOL CIpProxyAchieverDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CIpProxyAchieverDlg::OnPaint()
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
HCURSOR CIpProxyAchieverDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CIpProxyAchieverDlg::OnBnClickedBtnGetProxy()
{
    // TODO:  �ڴ���ӿؼ�֪ͨ����������
    HttpRequest request;
    request.url = "http://www.youdaili.net/Daili/Socks/";
    request.method = HttpRequest::HTTP_METHOD::HTTP_METHOD_GET;
    request.useragent = "Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/44.0.2403.130 Safari/537.36";
    HttpResponse response;
    if (!curlWrapper_->Execute(request, &response))
        return ;

    std::string rootdata(response.content.begin(), response.content.end());
    std::regex pattern(R"(http://www\.youdaili.net/Daili/Socks/[0-9]*\.html)");

    std::string s = rootdata;
    std::smatch match;
    std::vector<std::string> urllist;
    while (std::regex_search(s, match, pattern)) {
        for (auto x : match)
            urllist.push_back(x);
        std::cout << std::endl;
        s = match.suffix().str();
    }

    return ;
}
