
// UserTracker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUserTrackerApp: 
// �йش����ʵ�֣������ UserTracker.cpp
//

class CUserTrackerApp : public CWinApp
{
public:
	CUserTrackerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CUserTrackerApp theApp;