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

#include "igdeGizmoManager.h"
#include "../igdeCamera.h"
#include "../igdeViewRenderWindow.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Class cFindGizmoListener
/////////////////////////////

class cFindGizmoListener : public deBaseScriptingCollider {
private:
	igdeGizmoManager &pManager;
	igdeEnvironment &pEnvironment;
	
public:
	igdeGizmo *gizmo;
	float distance;
	int bone;
	int shape;
	
	cFindGizmoListener( igdeGizmoManager &manager, igdeEnvironment &environment ) :
	pManager( manager ), pEnvironment( environment ){
		Reset();
	}
	
	void Reset(){
		gizmo = nullptr;
		distance = 0.0f;
		bone = -1;
		shape = -1;
	}
	
	virtual void CollisionResponse( deCollider*, deCollisionInfo *info ) override{
		if( ! info->GetCollider() ){
			return;
		}
		
		void * const userPointer = pEnvironment.GetColliderUserPointer( info->GetCollider() );
		if( ! userPointer ){
			return;
		}
		
		igdeGizmo * const hitGizmo = pManager.GizmoFromColliderUserPointer( userPointer );
		if( ! hitGizmo ){
			return;
		}
		
		if( gizmo && info->GetDistance() >= distance ){
			return;
		}
		
		gizmo = hitGizmo;
		distance = info->GetDistance();
		bone = info->GetBone();
		shape = info->GetShape();
	}
	
	virtual bool CanHitCollider( deCollider*, deCollider* ) override{
		return true;
	}
	
	virtual void ColliderChanged( deCollider* ) override{}
};



// Class igdeGizmoManager
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeGizmoManager::igdeGizmoManager() :
pRayLength( 1e4f ){
}

igdeGizmoManager::~igdeGizmoManager(){
	StopEditing( true );
	ClearHover();
}



// Management
///////////////

void igdeGizmoManager::Hover( igdeGizmo *gizmo, const decDVector &rayOrigin,
const decDVector &rayDirection, double distance, int bone, int shape, int modifiers ){
	DEASSERT_NOTNULL( gizmo )
	
	if( pHoverGizmo == gizmo ){
		gizmo->UpdateHovering( rayOrigin, rayDirection, distance, bone, shape, modifiers );
		
	}else{
		ClearHover();
		gizmo->StartHovering( rayOrigin, rayDirection, distance, bone, shape, modifiers );
		pHoverGizmo = gizmo;
	}
}

void igdeGizmoManager::ClearHover(){
	if( pHoverGizmo ){
		pHoverGizmo->StopHovering();
		pHoverGizmo = nullptr;
	}
}



bool igdeGizmoManager::StartEditing( igdeGizmo *gizmo, const decDVector &rayOrigin,
const decDVector &rayDirection, const decDMatrix &viewMatrix, double distance,
int bone, int shape, int modifiers ){
	DEASSERT_NOTNULL( gizmo )
	DEASSERT_NULL( pEditingGizmo )
	
	if( gizmo->StartEditing( rayOrigin, rayDirection, viewMatrix, distance, bone, shape, modifiers ) ){
		pEditingGizmo = gizmo;
		return true;
		
	}else{
		return false;
	}
}

void igdeGizmoManager::UpdateEditing( const decDVector &rayOrigin, const decDVector &rayDirection,
const decDMatrix &viewMatrix, int modifiers ){
	if( pEditingGizmo ){
		pEditingGizmo->UpdateEditing( rayOrigin, rayDirection, viewMatrix, modifiers );
	}
}

void igdeGizmoManager::MouseWheeledEditing( const decDVector &rayOrigin,
const decDVector &rayDirection, const decDMatrix &viewMatrix, const decPoint &change, int modifiers ){
	if( pEditingGizmo ){
		pEditingGizmo->MouseWheeledEditing( rayOrigin, rayDirection, viewMatrix, change, modifiers );
	}
}

void igdeGizmoManager::StopEditing( bool cancel ){
	if( pEditingGizmo ){
		pEditingGizmo->StopEditing( cancel );
		pEditingGizmo = nullptr;
	}
}



void igdeGizmoManager::OnFrameUpdate( float elapsed ){
	if( pEditingGizmo ){
		pEditingGizmo->OnFrameUpdate( elapsed );
		
	}else if( pHoverGizmo ){
		pHoverGizmo->OnFrameUpdate( elapsed );
	}
}



