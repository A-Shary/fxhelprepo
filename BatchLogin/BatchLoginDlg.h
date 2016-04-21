
// BatchLoginDlg.h : 头文件
//

#pragma once

#include <memory>

class UserRoomManager;
// CBatchLoginDlg 对话框
class CBatchLoginDlg : public CDialogEx
{
// 构造
public:
	CBatchLoginDlg(CWnd* pParent = NULL);	// 标准构造函数
    virtual ~CBatchLoginDlg();
// 对话框数据
	enum { IDD = IDD_BATCHLOGIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
