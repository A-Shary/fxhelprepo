
// RechargeAgent.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRechargeAgentApp: 
// �йش����ʵ�֣������ RechargeAgent.cpp
//

class CRechargeAgentApp : public CWinApp
{
public:
	CRechargeAgentApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CRechargeAgentApp theApp;