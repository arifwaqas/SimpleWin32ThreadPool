#include "ThreadPool.h"

ThreadPool::ThreadPool() : pPool(nullptr), pWorkCallback(nullptr), pWork(nullptr), pCleanupGroup(nullptr)
{
	// std::cout<<"Initialized"<<std::endl;
	memset(&callbackEnv, 0, sizeof(TP_CALLBACK_ENVIRON));
}

ThreadPool::~ThreadPool()
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

void ThreadPool::SetMaxThreadCount(const DWORD count)
{
	SetThreadpoolThreadMaximum(pPool, count);
}

void ThreadPool::SetMinThreadCount(const DWORD count)
{
	SetThreadpoolThreadMinimum(pPool, count);
}

PTP_CLEANUP_GROUP_CANCEL_CALLBACK ThreadPool::CleanupCallback()
{
	// std::cout << "Cleanup callback called!" << std::endl;
	return PTP_CLEANUP_GROUP_CANCEL_CALLBACK();
}

void ThreadPool::WaitCallbackEnd(bool bForceTerminate)
{
	CloseThreadpoolCleanupGroupMembers(pCleanupGroup, bForceTerminate, nullptr);
}

bool ThreadPool::Init()
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

template<typename T>
bool ThreadPool::SetCallback(T func, void* params)
{
	ThreadPoolItem<T> WorkItem = new ThreadPoolItem<T>(func, params, &callbackEnv);

	if(!WorkItem){
		// LOG ERROR
		return false;
	}

	if (!WorkItem->StartWork()) {
		// LOG ERROR
		return false;
	}
	return true;
}
