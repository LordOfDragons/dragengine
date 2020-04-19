/* 
 * Drag[en]gine OpenAL Audio Module
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "deoalParameterFloat.h"

#include <dragengine/common/exceptions.h>



// Class deoalParameterFloat
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoalParameterFloat::deoalParameterFloat( deAudioOpenAL &oal ) : deoalParameter( oal )
{
	SetType( deModuleParameter::eptNumeric );
}

deoalParameterFloat::~deoalParameterFloat(){
}



// Management
///////////////

decString deoalParameterFloat::GetParameterValue(){
	decString value;
	
	value.Format( "%f", GetParameterFloat() );
	
	int len = value.GetLength();
	while( len > 1 ){
		len--;
		if( value[ len ] == '.' ){
			value[ len ] = 0;
			break;
			
		}else if( value[ len ] == '0' ){
			value[ len ] = 0;
			
		}else{
			break;
		}	
	}
	
	return value;
}

void deoalParameterFloat::SetParameterValue( const char *value ){
	SetParameterFloat( decString( value ).ToFloat() );
}
