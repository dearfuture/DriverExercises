#pragma once
#include <ntddk.h>
#include <stddef.h>

typedef struct _KDPC_DATA
{
	LIST_ENTRY DpcListHead;
	PVOID DpcLock;
	ULONG DpcQueueDepth;
	ULONG DpcCount;
}KDPC_DATA, *PKDPC_DATA;


VOID EnumDPC();

extern KTIMER Timer;
extern KDPC TimerDPC;
VOID TimerDPCRoutine
(
	KDPC *DPC,
	PVOID DeferredContext,
	PVOID SystemArgument1,
	PVOID SystemArgument2
);

void TestTimerDPC(ULONG msec);