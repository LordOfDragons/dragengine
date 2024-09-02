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

#include "aeCLClosestHit.h"
#include "aeElementVisitable.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>



// Class aeCLClosestHit
/////////////////////////

// Constructor, destructor
////////////////////////////

aeCLClosestHit::aeCLClosestHit() :
pHitDistance( 0.0f ),
pHasHit( false ),
pHitCollider( nullptr ),
pHitBone( -1 ),
pHitShape( -1 ),
pHitGizmo( nullptr ){
}

aeCLClosestHit::~aeCLClosestHit(){
}



// Management
///////////////

void aeCLClosestHit::Reset(){
	pHitDistance = 0.0f;
	pHasHit = false;
	pHitCollider = nullptr;
	pHitBone = -1;
	pHitShape = -1;
	pHitGizmo = nullptr;
}

void aeCLClosestHit::IdentifyHitElement( igdeEnvironment &environment ){
	if( ! pHitCollider ){
		return;
	}
	
	aeElementVisitable * const visitable =
		( aeElementVisitable* )environment.GetColliderUserPointer( pHitCollider );
	
	if( visitable ){
		visitable->VisitElement( *this );
	}
}



// Notifications
//////////////////

void aeCLClosestHit::CollisionResponse( deCollider*, deCollisionInfo *info ){
	float distance = info->GetDistance();
	
	if( pHasHit && distance >= pHitDistance ){
		return;
	}
	
	pHitDistance = distance;
	pHitNormal = info->GetNormal();
	pHitCollider = info->GetCollider();
	pHitBone = info->GetBone();
	pHitShape = info->GetShape();
	pHasHit = true;
}

bool aeCLClosestHit::CanHitCollider( deCollider*, deCollider* ){
	return true;
}

void aeCLClosestHit::ColliderChanged( deCollider* ){
}



// Visiting
/////////////

void aeCLClosestHit::VisitGizmo( igdeGizmo *gizmo ){
	pHitGizmo = gizmo;
}
