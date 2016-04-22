
// BatchLoginDlg.h : ͷ�ļ�
//

#pragma once

#include <memory>

class UserRoomManager;
// CBatchLoginDlg �Ի���
class CBatchLoginDlg : public CDialogEx
{
// ����
public:
	CBatchLoginDlg(CWnd* pParent = NULL);	// ��׼���캯��
    virtual ~CBatchLoginDlg();
// �Ի�������
	enum { IDD = IDD_BATCHLOGIN_DIALOG };

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
    afx_msg void OnBnClickedBtnImportUser();

private:
    std::unique_ptr<UserRoomManager> userRoomManager_;
public:
    afx_msg void OnBnClickedBtnGetProxy();
    afx_msg void OnBnClickedBtnBatchEnterRoom();
    afx_msg void OnBnClickedBtnImportRoom();
};
