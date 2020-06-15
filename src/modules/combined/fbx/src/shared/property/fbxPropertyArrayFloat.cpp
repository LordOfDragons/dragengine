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

int fbxPropertyArrayFloat::GetValueCount(){
	return GetCount();
}

bool fbxPropertyArrayFloat::GetValueAtAsBool( int index ){
	return GetValueAt( index );
}

int fbxPropertyArrayFloat::GetValueAtAsInt( int index ){
	return GetValueAt( index );
}

float fbxPropertyArrayFloat::GetValueAtAsFloat( int index ){
	return GetValueAt( index );
}

double fbxPropertyArrayFloat::GetValueAtAsDouble( int index ){
	return GetValueAt( index );
}



void fbxPropertyArrayFloat::Save(decBaseFileWriter &writer ){
}

void fbxPropertyArrayFloat::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Float[%d]", prefix.GetString(), pCount );
}
