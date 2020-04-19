/* 
 * Drag[en]gine Console Launcher
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

#include "declGPDisableModuleVersion.h"

#include <dragengine/common/exceptions.h>



// Class declGPDisableModuleVersion
/////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

declGPDisableModuleVersion::declGPDisableModuleVersion(){
}

declGPDisableModuleVersion::declGPDisableModuleVersion( const char *name, const char *version ) :
pName( name ),
pVersion( version ){
}

declGPDisableModuleVersion::declGPDisableModuleVersion( const declGPDisableModuleVersion &copy ) :
pName( copy.pName ),
pVersion( copy.pVersion ){
}

declGPDisableModuleVersion::~declGPDisableModuleVersion(){
}



// Management
///////////////

void declGPDisableModuleVersion::SetName( const char *name ){
	pName = name;
}

void declGPDisableModuleVersion::SetVersion( const char *version ){
	pVersion = version;
}



// Operators
//////////////

bool declGPDisableModuleVersion::operator==( const declGPDisableModuleVersion &other ) const{
	return pName == other.pName && pVersion == other.pVersion;
}

bool declGPDisableModuleVersion::operator!=( const declGPDisableModuleVersion &other ) const{
	return pName != other.pName || pVersion != other.pVersion;
}

declGPDisableModuleVersion &declGPDisableModuleVersion::operator=( const declGPDisableModuleVersion &other ){
	pName = other.pName;
	pVersion = other.pVersion;
	return *this;
}
