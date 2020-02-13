#pragma once
#include <ntddk.h>

VOID KeGenericCallDpc(
	__in PKDEFERRED_ROUTINE Routine,
	__in_opt PVOID Context
);

LOGICAL KeSignalCallDpcSynchronize(
	__in PVOID SystemArgument2
);

VOID KeSignalCallDpcDone(__in PVOID SystemArgument1);

VOID DPCRoutine
(
	KDPC *DPC,
	PVOID DeferredContext,
	PVOID SystemArgument1,
	PVOID SystemArgument2
);
VOID TestDPC();

VOID DPCRoutine2
(
	KDPC *DPC,
	PVOID DeferredContext,
	PVOID SystemArgument1,
	PVOID SystemArgument2
);
VOID TestDPC2();


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