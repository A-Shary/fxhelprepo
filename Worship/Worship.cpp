
// Worship.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include <memory>

#undef max
#undef min
#include "third_party/chromium/base/path_service.h"
#include "third_party/chromium/base/files/file_util.h"
#include "third_party/chromium/base/command_line.h"
#include "third_party/chromium/base/at_exit.h"

#include "WorshipHelper.h"
#include "Worship.h"
#include "WorshipDlg.h"
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// WorshipApp

BEGIN_MESSAGE_MAP(WorshipApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// WorshipApp ����

WorshipApp::WorshipApp()
    :atExitManager_(nullptr)
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
    atExitManager_.reset(new base::AtExitManager);
    InitAppLog();
    LOG(INFO) << __FUNCTION__;
}


// Ψһ��һ�� WorshipApp ����

WorshipApp theApp;


// WorshipApp ��ʼ��

BOOL WorshipApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

    WorshipHelper tracker_helper;
    tracker_helper.Initialize();

    if (LoginProcedure(&tracker_helper))
    {
        TrackerProcedure(&tracker_helper);
    }
    tracker_helper.Finalize();

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

    
	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

bool WorshipApp::LoginProcedure(WorshipHelper* tracker_helper)
{
    CLoginDlg login_dlg(NULL, tracker_helper);
    INT_PTR nResponse = login_dlg.DoModal();
    if (nResponse == IDOK)
    {
        return true;
    }
    else if (nResponse == IDCANCEL)
    {
        return false;
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
        TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
    }
    return false;
}

bool WorshipApp::TrackerProcedure(WorshipHelper* tracker_helper)
{
    WorshipDlg tracker_dlg(tracker_helper);
    m_pMainWnd = &tracker_dlg;
    INT_PTR nResponse = tracker_dlg.DoModal();
    if (nResponse == IDOK)
    {
        return true;
    }
    else if (nResponse == IDCANCEL)
    {
        return false;
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
        TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
    }
    return false;
}

void WorshipApp::InitAppLog()
{
    CommandLine::Init(0, NULL);
    base::FilePath path;
    PathService::Get(base::DIR_APP_DATA, &path);
    path = path.Append(L"FanXingHelper").Append(L"fanxinghelper.log");
    logging::LoggingSettings setting;
#ifdef DEBUG
    setting.logging_dest = logging::LOG_TO_ALL;
    setting.lock_log = logging::LOCK_LOG_FILE;
#else
    setting.logging_dest = logging::LOG_NONE;
    setting.lock_log = logging::DONT_LOCK_LOG_FILE;
#endif
    setting.log_file = path.value().c_str();
    setting.delete_old = logging::APPEND_TO_OLD_LOG_FILE;
    logging::InitLogging(setting);
    logging::SetLogItems(false, true, true, true);
}