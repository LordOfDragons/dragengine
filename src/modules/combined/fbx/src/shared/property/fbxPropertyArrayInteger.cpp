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
	
	decBaseFileReaderReference valueReader;
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

float fbxPropertyArrayInteger::GetValueAtAsFloat( int index ) const{
	return GetValueAt( index );
}

double fbxPropertyArrayInteger::GetValueAtAsDouble( int index ) const{
	return GetValueAt( index );
}



void fbxPropertyArrayInteger::Save(decBaseFileWriter &writer ){
}

void fbxPropertyArrayInteger::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Integer[%d]", prefix.GetString(), pCount );
}
