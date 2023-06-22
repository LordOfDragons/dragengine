/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "igdeGizmo.h"
#include "igdeGizmoManager.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/world/deWorld.h>



// Class igdeGizmoManager
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGizmoManager::igdeGizmoManager() :
pEditingGizmo( nullptr ){
}

igdeGizmoManager::~igdeGizmoManager(){
	StopEditing();
}



// Management
///////////////

void igdeGizmoManager::Add( igdeGizmo *gizmo ){
	DEASSERT_NOTNULL( gizmo )
	pGizmos.Add( gizmo );
}

void igdeGizmoManager::Remove( igdeGizmo *gizmo ){
	DEASSERT_NOTNULL( gizmo )
	
	if( pEditingGizmo == gizmo ){
		StopEditing();
	}
	
	pGizmos.Remove( gizmo );
}

bool igdeGizmoManager::StartEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
const decDMatrix &viewMatrix, const deCollider *hitCollider, int hitShape, double hitDistance ){
	StopEditing();
	
	const int count = pGizmos.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeGizmo * const gizmo = ( igdeGizmo* )pGizmos.GetAt( i );
		if( gizmo->StartEditing( rayOrigin, rayDirection, viewMatrix, hitCollider, hitShape, hitDistance ) ){
			pEditingGizmo = gizmo;
			return true;
		}
	}
	
	return false;
}

void igdeGizmoManager::UpdateEditing( const decDVector &rayOrigin,
const decDVector &rayDirection, const decDMatrix &viewMatrix ){
	if( pEditingGizmo ){
		pEditingGizmo->UpdateEditing( rayOrigin, rayDirection, viewMatrix );
	}
}

void igdeGizmoManager::OnFrameUpdate( float elapsed ){
	if( pEditingGizmo ){
		pEditingGizmo->OnFrameUpdate( elapsed );
	}
}

void igdeGizmoManager::StopEditing(){
	if( pEditingGizmo ){
		pEditingGizmo->StopEditing();
		pEditingGizmo = nullptr;
	}
}
