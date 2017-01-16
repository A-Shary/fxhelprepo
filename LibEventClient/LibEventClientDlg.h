
// LibEventClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ClientController.h"


// CLibEventClientDlg �Ի���
class CLibEventClientDlg : public CDialogEx
{
// ����
public:
	CLibEventClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

    ~CLibEventClientDlg();

// �Ի�������
	enum { IDD = IDD_LIBEVENTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedBtnBegin();
	DECLARE_MESSAGE_MAP()

private:
    CEdit m_edit_ip;
    CEdit m_edit_port;
    CEdit m_edit_target_count;
    CListBox m_list_message;
    CListCtrl m_listctrl_ip_target_status;

    ClientController client_controller_;

    void ConnectNotify(bool result);
    void DataReceiveNotify(bool result, std::vector<uint8>& data);
    
};
