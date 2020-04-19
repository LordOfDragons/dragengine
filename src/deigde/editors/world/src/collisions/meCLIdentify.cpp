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

#include <math.h>

#include "meCLIdentify.h"
#include "../world/meWorld.h"
#include "../world/meColliderOwner.h"
#include "../world/decal/meDecal.h"
#include "../world/object/meObject.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>



// Class meCLIdentify
///////////////////////

// Constructor, destructor
////////////////////////////

meCLIdentify::meCLIdentify( meWorld *world ){
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	pWorld = world;
	pObject = NULL;
	pDecal = NULL;
}

meCLIdentify::~meCLIdentify(){
}



// Management
///////////////

void meCLIdentify::Reset(){
	pObject = NULL;
	pDecal = NULL;
}

bool meCLIdentify::HasObject() const{
	return pObject != NULL;
}

bool meCLIdentify::HasDecal() const{
	return pDecal != NULL;
}



// Notifications
//////////////////

void meCLIdentify::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	if( info->IsCollider() ){
		pObject = pGetObjectForCollider( info->GetCollider() );
	}
}

bool meCLIdentify::CanHitCollider( deCollider *owner, deCollider *collider ){
	const meObject * const object = pGetObjectForCollider( collider );
	if( object ){
		return object->GetVisible();
	}
	
	return false;
}

void meCLIdentify::ColliderChanged( deCollider *owner ){
}



// Private Functions
//////////////////////

meObject *meCLIdentify::pGetObjectForCollider( deCollider *collider ) const{
	const meColliderOwner * const colliderOwner = meColliderOwner::GetColliderOwner(
		*pWorld->GetEnvironment(), collider );
	if( ! colliderOwner ){
		return NULL;
	}
	
	return colliderOwner->GetObject();
}
