// SimpleWinTP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SimpleWinTPWrapper.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include "ThreadPool.h"
#include "ThreadPoolItem.h"

VOID CALLBACK ThreadProc(PTP_CALLBACK_INSTANCE instance, PVOID params, PTP_WORK work)
{
	if (params == nullptr) { return; }

	auto param = *(static_cast<int*>(params));

	DWORD dwId = GetCurrentThreadId();

	for(int i = 0; i < param; i++)
	{
		std::cout << "Hello From Thread ID : " << dwId << " , i : " << i << std::endl;
		Sleep(100);
	}

	return;
}

int main()
{
	//Init ThreadPoolWrapper
	ThreadPool wrapper;
	wrapper.Init();

	//Set Min/Max Thread Count
	wrapper.SetMinThreadCount(1);
	wrapper.SetMaxThreadCount(4);

	//put callback with pram into the Threadpool
	std::vector<int> parameters = { 2, 4, 6, 7 };
	std::for_each(parameters.begin(), parameters.end(), [&wrapper](int& p) {
		//wrapper.SetCallback(ThreadProc, (void*)&p);
	});

	//Wait for Callback finish
	wrapper.WaitCallbackEnd( false );

	return 1;
}