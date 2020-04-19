/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglSharedSPBRTIGroup.h"
#include "deoglSharedSPBRTIGroupList.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedSPBRTIGroupList
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedSPBRTIGroupList::deoglSharedSPBRTIGroupList(){
}

deoglSharedSPBRTIGroupList::~deoglSharedSPBRTIGroupList(){
}



// Management
///////////////

int deoglSharedSPBRTIGroupList::GetCount() const{
	return pGroups.GetCount();
}

deoglSharedSPBRTIGroup *deoglSharedSPBRTIGroupList::GetAt( int index ) const{
	return ( deoglSharedSPBRTIGroup* )pGroups.GetAt( index );
}

deoglSharedSPBRTIGroup *deoglSharedSPBRTIGroupList::GetWith( deoglSharedSPB &sharedSPB ){
	const int count = pGroups.GetCount();
	int i;
	
	// find group with shared SPB
	deoglSharedSPBRTIGroup *group = NULL;
	
	for( i=0; i<count; i++ ){
		group = ( deoglSharedSPBRTIGroup* )pGroups.GetAt( i );
		if( &group->GetSharedSPB() == &sharedSPB ){
			group->AddReference();
			return group;
		}
	}
	
	// matching group not found. create a new group
	try{
		group = new deoglSharedSPBRTIGroup( *this, sharedSPB );
		pGroups.Add( group );
		return group;
		
	}catch( const deException & ){
		if( group ){
			group->FreeReference();
		}
		throw;
	}
}

void deoglSharedSPBRTIGroupList::Remove( deoglSharedSPBRTIGroup *group ){
	pGroups.RemoveFrom( pGroups.IndexOf( group ) );
}
