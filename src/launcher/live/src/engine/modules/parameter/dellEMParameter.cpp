/* 
 * Drag[en]gine Live Launcher
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

#include "dellEMParameter.h"

#include <dragengine/common/exceptions.h>



// Class dellEMParameter
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

dellEMParameter::dellEMParameter(){
	pIndex = -1;
}

dellEMParameter::~dellEMParameter(){
}



// Management
///////////////

void dellEMParameter::SetIndex( int index ){
	if( index < -1 ){
		DETHROW( deeInvalidParam );
	}
	
	pIndex = index;
}

void dellEMParameter::SetName( const char *name ){
	pName = name;
}

void dellEMParameter::SetDescription( const char *description ){
	pDescription = description;
}

void dellEMParameter::SetValue( const char *value ){
	pValue = value;
}
