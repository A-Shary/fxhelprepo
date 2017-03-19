
// LibEventClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ClientController.h"
#include <set>


// CLibEventClientDlg �Ի���
class CLibEventClientDlg : public CDialogEx
{
// ����
public:
	CLibEventClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

    ~CLibEventClientDlg();

// �Ի�������
	enum { IDD = IDD_LIBEVENTCLIENT_DIALOG };

    enum
    {
        WM_USER_DISPLAY_MESSAGE = WM_USER + 1,
    };

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
    afx_msg void OnBnClickedBtnSendMsg();

	DECLARE_MESSAGE_MAP()

    void SetHScroll();
    LRESULT OnNotifyMessage(WPARAM wParam, LPARAM lParam);

private:
    CEdit m_edit_ip;
    CEdit m_edit_port;
    CEdit m_edit_target_count;
    int message_index_ = 0;
    CListBox m_list_message;
    CListCtrl m_listctrl_ip_target_status;
    CEdit m_edit_message_send;

    ClientController client_controller_;

    void ConnectNotify(const std::string& ip, const std::string& port, 
                       bool result, TCPHANDLE handle);
    void DataReceiveNotify(
        const std::string& ip, const std::string& port, 
        bool result, std::vector<uint8>& data);

    void SendCallback(const std::string& ip, const std::string& port,
                      bool result);

    std::map<TCPHANDLE, std::pair<std::string, std::string>> handles_;
};
