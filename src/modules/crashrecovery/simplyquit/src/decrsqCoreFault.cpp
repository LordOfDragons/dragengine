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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>

#include <dragengine/dragengine_configuration.h>

#ifdef OS_W32
#include <dragengine/app/include_windows.h>
#include <dbghelp.h>
#endif

#ifdef OS_UNIX
#ifndef OS_ANDROID
#ifndef OS_BEOS
#include <execinfo.h>
#include <unistd.h>
#endif
#endif
#endif

#ifdef OS_ANDROID
#include <android/log.h>
#include <unwind.h>
#include <dlfcn.h>
#include <cxxabi.h>
#define printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Dragengine", __VA_ARGS__);
#endif

#include "decrsqCoreFault.h"
#include "deCRSimplyQuit.h"

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
static sExceptionInfo vExceptionInfo[ EXCEPTION_INFO_COUNT ] = {
	{ EXCEPTION_ACCESS_VIOLATION, "The thread tried to read from or write to a virtual address for which it does not have the appropriate access." },
	{ EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "The thread tried to access an array element that is out of bounds and the underlying hardware supports bounds checking." },
	{ EXCEPTION_BREAKPOINT, "A breakpoint was encountered." },
	{ EXCEPTION_DATATYPE_MISALIGNMENT, "The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16-bit values must be aligned on 2-byte boundaries; 32-bit values on 4-byte boundaries, and so on." },
	{ EXCEPTION_FLT_DENORMAL_OPERAND, "One of the operands in a floating-point operation is denormal. A denormal value is one that is too small to represent as a standard floating-point value." },
	{ EXCEPTION_FLT_DIVIDE_BY_ZERO, "The thread tried to divide a floating-point value by a floating-point divisor of zero." },
	{ EXCEPTION_FLT_INEXACT_RESULT, "The result of a floating-point operation cannot be represented exactly as a decimal fraction." },
	{ EXCEPTION_FLT_INVALID_OPERATION, "This exception represents any floating-point exception not included in this list." },
	{ EXCEPTION_FLT_OVERFLOW, "The exponent of a floating-point operation is greater than the magnitude allowed by the corresponding type." },
	{ EXCEPTION_FLT_STACK_CHECK, "The stack overflowed or underflowed as the result of a floating-point operation." },
	{ EXCEPTION_FLT_UNDERFLOW, "The exponent of a floating-point operation is less than the magnitude allowed by the corresponding type." },
	{ EXCEPTION_ILLEGAL_INSTRUCTION, "The thread tried to execute an invalid instruction." },
	{ EXCEPTION_IN_PAGE_ERROR, "The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network." },
	{ EXCEPTION_INT_DIVIDE_BY_ZERO, "The thread tried to divide an integer value by an integer divisor of zero." },
	{ EXCEPTION_INT_OVERFLOW, "The result of an integer operation caused a carry out of the most significant bit of the result." },
	{ EXCEPTION_INVALID_DISPOSITION, "An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high-level language such as C should never encounter this exception." },
	{ EXCEPTION_NONCONTINUABLE_EXCEPTION, "The thread tried to continue execution after a noncontinuable exception occurred." },
	{ EXCEPTION_PRIV_INSTRUCTION, "The thread tried to execute an instruction whose operation is not allowed in the current machine mode." },
	{ EXCEPTION_SINGLE_STEP, "A trace trap or other single-instruction mechanism signaled that one instruction has been executed." },
	{ EXCEPTION_STACK_OVERFLOW, "The thread used up its stack." }
};

