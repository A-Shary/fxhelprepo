#pragma once

#include "explorer_web.h"

// AdvertisementDlg �Ի���

class AdvertisementDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AdvertisementDlg)

public:
	AdvertisementDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~AdvertisementDlg();

// �Ի�������
	enum { IDD = IDD_ADVERTISEMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
    CExplorer1 explorer_web_;
};
