/* 
 * Drag[en]gine Android Input Module
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

#include "deaiOverlay.h"
#include "deaiOverlayList.h"

#include <dragengine/common/exceptions.h>



// Class deaiOverlayList
//////////////////////////

// Constructor, destructor
////////////////////////////

deaiOverlayList::deaiOverlayList(){
}

deaiOverlayList::~deaiOverlayList(){
}



// Management
///////////////

int deaiOverlayList::GetCount() const{
	return pOverlays.GetCount();
}

deaiOverlay *deaiOverlayList::GetAt( int index ) const{
	return ( deaiOverlay* )pOverlays.GetAt( index );
}

void deaiOverlayList::Add( deaiOverlay *overlay ){
	pOverlays.Add( overlay );
}

void deaiOverlayList::Remove( deaiOverlay *overlay ){
	pOverlays.Remove( overlay );
}

void deaiOverlayList::RemoveAll(){
	pOverlays.RemoveAll();
}