static LONG WINAPI unhandledException( _EXCEPTION_POINTERS *ei){
	decrsqCoreFault *coreFault = decrsqCoreFault::GetGlobalCoreFault();
	deCRSimplyQuit *module = NULL;
	int i;
	
	if( coreFault ){
		module = &coreFault->GetModule();
	}
	
	// some infos
	// ei->ExceptionAddress
	for( i=0; i<EXCEPTION_INFO_COUNT; i++ ){
		if( ei->ExceptionRecord->ExceptionCode == vExceptionInfo[ i ].code ){
			if( module ){
				module->LogErrorFormat( "%s", vExceptionInfo[ i ].message );
				
			}else{
				printf( "%s\n", vExceptionInfo[ i ].message );
			}
			break;
		}
	}
	
	if( i == EXCEPTION_INFO_COUNT ){
		if( module ){
			module->LogError( "Unknown exception" );
			
		}else{
			printf( "Unknown exception\n" );
		}
	}
	
	// stack trace
	void *stack[ 100 ];
	unsigned short frames;
	char symbolData[ sizeof( SYMBOL_INFO ) + 1024 ];
	memset( &symbolData, 0, sizeof( symbolData ) );
	
	void *hack1 = ( void* )&symbolData;
	SYMBOL_INFO symbol = *( ( SYMBOL_INFO* )hack1 );
	const HANDLE process = GetCurrentProcess();
	
	if( ! SymSetOptions( SYMOPT_DEBUG | SYMOPT_DEFERRED_LOADS
	| SYMOPT_INCLUDE_32BIT_MODULES | SYMOPT_CASE_INSENSITIVE | SYMOPT_LOAD_LINES ) ){
		if( module ){
			module->LogError( "Failed calling SymSetOptions\n" );
		}else{
			printf( "Failed calling SymSetOptions\n" );
		}
	}
	
	if( ! SymInitialize( process, NULL, true ) ){
		if( module ){
			module->LogError( "Failed calling SymInitialize\n" );
		}else{
			printf( "Failed calling SymInitialize\n" );
		}
	}
	
	symbol.MaxNameLen = 1020;
	
	frames = CaptureStackBackTrace( 0, 100, stack, NULL );
	
	for( i=0; i<frames; i++ ){
		SymFromAddr( process, ( DWORD64 )stack[ i ], 0, &symbol );
		
		if( module ){
			module->LogErrorFormat( "%p: %s", ( void* )symbol.Address, symbol.Name );
			
		}else{
			printf( "%p: %s\n", ( void* )symbol.Address, symbol.Name );
		}
	}
	
	exit( -1 );
}
#endif



#ifdef OS_UNIX

// we want this many entries at max
#define MAX_BACKTRACE_COUNT		25

// this many first entries are related to exception handling. skip them
#define SKIP_SELF_TRACE_COUNT	3

#ifdef OS_ANDROID
struct sBacktraceState{
	void **current;
	void **end;
};

static _Unwind_Reason_Code unwindCallback( struct _Unwind_Context *context, void *arg ){
	sBacktraceState &state = *( ( sBacktraceState* )arg );
	const uintptr_t pc = _Unwind_GetIP( context );
	if( pc ){
		if( state.current == state.end ){
			return _URC_END_OF_STACK;
			
		}else{
			*state.current++ = ( void* )pc;
		}
	}
	
	return _URC_NO_REASON;
}
#endif

