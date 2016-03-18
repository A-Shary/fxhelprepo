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
    , m_time_all(0)
    , m_time_left(0)
    , m_coin_left(0)
    , m_coin_right(0)
    , m_static_time(_T(""))
    , display_(false)
{
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
    DDX_Text(pDX, IDC_STATIC_TIME, m_static_time);
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
    SetTimer(TIME_SHOW, 1000, NULL);
    return TRUE;
}
// CDlgGiftNotify ��Ϣ�������
void CDlgGiftNotify::OnBnClickedBtnBegin()
{
    UpdateData(TRUE);
    display_ = true;
    ClearList();
    SetTimer(TIME_SKIP, 1000, NULL);

    // ��ȡ���������б�
    std::string giftliststr;
    bool result = networkLeft_->GetGiftList(m_room_left);
    if (!result)
    {
        return;
    }

    // �ֱ���뷿��
    networkLeft_->SetNotify(
        std::bind(&CDlgGiftNotify::Notify, this, std::placeholders::_1));
    networkLeft_->SetNotify601(
        std::bind(&CDlgGiftNotify::Notify601, this, ROOM_TYPE::ROOM_LEFT, 
        std::placeholders::_1, std::placeholders::_2));

    if (!networkLeft_->EnterRoom(m_room_left))
    {
        assert(false && L"���뷿��ʧ��");
        return;
    }

    //networkRight_->SetNotify(
    //    std::bind(&CDlgGiftNotify::Notify, this, std::placeholders::_1));
    //networkRight_->SetNotify601(
    //    std::bind(&CDlgGiftNotify::Notify601, this, ROOM_TYPE::ROOM_RIGHT,  std::placeholders::_1));
    //networkRight_->EnterRoom(m_room_right);
}

void CDlgGiftNotify::OnTimer(UINT_PTR nIDEvent)
{
    UpdateData(TRUE);
    std::wstring showtime;
    switch (nIDEvent)
    {
    case TIME_SHOW:// ��ʾʱ��
        showtime = base::UTF8ToWide(MakeFormatTimeString(base::Time::Now()));
        m_static_time = showtime.c_str();
        break;
    case TIME_SKIP:// ��ʾʣ��ʱ��
        m_time_left--;
        if (m_time_left<=0)
        {
            KillTimer(TIME_SKIP);
            StopAccumulative();
        }
        break;
    default:
        break;
    }
    UpdateData(FALSE);
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
            m_coin_left += it.giftcoin;
            m_list_left.InsertString(m_list_left.GetCount(), wstr.c_str());
            break;;
        case ROOM_TYPE::ROOM_RIGHT:
            m_coin_right += it.giftcoin;
            m_list_right.InsertString(m_list_right.GetCount(),wstr.c_str());
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
    if (!roomgiftinfo.tips.empty())
    {
        giftAccumulative.displaymsg = roomgiftinfo.tips;
    }
    else
    {
        giftAccumulative.displaymsg = std::string("[") + 
            base::UintToString(income)+ std::string("] ") + 
            roomgiftinfo.sendername + std::string("�͸�") + 
            roomgiftinfo.receivername +
            base::UintToString(roomgiftinfo.gitfnumber) + 
            std::string("��") + roomgiftinfo.giftname;
    }
    
    giftAccumulative.accumulative = 0;//��ʱ��ʹ��
    messageLock_.lock();
    messageQueue_.push_back(giftAccumulative);
    messageLock_.unlock();
    SendMessage(WM_USER_ADD_GIFT_INFO, 0, 0);

    if (!display_)
        return;

    switch (roomtype)
    {
    case CDlgGiftNotify::ROOM_TYPE::ROOM_LEFT:
        m_coin_left += income;
        break;
    case CDlgGiftNotify::ROOM_TYPE::ROOM_RIGHT:
        m_coin_right += income;
        break;
    default:
        break;
    }
    

}

void CDlgGiftNotify::ClearList()
{
    messageLock_.lock();
    messageQueue_.clear();
    messageLock_.unlock();

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