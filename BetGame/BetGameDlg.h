
// BetGameDlg.h : ͷ�ļ�
//

#pragma once

#include <memory>
#include "afxcmn.h"
#include "afxwin.h"

#include "BetNetworkHelper.h"

class BetNetworkHelper;
class BetResult;
// CBetGameDlg �Ի���
class CBetGameDlg : public CDialogEx
{
// ����
public:
	CBetGameDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BETGAME_DIALOG };

    enum
    {
        WM_USER_TIPS = WM_USER + 1,
        WM_USER_BET_RESULT = WM_USER + 2,
        WM_USER_BET_TIME = WM_USER + 3
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
	DECLARE_MESSAGE_MAP()

private:
    afx_msg void OnBnClickedButtonEnterRoom();
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnHdnBegintrackListBetdata(NMHDR *pNMHDR, LRESULT *pResult);

    void TipMessageCallback(const std::wstring& message);
    void DisplayBetResultCallback(const BetResult& bet_result);
    void DisplayBetTimeCallback(uint32 time);

    LRESULT OnTipsMessage(WPARAM wParam, LPARAM lParam);
    LRESULT OnDisplayBetResult(WPARAM wParam, LPARAM lParam);
    LRESULT OnDisplayBetTime(WPARAM wParam, LPARAM lParam);

    int message_count_;
    CListBox m_list_msg;
    CListCtrl m_list_bet_data;
    CEdit m_edit_username;
    CEdit m_edit_password;
    CEdit m_edit_room_id;
    CButton m_check_remember;

    std::unique_ptr<BetNetworkHelper> bet_network_;
};