static void signalSegV( int number, siginfo_t *infos, void *ptrContext ){
	decrsqCoreFault *coreFault = decrsqCoreFault::GetGlobalCoreFault();
	deCRSimplyQuit *module = NULL;
	
	if( coreFault ){
		module = &coreFault->GetModule();
	}
	
	// some infos
	if( infos->si_code == SEGV_MAPERR ){
		if( module ){
			module->LogErrorFormat( "Segmentation Fault! Tried to access not allocated memory at %p.", infos->si_addr );
			
		}else{
			printf( "Segmentation Fault! Tried to access not allocated memory at %p.\n", infos->si_addr );
		}
		
	}else if( infos->si_code == SEGV_ACCERR ){
		if( module ){
			module->LogErrorFormat( "Segmentation Fault! Permission denied accessing memory at %p.", infos->si_addr );
			
		}else{
			printf( "Segmentation Fault! Permission denied accessing memory at %p.\n", infos->si_addr );
		}
		
	}else{
		if( module ){
			module->LogErrorFormat( "Segmentation Fault! Unknown memory error at %p.", infos->si_addr );
			
		}else{
			printf( "Segmentation Fault! Unknown memory error at %p.\n", infos->si_addr );
		}
	}
	
	// stack trace
	if( module ){
		module->LogError( "Backtrace:" );
	}
	
#if ! defined OS_ANDROID && ! defined OS_BEOS
	void *btentries[ 50 ]; // should be enough as usually only the last few are important
	size_t btentryCount;
	
	btentryCount = backtrace( btentries, 50 );
	
	if( module ){
		char **btStrings = backtrace_symbols( btentries, btentryCount );
		int i;
		
		for( i=0; i<(int)btentryCount; i++ ){
			module->LogError( btStrings[ i ] );
		}
		
		free( btStrings );
		
	}else{
		printf( "Backtrace:" );
		backtrace_symbols_fd( btentries, btentryCount, fileno( stdout ) );
	}
#endif
	
#ifdef OS_ANDROID
	// NOTE unwindCallback can segfault for strange reasons. The void* pointers are not
	//      const on purpose. Using them const can result in segfault due to compiler
	//      trying to optimize the wrong way
	// 
	// NOTE -fvisibility=hidden prevent demangling from working
	
	void *context[ MAX_BACKTRACE_COUNT ];
	void ** const stateBegin = &context[ 0 ];
	sBacktraceState state;
	state.current = stateBegin;
	state.end = stateBegin + MAX_BACKTRACE_COUNT;
	_Unwind_Backtrace( unwindCallback, &state );
	const size_t count = ( size_t )( state.current - stateBegin );
	decString symbol;
	size_t i;
	
	for( i=SKIP_SELF_TRACE_COUNT; i<count; i++ ){
		const void * const addr = context[ i ];
		
		Dl_info info;
		if( dladdr( addr, &info ) && info.dli_sname ){
			int status = 0;
			char * const demangled = abi::__cxa_demangle( symbol, 0, 0, &status );
			if( demangled ){
				symbol.Format( "%s(%s+0x%x) [%p] %s", info.dli_fname, info.dli_sname,
					( unsigned int )( ( const char* )addr - ( const char* )info.dli_saddr ),
					addr, demangled );
				free( demangled );
				
			}else{
				symbol.Format( "%s(%s+0x%x) [%p]", info.dli_fname, info.dli_sname,
					( unsigned int )( ( const char* )addr - ( const char* )info.dli_saddr ),
					addr );
			}
			
		}else{
			symbol.Format( "%p ??", addr );
		}
		
		if( module ){
			module->LogError( symbol.GetString() );
			
		}else{
			printf( "%s", symbol.GetString() );
		}
	}
#endif
	
	if( coreFault ){
		if( infos->si_code == SEGV_MAPERR ){
			coreFault->HandleSegFault( decrsqCoreFault::eecMemoryNotAllocated, infos->si_addr, ptrContext );
			
		}else if( infos->si_code == SEGV_ACCERR ){
			coreFault->HandleSegFault( decrsqCoreFault::eecMemoryNoPermision, infos->si_addr, ptrContext );
			
		}else{
			coreFault->HandleSegFault( decrsqCoreFault::eecMemoryUnknown, infos->si_addr, ptrContext );
		}
		
	}else{
		printf( "No global core fault found. Can not gather crash information!\n" );
	}
	
	if( module ){
		module->LogError( "Done, exiting." );
		
	}else{
		printf( "Done, exiting.\n" );
	}
	
	// try to break into gdb
#if 0
	const bool breakIntoGdb = true;
	
	if( breakIntoGdb ){
		const pid_t pid = getpid();
		char commandBuffer[ 256 ];
		
		sprintf( ( char* )&commandBuffer, "/usr/bin/konsole -e 'gdb attach %i'", pid );
		system( commandBuffer );
	}
#endif
	
	exit( -1 );
}

