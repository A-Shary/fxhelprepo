
// GiftDisplay.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
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


// CGiftDisplayApp: 
// �йش����ʵ�֣������ GiftDisplay.cpp
//

class CGiftDisplayApp : public CWinApp
{
public:
	CGiftDisplayApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

private:
    void InitAppLog();
    scoped_ptr<base::AtExitManager> atExitManager_;
};

extern CGiftDisplayApp theApp;