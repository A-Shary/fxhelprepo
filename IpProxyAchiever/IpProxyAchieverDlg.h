
// IpProxyAchieverDlg.h : ͷ�ļ�
//

#pragma once
#include <memory>
class CurlWrapper;
// CIpProxyAchieverDlg �Ի���
class CIpProxyAchieverDlg : public CDialogEx
{
// ����
public:
	CIpProxyAchieverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_IPPROXYACHIEVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnGetProxy();

private:
    std::unique_ptr<CurlWrapper> curlWrapper_;
};
