// ButtonBasic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BetGame.h"
#include "ButtonBasic.h"
#include "afxdialogex.h"


// CButtonBasic �Ի���

IMPLEMENT_DYNAMIC(CButtonBasic, CButton)

CButtonBasic::CButtonBasic(CWnd* pParent /*=NULL*/)
    : CButton()
{
    m_Style = 0;                //��ť��״���
    b_InRect = false;           //�������־
    m_strText = _T("");         //��ť���֣�ʹ��Ĭ�����֣�
    m_ForeColor = RGB(0, 0, 0);             //������ɫ����ɫ��
    m_BackColor = RGB(243, 243, 243);       //����ɫ���Ұ�ɫ��
    m_LockForeColor = GetSysColor(COLOR_GRAYTEXT);     //������ť��������ɫ
    p_Font = NULL;                        //����ָ��
}

CButtonBasic::~CButtonBasic()
{
    if (p_Font)     delete p_Font;         //ɾ������
}

void CButtonBasic::DoDataExchange(CDataExchange* pDX)
{
    CButton::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CButtonBasic, CButton)
    ON_WM_DRAWITEM()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CButtonBasic ��Ϣ�������


void CButtonBasic::PreSubclassWindow()
{
    ModifyStyle(0, BS_OWNERDRAW);         //���ð�ť����Ϊ�Ի�ʽ
    CButton::PreSubclassWindow();
}


void CButtonBasic::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    m_ButRect = lpDrawItemStruct->rcItem;     //��ȡ��ť�ߴ�

    if (m_strText.IsEmpty())
        GetWindowText(m_strText);           //��ȡ��ť�ı�

    int nSavedDC = pDC->SaveDC();
    VERIFY(pDC);
    DrawButton(pDC);                 //���ư�ť
    pDC->RestoreDC(nSavedDC);
    //CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CButtonBasic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    m_ButRect = lpDrawItemStruct->rcItem;     //��ȡ��ť�ߴ�

    if (m_strText.IsEmpty())
        GetWindowText(m_strText);           //��ȡ��ť�ı�

    int nSavedDC = pDC->SaveDC();
    VERIFY(pDC);
    DrawButton(pDC);                 //���ư�ť
    pDC->RestoreDC(nSavedDC);
    //CButton::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void  CButtonBasic::DrawButton(CDC *pDC)
{
    //����״̬
    if (m_Style == 3) m_Style = 0;
    if (GetStyle() & WS_DISABLED)
        m_Style = 3;     //��ֹ״̬
    //����״̬�����߿���ɫ��������ɫ
    COLORREF bColor, fColor;     //bColorΪ�߿���ɫ��fColorΪ������ɫ
    switch (m_Style)
    {
    case 0: bColor = RGB(192, 192, 192); fColor = m_ForeColor; break;   //������ť
    case 1: bColor = RGB(255, 255, 255); fColor = m_ForeColor; break;   //������ʱ��ť
    case 2: bColor = RGB(192, 192, 192); fColor = m_ForeColor; break;   //���µİ�ť
    case 3: bColor = m_BackColor; fColor = m_LockForeColor; break;    //�����İ�ť
    }
    //���ư�ť����
    CBrush Brush;
    Brush.CreateSolidBrush(m_BackColor);     //����ˢ
    pDC->SelectObject(&Brush);
    CPen Pen;
    Pen.CreatePen(PS_SOLID, 1, bColor);
    pDC->SelectObject(&Pen);
    pDC->RoundRect(&m_ButRect, CPoint(5, 5));    //��Բ�Ǿ���
    //���ư�ť����ʱ�ı߿�
    if (m_Style != 2)
    {
        CRect Rect;
        Rect.SetRect(m_ButRect.left + 2, m_ButRect.top + 1, m_ButRect.right, m_ButRect.bottom);
        pDC->DrawEdge(&Rect, BDR_RAISEDINNER, BF_RECT);     //���߿�
    }
    //���ư�ť����
    pDC->SetTextColor(fColor);         //������
    pDC->SetBkMode(TRANSPARENT);
    pDC->DrawText(m_strText, &m_ButRect, DT_SINGLELINE | DT_CENTER
                  | DT_VCENTER | DT_END_ELLIPSIS);
    //����ӵ�н��㰴ť�����߿�
    if (GetFocus() == this)
    {
        CRect Rect;
        Rect.SetRect(m_ButRect.left + 3, m_ButRect.top + 2, m_ButRect.right - 3, m_ButRect.bottom - 2);
        pDC->DrawFocusRect(&Rect);     //��ӵ�н�������߿�
    }
}

void CButtonBasic::OnMouseMove(UINT nFlags, CPoint point)
{
    if (!b_InRect || GetCapture() != this)     //�����밴ť
    {
        b_InRect = true;     //���ý����־
        SetCapture();        //�������
        m_Style = 1;         //���ð�ť״̬
        Invalidate();        //�ػ水ť
    }
    else
    {
        if (!m_ButRect.PtInRect(point))     //����뿪��ť
        {
            b_InRect = false;    //��������־
            ReleaseCapture();    //�ͷŲ�������
            m_Style = 0;         //���ð�ť״̬
            Invalidate();        //�ػ水ť
        }
    }

    CButton::OnMouseMove(nFlags, point);
}


void CButtonBasic::OnLButtonDown(UINT nFlags, CPoint point)
{
    m_Style = 2;
    Invalidate();         //�ػ水ť

    CButton::OnLButtonDown(nFlags, point);
}


void CButtonBasic::OnLButtonUp(UINT nFlags, CPoint point)
{
    m_Style = 1;
    Invalidate();         //�ػ水ť

    CButton::OnLButtonUp(nFlags, point);
}

void CButtonBasic::SetText(CString str)
{
    m_strText = _T("");
    SetWindowText(str);
}

void CButtonBasic::SetForeColor(COLORREF color)       //�����ı���ɫ
{
    m_ForeColor = color;
    Invalidate();
}

void CButtonBasic::SetBkColor(COLORREF color)         //���ñ�����ɫ
{
    m_BackColor = color;
    Invalidate();
}

void CButtonBasic::SetTextFont(int FontHight, LPCTSTR FontName)    //��������
{
    if (p_Font)     delete p_Font;     //ɾ��������
    p_Font = new CFont;
    BOOL result = p_Font->CreatePointFont(FontHight, FontName);     //����������
    SetFont(p_Font);                 //��������
}