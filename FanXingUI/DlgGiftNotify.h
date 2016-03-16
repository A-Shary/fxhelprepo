#pragma once
#include "resource.h"
#include "afxwin.h"
#include <memory>
#include <string>
#include "NetworkHelper.h"

// CDlgGiftNotify �Ի���

class CDlgGiftNotify : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGiftNotify)

public:
	CDlgGiftNotify(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgGiftNotify();

// �Ի�������
	enum { IDD = IDD_DLG_GIFT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnBegin();

    void Notify(const std::wstring& message);
    void Notify601Left(const RoomGiftInfo601& roomgiftinfo);
    void Notify601Right(const RoomGiftInfo601& roomgiftinfo);
private:
    std::unique_ptr<NetworkHelper> networkLeft_;
    std::unique_ptr<NetworkHelper> networkRight_;
    int m_room_left;
    int m_room_right;
    int m_time_all;
    int m_time_left;
    CListBox m_list_left;
    CListBox m_list_right;
};
