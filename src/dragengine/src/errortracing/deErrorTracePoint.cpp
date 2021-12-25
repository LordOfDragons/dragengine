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
#include "deErrorTracePoint.h"
#include "deErrorTraceValue.h"
#include "../systems/modules/deLoadableModule.h"
#include "../systems/modules/deBaseModule.h"
#include "../systems/modules/deModuleParameter.h"
#include "../common/exceptions.h"



// class deErrorTracePoint
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deErrorTracePoint::deErrorTracePoint( const char *sourceFunc, int sourceLine ) :
pSourceModule( nullptr ),
pSourceFunc( sourceFunc ),
pSourceLine( sourceLine ),
pValues( nullptr ),
pValueCount( 0 ),
pValueSize( 0 )
{
	if( pSourceFunc.IsEmpty() ){
		DETHROW( deeInvalidParam );
	}
}

deErrorTracePoint::deErrorTracePoint( deLoadableModule *module, const char *sourceFunc, int sourceLine ) :
pSourceModule( module ),
pSourceFunc( sourceFunc ),
pSourceLine( sourceLine ),
pValues( nullptr ),
pValueCount( 0 ),
pValueSize( 0 )
{
	if( pSourceFunc.IsEmpty() ){
		DETHROW( deeInvalidParam );
	}
	if( pSourceModule ) pSourceModule->AddReference();
}

deErrorTracePoint::~deErrorTracePoint(){
	RemoveAllValues();
	if( pValues ) delete [] pValues;
	if( pSourceModule ) pSourceModule->FreeReference();
}



// Trace Value Management
///////////////////////////

deErrorTraceValue *deErrorTracePoint::GetValue( int index ) const{
	if( index < 0 || index >= pValueCount ) DETHROW( deeInvalidParam );
	return pValues[ index ];
}

deErrorTraceValue *deErrorTracePoint::FindValue( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	int i;
	for( i=0; i<pValueCount; i++ ){
		if( strcmp( name, pValues[ i ]->GetName() ) == 0 ) return pValues[ i ];
	}
	return NULL;
}

void deErrorTracePoint::AddValue( deErrorTraceValue *value ){
	if( ! value || FindValue( value->GetName() ) ) DETHROW( deeInvalidParam );
	if( pValueCount == pValueSize ){
		int i, newSize = pValueSize * 3 / 2 + 1;
		deErrorTraceValue **newArray = new deErrorTraceValue*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pValues ){
			for( i=0; i<pValueCount; i++ ) newArray[ i ] = pValues[ i ];
			delete [] pValues;
		}
		pValues = newArray;
		pValueSize = newSize;
	}
	pValues[ pValueCount ] = value;
	pValueCount++;
}

void deErrorTracePoint::RemoveAllValues(){
	int i;
	for( i=0; i<pValueCount; i++ ){
		if( pValues[ i ] ) delete pValues[ i ];
	}
	pValueCount = 0;
}



// Convenience Functions
//////////////////////////

deErrorTraceValue *deErrorTracePoint::AddValue( const char *name, const char *value ){
	deErrorTraceValue *newValue = NULL;
	try{
		newValue = new deErrorTraceValue( name, value );
		if( ! newValue ) DETHROW( deeOutOfMemory );
		AddValue( newValue );
	}catch( const deException & ){
		if( newValue ) delete newValue;
		throw;
	}
	return newValue;
}

deErrorTraceValue *deErrorTracePoint::AddValueInt( const char *name, int value ){
	deErrorTraceValue *newValue = NULL;
	char buffer[ 20 ];
	#ifdef _MSC_VER
		sprintf_s( ( char* )&buffer, 20, "%i", value );
	#else
		sprintf( ( char* )&buffer, "%i", value );
	#endif
	try{
		newValue = new deErrorTraceValue( name, buffer );
		if( ! newValue ) DETHROW( deeOutOfMemory );
		AddValue( newValue );
	}catch( const deException & ){
		if( newValue ) delete newValue;
		throw;
	}
	return newValue;
}

deErrorTraceValue *deErrorTracePoint::AddValueFloat( const char *name, float value ){
	deErrorTraceValue *newValue = NULL;
	char buffer[ 20 ];
	#ifdef _MSC_VER
		sprintf_s( ( char* )&buffer, 20, "%g", value );
	#else
		sprintf( ( char* )&buffer, "%g", value );
	#endif
	try{
		newValue = new deErrorTraceValue( name, buffer );
		if( ! newValue ) DETHROW( deeOutOfMemory );
		AddValue( newValue );
	}catch( const deException & ){
		if( newValue ) delete newValue;
		throw;
	}
	return newValue;
}

deErrorTraceValue *deErrorTracePoint::AddValueBool( const char *name, bool value ){
	deErrorTraceValue *newValue = NULL;
	try{
		if( value ){
			newValue = new deErrorTraceValue( name, "True" );
		}else{
			newValue = new deErrorTraceValue( name, "False" );
		}		
		if( ! newValue ) DETHROW( deeOutOfMemory );
		AddValue( newValue );
	}catch( const deException & ){
		if( newValue ) delete newValue;
		throw;
	}
	return newValue;
}
