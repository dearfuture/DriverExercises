#include "DPCTest.h"

VOID EnumDPC()
{
	int i;
	for (i = 0; i < KeNumberProcessors; i++)
	{
		KeSetSystemAffinityThread((KAFFINITY)1 << i);
		DbgPrint("CPU: %d\n", i);

		ULONG SelfOfKpcr = offsetof(KPCR, Self);
		PKPCR kpcr = (PKPCR)__readgsqword(SelfOfKpcr);
		//struct _KPRCB *prcb = kpcr->CurrentPrcb;

		DbgPrint("KPCR(%d): %p\n", i, kpcr);
		PUCHAR prcb = (PUCHAR)kpcr->CurrentPrcb;
		ULONG DpcDataOfPrcb = 0x2180;
		PKDPC_DATA DpcData = (PKDPC_DATA)(prcb + DpcDataOfPrcb);
		PLIST_ENTRY firstentry = &DpcData[1].DpcListHead;
		PLIST_ENTRY entry = firstentry->Flink;
		while (entry != firstentry)
		{
			PKDPC dpc = CONTAINING_RECORD(entry, KDPC, DpcListEntry);
			DbgPrint("DPC: %p\n", dpc);
			entry = entry->Flink;
		}

		DbgPrint("\n");
	}

}

KTIMER Timer;
KDPC TimerDPC;
VOID TimerDPCRoutine
(
	KDPC *DPC,
	PVOID DeferredContext,
	PVOID SystemArgument1,
	PVOID SystemArgument2
)
{
	DbgPrint("TimerDPC Expired\n");
	//KeSetTimer
}

void TestTimerDPC(ULONG msec)
{
	KeInitializeTimer(&Timer);
	KeInitializeDpc(&TimerDPC, TimerDPCRoutine, NULL);

	LARGE_INTEGER interval;
	interval.QuadPart = -10 * 1000LL * msec;
	KeSetTimer(&Timer, interval, &TimerDPC);
}