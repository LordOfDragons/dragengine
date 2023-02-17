/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include "deoglDebugNamesEnum.h"


// Class deoglDebugNamesEnum
//////////////////////////////

const deoglDebugNamesEnum::sEntry deoglDebugNamesEnum::EndOfList = { 0, "" };

// Constructor, destructor
////////////////////////////

deoglDebugNamesEnum::deoglDebugNamesEnum( const char *name, const sEntry *entries ) :
pName( name ),
pEntries( entries ){
}



// Management
///////////////

decString deoglDebugNamesEnum::EntryName( int value, bool full ) const{
	const sEntry *entry = pEntries;
	
	while( entry->name ){
		if( entry->value == value ){
			if( full ){
				decString name;
				name.Format( "%s::%s", pName, entry->name );
				return name;
				
			}else{
				return entry->name;
			}
		}
		entry++;
	}
	
	decString name;
	if( full ){
		name.Format( "%s::%d", pName, value );
		
	}else{
		name.Format( "%d", value );
	}
	return name;
}
