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
#include "deErrorTraceValue.h"
#include "../common/exceptions.h"



// class deErrorTraceValue
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deErrorTraceValue::deErrorTraceValue( const char *name, const char *value ) :
pName( name ),
pValue( value ),
pSubValues( nullptr ),
pSubValueCount( 0 ),
pSubValueSize( 0 ){
}

deErrorTraceValue::~deErrorTraceValue(){
	RemoveAllSubValues();
	if( pSubValues ) delete [] pSubValues;
}

		

// Management
////////////////

void deErrorTraceValue::SetValue( const char *value ){
	pValue = value;
}

void deErrorTraceValue::SetValueFrom( float value ){
	pValue.SetValue( value );
}



// Trace SubValue Management
//////////////////////////////

deErrorTraceValue *deErrorTraceValue::GetSubValue( int index ) const{
	if( index < 0 || index >= pSubValueCount ) DETHROW( deeInvalidParam );
	return pSubValues[ index ];
}

deErrorTraceValue *deErrorTraceValue::FindSubValue( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	int i;
	for( i=0; i<pSubValueCount; i++ ){
		if( strcmp( name, pSubValues[ i ]->GetName() ) == 0 ) return pSubValues[ i ];
	}
	return NULL;
}

void deErrorTraceValue::AddSubValue( deErrorTraceValue *value ){
	if( ! value || FindSubValue( value->GetName() ) ) DETHROW( deeInvalidParam );
	if( pSubValueCount == pSubValueSize ){
		int i, newSize = pSubValueSize * 3 / 2 + 1;
		deErrorTraceValue **newArray = new deErrorTraceValue*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pSubValues ){
			for( i=0; i<pSubValueCount; i++ ) newArray[ i ] = pSubValues[ i ];
			delete [] pSubValues;
		}
		pSubValues = newArray;
		pSubValueSize = newSize;
	}
	pSubValues[ pSubValueCount ] = value;
	pSubValueCount++;
}

void deErrorTraceValue::RemoveAllSubValues(){
	int i;
	for( i=0; i<pSubValueCount; i++ ){
		if( pSubValues[ i ] ) delete pSubValues[ i ];
	}
	pSubValueCount = 0;
}



// Convenience Functions
//////////////////////////

deErrorTraceValue *deErrorTraceValue::AddSubValue( const char *name, const char *value ){
	deErrorTraceValue *newSubValue = NULL;
	try{
		newSubValue = new deErrorTraceValue( name, value );
		if( ! newSubValue ) DETHROW( deeOutOfMemory );
		AddSubValue( newSubValue );
	}catch( const deException & ){
		if( newSubValue ) delete newSubValue;
		throw;
	}
	return newSubValue;
}

deErrorTraceValue *deErrorTraceValue::AddSubValueInt( const char *name, int value ){
	deErrorTraceValue *newSubValue = NULL;
	char buffer[ 20 ];
	#ifdef _MSC_VER
		sprintf_s( ( char* )&buffer, 20, "%i", value );
	#else
		sprintf( ( char* )&buffer, "%i", value );
	#endif
	try{
		newSubValue = new deErrorTraceValue( name, buffer );
		if( ! newSubValue ) DETHROW( deeOutOfMemory );
		AddSubValue( newSubValue );
	}catch( const deException & ){
		if( newSubValue ) delete newSubValue;
		throw;
	}
	return newSubValue;
}

deErrorTraceValue *deErrorTraceValue::AddSubValueFloat( const char *name, float value ){
	deErrorTraceValue *newSubValue = NULL;
	char buffer[ 20 ];
	#ifdef _MSC_VER
		sprintf_s( ( char* )&buffer, 20, "%g", value );
	#else
		sprintf( ( char* )&buffer, "%g", value );
	#endif
	try{
		newSubValue = new deErrorTraceValue( name, buffer );
		if( ! newSubValue ) DETHROW( deeOutOfMemory );
		AddSubValue( newSubValue );
	}catch( const deException & ){
		if( newSubValue ) delete newSubValue;
		throw;
	}
	return newSubValue;
}

deErrorTraceValue *deErrorTraceValue::AddSubValueBool( const char *name, bool value ){
	deErrorTraceValue *newSubValue = NULL;
	try{
		if( value ){
			newSubValue = new deErrorTraceValue( name, "True" );
		}else{
			newSubValue = new deErrorTraceValue( name, "False" );
		}		
		if( ! newSubValue ) DETHROW( deeOutOfMemory );
		AddSubValue( newSubValue );
	}catch( const deException & ){
		if( newSubValue ) delete newSubValue;
		throw;
	}
	return newSubValue;
}
