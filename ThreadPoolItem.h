#pragma once
#include <windows.h>

template <typename T>
class ThreadPoolItem
{
public:
	ThreadPoolItem(const T func, PVOID param, const PTP_CALLBACK_ENVIRON callback_env);

	bool StartWork();

	static void CALLBACK Callback(PTP_CALLBACK_INSTANCE pInstance, PVOID pParam, PTP_WORK pWork);

private:
	const T	worker;
	PVOID pParam;
	const PTP_CALLBACK_ENVIRON pCallbackEnv;
	PTP_WORK pWork;
};

template <typename T>
ThreadPoolItem<T>::ThreadPoolItem(const T worker, PVOID pParam, const PTP_CALLBACK_ENVIRON pCallbackEnv)
	: worker(worker), pParam(pParam), pCallbackEnv(pCallbackEnv), pWork(nullptr)
{

}


template <typename T>
bool ThreadPoolItem<T>::StartWork()
{
	pWork = CreateThreadpoolWork(Callback, this, pCallbackEnv);
	if (pWork == nullptr)
	{
		return false;
	}

	SubmitThreadpoolWork(pWork);
	return true;
}


template <typename T>
void CALLBACK ThreadPoolItem<T>::Callback(PTP_CALLBACK_INSTANCE pInstance, PVOID pFuncId, PTP_WORK pWork)
{
	UNREFERENCED_PARAMETER(pInstance);
	UNREFERENCED_PARAMETER(pWork);

	ThreadPoolItem<T>* item = reinterpret_cast<ThreadPoolItem<T>*>(pFuncId);
	if (!item)
	{
		//std::cout<<"Error allocating memory"<<std::endl;
		return;
	}

	item->worker(pInstance, item->pParam, pWork);
	delete item;

	return;
}
