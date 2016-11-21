#pragma once
#include <vector>
#include <utility>
#include "afxwin.h"
#include "afxdtctl.h"
#include "NetworkHelper.h"
#include "afxcmn.h"


// CWorshipDlg �Ի���

class WorshipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WorshipDlg)

public:
    WorshipDlg(NetworkHelper* network_helper, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~WorshipDlg();

    virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DLG_WORSHIP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CEdit m_edit_fanxing_id;
    CEdit m_edit_roomid;
    CDateTimeCtrl m_time_worship;
    CButton m_check_auto_worship;
    CListCtrl m_list_worship;
    afx_msg void OnBnClickedBtnWorship();
    afx_msg void OnBnClickedBtnAddToList();
    afx_msg void OnBnClickedBtnWorshipSelect();

private:
    typedef std::pair<std::wstring, std::wstring> worship_pair;
    bool GetSelectItems(std::vector<worship_pair>* select_items);
    NetworkHelper* network_helper_;    
};
