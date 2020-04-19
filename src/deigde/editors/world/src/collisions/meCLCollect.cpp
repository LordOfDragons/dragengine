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

#include "meCLCollect.h"
#include "meCLHitListEntry.h"
#include "../world/meWorld.h"
#include "../world/meColliderOwner.h"
#include "../world/decal/meDecal.h"
#include "../world/object/meObject.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>



// Class meCLCollect
//////////////////////

// Constructor, destructor
////////////////////////////

meCLCollect::meCLCollect( meWorld *world ){
	if( ! world ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	
	pCollectObjects = true;
	pCollectDecals = false;
}

meCLCollect::~meCLCollect(){
}



// Management
///////////////

void meCLCollect::SetCollectObjects( bool collect ){
	pCollectObjects = collect;
}

void meCLCollect::SetCollectDecals( bool collect ){
	pCollectDecals = collect;
}



void meCLCollect::Reset(){
	pElements.RemoveAllEntries();
}



// Notifications
//////////////////

void meCLCollect::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	meCLHitListEntry *entry = NULL;
	
	if( info->IsCollider() ){
		const meColliderOwner * const colliderOwner = meColliderOwner::GetColliderOwner(
			*pWorld->GetEnvironment(), info->GetCollider() );
		if( ! colliderOwner ){
			return;
		}
		
		if( colliderOwner->GetObject() ){
			if( ! pCollectObjects ){
				return;
			}
			
			try{
				entry = new meCLHitListEntry;
				entry->SetObject( colliderOwner->GetObject() );
				entry->SetDistance( info->GetDistance() );
				entry->SetNormal( info->GetNormal() );
				
				pElements.AddEntry( entry );
				entry = NULL;
				
			}catch( const deException & ){
				if( entry ){
					delete entry;
				}
				throw;
			}
			
		}else if( colliderOwner->GetDecal() ){
			if( ! pCollectDecals ){
				return;
			}
			
			try{
				entry = new meCLHitListEntry;
				entry->SetDecal( colliderOwner->GetDecal() );
				entry->SetDistance( info->GetDistance() );
				entry->SetNormal( info->GetNormal() );
				
				pElements.AddEntry( entry );
				entry = NULL;
				
			}catch( const deException & ){
				if( entry ){
					delete entry;
				}
				throw;
			}
		}
		
	}else if( info->IsHTSector() ){
	}
}

bool meCLCollect::CanHitCollider( deCollider *owner, deCollider *collider ){
	return true;
}

void meCLCollect::ColliderChanged( deCollider *owner ){
}
