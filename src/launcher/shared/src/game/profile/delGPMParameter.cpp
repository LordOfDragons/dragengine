/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "delGPMParameter.h"

#include <dragengine/common/exceptions.h>



// Class delGPMParameter
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPMParameter::delGPMParameter( const char *name, const char *value ) :
pName( name ),
pValue( value ){
}

delGPMParameter::delGPMParameter( const delGPMParameter &parameter ) :
pName( parameter.pName ),
pValue( parameter.pValue ){
}

delGPMParameter::~delGPMParameter(){
}



// Management
///////////////

void delGPMParameter::SetName( const char *name ){
	pName = name;
}

void delGPMParameter::SetValue( const char *value ){
	pValue = value;
}



// Operators
//////////////

delGPMParameter &delGPMParameter::operator=( const delGPMParameter& parameter ){
	pName = parameter.pName;
	pValue = parameter.pValue;
	return *this;
}
