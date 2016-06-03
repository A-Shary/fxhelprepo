
// FanXingDlg.h : ͷ�ļ�
//

#pragma once
#include <memory>
#include <mutex>

#include "NetworkHelper.h"
#include "BlacklistHelper.h"

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
        WM_USER_ADD_TO_BLACK_LIST = WM_USER + 3,
    };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    virtual void OnCancel();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnClose();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
    
    // �����������
    afx_msg void OnBnClickedButtonLogin();
    afx_msg void OnBnClickedButtonRewarstar();
    afx_msg void OnBnClickedButtonRewardgift();
    afx_msg void OnBnClickedButtonNav();
    afx_msg void OnBnClickedBtnGetmsg();
    
    // �����б����
    afx_msg void OnBnClickedBtnAdd();
    afx_msg void OnBnClickedButtonRemove();
    afx_msg void OnBnClickedBtnQuery();
    afx_msg void OnBnClickedBtnSelectAll();
    afx_msg void OnBnClickedBtnSelectReverse();
    afx_msg void OnBnClickedBtnKickoutMonth();
    afx_msg void OnBnClickedBtnKickoutHour();
    afx_msg void OnBnClickedBtnSilent();
    afx_msg void OnBnClickedBtnUnsilent();
    afx_msg void OnBnClickedBtnClear();
    afx_msg void OnBnClickedBtnGetViewerList();

    // �������б����
    afx_msg void OnBnClickedBtnKickoutMonthBlack();
    afx_msg void OnBnClickedBtnKickoutHourBlack();
    afx_msg void OnBnClickedBtnSilentBlack();
    afx_msg void OnBnClickedBtnUnsilentBlack();
    afx_msg void OnBnClickedBtnSelectAllBlack();
    afx_msg void OnBnClickedBtnSelectReverseBlack();
    afx_msg void OnBnClickedBtnRemoveBlack();
    afx_msg void OnBnClickedBtnLoadBlack();
    afx_msg void OnBnClickedBtnAddToBlack();
    afx_msg void OnBnClickedBtnSaveBlack();

    // �����ͷ����
    afx_msg void OnHdnItemclickListUserStatus(NMHDR *pNMHDR, LRESULT *pResult);
    static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

    LRESULT OnNotifyMessage(WPARAM wParam, LPARAM lParam);
    LRESULT OnDisplayDataToViewerList(WPARAM wParam, LPARAM lParam);
    LRESULT OnDisplayDtatToBlackList(WPARAM wParam, LPARAM lParam);

private:
    void SetHScroll();
    void Notify(const std::wstring& message);
    void NotifyEnterRoom(const RowData& rowdata);
    bool LoginByRequest(const std::wstring& username, const std::wstring& password);
	bool GetSelectViewers(std::vector<EnterRoomUserInfo>* enterRoomUserInfos);
    bool GetSelectBlacks(std::vector<EnterRoomUserInfo>* enterRoomUserInfos);
    bool KickOut_(const std::vector<EnterRoomUserInfo>& enterRoomUserInfos,
        KICK_TYPE kicktype);
    bool BanChat_(const std::vector<EnterRoomUserInfo>& enterRoomUserInfos);
    bool UnbanChat_(const std::vector<EnterRoomUserInfo>& enterRoomUserInfos);
    bool SendChatMessage_(uint32 roomid, const std::wstring& message);

    HICON m_hIcon;

    std::unique_ptr<NetworkHelper> network_;
    std::shared_ptr<AntiStrategy> antiStrategy_;

    std::mutex messageMutex_;
    std::vector<std::wstring> messageQueen_;

    std::mutex viewerRowdataMutex_;
    std::vector<RowData> viewerRowdataQueue_;

    std::mutex blackRowdataMutex_;
    std::vector<RowData> blackRowdataQueue_;

    uint32 listCtrlRowIndex_;
    CButton m_check_remember;
    uint32 roomid_ = 0;
    uint32 singerid_ = 0;

    CString m_query_key;
    CListBox InfoList_;
    int infoListCount_;
    CListCtrl m_ListCtrl_Viewers;
    CListCtrl m_ListCtrl_Blacks;
    std::unique_ptr<BlacklistHelper> blacklistHelper_;

public:
    afx_msg void OnBnClickedBtnClearInfo();
    afx_msg void OnBnClickedCancel();
    CStatic m_static_auth_info;
    CStatic m_static_login_info;
    afx_msg void OnBnClickedBtnAddVest();
    afx_msg void OnBnClickedBtnRemoveVest();
    afx_msg void OnBnClickedBtnSendChat();
    CEdit m_edit_vest;
    CEdit m_edit_chatmsg;
    CListCtrl m_list_vest;
    int m_radiogroup;
    afx_msg void OnBnClickedRadioNoaction();
    CButton m_chk_handle_all;
    afx_msg void OnBnClickedChkHandleAll();
};
