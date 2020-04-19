/* 
 * Drag[en]gine Simply Quit Crash Recovery Module
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

#include <stdio.h>
#include <stdlib.h>

#include "deCRSimplyQuit.h"
#include "decrsqCoreFault.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/systems/modules/deLoadableModule.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *SQCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry Point
////////////////

deBaseModule *SQCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deCRSimplyQuit( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deCRSimplyQuit
/////////////////////////

// Constructor, destructor
////////////////////////////

deCRSimplyQuit::deCRSimplyQuit( deLoadableModule &loadableModule ) :
deBaseCrashRecoveryModule( loadableModule ),
pCoreFault( NULL ){
}

deCRSimplyQuit::~deCRSimplyQuit(){
}



// Management
///////////////

bool deCRSimplyQuit::Init(){
	pCoreFault = new decrsqCoreFault( *this );
	return true;
}

void deCRSimplyQuit::CleanUp(){
	if( pCoreFault ){
		delete pCoreFault;
		pCoreFault = NULL;
	}
}



// Crash Management
/////////////////////

bool deCRSimplyQuit::RecoverFromError(){
	pPrintTrace();
	
	return false;
}



// Private Functions
//////////////////////

void deCRSimplyQuit::pPrintTrace(){
	deErrorTrace *trace = GetGameEngine()->GetErrorTrace();
	int i, count;
	
	LogError( "Error Trace:" );
	
	count = trace->GetPointCount();
	for( i=0; i<count; i++ ){
		pPrintTracePoint( i + 1, trace->GetPoint( i ) );
	}
}

void deCRSimplyQuit::pPrintTracePoint(int number, deErrorTracePoint *point ){
	int i, count = point->GetValueCount();
	
	if( point->GetSourceModule() ){
		LogErrorFormat( "%i) %s %s at %i\n", number, point->GetSourceModule()->GetName().GetString(),
			point->GetSourceFunction(), point->GetSourceLine() );
		
	}else{
		LogErrorFormat( "%i) game engine %s at %i\n", number, point->GetSourceFunction(), point->GetSourceLine() );
	}
	
	for( i=0; i<count; i++ ){
		pPrintTraceValue( 1, point->GetValue( i ) );
	}
}

void deCRSimplyQuit::pPrintTraceValue( int level, deErrorTraceValue *value ){
	int i, count = value->GetSubValueCount();
	decString text;
	
	for( i=0; i<level; i++ ) text.Append( "  " );
	LogErrorFormat( "%s- %s = '%s'", text.GetString(), value->GetName(), value->GetValue() );
	
	for( i=0; i<count; i++ ){
		pPrintTraceValue( level + 1, value->GetSubValue( i ) );
	}
}
