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
