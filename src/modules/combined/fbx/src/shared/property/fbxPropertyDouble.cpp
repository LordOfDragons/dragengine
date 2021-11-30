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

#include "fbxPropertyDouble.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyDouble
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyDouble::fbxPropertyDouble() :
fbxProperty( etDouble ),
pValue( 0.0 ){
}

fbxPropertyDouble::fbxPropertyDouble( decBaseFileReader &reader ) :
fbxProperty( etDouble ),
pValue( reader.ReadDouble() ){
}

fbxPropertyDouble::~fbxPropertyDouble(){
}



// Loading and Saving
///////////////////////

void fbxPropertyDouble::SetValue( double value ){
	pValue = value;
}

fbxPropertyDouble &fbxPropertyDouble::CastDouble(){
	return *this;
}

bool fbxPropertyDouble::GetValueAsBool() const{
	return ( bool )pValue;
}

int fbxPropertyDouble::GetValueAsInt() const{
	return ( int )pValue;
}

int64_t fbxPropertyDouble::GetValueAsLong() const{
	return ( int64_t )pValue;
}

float fbxPropertyDouble::GetValueAsFloat() const{
	return ( float )pValue;
}

double fbxPropertyDouble::GetValueAsDouble() const{
	return pValue;
}



void fbxPropertyDouble::Save(decBaseFileWriter &writer ){
}

void fbxPropertyDouble::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
	module.LogInfoFormat( "%sProperty Double: %g", prefix.GetString(), pValue );
}
