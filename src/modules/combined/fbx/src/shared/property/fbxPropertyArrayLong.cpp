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
	return GetValueAt( index );
}

float fbxPropertyArrayLong::GetValueAtAsFloat( int index ) const{
	return GetValueAt( index );
}

double fbxPropertyArrayLong::GetValueAtAsDouble( int index ) const{
	return GetValueAt( index );
}



void fbxPropertyArrayLong::Save(decBaseFileWriter &writer ){
}

void fbxPropertyArrayLong::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Long[%d]", prefix.GetString(), pCount );
}
