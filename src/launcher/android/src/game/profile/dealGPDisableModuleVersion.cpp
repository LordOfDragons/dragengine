/* 
 * Drag[en]gine Android Launcher
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

#include "dealGPDisableModuleVersion.h"

#include "../../common/exceptions.h"



// Class dealGPDisableModuleVersion
/////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dealGPDisableModuleVersion::dealGPDisableModuleVersion(){
}

dealGPDisableModuleVersion::dealGPDisableModuleVersion( const char *name, const char *version ) :
pName( name ),
pVersion( version ){
}

dealGPDisableModuleVersion::dealGPDisableModuleVersion( const dealGPDisableModuleVersion &copy ) :
pName( copy.pName ),
pVersion( copy.pVersion ){
}

dealGPDisableModuleVersion::~dealGPDisableModuleVersion(){
}



// Management
///////////////

void dealGPDisableModuleVersion::SetName( const char *name ){
	pName = name;
}

void dealGPDisableModuleVersion::SetVersion( const char *version ){
	pVersion = version;
}



// Operators
//////////////

bool dealGPDisableModuleVersion::operator==( const dealGPDisableModuleVersion &other ) const{
	return pName == other.pName && pVersion == other.pVersion;
}

bool dealGPDisableModuleVersion::operator!=( const dealGPDisableModuleVersion &other ) const{
	return pName != other.pName || pVersion != other.pVersion;
}

dealGPDisableModuleVersion &dealGPDisableModuleVersion::operator=( const dealGPDisableModuleVersion &other ){
	pName = other.pName;
	pVersion = other.pVersion;
	return *this;
}
