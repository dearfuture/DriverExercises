#include "DPCTest.h"

VOID DPCRoutine
(
	KDPC *DPC,
	PVOID DeferredContext,
	PVOID SystemArgument1,
	PVOID SystemArgument2
)
{
	ULONG cpuID = KeGetCurrentProcessorNumber();
	DbgPrint("Hello CPU %ld\n", cpuID);

	//KeSignalCallDpcSynchronize(SystemArgument2);
	//KeSignalCallDpcDone(SystemArgument1);
}

VOID TestDPC()
{
	PKDPC dpcArray;
	//extern NTSYSAPI volatile CCHAR KeNumberProcessors;
	dpcArray = (PKDPC)ExAllocatePool( NonPagedPool, sizeof(KDPC)*KeNumberProcessors );

	int i;
	for (i = 0; i < KeNumberProcessors; i++)
	{
		KDPC dpc = dpcArray[i];
		KeInitializeDpc(&dpc, DPCRoutine, NULL);
		KeSetTargetProcessorDpc(&dpc, i);
		KeInsertQueueDpc(&dpc, NULL, NULL);
	}
}

VOID DPCRoutine2
(
	KDPC *DPC,
	PVOID DeferredContext,
	PVOID SystemArgument1,
	PVOID SystemArgument2
)
{
	ULONG cpuID = KeGetCurrentProcessorNumber();
	DbgPrint("fuck CPU %ld\n", cpuID);

	KeSignalCallDpcSynchronize(SystemArgument2);
	KeSignalCallDpcDone(SystemArgument1);
}

VOID TestDPC2()
{
	KeGenericCallDpc(DPCRoutine2, NULL);
}

KTIMER Timer;
KDPC TimerDPC;