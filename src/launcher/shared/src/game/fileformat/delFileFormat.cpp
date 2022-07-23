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

#include "delFileFormat.h"



// Class delFileFormat
////////////////////////

// Constructors and Destructors
/////////////////////////////////

delFileFormat::delFileFormat() :
pType( deModuleSystem::emtUnknown ),
pSupported( false ){
}

delFileFormat::delFileFormat( deModuleSystem::eModuleTypes type, const char *pattern ) :
pType( type ),
pPattern( pattern ),
pSupported( false ){
}

delFileFormat::~delFileFormat(){
}



// Management
///////////////

void delFileFormat::SetType( deModuleSystem::eModuleTypes type ){
	pType = type;
}

void delFileFormat::SetPattern( const char *pattern ){
	pPattern = pattern;
}

void delFileFormat::SetSupported( bool supported ){
	pSupported = supported;
}
