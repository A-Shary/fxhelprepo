#pragma once
#include "afxdialogex.h"
// CDlgSaleRecord �Ի���

class CDlgSaleRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSaleRecord)

public:
	CDlgSaleRecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSaleRecord();

// �Ի�������
	enum { IDD = IDD_DLG_SALE_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
