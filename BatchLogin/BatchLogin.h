
// BatchLogin.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBatchLoginApp: 
// �йش����ʵ�֣������ BatchLogin.cpp
//

class CBatchLoginApp : public CWinApp
{
public:
	CBatchLoginApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBatchLoginApp theApp;