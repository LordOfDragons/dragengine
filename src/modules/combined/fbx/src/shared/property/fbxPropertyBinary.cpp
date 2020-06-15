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

#include "fbxPropertyBinary.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyBinary
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyBinary::fbxPropertyBinary() :
fbxProperty( etBinary ),
pValue( NULL ),
pLength( 0 ){
}

fbxPropertyBinary::fbxPropertyBinary( decBaseFileReader &reader ) :
fbxProperty( etBinary ),
pValue( NULL ),
pLength( 0 )
{
	const int length = reader.ReadUInt();
	if( length > 0 ){
		pValue = new uint8_t[ length ];
		reader.Read( pValue, length );
	}
}

fbxPropertyBinary::~fbxPropertyBinary(){
	if( pValue ){
		delete [] pValue;
	}
}



// Loading and Saving
///////////////////////

void fbxPropertyBinary::SetValue( const uint8_t *value, int length ){
	if( length < 0 || ( length > 0 && ! value ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pValue ){
		delete [] pValue;
		pValue = NULL;
		pLength = 0;
	}
	
	if( length > 0 ){
		pValue = new uint8_t[ length ];
		memcpy( pValue, value, length );
		pLength = length;
	}
}

fbxPropertyBinary &fbxPropertyBinary::CastBinary(){
	return *this;
}



void fbxPropertyBinary::Save(decBaseFileWriter &writer ){
}

void fbxPropertyBinary::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Binary: length %d", prefix.GetString(), pLength );
}
