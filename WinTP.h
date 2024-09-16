#pragma once

#include "Windows.h"

class WinTP
{
public:
	WinTP();
	~WinTP();

	bool Init();

	void SetMaxThreadCount(const DWORD count);
	void SetMinThreadCount(const DWORD count);

	void WaitCallbackEnd(bool bForceTerminate);

	// Implement your own if required
	virtual PTP_CLEANUP_GROUP_CANCEL_CALLBACK CleanupCallback();

private:

	TP_CALLBACK_ENVIRON callbackEnv;
	PTP_POOL pPool;
	PTP_WORK_CALLBACK pWorkCallback;
	PTP_WORK pWork;
	PTP_CLEANUP_GROUP pCleanupGroup;
};