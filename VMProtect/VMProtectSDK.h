#pragma once

#ifdef _WIN64
	#pragma comment(lib, "VMProtectSDK64.lib")
#else
	#pragma comment(lib, "VMProtectSDK32.lib")
#endif

#ifdef __cplusplus
extern "C" {
#endif

// protection
// ����:���ÿ�ʼ���
__declspec(dllimport) void __stdcall VMProtectBegin(const char *);
// ��ʼ���⻯���봦��ǣ������������ã�
__declspec(dllimport) void __stdcall VMProtectBeginVirtualization(const char *);
// ��ʼ������봦��ǣ������������ã�
__declspec(dllimport) void __stdcall VMProtectBeginMutation(const char *);
// ��ʼ����+��������Ǵ�
__declspec(dllimport) void __stdcall VMProtectBeginUltra(const char *);
// �󶨴��뵽���к�,�������Ᵽ��
__declspec(dllimport) void __stdcall VMProtectBeginVirtualizationLockByKey(const char *);
// �󶨴��뵽���к�, �������� + ���챣��
__declspec(dllimport) void __stdcall VMProtectBeginUltraLockByKey(const char *);
// ����������/����ȹ�����ԵĽ������
__declspec(dllimport) void __stdcall VMProtectEnd(void);

// utils
// ��������
__declspec(dllimport) BOOL __stdcall VMProtectIsDebuggerPresent(BOOL);
// ��������
__declspec(dllimport) BOOL __stdcall VMProtectIsVirtualMachinePresent(void);
// ӳ���ļ�CRCУ��
__declspec(dllimport) BOOL __stdcall VMProtectIsValidImageCRC(void);
// ����:����Ansi�ַ�������, ���ؼ��ܺ���ַ���ָ�룬��Ҫ�Ƿ�ֹ�ַ�����������
__declspec(dllimport) char * __stdcall VMProtectDecryptStringA(const char *value);
// ����:��Unicode�ַ���,���ؼ��ܺ���ַ���ָ��
__declspec(dllimport) wchar_t * __stdcall VMProtectDecryptStringW(const wchar_t *value);
// �ͷ����б����ܵ��ַ���
__declspec(dllimport) BOOL __stdcall VMProtectFreeString(void *value);

// licensing
enum VMProtectSerialStateFlags
{
	SERIAL_STATE_FLAG_CORRUPTED			= 0x00000001,
	SERIAL_STATE_FLAG_INVALID			= 0x00000002,
	SERIAL_STATE_FLAG_BLACKLISTED		= 0x00000004,
	SERIAL_STATE_FLAG_DATE_EXPIRED		= 0x00000008,
	SERIAL_STATE_FLAG_RUNNING_TIME_OVER	= 0x00000010,
	SERIAL_STATE_FLAG_BAD_HWID			= 0x00000020,
	SERIAL_STATE_FLAG_MAX_BUILD_EXPIRED	= 0x00000040,
};

#pragma pack(push, 1)

typedef struct
{
	WORD			wYear;
	BYTE			bMonth;
	BYTE			bDay;
} VMProtectDate;

typedef struct
{
	INT				nState;				// VMProtectSerialStateFlags ��Ҫ���õ�ע����Ϣ
	wchar_t			wUserName[256];		// �û���
	wchar_t			wEMail[256];		// ����
	VMProtectDate	dtExpire;			// ��ֹ����
	VMProtectDate	dtMaxBuild;			// ��Ȩ����
	INT				bRunningTime;		// ����ʱ��
	BYTE			nUserDataLength;	// ʹ������Ϣ����
	BYTE			bUserData[255];		// ʹ������Ϣ
} VMProtectSerialNumberData;

#pragma pack(pop)

// �������к�, �����кŴ��ݸ���Ȩģ��, ������ɹ�
__declspec(dllimport) INT  __stdcall VMProtectSetSerialNumber(const char * SerialNumber);
// �������к�״̬,�ɳ��������λ���,��������0,���Ϊ�������������һλ����ֵ,�����к�Ϊ�Ƿ����к�
__declspec(dllimport) INT  __stdcall VMProtectGetSerialNumberState();
// �����Ȩģ����������VMProtectSerialNumberData�ṹ�������С�쳣, ������������False
__declspec(dllimport) BOOL __stdcall VMProtectGetSerialNumberData(VMProtectSerialNumberData *pData, UINT nSize);
// ��ȡ��ǰ���Ե�Ӳ��ʶ����.���HWID����ΪNULL,nSizeΪ0,�򷵻ش���Ӳ��ʶ��������Ҫ�Ŀռ��С
__declspec(dllimport) INT  __stdcall VMProtectGetCurrentHWID(char * HWID, UINT nSize);

// activation
enum VMProtectActivationFlags
{
	ACTIVATION_OK = 0,
	ACTIVATION_SMALL_BUFFER,
	ACTIVATION_NO_CONNECTION,
	ACTIVATION_BAD_REPLY,
	ACTIVATION_BANNED,
	ACTIVATION_CORRUPTED,
	ACTIVATION_BAD_CODE,
	ACTIVATION_ALREADY_USED,
	ACTIVATION_SERIAL_UNKNOWN,
};
// �������֤
__declspec(dllimport) INT __stdcall VMProtectActivateLicense(const char *code, char *serial, int size);
// ͣ�����֤
__declspec(dllimport) INT __stdcall VMProtectDeactivateLicense(const char *serial);
// ������߼����ַ���
__declspec(dllimport) INT __stdcall VMProtectGetOfflineActivationString(const char *code, char *buf, int size);
// �õ����߷������ַ���
__declspec(dllimport) INT __stdcall VMProtectGetOfflineDeactivationString(const char *serial, char *buf, int size);


#ifdef __cplusplus
}
#endif
