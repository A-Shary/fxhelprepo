
// BetGame.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "third_party/chromium/base/memory/scoped_ptr.h"

// CBetGameApp: 
// �йش����ʵ�֣������ BetGame.cpp
//

namespace base
{
    class AtExitManager;
}

class CBetGameApp : public CWinApp
{
public:
	CBetGameApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

private:
    void InitAppLog();
    scoped_ptr<base::AtExitManager> atExitManager_;
};

extern CBetGameApp theApp;