/* 
 * Drag[en]gine Synthesizer Module
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

#include "desynParameterFloat.h"

#include <dragengine/common/exceptions.h>



// class desynParameterFloat
//////////////////////////////

// Constructor, destructor
////////////////////////////

desynParameterFloat::desynParameterFloat( deDESynthesizer &synthesizer ) :
desynParameter( synthesizer ){
}



// Parameter Value
////////////////////

decString desynParameterFloat::GetParameterValue(){
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

void desynParameterFloat::SetParameterValue( const char *value ){
	SetParameterFloat( decString( value ).ToFloat() );
}
