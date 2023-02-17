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

#include "deoglDebugNamesEnumSet.h"


// Class deoglDebugNamesEnumSet
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugNamesEnumSet::deoglDebugNamesEnumSet( const char *name, const sEntry *entries ) :
deoglDebugNamesEnum( name, entries ){
}



// Management
///////////////

decString deoglDebugNamesEnumSet::SetName( int value, bool full ) const{
	decString name;
	if( full ){
		name = pName;
	}
	name.AppendCharacter( '{' );
	
	const char *separator = "";
	const sEntry *entry = pEntries;
	
	while( entry->name ){
		if( value & entry->value ){
			name.AppendFormat( "%s%s", separator, entry->name );
			separator = ", ";
			value &= ~entry->value;
		}
		entry++;
	}
	
	int i;
	for( i=0; i<31; i++ ){
		if( value & ( 1 << i ) ){
			name.AppendFormat( "%s%x", separator, 1 << i );
			separator = ", ";
		}
	}
	
	name.AppendCharacter( '}' );
	return name;
}
