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

#include "../../dragengine_configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined OS_UNIX && !defined OS_ANDROID && !defined OS_WEBWASM && !defined OS_BEOS
#include <execinfo.h>
#endif

#include "deException.h"
#include "../string/decStringList.h"
#include "../math/decMath.h"

#ifdef OS_ANDROID
#include <android/log.h>
#include <unwind.h>
#include <dlfcn.h>
#include <cxxabi.h>
#define printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Dragengine", __VA_ARGS__);
#endif

#ifdef OS_W32
#include "../../app/include_windows.h"
#ifdef WITH_DBGHELP
#include <dbghelp.h>
#endif
#endif


// Definitions
////////////////

#define STR_NULL "(null)"



// Class deException
//////////////////////

// Constructors, Destructors
//////////////////////////////

deException::deException(const char *name, const char *description, const char *file, int line) :
pName(name ? name : STR_NULL),
pDescription(description ? description : STR_NULL),
pFile(file ? file : STR_NULL),
pLine(decMath::max(line, 0)){
	pBuildBacktrace();
}

deException::~deException(){
}



// Management
///////////////
bool deException::IsNamed(const char *name) const{
	return pName == name;
}



// Display
////////////

decStringList deException::AdditionalInformation() const{
	return decStringList();
}

decStringList deException::FormatOutput() const{
	decStringList output;
	
	decString text;
	text.Format("Exception: %s", pName.GetString());
	output.Add(text);
	
	text.Format("Description: %s", pDescription.GetString());
	output.Add(text);
	
	text.Format("Source File: %s", pFile.GetString());
	output.Add(text);
	
	text.Format("Source Line: %i", pLine);
	output.Add(text);
	
	const int backtraceCount = pBacktrace.GetCount();
	int i;
	for(i=0; i<backtraceCount; i++){
		text.Format("Backtrace: %s", pBacktrace[i].GetString());
		output.Add(text);
	}
	
	output += AdditionalInformation();
	
	return output;
}

void deException::PrintError() const{
	const decStringList output = FormatOutput();
	const int count = output.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		printf("%s\n", output.GetAt(i).GetString());
	}
}



// Private Functions
//////////////////////

// we want this many entries at max
#define MAX_BACKTRACE_COUNT		25

// this many first entries are related to exception handling. skip them
#define SKIP_SELF_TRACE_COUNT	3

#ifdef OS_ANDROID
struct sBacktraceState{
	void **current;
	void **end;
};

static _Unwind_Reason_Code unwindCallback(struct _Unwind_Context *context, void *arg){
	sBacktraceState &state = *((sBacktraceState*)arg);
	const uintptr_t pc = _Unwind_GetIP(context);
	if(pc){
		if(state.current == state.end){
			return _URC_END_OF_STACK;
			
		}else{
			*state.current++ = ( void* )pc;
		}
	}
	
	return _URC_NO_REASON;
}
#endif

void deException::pBuildBacktrace(){
#if defined OS_UNIX && !defined OS_ANDROID && !defined OS_WEBWASM && !defined OS_BEOS
	const int maxFramepointerCount = MAX_BACKTRACE_COUNT + SKIP_SELF_TRACE_COUNT;
	void *framepointers[maxFramepointerCount];
	const int fpcount = backtrace(framepointers, maxFramepointerCount);
	if(fpcount == 0){
		return;
	}
	
	char ** const symbols = backtrace_symbols(framepointers, fpcount);
	int i;
	
	for(i=SKIP_SELF_TRACE_COUNT; i<fpcount; i++){
		pBacktrace.Add(symbols[i]);
	}
	
	free(symbols);
#endif
	
#ifdef OS_ANDROID
	// NOTE unwindCallback can segfault for strange reasons. The void* pointers are not
	//      const on purpose. Using them const can result in segfault due to compiler
	//      trying to optimize the wrong way
	// 
	// NOTE -fvisibility=hidden prevent demangling from working
	
	void *context[MAX_BACKTRACE_COUNT];
	void ** const stateBegin = &context[0];
	sBacktraceState state;
	state.current = stateBegin;
	state.end = stateBegin + MAX_BACKTRACE_COUNT;
	_Unwind_Backtrace(unwindCallback, &state);
	const size_t count = (size_t)(state.current - stateBegin);
	decString symbol;
	size_t i;
	
	for(i=SKIP_SELF_TRACE_COUNT; i<count; i++){
		const void * const addr = context[i];
		
		Dl_info info;
		if(dladdr(addr, &info) && info.dli_sname){
			int status = 0;
			char * const demangled = abi::__cxa_demangle(symbol, 0, 0, &status);
			if(demangled){
				symbol.Format("%s(%s+0x%x) [%p] %s", info.dli_fname, info.dli_sname,
					(unsigned int)((const char*)addr - (const char*)info.dli_saddr),
					addr, demangled);
				free(demangled);
				
			}else{
				symbol.Format("%s(%s+0x%x) [%p]", info.dli_fname, info.dli_sname,
					(unsigned int)((const char*)addr - (const char*)info.dli_saddr),
					addr);
			}
			
		}else{
			symbol.Format("%p ??", addr);
		}
		
		pBacktrace.Add(symbol);
	}
#endif

#ifdef OS_W32
#ifdef WITH_DBGHELP
	const HANDLE process = GetCurrentProcess();
	const HANDLE thread = GetCurrentThread();
	
	DWORD symOptions = SYMOPT_LOAD_LINES | SYMOPT_CASE_INSENSITIVE;
	symOptions |= SYMOPT_DEBUG | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME;
	symOptions |= SYMOPT_INCLUDE_32BIT_MODULES;

	if(!SymSetOptions(symOptions)){
		return;
	}
	if(!SymInitialize(process, NULL, TRUE)){
		return;
	}
	
	CONTEXT context;
	memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL;
	RtlCaptureContext(&context);
	
	const int symbolBufferLen = sizeof(IMAGEHLP_SYMBOL64) + MAX_SYM_NAME;
	char * const symbolBuffer = new char[symbolBufferLen + 1];
	memset(symbolBuffer, 0, symbolBufferLen + 1);
	PIMAGEHLP_SYMBOL64 const symbol = (PIMAGEHLP_SYMBOL64)symbolBuffer;
	symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
	symbol->MaxNameLength = MAX_SYM_NAME;

	STACKFRAME_EX stack;
	memset(&stack, 0, sizeof(stack));
	DWORD machineType;

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
	decString desymbol;

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
		if(SymGetLineFromAddr64(process, stack.AddrPC.Offset, &offsetLine, &symbolLine)){
			sourceLine = (int)symbolLine.LineNumber;
			sourceFile = symbolLine.FileName;
		}
		
		desymbol.Format("%s [%p] %s:%d", name, address, sourceFile, sourceLine);
		pBacktrace.Add(desymbol);
	}
	
	SymCleanup(process);
#endif
#endif
}
