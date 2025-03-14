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

#include "../../alconfig.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deException.h"
#include "../string/decStringList.h"
#include "../math/decMath.h"

#include <android/log.h>
#include <unwind.h>
#include <dlfcn.h>



// Definitions
////////////////

#define STR_NULL "(null)"



// Class deException
//////////////////////

// Constructors
/////////////////

deException::deException( const char *name, const char *description, const char *file, int line ) :
pName( name ? name : STR_NULL ),
pDescription( description ? description : STR_NULL ),
pFile( file ? file : STR_NULL ),
pLine( decMath::max( line, 0 ) ){
	pBuildBacktrace();
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
		__android_log_print( ANDROID_LOG_VERBOSE, "DEAndroidLauncher", "%s", output.GetAt( i ).GetString() );
	}
}



// Private Functions
//////////////////////

struct sBacktraceState{
	const void **current;
	const void **end;
};

static _Unwind_Reason_Code unwindCallback( _Unwind_Context *context, void *arg ){
	__android_log_print( ANDROID_LOG_VERBOSE, "CHECKPOINT", "%i", __LINE__ );
	sBacktraceState &state = *( ( sBacktraceState* )arg );
	const uintptr_t pc = _Unwind_GetIP( context );
	if( pc ){
	__android_log_print( ANDROID_LOG_VERBOSE, "CHECKPOINT", "%i", __LINE__ );
		if( state.current == state.end ){
	__android_log_print( ANDROID_LOG_VERBOSE, "CHECKPOINT", "%i", __LINE__ );
			return _URC_END_OF_STACK;
			
		}else{
	__android_log_print( ANDROID_LOG_VERBOSE, "CHECKPOINT", "%i", __LINE__ );
			*state.current++ = ( const void* )pc;
		}
	}
	__android_log_print( ANDROID_LOG_VERBOSE, "CHECKPOINT", "%i", __LINE__ );
	
	return _URC_NO_REASON;
}

// we want this many entries at max
#define MAX_BACKTRACE_COUNT		25

// this many first entries are related to exception handling. skip them
#define SKIP_SELF_TRACE_COUNT	3

void deException::pBuildBacktrace(){
	// !!! WARNING !!!
	// _Unwind_Backtrace segfaults after calling unwindCallback for the first time. If used
	// buildnig a stack traces kills the application with SEGV. Why the hell can't google
	// developers once in life-time implement something which does NOT SUCK!!!!
	return;
	
	const void *context[ MAX_BACKTRACE_COUNT ];
	const void ** const stateBegin = &context[ 0 ];
	sBacktraceState state;
	state.current = stateBegin;
	state.end = stateBegin + MAX_BACKTRACE_COUNT;
	__android_log_print( ANDROID_LOG_VERBOSE, "CHECKPOINT", "%i", __LINE__ );
	_Unwind_Backtrace( unwindCallback, &state );
	__android_log_print( ANDROID_LOG_VERBOSE, "CHECKPOINT", "%i", __LINE__ );
	const size_t count = ( size_t )( state.current - stateBegin );
	decString symbol;
	size_t i;
	
	for( i=SKIP_SELF_TRACE_COUNT; i<count; i++ ){
		const void * const addr = context[ i ];
		
		Dl_info info;
		if( dladdr( addr, &info ) && info.dli_sname ){
			symbol.Format( "%s(%s+%p) [%p]", info.dli_fname, info.dli_sname,
				( const char* )addr - ( const char* )info.dli_saddr, addr );
			
		}else{
			symbol.Format( "%p ??", addr );
		}
		
		pBacktrace.Add( symbol );
	}
}
