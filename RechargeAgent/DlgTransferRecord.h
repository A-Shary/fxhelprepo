#pragma once
#include "afxdialogex.h"
// CDlgTransferRecord �Ի���

class CDlgTransferRecord : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTransferRecord)

public:
	CDlgTransferRecord(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgTransferRecord();

// �Ի�������
	enum { IDD = IDD_DLG_TRANSFER_RECORD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
