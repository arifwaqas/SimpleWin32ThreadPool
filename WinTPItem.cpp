#include "WinTPItem.h"

template <typename T>
WinTPItem<T>::WinTPItem(const T worker, PVOID pParam, const PTP_CALLBACK_ENVIRON pCallbackEnv)
	: worker(worker), pParam(pParam), pCallbackEnv(pCallbackEnv), pWork(nullptr)
{

}


template <typename T>
bool WinTPItem<T>::StartWork()
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
void CALLBACK WinTPItem<T>::Callback(PTP_CALLBACK_INSTANCE pInstance, PVOID pFuncId, PTP_WORK pWork)
{
	UNREFERENCED_PARAMETER(pInstance);
	UNREFERENCED_PARAMETER(pWork);

	WinTPItem<T>* item = reinterpret_cast<WinTPItem<T>*>(pFuncId);
	if (!item)
	{
		//std::cout<<"Error allocating memory"<<std::endl;
		return;
	}

	item->worker(pInstance, item->pParam, pWork);
	delete item;

	return;
}