void igdeGizmoManager::OnButtonPress( const igdeViewRenderWindow &view,
const igdeCamera &camera, int button, const decPoint &position, int modifiers ){
	if( button != deInputEvent::embcLeft || ! camera.GetEngineWorld() ){
		return;
	}
	
	deBasePhysicsWorld * const peer = camera.GetEngineWorld()->GetPeerPhysics();
	if( ! peer ){
		return;
	}
	
	cFindGizmoListener visitor( *this, view.GetEnvironment() );
	const decDMatrix &viewMatrix = camera.GetViewMatrix();
	const decDVector rayPosition( viewMatrix.GetPosition() );
	const decVector rayDirection( camera.GetDirectionFor( view.GetRenderAreaSize().x,
		view.GetRenderAreaSize().y, position.x, position.y ) * pRayLength );
	
	peer->RayHits( rayPosition, rayDirection, &visitor, pRayCollisionFilter );
	
	if( visitor.gizmo ){
		StartEditing( visitor.gizmo, rayPosition, rayDirection, viewMatrix,
			( double )visitor.distance, visitor.bone, visitor.shape, modifiers );
	}
}

void igdeGizmoManager::OnButtonRelease( const igdeViewRenderWindow &view,
const igdeCamera &camera, int button, const decPoint &position, int modifiers ){
	if( ! pEditingGizmo || button != deInputEvent::embcLeft ){
		return;
	}
	
	OnMouseMoved( view, camera, position, modifiers );
	
	if( pEditingGizmo ){
		StopEditing( false );
	}
}

void igdeGizmoManager::OnMouseMoved( const igdeViewRenderWindow &view,
const igdeCamera &camera, const decPoint &position, int modifiers ){
	const decDMatrix &viewMatrix = camera.GetViewMatrix();
	const decDVector rayPosition( viewMatrix.GetPosition() );
	const decVector rayDirection( camera.GetDirectionFor( view.GetRenderAreaSize().x,
		view.GetRenderAreaSize().y, position.x, position.y ) );
	
	if( pEditingGizmo ){
		UpdateEditing( rayPosition, decDVector( rayDirection ), viewMatrix, modifiers );
		return;
	}
	
	if( ! camera.GetEngineWorld() ){
		ClearHover();
		return;
	}
	
	deBasePhysicsWorld * const peer = camera.GetEngineWorld()->GetPeerPhysics();
	if( ! peer ){
		ClearHover();
		return;
	}
	
	cFindGizmoListener visitor( *this, view.GetEnvironment() );
	peer->RayHits( rayPosition, decDVector( rayDirection * pRayLength ),
		&visitor, pRayCollisionFilter );
	
	if( visitor.gizmo ){
		Hover( visitor.gizmo, rayPosition, rayDirection, ( double )visitor.distance,
			visitor.bone, visitor.shape, modifiers );
		
	}else{
		ClearHover();
	}
}

void igdeGizmoManager::OnMouseWheeled( const igdeViewRenderWindow &view,
const igdeCamera &camera, const decPoint &position, const decPoint &change, int modifiers ){
	if( pEditingGizmo ){
		const decDMatrix &viewMatrix = camera.GetViewMatrix();
		const decDVector rayPosition( viewMatrix.GetPosition() );
		const decVector rayDirection( camera.GetDirectionFor( view.GetRenderAreaSize().x,
			view.GetRenderAreaSize().y, position.x, position.y ) );
		
		MouseWheeledEditing( rayPosition, decDVector( rayDirection ), viewMatrix, change, modifiers );
		return;
	}
}

void igdeGizmoManager::OnKeyPress( deInputEvent::eKeyCodes keyCode, int /*key*/ ){
	if( keyCode == deInputEvent::ekcEscape ){
		StopEditing( true );
	}
}

void igdeGizmoManager::OnKeyRelease( deInputEvent::eKeyCodes /*keyCode*/, int /*key*/ ){
}

void igdeGizmoManager::SetRayLength( float length ){
	pRayLength = decMath::max( length, 0.01f );
}

void igdeGizmoManager::SetRayCollisionFilter( const decCollisionFilter &collisionFilter ){
	pRayCollisionFilter = collisionFilter;
}



// Protected Functions
////////////////////////

igdeGizmo *igdeGizmoManager::GizmoFromColliderUserPointer( void* ){
	return nullptr;
}
