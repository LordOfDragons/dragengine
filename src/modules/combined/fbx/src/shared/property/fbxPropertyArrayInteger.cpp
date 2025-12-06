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

#include "fbxPropertyArrayInteger.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyArrayInteger
///////////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyArrayInteger::fbxPropertyArrayInteger() :
fbxProperty( etArrayInteger ),
pValues( NULL ),
pCount( 0 ){
}

fbxPropertyArrayInteger::fbxPropertyArrayInteger( decBaseFileReader &reader ) :
fbxProperty( etArrayInteger ),
pValues( NULL ),
pCount( 0 )
{
	const int count = reader.ReadUInt();
	
	decBaseFileReader::Ref valueReader;
	pGetValueReader( reader, valueReader );
	
	if( count == 0 ){
		return;
	}
	
	try{
		pValues = new int[ count ];
		for( pCount=0; pCount<count; pCount++ ){
			pValues[ pCount ] = valueReader->ReadInt();
		}
		
	}catch( const deException & ){
		if( pValues ){
			delete [] pValues;
		}
		throw;
	}
}

fbxPropertyArrayInteger::~fbxPropertyArrayInteger(){
	if( pValues ){
		delete [] pValues;
	}
}



// Loading and Saving
///////////////////////

int fbxPropertyArrayInteger::GetValueAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return pValues[ index ];
}

void fbxPropertyArrayInteger::AddValue( int value ){
	int * const newArray = new int[ pCount + 1 ];
	if( pValues ){
		memcpy( newArray, pValues, sizeof( int ) * pCount );
		delete [] pValues;
	}
	pValues = newArray;
	pValues[ pCount++ ] = value;
}

fbxPropertyArrayInteger &fbxPropertyArrayInteger::CastArrayInteger(){
	return *this;
}

int fbxPropertyArrayInteger::GetValueCount() const{
	return GetCount();
}

bool fbxPropertyArrayInteger::GetValueAtAsBool( int index ) const{
	return GetValueAt( index );
}

int fbxPropertyArrayInteger::GetValueAtAsInt( int index ) const{
	return GetValueAt( index );
}

int64_t fbxPropertyArrayInteger::GetValueAtAsLong( int index ) const{
	return GetValueAt( index );
}

float fbxPropertyArrayInteger::GetValueAtAsFloat( int index ) const{
	return ( float )GetValueAt( index );
}

double fbxPropertyArrayInteger::GetValueAtAsDouble( int index ) const{
	return GetValueAt( index );
}



void fbxPropertyArrayInteger::Save(decBaseFileWriter &writer ){
}

void fbxPropertyArrayInteger::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Integer[%d]", prefix.GetString(), pCount );
}
