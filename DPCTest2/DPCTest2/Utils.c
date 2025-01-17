#include "MyDriver.h"

VOID EnumDriver(PDRIVER_OBJECT pDriverObject)
{
	PKLDR_DATA_TABLE_ENTRY entry = (PKLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
	PKLDR_DATA_TABLE_ENTRY firstentry;
	ULONG64 pDrvBase = 0;
	KIRQL OldIrql;
	firstentry = entry;
	
	while ((PKLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink != firstentry)
	{
		DbgPrint("BASE=%p\tPATH=%wZ", entry->DllBase, entry->FullDllName);
		entry = (PKLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink;
	}
	
}


/*typedef struct TIME_FIELDS
{
	CSHORT Year;
	CSHORT Month;
	CSHORT Day;
	CSHORT Hour;
	CSHORT Minute;
	CSHORT Second;
	CSHORT Milliseconds;
	CSHORT Weekday;
} TIME_FIELDS;*/
VOID MyGetCurrentTime()
{
	LARGE_INTEGER CurrentTime;
	LARGE_INTEGER LocalTime;
	TIME_FIELDS   TimeFiled;
	// 这里得到的其实是格林威治时间
	KeQuerySystemTime(&CurrentTime);
	// 转换成本地时间
	ExSystemTimeToLocalTime(&CurrentTime, &LocalTime);
	// 把时间转换为容易理解的形式
	RtlTimeToTimeFields(&LocalTime, &TimeFiled);
	DbgPrint("[TimeTest] NowTime : %4d-%2d-%2d %2d:%2d:%2d",
		TimeFiled.Year, TimeFiled.Month, TimeFiled.Day,
		TimeFiled.Hour, TimeFiled.Minute, TimeFiled.Second);
}


#define DELAY_ONE_MICROSECOND 	(-10)
#define DELAY_ONE_MILLISECOND	(DELAY_ONE_MICROSECOND*1000)

VOID MySleep(LONG msec)
{
	LARGE_INTEGER my_interval;
	my_interval.QuadPart = DELAY_ONE_MILLISECOND;
	my_interval.QuadPart *= msec;
	KeDelayExecutionThread(KernelMode, 0, &my_interval);
}

KEVENT kEvent;

VOID MyThreadFunc(IN PVOID context)
{
	PUNICODE_STRING str = (PUNICODE_STRING)context;
	DbgPrint("Kernel thread running: %wZ\n", str);
	DbgPrint("Wait 3s!\n");
	MySleep(3000);
	DbgPrint("Kernel thread exit!\n");
	KeSetEvent(&kEvent, 0, TRUE);
	PsTerminateSystemThread(STATUS_SUCCESS);
}

VOID CreateThreadTest()
{
	HANDLE     hThread;
	UNICODE_STRING ustrTest = RTL_CONSTANT_STRING(L"This is a string for test!");
	NTSTATUS status;
	// 初始化事件
	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);
	status = PsCreateSystemThread(&hThread, 0, NULL, NULL, NULL, MyThreadFunc, (PVOID)&ustrTest);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("PsCreateSystemThread failed!");
		return;
	}
	ZwClose(hThread);
	// 等待事件
	KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);
	DbgPrint("CreateThreadTest OVER!\n");
}