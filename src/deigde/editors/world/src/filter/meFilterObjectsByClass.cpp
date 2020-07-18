/* 
 * Drag[en]gine IGDE World Editor
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

#include "meFilterObjectsByClass.h"
#include "../world/object/meObject.h"

#include <deigde/gamedefinition/class/igdeGDClass.h>

#include <dragengine/common/exceptions.h>



// Class meFilterObjectsByClass
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meFilterObjectsByClass::meFilterObjectsByClass(){
	pMatchInclusive = false;
	pRejectObject = NULL;
	pRejectGhosts = true;
}

meFilterObjectsByClass::~meFilterObjectsByClass(){
	if( pRejectObject ) pRejectObject->FreeReference();
	RemoveAllClassNames();
}



// Management
///////////////

void meFilterObjectsByClass::SetMatchInclusive( bool matchInclusive ){
	pMatchInclusive = matchInclusive;
}

void meFilterObjectsByClass::SetRejectGhosts( bool rejectGhosts ){
	pRejectGhosts = rejectGhosts;
}

void meFilterObjectsByClass::SetRejectObject( meObject *object ){
	if( object != pRejectObject ){
		if( pRejectObject ) pRejectObject->FreeReference();
		
		pRejectObject = object;
		
		if( object ) object->AddReference();
	}
}



void meFilterObjectsByClass::AddClassName( const char *className ){
	pClassNames.Add( className );
}

void meFilterObjectsByClass::RemoveAllClassNames(){
	pClassNames.RemoveAll();
}

void meFilterObjectsByClass::SetClassNamesFrom( const decStringSet &set ){
	pClassNames = set;
}



bool meFilterObjectsByClass::AcceptObject( meObject *object ) const{
	if( ! object ) DETHROW( deeInvalidParam );
	
	const decString &objclass = object->GetClassName();
	int c, count = pClassNames.GetCount();
	
	// reject if matching the given object
	if( object == pRejectObject ) return false;
	
	// reject if this is a ghost object
	if( pRejectGhosts ){
		const igdeGDClass *gdclass = object->GetGDClass();
		if( gdclass && ( gdclass->GetIsGhost() || ! gdclass->GetCanInstantiate() ) ){
			return false;
		}
	}
	
	// accept or reject according to class
	for( c=0; c<count; c++ ){
		if( objclass.MatchesPattern( pClassNames.GetAt( c )  ) ){
			return pMatchInclusive;
		}
	}
	
	return ! pMatchInclusive;
}
