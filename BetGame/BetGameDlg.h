
// BetGameDlg.h : ͷ�ļ�
//

#pragma once

#include <memory>
#include "afxcmn.h"
#include "afxwin.h"

#include "BetNetworkHelper.h"

class BetNetworkHelper;
// CBetGameDlg �Ի���
class CBetGameDlg : public CDialogEx
{
// ����
public:
	CBetGameDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BETGAME_DIALOG };

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
    CListCtrl m_list_bet_data;
    CEdit m_edit_username;
    CEdit m_edit_password;
    CEdit m_edit_room_id;

    std::unique_ptr<BetNetworkHelper> bet_network_;
    CButton m_check_remember;
};
