PVOID NTAPI MmGetSystemRoutineAddressx(PUNICODE_STRING SystemRoutineName)
{
	PUNICODE_STRING FunctionName;
	const UNICODE_STRING* SourceString; 
	PVOID SystemRoutineAddress; 
	PVOID result; 
	STRING DestinationString; 
	const LARGE_INTEGER MiShortTime;
	DestinationString.Length = 0;

	for (SourceString = SystemRoutineName; RtlUnicodeStringToAnsiString(&DestinationString, SourceString, 1) < 0; SourceString = FunctionName) 
		KeDelayExecutionThread(0, 0, (PLARGE_INTEGER)&MiShortTime);
	// Finds The Function By Address Through Walking The PE Export Table Passing ntoskrnl.exe base address
	SystemRoutineAddress = (PVOID)RtlFindExportedRoutineByName((INT64)PsNtosImageBase, DestinationString.Buffer);	
	
	if (SystemRoutineAddress == nullptr) 
		// If It Still a null pointer call the same function but passing Hal (Hardware Abstraction Layer) Base Address Not All Functions will be in ntoskrnl.exe
		SystemRoutineAddress = (PVOID)RtlFindExportedRoutineByName(PsHalImageBase, DestinationString.Buffer);	
	RtlFreeAnsiString((PANSI_STRING)&DestinationString);

	if (SystemRoutineAddress && (int)MiMarkKernelCfgTarget(SystemRoutineAddress) < 0)
		return nullptr;
	return SystemRoutineAddress;
}
