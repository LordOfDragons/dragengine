/* 
 * Drag[en]gine IGDE
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

#include <stdlib.h>
#include <string.h>

#include "igdeFilePattern.h"

#include "dragengine/common/exceptions.h"



// Class igdeFilePattern
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeFilePattern::igdeFilePattern( const char *name, const char *pattern, const char *defaultExtension ){
	if( ! name || ! pattern || ! defaultExtension ){
		DETHROW( deeInvalidParam );
	}
	
	pName = name;
	pPattern = pattern;
	pDefExtension = defaultExtension;
}

igdeFilePattern::igdeFilePattern( const igdeFilePattern &filePattern ){
	pName = filePattern.GetName();
	pPattern = filePattern.GetPattern();
	pDefExtension = filePattern.GetDefaultExtension();
}

igdeFilePattern::~igdeFilePattern(){
}
