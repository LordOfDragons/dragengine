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

#include "fbxPropertyArrayDouble.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyArrayDouble
///////////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyArrayDouble::fbxPropertyArrayDouble() :
fbxProperty( etArrayDouble ),
pValues( NULL ),
pCount( 0 ){
}

fbxPropertyArrayDouble::fbxPropertyArrayDouble( decBaseFileReader &reader ) :
fbxProperty( etArrayDouble ),
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
		pValues = new double[ count ];
		for( pCount=0; pCount<count; pCount++ ){
			pValues[ pCount ] = valueReader->ReadDouble();
		}
		
	}catch( const deException & ){
		if( pValues ){
			delete [] pValues;
		}
		throw;
	}
}

fbxPropertyArrayDouble::~fbxPropertyArrayDouble(){
	if( pValues ){
		delete [] pValues;
	}
}



// Loading and Saving
///////////////////////

double fbxPropertyArrayDouble::GetValueAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return pValues[ index ];
}

void fbxPropertyArrayDouble::AddValue( double value ){
	double * const newArray = new double[ pCount + 1 ];
	if( pValues ){
		memcpy( newArray, pValues, sizeof( double ) * pCount );
		delete [] pValues;
	}
	pValues = newArray;
	pValues[ pCount++ ] = value;
}

fbxPropertyArrayDouble &fbxPropertyArrayDouble::CastArrayDouble(){
	return *this;
}

int fbxPropertyArrayDouble::GetValueCount() const{
	return GetCount();
}

bool fbxPropertyArrayDouble::GetValueAtAsBool( int index ) const{
	return GetValueAt( index );
}

int fbxPropertyArrayDouble::GetValueAtAsInt( int index ) const{
	return ( int )GetValueAt( index );
}

int64_t fbxPropertyArrayDouble::GetValueAtAsLong( int index ) const{
	return ( int64_t )GetValueAt( index );
}

float fbxPropertyArrayDouble::GetValueAtAsFloat( int index ) const{
	return ( float )GetValueAt( index );
}

double fbxPropertyArrayDouble::GetValueAtAsDouble( int index ) const{
	return GetValueAt( index );
}

decVector2 fbxPropertyArrayDouble::GetValueAtAsVector2( int index ) const{
	const int begin = index * 2;
	if( begin < 0 || begin + 1 >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return decVector2( ( float )pValues[ begin ], ( float )pValues[ begin + 1 ] );
}

decVector fbxPropertyArrayDouble::GetValueAtAsVector( int index ) const{
	const int begin = index * 3;
	if( begin < 0 || begin + 2 >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return decVector( ( float )pValues[ begin ], ( float )pValues[ begin + 1 ], ( float )pValues[ begin + 2 ] );
}

decMatrix fbxPropertyArrayDouble::GetValueAtAsMatrix( int index ) const{
	const int begin = index * 16;
	if( begin < 0 || begin + 15 >= pCount ){
		DETHROW( deeInvalidParam );
	}
	
	decMatrix matrix;
	matrix.a11 = ( float )pValues[ begin ];
	matrix.a12 = ( float )pValues[ begin + 4 ];
	matrix.a13 = ( float )pValues[ begin + 8 ];
	matrix.a14 = ( float )pValues[ begin + 12 ];
	matrix.a21 = ( float )pValues[ begin + 1 ];
	matrix.a22 = ( float )pValues[ begin + 5 ];
	matrix.a23 = ( float )pValues[ begin + 9 ];
	matrix.a24 = ( float )pValues[ begin + 13 ];
	matrix.a31 = ( float )pValues[ begin + 2 ];
	matrix.a32 = ( float )pValues[ begin + 6 ];
	matrix.a33 = ( float )pValues[ begin + 10 ];
	matrix.a34 = ( float )pValues[ begin + 14 ];
	matrix.a41 = ( float )pValues[ begin + 3 ];
	matrix.a42 = ( float )pValues[ begin + 7 ];
	matrix.a43 = ( float )pValues[ begin + 11 ];
	matrix.a44 = ( float )pValues[ begin + 15 ];
	return matrix;
}



void fbxPropertyArrayDouble::Save(decBaseFileWriter &writer ){
}

void fbxPropertyArrayDouble::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Double[%d]", prefix.GetString(), pCount );
}
