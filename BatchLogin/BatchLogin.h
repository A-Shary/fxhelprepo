
// BatchLogin.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include <memory>

// CBatchLoginApp: 
// �йش����ʵ�֣������ BatchLogin.cpp
//

namespace base
{
    class AtExitManager;
}

class CBatchLoginApp : public CWinApp
{
public:
	CBatchLoginApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

protected:
    void InitAppLog();

private:
    std::unique_ptr<base::AtExitManager> atExitManager_;
};

extern CBatchLoginApp theApp;