/* 
 * FBX Model Module
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
#include <string.h>

#include "fbxPropertyArrayFloat.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyArrayFloat
///////////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyArrayFloat::fbxPropertyArrayFloat() :
fbxProperty( etArrayFloat ),
pValues( NULL ),
pCount( 0 ){
}

fbxPropertyArrayFloat::fbxPropertyArrayFloat( decBaseFileReader &reader ) :
fbxProperty( etArrayFloat ),
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
		pValues = new float[ count ];
		for( pCount=0; pCount<count; pCount++ ){
			pValues[ pCount ] = valueReader->ReadFloat();
		}
		
	}catch( const deException & ){
		if( pValues ){
			delete [] pValues;
		}
		throw;
	}
}

fbxPropertyArrayFloat::~fbxPropertyArrayFloat(){
	if( pValues ){
		delete [] pValues;
	}
}



// Loading and Saving
///////////////////////

float fbxPropertyArrayFloat::GetValueAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return pValues[ index ];
}

void fbxPropertyArrayFloat::AddValue( float value ){
	float * const newArray = new float[ pCount + 1 ];
	if( pValues ){
		memcpy( newArray, pValues, sizeof( float ) * pCount );
		delete [] pValues;
	}
	pValues = newArray;
	pValues[ pCount++ ] = value;
}

fbxPropertyArrayFloat &fbxPropertyArrayFloat::CastArrayFloat(){
	return *this;
}

int fbxPropertyArrayFloat::GetValueCount() const{
	return GetCount();
}

bool fbxPropertyArrayFloat::GetValueAtAsBool( int index ) const{
	return GetValueAt( index );
}

int fbxPropertyArrayFloat::GetValueAtAsInt( int index ) const{
	return GetValueAt( index );
}

float fbxPropertyArrayFloat::GetValueAtAsFloat( int index ) const{
	return GetValueAt( index );
}

double fbxPropertyArrayFloat::GetValueAtAsDouble( int index ) const{
	return GetValueAt( index );
}

decVector2 fbxPropertyArrayFloat::GetValueAtAsVector2( int index ) const{
	const int begin = index * 2;
	if( begin < 0 || begin + 1 >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return decVector2( pValues[ begin ], pValues[ begin + 1 ] );
}

decVector fbxPropertyArrayFloat::GetValueAtAsVector( int index ) const{
	const int begin = index * 3;
	if( begin < 0 || begin + 2 >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return decVector( pValues[ begin ], pValues[ begin + 1 ], pValues[ begin + 2 ] );
}

decMatrix fbxPropertyArrayFloat::GetValueAtAsMatrix( int index ) const{
	const int begin = index * 16;
	if( begin < 0 || begin + 15 >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	decMatrix matrix;
	matrix.a11 = pValues[ begin ];
	matrix.a12 = pValues[ begin + 4 ];
	matrix.a13 = pValues[ begin + 8 ];
	matrix.a14 = pValues[ begin + 12 ];
	matrix.a21 = pValues[ begin + 1 ];
	matrix.a22 = pValues[ begin + 5 ];
	matrix.a23 = pValues[ begin + 9 ];
	matrix.a24 = pValues[ begin + 13 ];
	matrix.a31 = pValues[ begin + 2 ];
	matrix.a32 = pValues[ begin + 6 ];
	matrix.a33 = pValues[ begin + 10 ];
	matrix.a34 = pValues[ begin + 14 ];
	matrix.a41 = pValues[ begin + 3 ];
	matrix.a42 = pValues[ begin + 7 ];
	matrix.a43 = pValues[ begin + 11 ];
	matrix.a44 = pValues[ begin + 15 ];
	return matrix;
}



void fbxPropertyArrayFloat::Save(decBaseFileWriter &writer ){
}

void fbxPropertyArrayFloat::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Float[%d]", prefix.GetString(), pCount );
}
