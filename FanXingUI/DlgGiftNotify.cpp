// DlgGiftNotify.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgGiftNotify.h"
#include "afxdialogex.h"
#include "GiftInfoHelper.h"
#include "../Network/EncodeHelper.h"
#include "third_party/chromium/base/basictypes.h"

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "third_party/chromium/base/time/time.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"

// CDlgGiftNotify �Ի���

IMPLEMENT_DYNAMIC(CDlgGiftNotify, CDialogEx)

CDlgGiftNotify::CDlgGiftNotify(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgGiftNotify::IDD, pParent)
    , m_room_left(0)
    , m_room_right(0)
    , m_time_all(600)
    , m_time_left(0)
    , m_coin_left(0)
    , m_coin_right(0)
    , display_(false)
    , font16_(new CFont)
    , font18_(new CFont)
    , font22_(new CFont)
{
    font16_->CreateFont(16,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_BOLD,                   // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        DEFAULT_CHARSET,           // nCharSet
        OUT_CHARACTER_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        TEXT("����"));             // lpszFacename

    font18_->CreateFont(18,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_BOLD,                   // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        DEFAULT_CHARSET,           // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        TEXT("����"));

    font22_->CreateFont(22,                        // nHeight
        0,                         // nWidth
        0,                         // nEscapement
        0,                         // nOrientation
        FW_BOLD,                   // nWeight
        FALSE,                     // bItalic
        FALSE,                     // bUnderline
        0,                         // cStrikeOut
        DEFAULT_CHARSET,           // nCharSet
        OUT_DEFAULT_PRECIS,        // nOutPrecision
        CLIP_DEFAULT_PRECIS,       // nClipPrecision
        DEFAULT_QUALITY,           // nQuality
        DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
        TEXT("����"));

    networkLeft_.reset(new NetworkHelper);
    networkLeft_->Initialize();
    networkRight_.reset(new NetworkHelper);
    networkRight_->Initialize();
}

CDlgGiftNotify::~CDlgGiftNotify()
{
    networkLeft_->Finalize();
    networkRight_->Finalize();
}

void CDlgGiftNotify::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_ROOM_LEFT, m_room_left);
    DDX_Text(pDX, IDC_EDIT_ROOM_RIGHT, m_room_right);
    DDX_Text(pDX, IDC_EDIT_TIME_ALL, m_time_all);
    DDX_Text(pDX, IDC_EDIT_TIME_LEFT, m_time_left);
    DDX_Control(pDX, IDC_LIST_LEFT, m_list_left);
    DDX_Control(pDX, IDC_LIST_RIGHT, m_list_right);
    DDX_Text(pDX, IDC_EDIT_LEFT_GIFT, m_coin_left);
    DDX_Text(pDX, IDC_EDIT_RIGHT_GIFT, m_coin_right);
    DDX_Control(pDX, IDC_BTN_BEGIN, m_btn_begin);
    DDX_Control(pDX, IDC_STATIC_TIME, m_static_now_time);
    DDX_Control(pDX, IDC_EDIT_LEFT_GIFT, m_edit_coin_left);
    DDX_Control(pDX, IDC_EDIT_RIGHT_GIFT, m_edit_coin_right);
    DDX_Control(pDX, IDC_EDIT_TIME_LEFT, m_edit_time_left);
    DDX_Control(pDX, IDC_EDIT_TIME_ALL, m_edit_time_all);
    DDX_Control(pDX, IDC_STATIC_NOTICE, m_static_notice);
}


BEGIN_MESSAGE_MAP(CDlgGiftNotify, CDialogEx)
    ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BTN_BEGIN, &CDlgGiftNotify::OnBnClickedBtnBegin)
    ON_MESSAGE(WM_USER_ADD_GIFT_INFO, &CDlgGiftNotify::OnAddGiftInfo)
    ON_MESSAGE(WM_USER_UPDATE_TOTAL_COUNT, &CDlgGiftNotify::OnUpdateTotalCount)
END_MESSAGE_MAP()

