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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeWDebugDrawerShape.h"
#include "igdeWDebugDrawerShapeList.h"

#include <dragengine/common/exceptions.h>



// Class igdeWDebugDrawerShape
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeWDebugDrawerShapeList::igdeWDebugDrawerShapeList(){
}

igdeWDebugDrawerShapeList::~igdeWDebugDrawerShapeList(){
}



// Management
///////////////

int igdeWDebugDrawerShapeList::GetCount() const{
	return pList.GetCount();
}

igdeWDebugDrawerShape *igdeWDebugDrawerShapeList::GetAt( int index ) const{
	return ( igdeWDebugDrawerShape* )pList.GetAt( index );
}

bool igdeWDebugDrawerShapeList::Has( igdeWDebugDrawerShape *wrapper ) const{
	return pList.Has( wrapper );
}

void igdeWDebugDrawerShapeList::Add( igdeWDebugDrawerShape *wrapper ){
	if( ! wrapper ){
		DETHROW( deeInvalidParam );
	}
	
	pList.Add( wrapper );
}

void igdeWDebugDrawerShapeList::Remove( igdeWDebugDrawerShape *wrapper ){
	pList.Remove( wrapper );
	delete wrapper;
}

void igdeWDebugDrawerShapeList::RemoveAll(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( igdeWDebugDrawerShape* )pList.GetAt( i );
	}
	pList.RemoveAll();
}



void igdeWDebugDrawerShapeList::SetParentDebugDrawerAll( deDebugDrawer *debugDrawer ){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeWDebugDrawerShape* )pList.GetAt( i ) )->SetParentDebugDrawer( debugDrawer );
	}
}

void igdeWDebugDrawerShapeList::SetVisibleAll( bool visible ){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeWDebugDrawerShape* )pList.GetAt( i ) )->SetVisible( visible );
	}
}
