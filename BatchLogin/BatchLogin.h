
// BatchLogin.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "third_party/chromium/base/memory/scoped_ptr.h"
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
    virtual ~CBatchLoginApp();
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

protected:
    void InitAppLog();

private:
    scoped_ptr<base::AtExitManager> atExitManager_;
};

extern CBatchLoginApp theApp;