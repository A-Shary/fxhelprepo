
// RechargeAgentDlg.h : ͷ�ļ�
//

#pragma once
#include <memory>
#include <vector>
#include "afxcmn.h"
#include "afxwin.h"
#include "third_party/chromium/base/basictypes.h"
#include "DlgSaleRecord.h"
#include "DlgTransferRecord.h"
#include "RechargeAgentHelper.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

class RechargeAgentHelper;
// CRechargeAgentDlg �Ի���
class CRechargeAgentDlg : public CDialogEx
{
// ����
public:
	CRechargeAgentDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RECHARGEAGENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnTcnSelchangeTabMainPage(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnLogin();
	DECLARE_MESSAGE_MAP()
private:
    void ShowPage(int index);
    bool LoginByRequest(const std::wstring& account, const std::wstring& password,
                        const std::wstring& verifycode, std::wstring* errorcode);
    bool GetVerifyCode(std::vector<uint8>* picture);

    CTabCtrl m_tabctrl;
    CDlgSaleRecord m_dlgsalerecord;
    CDlgTransferRecord m_dlgtransferrecord;
    CEdit m_edit_account;
    CEdit m_edit_password;
    CButton m_chk_remember;
    CImage image;
    int m_curtabid;
    std::vector<CDialog*> pDialog;

    std::unique_ptr<RechargeAgentHelper> rechargeAgentHelper_;
public:
    afx_msg void OnBnClickedBtnReflashVerifycode();
    CStatic m_static_verifycode;
    CEdit m_register_verifycode;
};
