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

#include "fbxPropertyArrayLong.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyArrayLong
///////////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyArrayLong::fbxPropertyArrayLong() :
fbxProperty( etArrayLong ),
pValues( NULL ),
pCount( 0 ){
}

fbxPropertyArrayLong::fbxPropertyArrayLong( decBaseFileReader &reader ) :
fbxProperty( etArrayLong ),
pValues( NULL ),
pCount( 0 )
{
	const int count = reader.ReadUInt();
	
	decBaseFileReaderReference valueReader;
	pGetValueReader( reader, valueReader );
	
	if( count == 0 ){
		return;
	}
	
	try{
		pValues = new int64_t[ count ];
		for( pCount=0; pCount<count; pCount++ ){
			pValues[ pCount ] = valueReader->ReadLong();
		}
		
	}catch( const deException & ){
		if( pValues ){
			delete [] pValues;
		}
		throw;
	}
}

fbxPropertyArrayLong::~fbxPropertyArrayLong(){
	if( pValues ){
		delete [] pValues;
	}
}



// Loading and Saving
///////////////////////

int64_t fbxPropertyArrayLong::GetValueAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return pValues[ index ];
}

void fbxPropertyArrayLong::AddValue( int64_t value ){
	int64_t * const newArray = new int64_t[ pCount + 1 ];
	if( pValues ){
		memcpy( newArray, pValues, sizeof( int64_t ) * pCount );
		delete [] pValues;
	}
	pValues = newArray;
	pValues[ pCount++ ] = value;
}

fbxPropertyArrayLong &fbxPropertyArrayLong::CastArrayLong(){
	return *this;
}

int fbxPropertyArrayLong::GetValueCount() const{
	return GetCount();
}

bool fbxPropertyArrayLong::GetValueAtAsBool( int index ) const{
	return GetValueAt( index );
}

int fbxPropertyArrayLong::GetValueAtAsInt( int index ) const{
	return ( int )GetValueAt( index );
}

int64_t fbxPropertyArrayLong::GetValueAtAsLong( int index ) const{
	return GetValueAt( index );
}

float fbxPropertyArrayLong::GetValueAtAsFloat( int index ) const{
	return ( float )GetValueAt( index );
}

double fbxPropertyArrayLong::GetValueAtAsDouble( int index ) const{
	return ( double )GetValueAt( index );
}



void fbxPropertyArrayLong::Save(decBaseFileWriter &writer ){
}

void fbxPropertyArrayLong::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Long[%d]", prefix.GetString(), pCount );
}
