
// AuthorityServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


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
};

extern CAuthorityServerApp theApp;