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

#include "fbxPropertyFloat.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyFloat
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyFloat::fbxPropertyFloat() :
fbxProperty( etFloat ),
pValue( 0.0f ){
}

fbxPropertyFloat::fbxPropertyFloat( decBaseFileReader &reader ) :
fbxProperty( etFloat ),
pValue( reader.ReadFloat() ){
}

fbxPropertyFloat::~fbxPropertyFloat(){
}



// Loading and Saving
///////////////////////

void fbxPropertyFloat::SetValue( float value ){
	pValue = value;
}

fbxPropertyFloat &fbxPropertyFloat::CastFloat(){
	return *this;
}

bool fbxPropertyFloat::GetValueAsBool(){
	return pValue;
}

int fbxPropertyFloat::GetValueAsInt(){
	return pValue;
}

float fbxPropertyFloat::GetValueAsFloat(){
	return pValue;
}

double fbxPropertyFloat::GetValueAsDouble(){
	return pValue;
}



void fbxPropertyFloat::Save(decBaseFileWriter &writer ){
}

void fbxPropertyFloat::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Float: %g", prefix.GetString(), pValue );
}
