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

#include "fbxPropertyBool.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyBool
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyBool::fbxPropertyBool() :
fbxProperty( etBoolean ),
pValue( false ){
}

fbxPropertyBool::fbxPropertyBool( decBaseFileReader &reader ) :
fbxProperty( etBoolean ),
pValue( reader.ReadChar() != 0 ){
}

fbxPropertyBool::~fbxPropertyBool(){
}



// Loading and Saving
///////////////////////

void fbxPropertyBool::SetValue( bool value ){
	pValue = value;
}

fbxPropertyBool &fbxPropertyBool::CastBool(){
	return *this;
}

bool fbxPropertyBool::GetValueAsBool() const{
	return pValue;
}

int fbxPropertyBool::GetValueAsInt() const{
	return pValue;
}

int64_t fbxPropertyBool::GetValueAsLong() const{
	return pValue;
}

float fbxPropertyBool::GetValueAsFloat() const{
	return pValue;
}

double fbxPropertyBool::GetValueAsDouble() const{
	return pValue;
}



void fbxPropertyBool::Save(decBaseFileWriter &writer ){
}

void fbxPropertyBool::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Bool: %s", prefix.GetString(), pValue ? "true" : "false" );
}
