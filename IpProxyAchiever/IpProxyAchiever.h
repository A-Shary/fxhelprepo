
// IpProxyAchiever.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
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
class CurlWrapper;

// CIpProxyAchieverApp: 
// �йش����ʵ�֣������ IpProxyAchiever.cpp
//

class CIpProxyAchieverApp : public CWinApp
{
public:
	CIpProxyAchieverApp();

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

extern CIpProxyAchieverApp theApp;