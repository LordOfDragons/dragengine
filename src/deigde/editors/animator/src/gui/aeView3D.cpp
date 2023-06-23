/* 
 * Drag[en]gine IGDE Animator Editor
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
#include <stdlib.h>
#include <string.h>

#include "aeView3D.h"
#include "aeWindowMain.h"
#include "../animator/aeCamera.h"
#include "../animator/aeAnimator.h"
#include "../animator/locomotion/aeAnimatorLocomotion.h"
#include "../animator/wakeboard/aeWakeboard.h"
#include "../configuration/aeConfiguration.h"
#include "../visitors/aeCLClosestHit.h"
#include "../visitors/aeElementVisitor.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>
#include <deigde/gui/event/igdeMouseKeyListener.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decCollisionFilter.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/physics/deBasePhysicsWorld.h>



// Definitions
////////////////

namespace {

class cCameraInteraction : public igdeMouseCameraListener {
	aeView3D &pView;
	
public:
	cCameraInteraction( aeView3D &view ) : pView( view ){ }
	
public:
	virtual igdeMouseCameraListener::eInteraction ChooseInteraction(){
		const aeAnimator * const animator = pView.GetAnimator();
		if( animator ){
			if( animator->GetCamera()->GetAttachToBone()
			|| animator->GetLocomotion().GetEnabled()
			|| animator->GetWakeboard().GetEnabled() ){
				return eiNone;
			}
		}
		return igdeMouseCameraListener::ChooseInteraction();
	}
	
	virtual void OnCameraChanged(){
		if( ! pView.GetAnimator() ){
			return;
		}
		
		aeCamera &camera = *pView.GetAnimator()->GetCamera();
		camera.SetFreePosition( camera.GetPosition() );
		camera.SetFreeOrientation( camera.GetOrientation() );
		camera.SetFreeDistance( camera.GetDistance() );
		
		pView.GetAnimator()->NotifyCameraViewChanged();
	}
};

class cLocomotionInteraction : public igdeMouseKeyListener {
	aeView3D &pView;
	decPoint pMouseLocation;
	float pSpeed;
	
public:
	cLocomotionInteraction( aeView3D &view ) : pView( view ), pSpeed( 0.3f ){ }
	
public:
	void Start(){
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || animator->GetLocomotion().GetEnabled() || animator->GetWakeboard().GetEnabled() ){
			return;
		}
		
		animator->ResetSimulation();
		animator->GetLocomotion().SetEnabled( true );
		animator->NotifyLocomotionChanged();
		pView.GrabInput();
	}
	
	void Stop(){
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || ! animator->GetLocomotion().GetEnabled() ){
			return;
		}
		
		pView.ReleaseInput();
		animator->GetLocomotion().SetEnabled( false );
		animator->NotifyLocomotionChanged();
		animator->ResetSimulation();
	}
	
	virtual void OnMouseMoved( igdeWidget*, const decPoint &position, int ){
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || ! animator->GetLocomotion().GetEnabled() ){
			pMouseLocation = position;
			return;
		}
		
		const decVector2 diff( decVector2( position - pMouseLocation ) * pSpeed );
		pMouseLocation = position;
		
		aeAnimatorLocomotion &locomotion = animator->GetLocomotion();
		locomotion.SetLookUpDownGoal( locomotion.GetLookUpDown().GetGoal() + diff.y );
		locomotion.GetLookLeftRight().SetGoal( locomotion.GetLookLeftRight().GetGoal() - diff.x );
		animator->NotifyLocomotionChanged();
	}
	
	virtual void OnKeyPress( igdeWidget*, deInputEvent::eKeyCodes keyCode, int ){
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || ! animator->GetLocomotion().GetEnabled() ){
			return;
		}
		
		aeAnimatorLocomotion &locomotion = animator->GetLocomotion();
		const aeConfiguration &config = pView.GetWindowMain().GetConfiguration();
		
		if( keyCode == deInputEvent::ekcEscape ){
			Stop();
			
		}else if( keyCode == config.GetLocoKeyForward() ){
			locomotion.SetKeyForward( true );
			
		}else if( keyCode == config.GetLocoKeyBackwards() ){
			locomotion.SetKeyBackward( true );
			
		}else if( keyCode == config.GetLocoKeyLeft() ){
			locomotion.SetKeyStepLeft( true );
			
		}else if( keyCode == config.GetLocoKeyRight() ){
			locomotion.SetKeyStepRight( true );
			
		}else if( keyCode == config.GetLocoKeyCrouch() ){
			locomotion.SetToggleCrouch( ! locomotion.GetToggleCrouch() );
			
		}else if( keyCode == config.GetLocoKeyRun() ){
			locomotion.SetToggleRun( ! locomotion.GetToggleRun() );
		}
	}
	
	virtual void OnKeyRelease( igdeWidget*, deInputEvent::eKeyCodes keyCode, int ){
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || ! animator->GetLocomotion().GetEnabled() ){
			return;
		}
		
		aeAnimatorLocomotion &locomotion = animator->GetLocomotion();
		const aeConfiguration &config = pView.GetWindowMain().GetConfiguration();

		if( keyCode == config.GetLocoKeyForward() ){
			locomotion.SetKeyForward( false );
			
		}else if( keyCode == config.GetLocoKeyBackwards() ){
			locomotion.SetKeyBackward( false );
			
		}else if( keyCode == config.GetLocoKeyLeft() ){
			locomotion.SetKeyStepLeft( false );
			
		}else if( keyCode == config.GetLocoKeyRight() ){
			locomotion.SetKeyStepRight( false );
		}
	}
};

class cWakeboardInteraction : public igdeMouseKeyListener {
	aeView3D &pView;
	decPoint pMouseLocation;
	decVector pOldWakeTilt;
	decPoint pAnchorMouseLocation;
	float pSpeed;
	
public:
	cWakeboardInteraction( aeView3D &view ) : pView( view ), pSpeed( 0.3f ){ }
	
public:
	void Start(){
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || animator->GetWakeboard().GetEnabled() || animator->GetLocomotion().GetEnabled() ){
			return;
		}
		
		const aeWakeboard &wakeboard = animator->GetWakeboard();
		pOldWakeTilt.x = wakeboard.GetTiltLeftRight();
		pOldWakeTilt.y = wakeboard.GetTiltUpDown();
		pAnchorMouseLocation = pMouseLocation;
		
		animator->ResetSimulation();
		animator->GetWakeboard().SetEnabled( true );
		animator->NotifyLocomotionChanged();
		pView.GrabInput();
	}
	
	void Stop(){
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || ! animator->GetWakeboard().GetEnabled() ){
			return;
		}
		
		pView.ReleaseInput();
		animator->GetWakeboard().SetEnabled( false );
		animator->NotifyLocomotionChanged();
		animator->ResetSimulation();
	}
	
	virtual void OnMouseMoved( igdeWidget*, const decPoint &position, int ){
		pMouseLocation = position;
		
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || ! animator->GetWakeboard().GetEnabled() ){
			return;
		}
		
		const decVector2 diff( decVector2( position - pAnchorMouseLocation ) * pSpeed );
		pMouseLocation = position;
		
		aeWakeboard &wakeboard = animator->GetWakeboard();
		wakeboard.SetTiltUpDown( pOldWakeTilt.y + diff.y );
		wakeboard.SetTiltLeftRight( pOldWakeTilt.x + diff.x );
		animator->NotifyLocomotionChanged();
	}
	
	virtual void OnKeyPress( igdeWidget*, deInputEvent::eKeyCodes keyCode, int ){
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || ! animator->GetWakeboard().GetEnabled() ){
			return;
		}
		
		if( keyCode == deInputEvent::ekcEscape ){
			Stop();
		}
	}
};

class cEditorInteraction : public igdeMouseKeyListener {
	aeView3D &pView;
	
public:
	cEditorInteraction( aeView3D &view ) : pView( view ){ }
	
public:
	void OnButtonPress( igdeWidget*, int button, const decPoint &position, int modifiers ) override{
		aeAnimator * const animator = pView.GetAnimator();
		if( ! animator || animator->GetWakeboard().GetEnabled() || animator->GetLocomotion().GetEnabled() ){
			return;
		}
		
		switch( button ){
		case deInputEvent::embcLeft:
			{
			deBasePhysicsWorld * const peer = animator->GetEngineWorld()->GetPeerPhysics();
			if( ! peer ){
				return;
			}
			
			const decDMatrix viewMatrix( animator->GetCamera()->GetViewMatrix() );
			decLayerMask layerMask;
			aeCLClosestHit visitor;
			
			const decDVector rayPosition = viewMatrix.GetPosition();
			const decVector rayDirection = animator->GetCamera()->GetDirectionFor(
				pView.GetRenderAreaSize().x, pView.GetRenderAreaSize().y, position.x, position.y ) * 500.0f;
			
			// test for gizmos only first. stop if a gizmo is hit and started editing
			layerMask.SetBit( aeAnimator::eclGizmo );
			
			peer->RayHits( rayPosition, rayDirection, &visitor, decCollisionFilter( layerMask ) );
			if( visitor.GetHasHit() ){
				visitor.IdentifyHitElement( pView.GetEnvironment() );
				if( visitor.GetHitGizmo() ){
					if( pView.GetGizoms().StartEditing( visitor.GetHitGizmo(), rayPosition,
					decDVector( rayDirection ), viewMatrix, ( double )visitor.GetHitDistance(),
					visitor.GetHitShape() ) ){
						return;
					}
				}
			}
			
			// test for anything else but gizmos
			layerMask = decLayerMask();
			layerMask.SetBit( aeAnimator::eclTerrain );
			layerMask.SetBit( aeAnimator::eclElements );
			layerMask.SetBit( aeAnimator::eclAI );
			layerMask.SetBit( aeAnimator::eclGround );
			
			peer->RayHits( rayPosition, rayDirection, &visitor, decCollisionFilter( layerMask ) );
			if( visitor.GetHasHit() ){
				visitor.IdentifyHitElement( pView.GetEnvironment() );
				// TODO
			}
			} break;
			
		case deInputEvent::embcRight:
			break;
			
		default:
			break;
		}
	}
	
	void OnButtonRelease( igdeWidget*, int button, const decPoint &position, int modifiers ) override{
		switch( button ){
		case deInputEvent::embcLeft:
			if( pView.GetGizoms().GetEditingGizmo() ){
				pView.GetGizoms().StopEditing( false );
			}
			break;
			
		case deInputEvent::embcRight:
			break;
			
		default:
			break;
		}
	}
	
	void OnMouseMoved(igdeWidget*, const decPoint &position, int modifiers ) override{
		if( pView.GetGizoms().GetEditingGizmo() ){
			aeAnimator * const animator = pView.GetAnimator();
			if( ! animator || animator->GetWakeboard().GetEnabled() || animator->GetLocomotion().GetEnabled() ){
				return;
			}
			
			const decDMatrix viewMatrix( animator->GetCamera()->GetViewMatrix() );
			const decDVector rayPosition = viewMatrix.GetPosition();
			const decVector rayDirection = animator->GetCamera()->GetDirectionFor(
				pView.GetRenderAreaSize().x, pView.GetRenderAreaSize().y, position.x, position.y );
			pView.GetGizoms().UpdateEditing( rayPosition, decDVector( rayDirection ), viewMatrix );
		}
	}
	
	void OnMouseWheeled( igdeWidget*, const decPoint &, const decPoint &change, int modifiers ) override{
	}
	
	void OnKeyPress( igdeWidget*, deInputEvent::eKeyCodes keyCode, int ) override{
		switch( keyCode ){
		case deInputEvent::ekcEscape:
			if( pView.GetGizoms().GetEditingGizmo() ){
				pView.GetGizoms().StopEditing( true );
			}
			break;
			
		default:
			break;
		}
	}
	
	void OnKeyRelease( igdeWidget*, deInputEvent::eKeyCodes keyCode, int ) override{
	}
};

}



// Class aeView3D
///////////////////

// Constructor, destructor
////////////////////////////

aeView3D::aeView3D( aeWindowMain &windowMain ) :
igdeViewRenderWindow( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pAnimator( NULL )
{
	pCameraInteraction.TakeOver( new cCameraInteraction( *this ) );
	pLocomotionInteraction.TakeOver( new cLocomotionInteraction( *this ) );
	pWakeboardInteraction.TakeOver( new cWakeboardInteraction( *this ) );
	pEditorInteraction.TakeOver( new cEditorInteraction( *this ) );
	
	AddListener( pCameraInteraction );
	AddListener( pLocomotionInteraction );
	AddListener( pWakeboardInteraction );
	AddListener( pEditorInteraction );
}

aeView3D::~aeView3D(){
	SetAnimator( nullptr );
}



// Management
///////////////

void aeView3D::ResetView(){
}

void aeView3D::SetAnimator( aeAnimator *animator ){
	if( animator == pAnimator ){
		return;
	}
	
	StopWakeboarding();
	StopLocomotionTesting();
	
	pCameraInteraction->SetCamera( NULL );
	
	SetRenderWorld( NULL );
	
	if( pAnimator ){
		if( pAnimator->GetLocomotion().GetEnabled() ){
			StopLocomotionTesting();
			
		}else if( pAnimator->GetWakeboard().GetEnabled() ){
			StopWakeboarding();
		}
		
		pAnimator->FreeReference();
	}
	
	pAnimator = animator;
	
	if( animator ){
		animator->AddReference();
		SetRenderWorld( pAnimator->GetCamera()->GetEngineCamera() );
		pCameraInteraction->SetCamera( animator->GetCamera() );
	}
}



void aeView3D::StartLocomotionTesting(){
	( ( cLocomotionInteraction& )( igdeMouseKeyListener& )pLocomotionInteraction ).Start();
}

void aeView3D::StopLocomotionTesting(){
	( ( cLocomotionInteraction& )( igdeMouseKeyListener& )pLocomotionInteraction ).Stop();
}

void aeView3D::StartWakeboarding(){
	( ( cWakeboardInteraction& )( igdeMouseKeyListener& )pWakeboardInteraction ).Start();
}

void aeView3D::StopWakeboarding(){
	( ( cWakeboardInteraction& )( igdeMouseKeyListener& )pWakeboardInteraction ).Stop();
}



void aeView3D::OnFrameUpdate( float elapsed ){
	igdeViewRenderWindow::OnFrameUpdate( elapsed );
	
	if( pAnimator ){
		pAnimator->UpdateWorld( elapsed );
	}
	
	pGizmos.OnFrameUpdate( elapsed );
}

void aeView3D::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if( pAnimator ){
		SetRenderWorld( pAnimator->GetCamera()->GetEngineCamera() );
	}
}
