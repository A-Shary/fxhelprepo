#pragma once


// GiftThanksSetting �Ի���

class GiftThanksSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(GiftThanksSettingDlg)

public:
	GiftThanksSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GiftThanksSettingDlg();

// �Ի�������
	enum { IDD = IDD_DLG_THANKS_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
