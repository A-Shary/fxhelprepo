
// HousingGzccFetchDlg.h : ͷ�ļ�
//

#pragma once

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "third_party/chromium/base/files/file_path.h"

#include "afxwin.h"
#include "afxcmn.h"

typedef std::vector<std::wstring> RowData;
typedef std::vector<RowData> GridData;

// CHousingGzccFetchDlg �Ի���
class CHousingGzccFetchDlg : public CDialogEx
{
// ����
public:
	CHousingGzccFetchDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HOUSINGGZCCFETCH_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

    afx_msg void OnBnClickedBtnFetchData();
    afx_msg void OnLvnGetdispinfoListSummaryData(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedBtnExport();

	DECLARE_MESSAGE_MAP()

private:

    void DisplayDataToGrid(const std::vector<std::wstring> columnlist,
        const GridData& griddata);

    bool ExportToExcel();

    CEdit m_edit_max_pages;
    CListCtrl m_list_display;

    GridData m_griddata;
    base::FilePath exePath_;
};
