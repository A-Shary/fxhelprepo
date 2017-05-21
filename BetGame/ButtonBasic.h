#pragma once


// CButtonBasic �Ի���

class CButtonBasic : public CButton
{
	DECLARE_DYNAMIC(CButtonBasic)

public:
	CButtonBasic(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CButtonBasic();

// �Ի�������
	enum { IDD = IDD_BETGAME_DIALOG };

public:
    void SetText(CString str);
    void SetForeColor(COLORREF color);       //�����ı���ɫ
    void SetBkColor(COLORREF color);         //���ñ�����ɫ
    void SetTextFont(int FontHight, LPCTSTR FontName);    //�������� 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

private:
    int          m_Style;     //��ť��״��0-������1-��ǰ��2-���£�3-������
    BOOL         b_InRect;            //�������־
    CString      m_strText;           //��ť����
    COLORREF     m_ForeColor;         //�ı���ɫ
    COLORREF     m_BackColor;         //����ɫ
    COLORREF     m_LockForeColor;     //������ť��������ɫ
    CRect        m_ButRect;           //��ť�ߴ�
    CFont*       p_Font;              //����

    void   DrawButton(CDC *pDC);      //�������İ�ť
    virtual void PreSubclassWindow();

    void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
public:
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