static void signalAbort( int number, siginfo_t *infos, void *ptrContext ){
	decrsqCoreFault *coreFault = decrsqCoreFault::GetGlobalCoreFault();
	deCRSimplyQuit *module = NULL;
	
	if( coreFault ){
		module = &coreFault->GetModule();
	}
	
	if( module ){
		module->LogError( "Unhandled Exception!" );
		
	}else{
		printf( "Unhandled Exception!\n" );
	}
	
	// stack trace
	if( module ){
		module->LogError( "Backtrace:" );
	}
	
#if ! defined OS_ANDROID && ! defined OS_BEOS
	void *btentries[ 50 ]; // should be enough as usually only the last few are important
	size_t btentryCount;
	
	btentryCount = backtrace( btentries, 50 );
	
	if( module ){
		char **btStrings = backtrace_symbols( btentries, btentryCount );
		int i;
		
		for( i=0; i<(int)btentryCount; i++ ){
			module->LogError( btStrings[ i ] );
		}
		
		free( btStrings );
		
	}else{
		printf( "Backtrace:" );
		backtrace_symbols_fd( btentries, btentryCount, fileno( stdout ) );
	}
#endif
	
#ifdef OS_ANDROID
	// NOTE unwindCallback can segfault for strange reasons. The void* pointers are not
	//      const on purpose. Using them const can result in segfault due to compiler
	//      trying to optimize the wrong way
	// 
	// NOTE -fvisibility=hidden prevent demangling from working
	
	void *context[ MAX_BACKTRACE_COUNT ];
	void ** const stateBegin = &context[ 0 ];
	sBacktraceState state;
	state.current = stateBegin;
	state.end = stateBegin + MAX_BACKTRACE_COUNT;
	_Unwind_Backtrace( unwindCallback, &state );
	const size_t count = ( size_t )( state.current - stateBegin );
	decString symbol;
	size_t i;
	
	for( i=SKIP_SELF_TRACE_COUNT; i<count; i++ ){
		const void * const addr = context[ i ];
		
		Dl_info info;
		if( dladdr( addr, &info ) && info.dli_sname ){
			int status = 0;
			char * const demangled = abi::__cxa_demangle( symbol, 0, 0, &status );
			if( demangled ){
				symbol.Format( "%s(%s+0x%x) [%p] %s", info.dli_fname, info.dli_sname,
					( unsigned int )( ( const char* )addr - ( const char* )info.dli_saddr ),
					addr, demangled );
				free( demangled );
				
			}else{
				symbol.Format( "%s(%s+0x%x) [%p]", info.dli_fname, info.dli_sname,
					( unsigned int )( ( const char* )addr - ( const char* )info.dli_saddr ),
					addr );
			}
			
		}else{
			symbol.Format( "%p ??", addr );
		}
		
		if( module ){
			module->LogError( symbol.GetString() );
			
		}else{
			printf( "%s", symbol.GetString() );
		}
	}
#endif
	
	if( coreFault ){
		coreFault->HandleAbort( ptrContext );
		
	}else{
		printf( "No global core fault found. Can not gather crash information!\n" );
	}
	
	if( module ){
		module->LogError( "Done, exiting." );
		
	}else{
		printf( "Done, exiting.\n" );
	}
	
	exit( -1 );
}

static void signalBusError( int number, siginfo_t *infos, void *ptrContext ){
	decrsqCoreFault *coreFault = decrsqCoreFault::GetGlobalCoreFault();
	deCRSimplyQuit *module = NULL;
	
	if( coreFault ){
		module = &coreFault->GetModule();
	}
	
	// some infos
	if( module ){
		module->LogErrorFormat( "Bus Error! Code %d at %p.", infos->si_code, infos->si_addr );
		
	}else{
		printf( "Bus Error! Code %d at %p.\n", infos->si_code, infos->si_addr );
	}
	
	// stack trace
	if( module ){
		module->LogError( "Backtrace:" );
	}
	
#if ! defined OS_ANDROID && ! defined OS_BEOS
	void *btentries[ 50 ]; // should be enough as usually only the last few are important
	size_t btentryCount;
	
	btentryCount = backtrace( btentries, 50 );
	
	if( module ){
		char **btStrings = backtrace_symbols( btentries, btentryCount );
		int i;
		
		for( i=0; i<(int)btentryCount; i++ ){
			module->LogError( btStrings[ i ] );
		}
		
		free( btStrings );
		
	}else{
		printf( "Backtrace:" );
		backtrace_symbols_fd( btentries, btentryCount, fileno( stdout ) );
	}
#endif
	
#ifdef OS_ANDROID
	// NOTE unwindCallback can segfault for strange reasons. The void* pointers are not
	//      const on purpose. Using them const can result in segfault due to compiler
	//      trying to optimize the wrong way
	// 
	// NOTE -fvisibility=hidden prevent demangling from working
	
	void *context[ MAX_BACKTRACE_COUNT ];
	void ** const stateBegin = &context[ 0 ];
	sBacktraceState state;
	state.current = stateBegin;
	state.end = stateBegin + MAX_BACKTRACE_COUNT;
	_Unwind_Backtrace( unwindCallback, &state );
	const size_t count = ( size_t )( state.current - stateBegin );
	decString symbol;
	size_t i;
	
	for( i=SKIP_SELF_TRACE_COUNT; i<count; i++ ){
		const void * const addr = context[ i ];
		
		Dl_info info;
		if( dladdr( addr, &info ) && info.dli_sname ){
			int status = 0;
			char * const demangled = abi::__cxa_demangle( symbol, 0, 0, &status );
			if( demangled ){
				symbol.Format( "%s(%s+0x%x) [%p] %s", info.dli_fname, info.dli_sname,
					( unsigned int )( ( const char* )addr - ( const char* )info.dli_saddr ),
					addr, demangled );
				free( demangled );
				
			}else{
				symbol.Format( "%s(%s+0x%x) [%p]", info.dli_fname, info.dli_sname,
					( unsigned int )( ( const char* )addr - ( const char* )info.dli_saddr ),
					addr );
			}
			
		}else{
			symbol.Format( "%p ??", addr );
		}
		
		if( module ){
			module->LogError( symbol.GetString() );
			
		}else{
			printf( "%s", symbol.GetString() );
		}
	}
#endif
	
	if( coreFault ){
		if( infos->si_code == SEGV_MAPERR ){
			coreFault->HandleBusError( decrsqCoreFault::eecMemoryUnknown, infos->si_addr, ptrContext );
		}
		
	}else{
		printf( "No global core fault found. Can not gather crash information!\n" );
	}
	
	if( module ){
		module->LogError( "Done, exiting." );
		
	}else{
		printf( "Done, exiting.\n" );
	}
	
	exit( -1 );
}
#endif



