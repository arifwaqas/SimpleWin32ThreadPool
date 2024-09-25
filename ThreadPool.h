#pragma once
#include "Windows.h"
#include "ThreadPoolItem.h"

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	bool Init();

	void SetMaxThreadCount(const DWORD count);
	void SetMinThreadCount(const DWORD count);

	void WaitCallbackEnd(bool bForceTerminate);

	// Implement your own if required
	virtual PTP_CLEANUP_GROUP_CANCEL_CALLBACK CleanupCallback();

	template<typename T>
	bool SetCallback(T func, void* params);

private:

	TP_CALLBACK_ENVIRON callbackEnv;
	PTP_POOL pPool;
	PTP_WORK_CALLBACK pWorkCallback;
	PTP_WORK pWork;
	PTP_CLEANUP_GROUP pCleanupGroup;
};


template<typename T>
inline bool ThreadPool::SetCallback(T func, void* params)
{
	ThreadPoolItem<T> WorkItem = new ThreadPoolItem<T>(func, params, &callbackEnv);

	if (!WorkItem) {
		// LOG ERROR
		return false;
	}

	if (!WorkItem->StartWork()) {
		// LOG ERROR
		return false;
	}
	return true;
}