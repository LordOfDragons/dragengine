/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