// Class decrsqCoreFault
/////////////////////////

// Constructor, destructor
////////////////////////////

decrsqCoreFault::decrsqCoreFault( deCRSimplyQuit &module ) :
pModule( module )
{
	if( GetGlobalCoreFault() ){
		DETHROW( deeInvalidAction );
	}
	
	try{
		pRegisterHandlers();
		
		pGlobalCoreFault = this;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

decrsqCoreFault::~decrsqCoreFault(){
	pCleanUp();
}



// Management
///////////////

decrsqCoreFault *decrsqCoreFault::pGlobalCoreFault = NULL;

decrsqCoreFault *decrsqCoreFault::GetGlobalCoreFault(){
	return pGlobalCoreFault;
}



void decrsqCoreFault::HandleSegFault( int error, void *memoryLocation, void *ptrContext ){
	//const ucontext_t &ucontext = *( ( ucontext_t* )ptrContext );
}

void decrsqCoreFault::HandleAbort( void* ptrContext ){
}

void decrsqCoreFault::HandleBusError( int error, void *memoryLocation, void *ptrContext ){
	//const ucontext_t &ucontext = *( ( ucontext_t* )ptrContext );
}



// Private Functions
//////////////////////

void decrsqCoreFault::pCleanUp(){
	pGlobalCoreFault = NULL;
	
	pUnregisterHandlers();
}



void decrsqCoreFault::pRegisterHandlers(){
#ifdef OS_W32
	#if defined USE_DRMINGW || defined NO_CATCH_CRASH
		(void)unhandledException;
	#else
		SetUnhandledExceptionFilter( unhandledException );
	#endif
#else
	struct sigaction action;
	
	// add handler for SEGV signal
	memset( &action, 0, sizeof( action ) );
	
	action.sa_sigaction = signalSegV;
	action.sa_flags = SA_SIGINFO;
	
	if( sigaction( SIGSEGV, &action, NULL ) ){
		DETHROW( deeInvalidAction );
	}
	
	// add handler for ABORT signal
	memset( &action, 0, sizeof( action ) );
	
	action.sa_sigaction = signalAbort;
	action.sa_flags = SA_SIGINFO;
	
	if( sigaction( SIGABRT, &action, NULL ) ){
		DETHROW( deeInvalidAction );
	}
	
	// add handler for SIGBUS signal
	memset( &action, 0, sizeof( action ) );
	
	action.sa_sigaction = signalBusError;
	action.sa_flags = SA_SIGINFO;
	
	if( sigaction( SIGBUS, &action, NULL ) ){
		DETHROW( deeInvalidAction );
	}
#endif
}

void decrsqCoreFault::pUnregisterHandlers(){
#ifdef OS_W32
	#if defined USE_DRMINGW || defined NO_CATCH_CRASH
		SetUnhandledExceptionFilter( NULL );
	#endif
#endif	
}
