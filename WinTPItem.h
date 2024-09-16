#pragma once
#include <windows.h>

template <typename T>
class WinTPItem
{
public:
	WinTPItem(const T func, PVOID param, const PTP_CALLBACK_ENVIRON callback_env);

	bool StartWork();

	static void CALLBACK Callback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParam, PTP_WORK pWork);

private:
	const T	worker;
	PVOID pParam;
	const PTP_CALLBACK_ENVIRON pCallbackEnv;
	PTP_WORK pWork;
};
