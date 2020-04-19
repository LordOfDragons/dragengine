/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "../../dragengine_configuration.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_UNIX
#ifndef ANDROID
#ifndef OS_BEOS
#include <execinfo.h>
#endif
#endif
#endif

#include "deException.h"
#include "../string/decStringList.h"
#include "../math/decMath.h"

#ifdef ANDROID
#include <android/log.h>
#include <unwind.h>
#include <dlfcn.h>
#include <cxxabi.h>
#define printf(...) __android_log_print(ANDROID_LOG_VERBOSE, "Dragengine", __VA_ARGS__);
#endif



// Definitions
////////////////

#define STR_NULL "(null)"



// Class deException
//////////////////////

// Constructors, Destructors
//////////////////////////////

deException::deException( const char *name, const char *description, const char *file, int line ) :
pName( name ? name : STR_NULL ),
pDescription( description ? description : STR_NULL ),
pFile( file ? file : STR_NULL ),
pLine( decMath::max( line, 0 ) ){
	pBuildBacktrace();
}

deException::~deException(){
}



// Management
///////////////
bool deException::IsNamed( const char *name ) const{
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
	text.Format( "Exception: %s", pName.GetString() );
	output.Add( text );
	
	text.Format( "Description: %s", pDescription.GetString() );
	output.Add( text );
	
	text.Format( "Source File: %s", pFile.GetString() );
	output.Add( text );
	
	text.Format( "Source Line: %i", pLine );
	output.Add( text );
	
	const int backtraceCount = pBacktrace.GetCount();
	int i;
	for( i=0; i<backtraceCount; i++ ){
		text.Format( "Backtrace: %s", pBacktrace[ i ].GetString() );
		output.Add( text );
	}
	
	output += AdditionalInformation();
	
	return output;
}

void deException::PrintError() const{
	const decStringList output = FormatOutput();
	const int count = output.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		printf( "%s\n", output.GetAt( i ).GetString() );
	}
}



// Private Functions
//////////////////////

// we want this many entries at max
#define MAX_BACKTRACE_COUNT		25

// this many first entries are related to exception handling. skip them
#define SKIP_SELF_TRACE_COUNT	3

#ifdef ANDROID
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

void deException::pBuildBacktrace(){
#if defined OS_UNIX && ! defined ANDROID && ! defined OS_BEOS
	const int maxFramepointerCount = MAX_BACKTRACE_COUNT + SKIP_SELF_TRACE_COUNT;
	void *framepointers[ maxFramepointerCount ];
	const int fpcount = backtrace( framepointers, maxFramepointerCount );
	if( fpcount == 0 ){
		return;
	}
	
	char ** const symbols = backtrace_symbols( framepointers, fpcount );
	int i;
	
	for( i=SKIP_SELF_TRACE_COUNT; i<fpcount; i++ ){
		pBacktrace.Add( symbols[ i ] );
	}
	
	free( symbols );
#endif
	
#ifdef ANDROID
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
					( const char* )addr - ( const char* )info.dli_saddr, addr, demangled );
				free( demangled );
				
			}else{
				symbol.Format( "%s(%s+0x%x) [%p]", info.dli_fname, info.dli_sname,
					( const char* )addr - ( const char* )info.dli_saddr, addr );
			}
			
		}else{
			symbol.Format( "%p ??", addr );
		}
		
		pBacktrace.Add( symbol );
	}
#endif

#ifdef OS_W32
/*
	void *framepointers[ MAX_BACKTRACE_COUNT ];
	const int fpcount = CaptureStackBackTrace( SKIP_SELF_TRACE_COUNT, MAX_BACKTRACE_COUNT, &framepointers, NULL );
	if( fpcount == 0 ){
		return;
	}
	
	DWORD64 dwDisplacement = 0;
	
	char buffer[ sizeof( SYMBOL_INFO ) + MAX_SYM_NAME * sizeof( TCHAR ) ];
	PSYMBOL_INFO pSymbol = ( PSYMBOL_INFO )buffer;
	
	pSymbol->SizeOfStruct = sizeof( SYMBOL_INFO );
	pSymbol->MaxNameLen = MAX_SYM_NAME;
	
	if( ! SymFromAddr( GetCurrentProcess(), SOME_ADDRESS, &dwDisplacement, pSymbol ) ){
		return; // SymFromAddr failed
	}
	
	
	
	
	
	
	char ** const symbols = backtrace_symbols( framepointers, fpcount );
	int i;
	
	for( i=0; i<fpcount; i++ ){
		pBacktrace.Add( symbols[ i ] );
	}
	
	free( symbols );
	*/
#endif
}
