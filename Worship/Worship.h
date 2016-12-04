
// Worship.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "third_party/chromium/base/memory/scoped_ptr.h"

namespace base
{
    class AtExitManager;
}

// CWorshipApp: 
// �йش����ʵ�֣������ Worship.cpp
//
class WorshipHelper;
class CLoginDlg;
class CWorshipDlg;

class WorshipApp : public CWinApp
{
public:
    WorshipApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

private:
    bool LoginProcedure(WorshipHelper* tracker_helper);
    bool TrackerProcedure(WorshipHelper* tracker_helper);
    void InitAppLog();
    scoped_ptr<base::AtExitManager> atExitManager_;
};

extern WorshipApp theApp;