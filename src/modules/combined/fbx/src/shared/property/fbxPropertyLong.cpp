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
#include <inttypes.h>

#include "fbxPropertyLong.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/deBaseModule.h>



// Class fbxPropertyLong
//////////////////////////

// Constructor, destructor
////////////////////////////

fbxPropertyLong::fbxPropertyLong() :
fbxProperty( etLong ),
pValue( 0 ){
}

fbxPropertyLong::fbxPropertyLong( decBaseFileReader &reader ) :
fbxProperty( etLong ),
pValue( reader.ReadLong() ){
}

fbxPropertyLong::~fbxPropertyLong(){
}



// Loading and Saving
///////////////////////

void fbxPropertyLong::SetValue( int64_t value ){
	pValue = value;
}

fbxPropertyLong &fbxPropertyLong::CastLong(){
	return *this;
}

bool fbxPropertyLong::GetValueAsBool() const{
	return pValue;
}

int fbxPropertyLong::GetValueAsInt() const{
	return pValue;
}

int64_t fbxPropertyLong::GetValueAsLong() const{
	return pValue;
}

float fbxPropertyLong::GetValueAsFloat() const{
	return pValue;
}

double fbxPropertyLong::GetValueAsDouble() const{
	return pValue;
}



void fbxPropertyLong::Save(decBaseFileWriter &writer ){
}

void fbxPropertyLong::DebugPrintStructure( deBaseModule &module, const decString &prefix ) const{
#if defined __MINGW32__ || defined __MINGW64__
	#ifdef PRId64
		#undef PRId64
	#endif
	#define PRId64 "I64u"
#endif
	module.LogInfoFormat( "%sProperty Long: %" PRId64, prefix.GetString(), pValue );
}
