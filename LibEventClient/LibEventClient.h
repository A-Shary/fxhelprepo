
// LibEventClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CLibEventClientApp: 
// �йش����ʵ�֣������ LibEventClient.cpp
//

class CLibEventClientApp : public CWinApp
{
public:
	CLibEventClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CLibEventClientApp theApp;