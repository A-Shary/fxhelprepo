// TestProject.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Network/TcpClient.h"
#include "TcpProxyClient.h"
#include <iostream>
#include <assert.h>
#include "third_party/chromium/base/at_exit.h"
#include "third_party/chromium/base/run_loop.h"

void TcpProxyClientTest()
{
    MyTcpProxyClient proxyClient;
    proxyClient.SetProxy("61.177.248.202", 1080);
    proxyClient.ConnectToSocks4Proxy("114.54.2.205", 843);
}

int _tmain(int argc, _TCHAR* argv[])
{   
    base::AtExitManager atExitManager;
    TcpProxyClientTest();
	return 0;
}

