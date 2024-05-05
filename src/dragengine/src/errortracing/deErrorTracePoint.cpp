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
