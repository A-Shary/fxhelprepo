
// HousingGzccFetch.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��
#include "third_party/chromium/base/at_exit.h"

// CHousingGzccFetchApp: 
// �йش����ʵ�֣������ HousingGzccFetch.cpp
//

class CHousingGzccFetchApp : public CWinApp
{
public:
	CHousingGzccFetchApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()

private:
    base::AtExitManager at_exit_manager_;

    void InitAppLog();
};

extern CHousingGzccFetchApp theApp;