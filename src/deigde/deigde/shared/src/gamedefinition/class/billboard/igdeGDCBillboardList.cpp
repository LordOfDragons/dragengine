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

#include "igdeGDCBillboard.h"
#include "igdeGDCBillboardList.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCBillboard
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCBillboardList::igdeGDCBillboardList(){
}

igdeGDCBillboardList::igdeGDCBillboardList( const igdeGDCBillboardList &list ){
	pBillboards = list.pBillboards;
}

igdeGDCBillboardList::~igdeGDCBillboardList(){
}



// Management
///////////////

int igdeGDCBillboardList::GetCount() const{
	return pBillboards.GetCount();
}

igdeGDCBillboard *igdeGDCBillboardList::GetAt( int index ) const{
	return ( igdeGDCBillboard* )pBillboards.GetAt( index );
}

int igdeGDCBillboardList::IndexOf( igdeGDCBillboard *billboard ) const{
	return pBillboards.IndexOf( billboard );
}

bool igdeGDCBillboardList::Has( igdeGDCBillboard *billboard ) const{
	return pBillboards.Has( billboard );
}

void igdeGDCBillboardList::Add( igdeGDCBillboard *billboard ){
	if( ! billboard ){
		DETHROW( deeInvalidParam );
	}
	
	pBillboards.Add( billboard );
}

void igdeGDCBillboardList::InsertAt( igdeGDCBillboard *billboard, int index ){
	if( ! billboard ){
		DETHROW( deeInvalidParam );
	}
	
	pBillboards.Insert( billboard, index );
}

void igdeGDCBillboardList::MoveTo( igdeGDCBillboard *billboard, int index ){
	pBillboards.Move( billboard, index );
}

void igdeGDCBillboardList::Remove( igdeGDCBillboard *billboard ){
	pBillboards.Remove( billboard );
}

void igdeGDCBillboardList::RemoveAll(){
	pBillboards.RemoveAll();
}



void igdeGDCBillboardList::SetToDeepCopyFrom( const igdeGDCBillboardList &list ){
	const int count = list.GetCount();
	igdeGDCBillboard *billboard = NULL;
	
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			billboard = new igdeGDCBillboard( *list.GetAt( i ) );
			Add( billboard );
			billboard->FreeReference();
			billboard = NULL;
		}
		
	}catch( const deException & ){
		if( billboard ){
			billboard->FreeReference();
		}
		throw;
	}
}



// Operators
//////////////

igdeGDCBillboardList &igdeGDCBillboardList::operator=( const igdeGDCBillboardList &list ){
	pBillboards = list.pBillboards;
	return *this;
}
