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

#include "fbxPropertyArrayBool.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyArrayBool
///////////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyArrayBool::fbxPropertyArrayBool() :
fbxProperty( etArrayBoolean ),
pValues( NULL ),
pCount( 0 ){
}

fbxPropertyArrayBool::fbxPropertyArrayBool( decBaseFileReader &reader ) :
fbxProperty( etArrayBoolean ),
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
		pValues = new bool[ count ];
		for( pCount=0; pCount<count; pCount++ ){
			pValues[ pCount ] = valueReader->ReadByte() != 0;
		}
		
	}catch( const deException & ){
		if( pValues ){
			delete [] pValues;
		}
		throw;
	}
}

fbxPropertyArrayBool::~fbxPropertyArrayBool(){
	if( pValues ){
		delete [] pValues;
	}
}



// Loading and Saving
///////////////////////

bool fbxPropertyArrayBool::GetValueAt( int index ) const{
	if( index < 0 || index >= pCount ){
		DETHROW( deeInvalidParam );
	}
	return pValues[ index ];
}

void fbxPropertyArrayBool::AddValue( bool value ){
	bool * const newArray = new bool[ pCount + 1 ];
	if( pValues ){
		memcpy( newArray, pValues, sizeof( bool ) * pCount );
		delete [] pValues;
	}
	pValues = newArray;
	pValues[ pCount++ ] = value;
}

fbxPropertyArrayBool &fbxPropertyArrayBool::CastArrayBool(){
	return *this;
}

int fbxPropertyArrayBool::GetValueCount(){
	return GetCount();
}

bool fbxPropertyArrayBool::GetValueAtAsBool( int index ){
	return GetValueAt( index );
}

int fbxPropertyArrayBool::GetValueAtAsInt( int index ){
	return GetValueAt( index );
}

float fbxPropertyArrayBool::GetValueAtAsFloat( int index ){
	return GetValueAt( index );
}

double fbxPropertyArrayBool::GetValueAtAsDouble( int index ){
	return GetValueAt( index );
}



void fbxPropertyArrayBool::Save(decBaseFileWriter &writer ){
}

void fbxPropertyArrayBool::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Bool[%d]", prefix.GetString(), pCount );
}
