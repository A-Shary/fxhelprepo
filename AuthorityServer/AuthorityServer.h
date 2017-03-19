
// AuthorityServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
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


// CAuthorityServerApp: 
// �йش����ʵ�֣������ AuthorityServer.cpp
//

class CAuthorityServerApp : public CWinApp
{
public:
	CAuthorityServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
    void InitAppLog();
    scoped_ptr<base::AtExitManager> at_exit_manager_;
};

extern CAuthorityServerApp theApp;