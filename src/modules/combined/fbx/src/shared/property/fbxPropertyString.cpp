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

#include "fbxPropertyString.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyString
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyString::fbxPropertyString() :
fbxProperty( etString ){
}

fbxPropertyString::fbxPropertyString( decBaseFileReader &reader ) :
fbxProperty( etString )
{
	const int count = reader.ReadUInt();
	if( count > 0 ){
		pValue.Set( 0, count );
		reader.Read( ( char* )pValue.GetString(), count );
	}
}

fbxPropertyString::~fbxPropertyString(){
}



// Loading and Saving
///////////////////////

void fbxPropertyString::SetValue( const char *value ){
	pValue = value;
}

fbxPropertyString &fbxPropertyString::CastString(){
	return *this;
}



void fbxPropertyString::Save(decBaseFileWriter &writer ){
}

void fbxPropertyString::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty String: '%s'", prefix.GetString(), pValue.GetString() );
}
