// Network.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "CurlWrapper.h"

bool NetworkInitialize()
{
    return CurlWrapper::CurlInit();
}
void NetworkFainalize()
{
    CurlWrapper::CurlCleanup();
}