BOOL CDlgGiftNotify::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_btn_begin.SetFont(font16_.get(), FALSE);
    m_static_now_time.SetFont(font16_.get(), FALSE);
    m_edit_coin_left.SetFont(font18_.get(), FALSE);
    m_edit_coin_right.SetFont(font18_.get(), FALSE);
    m_edit_time_left.SetFont(font22_.get(), FALSE);
    m_edit_time_all.SetFont(font22_.get(), FALSE);
    m_static_notice.SetFont(font18_.get(), FALSE);

    m_static_notice.SetWindowTextW(L"˵���������ߴ��ڲ��Խ׶Σ�Ŀǰ�ݲ�֧�ַ��Ǳ�ֱ�ӹ��������ͳ��,��֧������λ�����ȡ���������������࣬������޷����뷿������⡣�����°汾�����ƹ��ܡ�");

    SetTimer(TIME_SHOW, 1000, NULL);
    return TRUE;
}
// CDlgGiftNotify ��Ϣ�������
void CDlgGiftNotify::OnBnClickedBtnBegin()
{
    UpdateData(TRUE);  
    ClearList();
    m_btn_begin.EnableWindow(FALSE);
 
    if (m_room_left)
    {
        // ��ȡ��߷��������б�
        if (!networkLeft_->GetGiftList(m_room_left))
        {
            ::MessageBoxW(0, L"��ȡ������������ʧ��", L"����", 0);
            m_btn_begin.EnableWindow(TRUE);
            return;
        }

        // ��ߵķ���
        networkLeft_->SetNotify(
            std::bind(&CDlgGiftNotify::Notify, this, std::placeholders::_1));
        networkLeft_->SetNotify601(
            std::bind(&CDlgGiftNotify::Notify601, this, ROOM_TYPE::ROOM_LEFT, 
            std::placeholders::_1, std::placeholders::_2));

        if (!networkLeft_->EnterRoom(m_room_left))
        {
            ::MessageBoxW(0, L"���뷿��ʧ��", L"����", 0);
            assert(false && L"���뷿��ʧ��");
            m_btn_begin.EnableWindow(TRUE);
            return;
        }
    }

    if (m_room_right)
    {
        // �ұߵķ���
        if (!networkRight_->GetGiftList(m_room_right))
        {
            ::MessageBoxW(0, L"��ȡ������������ʧ��", L"����", 0);
            m_btn_begin.EnableWindow(TRUE);
            return;
        }

        networkRight_->SetNotify(
            std::bind(&CDlgGiftNotify::Notify, this, std::placeholders::_1));
        networkRight_->SetNotify601(
            std::bind(&CDlgGiftNotify::Notify601, this, ROOM_TYPE::ROOM_RIGHT,
            std::placeholders::_1, std::placeholders::_2));

        if (!networkRight_->EnterRoom(m_room_right))
        {
            ::MessageBoxW(0, L"���뷿��ʧ��", L"����", 0);
            assert(false && L"���뷿��ʧ��");
            m_btn_begin.EnableWindow(TRUE);
            return;
        }
    }

    if (m_room_left || m_room_right)
    {
        SetTimer(TIME_SKIP, 1000, NULL);
    }
    else
    {
        ::MessageBoxW(0, L"��ʼ֮ǰ�����뷿���", L"����", 0);
        m_btn_begin.EnableWindow(TRUE);
    }
}

void CDlgGiftNotify::OnTimer(UINT_PTR nIDEvent)
{
    std::wstring showtime;
    switch (nIDEvent)
    {
    case TIME_SHOW:// ��ʾʱ��
        showtime = base::UTF8ToWide(MakeFormatTimeString(base::Time::Now()));
        m_static_now_time.SetWindowTextW(showtime.c_str());
        break;
    case TIME_SKIP:// ��ʾʣ��ʱ��
        m_time_left--;
        if (m_time_left<=0)
        {
            KillTimer(TIME_SKIP);
            StopAccumulative();
            m_btn_begin.EnableWindow(TRUE);
        }
        UpdateData(FALSE);
        break;
    default:
        break;
    }
    
    return;
}


LRESULT CDlgGiftNotify::OnAddGiftInfo(WPARAM wParam, LPARAM lParam)
{
    std::vector<UserGiftAccumulative> newmessage;
    messageLock_.lock();
    newmessage.swap(messageQueue_);
    messageLock_.unlock();

    for (const auto&it : newmessage)
    {
        std::wstring wstr = base::UTF8ToWide(it.displaymsg);
        switch (it.roomtype)
        {
        case ROOM_TYPE::ROOM_LEFT:
            m_list_left.InsertString(m_list_left.GetCount(), wstr.c_str());
            if (display_)
                m_coin_left += it.giftcoin;
            break;;
        case ROOM_TYPE::ROOM_RIGHT:
            m_list_right.InsertString(m_list_right.GetCount(), wstr.c_str());
            if (display_)
                m_coin_right += it.giftcoin;
            break;
        default:
            break;
        }
    }
    UpdateData(FALSE);
    return 0;
}

LRESULT CDlgGiftNotify::OnUpdateTotalCount(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

void CDlgGiftNotify::Notify(const std::wstring& message)
{
    return;
}

void CDlgGiftNotify::Notify601(ROOM_TYPE roomtype, 
                               const RoomGiftInfo601& roomgiftinfo,
                               const GiftInfo& giftinfo)
{
    uint32 income = roomgiftinfo.gitfnumber * giftinfo.price;

    UserGiftAccumulative giftAccumulative;
    giftAccumulative.roomtype = roomtype;
    giftAccumulative.userid = roomgiftinfo.senderid;
    giftAccumulative.nickname = roomgiftinfo.sendername;
    giftAccumulative.giftcoin = income;
    giftAccumulative.displaymsg = std::string("[") +
        base::UintToString(income) + std::string("] ");
    if (!roomgiftinfo.tips.empty())
    {
        giftAccumulative.displaymsg += roomgiftinfo.tips;
    }
    else
    {
        giftAccumulative.displaymsg += roomgiftinfo.sendername + 
            base::WideToUTF8(L"�͸�����") +
            base::UintToString(roomgiftinfo.gitfnumber) + 
            base::WideToUTF8(L"��") + roomgiftinfo.giftname;
    }
    
    giftAccumulative.accumulative = 0;//��ʱ��ʹ��
    messageLock_.lock();
    messageQueue_.push_back(giftAccumulative);
    messageLock_.unlock();
    SendMessage(WM_USER_ADD_GIFT_INFO, 0, 0);
}

void CDlgGiftNotify::ClearList()
{
    messageLock_.lock();
    messageQueue_.clear();
    messageLock_.unlock();

    display_ = true;
    m_time_left = m_time_all;
    m_coin_left = 0;
    m_coin_right = 0;
    int leftcount = m_list_left.GetCount();
    for (int i = leftcount; i > 0; --leftcount)
    {
        m_list_left.DeleteString(i);
    }
    int rightcount = m_list_left.GetCount();
    for (int i = rightcount; i > 0; --rightcount)
    {
        m_list_right.DeleteString(i);
    }
    UpdateData(FALSE);
}

void CDlgGiftNotify::StopAccumulative()
{
    display_ = false;
}