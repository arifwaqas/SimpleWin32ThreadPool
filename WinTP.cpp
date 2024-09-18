#include "Windows.h"
#include "WinTP.h"
#include "WinTPItem.h"

WinTP::WinTP() : pPool(nullptr), pWorkCallback(nullptr), pWork(nullptr), pCleanupGroup(nullptr)
{
	// std::cout<<"Initialized"<<std::endl;
	memset(&callbackEnv, 0, sizeof(TP_CALLBACK_ENVIRON));
}

WinTP::~WinTP()
{
	DestroyThreadpoolEnvironment(&callbackEnv);

	if (pCleanupGroup)
	{
		CloseThreadpoolCleanupGroup(pCleanupGroup);
	}

	if (pPool)
	{
		CloseThreadpool(pPool);
	}
}

void WinTP::SetMaxThreadCount(const DWORD count)
{
	SetThreadpoolThreadMaximum(pPool, count);
}

void WinTP::SetMinThreadCount(const DWORD count)
{
	SetThreadpoolThreadMinimum(pPool, count);
}

PTP_CLEANUP_GROUP_CANCEL_CALLBACK WinTP::CleanupCallback()
{
	// std::cout << "Cleanup callback called!" << std::endl;
	return PTP_CLEANUP_GROUP_CANCEL_CALLBACK();
}

template<typename T>
bool WinTP::SetCallback(T func, void* params)
{
	WinTPItem<T>* item = new WinTPItem<T>(func, params, &callbackEnv);
	if (!item) {
		// std::cout<<"Some error"<<std::endl;
		return false;
	}
	if (!item->StartWork()) {
		// std::cout<<"Some error"<<std::endl;
		return false;
	}
	return true;
}

void WinTP::WaitCallbackEnd(bool bForceTerminate)
{
	CloseThreadpoolCleanupGroupMembers(pCleanupGroup, bForceTerminate, nullptr);
}

bool WinTP::Init()
{
	InitializeThreadpoolEnvironment(&callbackEnv);

	pPool = CreateThreadpool(nullptr);
	if (!pPool) {
		//std::cout<<"Failed to CreateThreadpool"<<std::endl;
		return false;
	}

	// Create a cleanup group and associate with the TP environment
	pCleanupGroup = CreateThreadpoolCleanupGroup();
	if (!pCleanupGroup) {
		return false;
	}

	SetThreadpoolCallbackCleanupGroup(&callbackEnv, pCleanupGroup, nullptr);

	return true;
}
