/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceControllerValue.h"

#include <dragengine/common/exceptions.h>



// Class ceControllerValue
////////////////////////////

// Constructor, destructor
////////////////////////////

ceControllerValue::ceControllerValue( int controller, float value ) :
pControllerIndex( controller ),
pValue( value ){
}

ceControllerValue::ceControllerValue( const char *controller, float value ) :
pControllerIndex( -1 ),
pController( controller ),
pValue( value ){
}

ceControllerValue::~ceControllerValue(){
}



// Management
///////////////

void ceControllerValue::SetControllerIndex( int controller ){
	pControllerIndex = controller;
}

void ceControllerValue::SetController( const char *controller ){
	pController = controller;
}

void ceControllerValue::SetValue( float value ){
	pValue = value;
}
