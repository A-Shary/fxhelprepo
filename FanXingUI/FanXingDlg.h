
// FanXingDlg.h : ͷ�ļ�
//

#pragma once
#include <memory>
#include <mutex>
#include "explorer1.h"

#include "NetworkHelper.h"
#include "afxwin.h"
#include "afxcmn.h"


// CFanXingDlg �Ի���
class CFanXingDlg : public CDialogEx
{
// ����
public:
    CFanXingDlg(CWnd* pParent = NULL);
    virtual ~CFanXingDlg();

// �Ի�������
	enum { IDD = IDD_FANXING_DIALOG };

    enum
    {
        WM_USER_01 = WM_USER + 1,
        WM_USER_ADD_ENTER_ROOM_INFO = WM_USER + 2,
    };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    afx_msg void OnBnClickedButtonClick();
    afx_msg void OnBnClickedButtonRewarstar();
    afx_msg void OnBnClickedButtonRewardgift();
    afx_msg void OnBnClickedButtonNav();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedBtnGetmsg();
    afx_msg void OnBnClickedBtnAdd();
    LRESULT OnNotifyMessage(WPARAM wParam, LPARAM lParam);
    LRESULT OnDisplayDataToGrid(WPARAM wParam, LPARAM lParam);
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    afx_msg void OnClose();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedButtonLogin();

private:
    void Notify(const std::wstring& message);
    void Notify201(const RowData& rowdata);
    bool LoginByRequest(const std::wstring& username, const std::wstring& password);

    std::unique_ptr<NetworkHelper> network_;
    std::mutex messageMutex_;
    std::vector<std::wstring> messageQueen_;

    std::mutex rowdataMutex_;
    std::vector<RowData> rowdataQueue_;

    uint32 listCtrlRowIndex_;

public:
    CListBox InfoList_;
    int infoListCount_;
    CListCtrl m_ListCtrl_UserStatus;
    afx_msg void OnBnClickedButton2();
    afx_msg void OnHdnItemclickListUserStatus(NMHDR *pNMHDR, LRESULT *pResult);

    static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    afx_msg void OnBnClickedButtonRemove();
    afx_msg void OnBnClickedBtnModify();
    afx_msg void OnBnClickedBtnQuery();
    afx_msg void OnBnClickedBtnSelectAll();
    afx_msg void OnBnClickedBtnSelectReverse();
    afx_msg void OnBnClickedBtnKickoutMonth();
    afx_msg void OnBnClickedBtnKickoutHour();
    afx_msg void OnBnClickedBtnSilent();
    afx_msg void OnBnClickedBtnUnsilent();
    CString m_query_key;
    afx_msg void OnBnClickedBtnClear();
};
