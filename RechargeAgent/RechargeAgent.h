
// RechargeAgent.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "third_party/chromium/base/memory/scoped_ptr.h"


// CRechargeAgentApp: 
// �йش����ʵ�֣������ RechargeAgent.cpp
//

namespace base
{
    class AtExitManager;
}

class CRechargeAgentApp : public CWinApp
{
public:
	CRechargeAgentApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
private:
    void InitAppLog();
    scoped_ptr<base::AtExitManager> atExitManager_;
};

extern CRechargeAgentApp theApp;