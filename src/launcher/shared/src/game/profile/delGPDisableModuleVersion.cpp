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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "delGPDisableModuleVersion.h"

#include <dragengine/common/exceptions.h>



// Class delGPDisableModuleVersion
////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

delGPDisableModuleVersion::delGPDisableModuleVersion(){
}

delGPDisableModuleVersion::delGPDisableModuleVersion( const char *name, const char *version ) :
pName( name ),
pVersion( version ){
}

delGPDisableModuleVersion::delGPDisableModuleVersion( const delGPDisableModuleVersion &copy ) :
pName( copy.pName ),
pVersion( copy.pVersion ){
}

delGPDisableModuleVersion::~delGPDisableModuleVersion(){
}



// Management
///////////////

void delGPDisableModuleVersion::SetName( const char *name ){
	pName = name;
}

void delGPDisableModuleVersion::SetVersion( const char *version ){
	pVersion = version;
}



// Operators
//////////////

bool delGPDisableModuleVersion::operator==( const delGPDisableModuleVersion &other ) const{
	return pName == other.pName && pVersion == other.pVersion;
}

bool delGPDisableModuleVersion::operator!=( const delGPDisableModuleVersion &other ) const{
	return pName != other.pName || pVersion != other.pVersion;
}

delGPDisableModuleVersion &delGPDisableModuleVersion::operator=( const delGPDisableModuleVersion &other ){
	pName = other.pName;
	pVersion = other.pVersion;
	return *this;
}
