/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <dragengine/dragengine_configuration.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>
#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>
#else
#include <execinfo.h>
#include <unistd.h>
#endif

#include "decrbCoreFault.h"
#include "../deCRBasic.h"

#include <dragengine/app/deOS.h>
#include <dragengine/common/exceptions.h>



// Signal Handlers
////////////////////

#ifdef OS_W32
struct sExceptionInfo{
	DWORD code;
	const char *message;
};

#define EXCEPTION_INFO_COUNT 20
static sExceptionInfo vExceptionInfo[EXCEPTION_INFO_COUNT] = {
	{ EXCEPTION_ACCESS_VIOLATION, "The thread tried to read from or write to a virtual address for which it does not have the appropriate access." },
	{EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking."},
	{EXCEPTION_BREAKPOINT, "A breakpoint was encountered."},
	{EXCEPTION_DATATYPE_MISALIGNMENT, "The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on."},
	{EXCEPTION_FLT_DENORMAL_OPERAND, "One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value."},
	{EXCEPTION_FLT_DIVIDE_BY_ZERO, "The thread tried to divide a floating-point value by a floating-point divisor of zero."},
	{EXCEPTION_FLT_INEXACT_RESULT, "The result of a floating-point operation cannot be represented exactly as a decimal fraction."},
	{EXCEPTION_FLT_INVALID_OPERATION, "This exception represents any floating-point exception not included in this list."},
	{EXCEPTION_FLT_OVERFLOW, "The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type."},
	{EXCEPTION_FLT_STACK_CHECK, "The stack overflowed or underflowed as the result of a floating-point operation."},
	{EXCEPTION_FLT_UNDERFLOW, "The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type."},
	{EXCEPTION_ILLEGAL_INSTRUCTION, "The thread tried to execute an invalid instruction."},
	{ EXCEPTION_IN_PAGE_ERROR, "The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network." },
	{EXCEPTION_INT_DIVIDE_BY_ZERO, "The thread tried to divide an integer value by an integer divisor of zero."},
	{EXCEPTION_INT_OVERFLOW, "The result of an integer operation caused a carry out of the most significant bit of the result."},
	{EXCEPTION_INVALID_DISPOSITION, "An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception."},
	{EXCEPTION_NONCONTINUABLE_EXCEPTION, "The thread tried to continue execution after a noncontinuable exception occurred."},
	{EXCEPTION_PRIV_INSTRUCTION, "The thread tried to execute an instruction whose operation is not allowed in the current machine mode."},
	{EXCEPTION_SINGLE_STEP, "A trace trap or other single-instruction mechanism signaled that one instruction has been executed."},
	{EXCEPTION_STACK_OVERFLOW, "The thread used up its stack."}
};

static void logStackTraceVS(const _EXCEPTION_POINTERS &ei, HANDLE process, deCRBasic *module){
	/*
	CONTEXT capuredContext;
	memset(&capuredContext, 0, sizeof(capuredContext));
	capuredContext.ContextFlags = CONTEXT_FULL;
	RtlCaptureContext(&capuredContext);
	CONTEXT &context = capuredContext;
	*/
	
	CONTEXT &context = *ei.ContextRecord;
	
	const int symbolBufferLen = sizeof(IMAGEHLP_SYMBOL64) + MAX_SYM_NAME;
	char * const symbolBuffer = new char[symbolBufferLen + 1];
	memset(symbolBuffer, 0, symbolBufferLen + 1);
	PIMAGEHLP_SYMBOL64 const symbol = (PIMAGEHLP_SYMBOL64)symbolBuffer;
	symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
	symbol->MaxNameLength = MAX_SYM_NAME;

	const HANDLE thread = GetCurrentThread();
	DWORD machineType;

	STACKFRAME_EX stack;
	memset(&stack, 0, sizeof(stack));

	#ifdef _WIN64
		machineType = IMAGE_FILE_MACHINE_IA64;
		stack.AddrPC.Offset = context.Rip;
		stack.AddrPC.Mode = AddrModeFlat;
		stack.AddrStack.Offset = context.Rsp;
		stack.AddrStack.Mode = AddrModeFlat;
		stack.AddrFrame.Offset = context.Rbp;
		stack.AddrFrame.Mode = AddrModeFlat;
	#else
		machineType = IMAGE_FILE_MACHINE_I386;
		stack.AddrPC.Offset = context.Eip;
		stack.AddrPC.Mode = AddrModeFlat;
		stack.AddrStack.Offset = context.Esp;
		stack.AddrStack.Mode = AddrModeFlat;
		stack.AddrFrame.Offset = context.Ebp;
		stack.AddrFrame.Mode = AddrModeFlat;
	#endif
	
	BOOL result;
	char undecoratedName[256];

	IMAGEHLP_MODULE64 moduleInfo;
	memset(&moduleInfo, 0, sizeof(moduleInfo));
	moduleInfo.SizeOfStruct = sizeof(IMAGEHLP_MODULE64);

	char symbolInfoBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
	memset(&symbolInfoBuffer, 0, sizeof(symbolInfoBuffer));
	PSYMBOL_INFO symbolInfo = (PSYMBOL_INFO)&symbolInfoBuffer;
	symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbolInfo->MaxNameLen = MAX_SYM_NAME;

	int i;
	for(i=0; i<50; i++){
		result = StackWalkEx(machineType, process, thread, &stack, &context, NULL,
			SymFunctionTableAccess64, SymGetModuleBase64, NULL, 0);
		if(!result || !stack.AddrPC.Offset){
			break;
		}

		symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
		symbol->MaxNameLength = 255;

		DWORD64 offsetSymbol = 0;
		DWORD offsetLine = 0;
		IMAGEHLP_LINE64 symbolLine = (IMAGEHLP_LINE64)0;

		const void *address = 0;
		const char *name = "??";
		const char *sourceFile = "??";
		int sourceLine = 0;

		if(SymFromAddr(process, stack.AddrPC.Offset, &offsetSymbol, symbolInfo)){
			address = (void*)symbolInfo->Address;
			
			UnDecorateSymbolName(symbolInfo->Name, (PSTR)undecoratedName, sizeof(undecoratedName), UNDNAME_COMPLETE);
			name = undecoratedName;
		}
		/*
		if(SymGetSymFromAddr64(process, (ULONG64)stack.AddrPC.Offset, &offsetSymbol, symbol)){
			address = (void*)symbol->Address;
			//name = symbol->Name;

			UnDecorateSymbolName(symbol->Name, (PSTR)undecoratedName, sizeof(undecoratedName), UNDNAME_COMPLETE);
			name = undecoratedName;
		}
		*/

		if(SymGetLineFromAddr64(process, stack.AddrPC.Offset, &offsetLine, &symbolLine)){
			sourceLine = (int)symbolLine.LineNumber;
			sourceFile = symbolLine.FileName;
		}
		/*
		if(SymGetModuleInfo64(process, stack.AddrPC.Offset, &moduleInfo)){
			sourceFile = moduleInfo.ImageName;
		}
		*/
		
		if(module){
			module->LogErrorFormat("%p: %s ; %s:%d", address, name, sourceFile, sourceLine);
			
		}else{
			printf("%p: %s ; %s:%d\n", address, name, sourceFile, sourceLine);
		}
	}
	
	delete [] symbolBuffer;
}

static void logStackTrace(HANDLE process, deCRBasic *module){
	void *stack[100];
	unsigned short frames;
	char symbolData[sizeof(SYMBOL_INFO) + 1024];
	memset(&symbolData, 0, sizeof(symbolData));
	
	void *hack1 = (void*)&symbolData;
	SYMBOL_INFO symbol = *((SYMBOL_INFO*)hack1);
	
	symbol.MaxNameLen = 1020;
	
	frames = CaptureStackBackTrace(0, 100, stack, NULL);
	
	int i;
	for(i=0; i<frames; i++){
		SymFromAddr(process, (DWORD64)stack[i], 0, &symbol);
		
		if(module){
			module->LogErrorFormat(PRId64 "x: %s", symbol.Address, symbol.Name);
			
		}else{
			printf("%p: %s\n", (void*)symbol.Address, (char*)symbol.Name);
		}
	}
}

static void writeMiniDump(_EXCEPTION_POINTERS *ei){
    BOOL bMiniDumpSuccessful;
    WCHAR szPath[MAX_PATH]; 
    WCHAR szFileName[MAX_PATH]; 
    const WCHAR szAppName[] = L"Dragengine";
	const WCHAR szVersion[] = L"" MDUMP_VERSION;
    DWORD dwBufferSize = MAX_PATH;
    HANDLE hDumpFile;
    SYSTEMTIME stLocalTime;
    MINIDUMP_EXCEPTION_INFORMATION ExpParam;

    GetLocalTime(&stLocalTime);
    GetTempPath(dwBufferSize, szPath);

    StringCchPrintf(szFileName, MAX_PATH, L"%s%s", szPath, szAppName);
    CreateDirectory(szFileName, NULL);

    StringCchPrintf(szFileName, MAX_PATH, L"%s%s\\%s-%04d%02d%02d-%02d%02d%02d-%ld-%ld.dmp",
		szPath, szAppName, szVersion, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond,
		GetCurrentProcessId(), GetCurrentThreadId());
	hDumpFile = CreateFileW(szFileName, GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_WRITE|FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

    ExpParam.ThreadId = GetCurrentThreadId();
    ExpParam.ExceptionPointers = ei;
    ExpParam.ClientPointers = TRUE;

    bMiniDumpSuccessful = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithDataSegs, &ExpParam, NULL, NULL);
}

static LONG WINAPI unhandledException(_EXCEPTION_POINTERS *ei){
	decrbCoreFault *coreFault = decrbCoreFault::GetGlobalCoreFault();
	deCRBasic *module = nullptr;
	int i;
	
	if(coreFault){
		module = coreFault->GetModule();
	}
	
	// some infos
	// ei->ExceptionAddress
	for(i=0; i<EXCEPTION_INFO_COUNT; i++){
		if(ei->ExceptionRecord->ExceptionCode == vExceptionInfo[i].code){
			if(module){
				module->LogErrorFormat("%s", vExceptionInfo[i].message);
				
			}else{
				printf("%s\n", vExceptionInfo[i].message);
			}
			break;
		}
	}
	
	if(i == EXCEPTION_INFO_COUNT){
		if(module){
			module->LogError("Unknown exception");
			
		}else{
			printf("Unknown exception\n");
		}
	}
	
	DWORD symOptions = SYMOPT_LOAD_LINES | SYMOPT_CASE_INSENSITIVE;
	symOptions |= SYMOPT_DEBUG | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME;
	symOptions |= SYMOPT_INCLUDE_32BIT_MODULES;

	if(! SymSetOptions(symOptions)){
		if(module){
			module->LogError("Failed calling SymSetOptions\n");
		}else{
			printf("Failed calling SymSetOptions\n");
		}
	}
	
	const HANDLE process = GetCurrentProcess();
	if(! SymInitialize(process, NULL, true)){
		if(module){
			module->LogError("Failed calling SymInitialize\n");
		}else{
			printf("Failed calling SymInitialize\n");
		}
	}
	
	// stack trace
	#ifdef OS_W32_VS
	logStackTraceVS(*ei, process, module);
	#else
	logStackTrace(process, module);
	#endif

	if(ei){
		writeMiniDump(ei);
	}

	exit(EXCEPTION_CONTINUE_SEARCH);
}

#else
static void signalSegV(int number, siginfo_t *infos, void *ptrContext){
	decrbCoreFault *coreFault = decrbCoreFault::GetGlobalCoreFault();
	deCRBasic *module = NULL;
	
	if(coreFault){
		module = coreFault->GetModule();
	}
	
	// some infos
	if(infos->si_code == SEGV_MAPERR){
		if(module){
			module->LogErrorFormat("Segmentation Fault! Tried to access not allocated memory at %p.", infos->si_addr);
			
		}else{
			printf("Segmentation Fault! Tried to access not allocated memory at %p.\n", infos->si_addr);
		}
		
	}else if(infos->si_code == SEGV_ACCERR){
		if(module){
			module->LogErrorFormat("Segmentation Fault! Permission denied accessing memory at %p.", infos->si_addr);
			
		}else{
			printf("Segmentation Fault! Permission denied accessing memory at %p.\n", infos->si_addr);
		}
		
	}else{
		if(module){
			module->LogErrorFormat("Segmentation Fault! Unknown memory error at %p.", infos->si_addr);
			
		}else{
			printf("Segmentation Fault! Unknown memory error at %p.\n", infos->si_addr);
		}
	}
	
	// stack trace
	void *btentries[50]; // should be enough as usually only the last few are important
	size_t btentryCount;
	
	btentryCount = backtrace(btentries, 50);
	
	if(module){
		module->LogError("Backtrace:");
		
		char **btStrings = backtrace_symbols(btentries, btentryCount);
		int i;
		
		for(i=0; i<(int)btentryCount; i++){
			module->LogError(btStrings[i]);
		}
		
		free(btStrings);
		
	}else{
		printf("Backtrace:");
		backtrace_symbols_fd(btentries, btentryCount, fileno(stdout));
	}
	
	if(coreFault){
		if(infos->si_code == SEGV_MAPERR){
			coreFault->HandleSegFault(decrbCoreFault::eecMemoryNotAllocated, infos->si_addr, ptrContext);
			
		}else if(infos->si_code == SEGV_ACCERR){
			coreFault->HandleSegFault(decrbCoreFault::eecMemoryNoPermision, infos->si_addr, ptrContext);
			
		}else{
			coreFault->HandleSegFault(decrbCoreFault::eecMemoryUnknown, infos->si_addr, ptrContext);
		}
		
	}else{
		printf("No global core fault found. Can not gather crash information!\n");
	}
	
	if(module){
		module->LogError("Done, exiting.");
		
	}else{
		printf("Done, exiting.\n");
	}
	
	// try to break into gdb
#if 0
	const bool breakIntoGdb = true;
	
	if(breakIntoGdb){
		const pid_t pid = getpid();
		char commandBuffer[256];
		
		sprintf((char*)&commandBuffer, "/usr/bin/konsole -e 'gdb attach %i'", pid);
		system(commandBuffer);
	}
#endif
	
	exit(-1);
}

static void signalAbort(int number, siginfo_t *infos, void *ptrContext){
	decrbCoreFault *coreFault = decrbCoreFault::GetGlobalCoreFault();
	deCRBasic *module = NULL;
	
	if(coreFault){
		module = coreFault->GetModule();
	}
	
	if(module){
		module->LogError("Unhandled Exception!");
		
	}else{
		printf("Unhandled Exception!\n");
	}
	
	// stack trace
	void *btentries[50]; // should be enough as usually only the last few are important
	size_t btentryCount;
	
	btentryCount = backtrace(btentries, 50);
	
	if(module){
		module->LogError("Backtrace:");
		
		char **btStrings = backtrace_symbols(btentries, btentryCount);
		int i;
		
		for(i=0; i<(int)btentryCount; i++){
			module->LogError(btStrings[i]);
		}
		
		free(btStrings);
		
	}else{
		printf("Backtrace:");
		backtrace_symbols_fd(btentries, btentryCount, fileno(stdout));
	}
	
	if(coreFault){
		coreFault->HandleAbort(ptrContext);
		
	}else{
		printf("No global core fault found. Can not gather crash information!\n");
	}
	
	if(module){
		module->LogError("Done, exiting.");
		
	}else{
		printf("Done, exiting.\n");
	}
	
	exit(-1);
}
#endif



// Class decrbCoreFault
/////////////////////////

// Constructor, destructor
////////////////////////////

decrbCoreFault::decrbCoreFault(deCRBasic *module){
	if(! module) DETHROW(deeInvalidParam);
	
	if(GetGlobalCoreFault()) DETHROW(deeInvalidAction);
	
	pModule = module;
	
	try{
		pRegisterHandlers();
		
		pGlobalCoreFault = this;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

decrbCoreFault::~decrbCoreFault(){
	pCleanUp();
}



// Management
///////////////

decrbCoreFault *decrbCoreFault::pGlobalCoreFault = NULL;

decrbCoreFault *decrbCoreFault::GetGlobalCoreFault(){
	return pGlobalCoreFault;
}



void decrbCoreFault::HandleSegFault(int error, void *memoryLocation, void *ptrContext){
	//const ucontext_t &ucontext = *( ( ucontext_t* )ptrContext );
}

void decrbCoreFault::HandleAbort(void* ptrContext){
}



// Private Functions
//////////////////////

void decrbCoreFault::pCleanUp(){
	pGlobalCoreFault = NULL;
	
	pUnregisterHandlers();
}



void decrbCoreFault::pRegisterHandlers(){
#ifdef OS_W32
	#if defined USE_DRMINGW || defined NO_CATCH_CRASH
		(void)unhandledException;
	#else
		SetUnhandledExceptionFilter(unhandledException);
	#endif
#else
	struct sigaction action;
	
	// add handler for SEGV signal
	memset(&action, 0, sizeof(action));
	
	action.sa_sigaction = signalSegV;
	action.sa_flags = SA_SIGINFO;
	
	if(sigaction(SIGSEGV, &action, NULL)){
		DETHROW(deeInvalidAction);
	}
	
	// add handler for ABORT signal
	memset(&action, 0, sizeof(action));
	
	action.sa_sigaction = signalAbort;
	action.sa_flags = SA_SIGINFO;
	
	if(sigaction(SIGABRT, &action, NULL)){
		DETHROW(deeInvalidAction);
	}
#endif
}

void decrbCoreFault::pUnregisterHandlers(){
#ifdef OS_W32
	#if defined USE_DRMINGW || defined NO_CATCH_CRASH
		SetUnhandledExceptionFilter(NULL);
	#endif
#endif
}
