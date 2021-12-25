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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deErrorTrace.h"
#include "deErrorTracePoint.h"
#include "deErrorTraceValue.h"
#include "../dragengine_configuration.h"
#include "../common/exceptions.h"
#include "../systems/deModuleSystem.h"
#include "../systems/modules/deLoadableModule.h"
#include "../systems/modules/deBaseModule.h"
#include "../systems/modules/deModuleParameter.h"



// class deErrorTrace
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////
#include "../logger/deLogger.h"

deErrorTrace::deErrorTrace() :
pError( "Unknown" ),
pPoints( nullptr ),
pPointCount( 0 ),
pPointSize( 0 ){
}

deErrorTrace::~deErrorTrace(){
	RemoveAllPoints();
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

void deErrorTrace::SetError( const char *error ){
	pError = error;
}

void deErrorTrace::Clear(){
	RemoveAllPoints();
	pError = "Unknown";
}



// Trace Point Management
///////////////////////////

deErrorTracePoint *deErrorTrace::GetPoint( int index ) const{
	if( index < 0 || index >= pPointCount ) DETHROW( deeInvalidParam );
	return pPoints[ index ];
}

void deErrorTrace::AddPoint( deErrorTracePoint *point ){
	if( ! point ) DETHROW( deeInvalidParam );
	if( pPointCount == pPointSize ){
		int i, newSize = pPointSize * 3 / 2 + 1;
		deErrorTracePoint **newArray = new deErrorTracePoint*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pPoints ){
			for( i=0; i<pPointCount; i++ ) newArray[ i ] = pPoints[ i ];
			delete [] pPoints;
		}
		pPoints = newArray;
		pPointSize = newSize;
	}
	pPoints[ pPointCount ] = point;
	pPointCount++;
}

void deErrorTrace::RemoveAllPoints(){
	int i;
	for( i=0; i<pPointCount; i++ ){
		if( pPoints[ i ] ) delete pPoints[ i ];
	}
	pPointCount = 0;
}



// Convenience Functions
//////////////////////////

deErrorTracePoint *deErrorTrace::AddAndSetIfEmpty( const char *error, deLoadableModule *sourceModule, const char *sourceFunc, int sourceLine ){
	if( ! error || ! sourceFunc || sourceLine < 0 ) DETHROW( deeInvalidParam );
	if( pPointCount > 0 ) return NULL;
	pError = error;
	return AddPoint( sourceModule, sourceFunc, sourceLine );
}

deErrorTracePoint *deErrorTrace::AddPoint( deLoadableModule *sourceModule, const char *sourceFunc, int sourceLine ){
	if( ! sourceFunc || sourceLine < 0 ) DETHROW( deeInvalidParam );
	deErrorTracePoint *newPoint = NULL;
	try{
		newPoint = new deErrorTracePoint( sourceModule, sourceFunc, sourceLine );
		if( ! newPoint ) DETHROW( deeOutOfMemory );
		AddPoint( newPoint );
	}catch( const deException & ){
		if( newPoint ) delete newPoint;
		throw;
	}
	return newPoint;
}

void deErrorTrace::PrintTrace( deLogger &logger ){
	const char *loggingName = "ErrorTrace";
	int i, j;
	
	logger.LogErrorFormat( loggingName, "Error %s.", pError.GetString() );
	for( i=0; i<pPointCount; i++ ){
		if( pPoints[ i ]->GetSourceModule() ){
			logger.LogErrorFormat( loggingName, "%i) %s %s at %i.", i + 1,
				pPoints[ i ]->GetSourceModule()->GetName().GetString(),
				pPoints[ i ]->GetSourceFunction().GetString(), pPoints[ i ]->GetSourceLine() );
		}else{
			//moduleType = "Drag[en]gine";
			logger.LogErrorFormat( loggingName, "%i) Game Engine %s at %i.", i + 1,
				pPoints[ i ]->GetSourceFunction().GetString(), pPoints[ i ]->GetSourceLine() );
		}
		for( j=0; j<pPoints[ i ]->GetValueCount(); j++ ){
			logger.LogErrorFormat( loggingName, "  - %s = '%s'.",
				pPoints[ i ]->GetValue( j )->GetName().GetString(),
				pPoints[ i ]->GetValue( j )->GetValue().GetString() );
		}
	}
}
