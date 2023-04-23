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

meCLIdentify::meCLIdentify( meWorld &world ) :
pWorld( world ),
pObject( nullptr ),
pDecal( nullptr ),
pBestDistance( 0.0f ){
}

meCLIdentify::~meCLIdentify(){
}



// Management
///////////////

void meCLIdentify::Reset(){
	pObject = nullptr;
	pDecal = nullptr;
	pBestDistance = 0.0f;
}

bool meCLIdentify::HasObject() const{
	return pObject != nullptr;
}

bool meCLIdentify::HasDecal() const{
	return pDecal != nullptr;
}



// Notifications
//////////////////

void meCLIdentify::CollisionResponse( deCollider*, deCollisionInfo *info ){
	if( info->IsCollider() ){
		meObject * const object = pGetObjectForCollider( info->GetCollider() );
		if( ! object ){
			return;
		}
		
		if( ! pObject || info->GetDistance() < pBestDistance ){
			pObject = object;
		}
	}
}

bool meCLIdentify::CanHitCollider( deCollider*, deCollider *collider ){
	const meObject * const object = pGetObjectForCollider( collider );
	return object && object->GetVisible();
}

void meCLIdentify::ColliderChanged( deCollider* ){
}



// Private Functions
//////////////////////

meObject *meCLIdentify::pGetObjectForCollider( deCollider *collider ) const{
	const meColliderOwner * const colliderOwner =
		meColliderOwner::GetColliderOwner( *pWorld.GetEnvironment(), collider );
	return colliderOwner ? colliderOwner->GetObject() : nullptr;
}
