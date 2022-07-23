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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aeCamera.h"
#include "aeAnimator.h"
#include "aeAnimatorNotifier.h"
#include "aeSubAnimator.h"
#include "attachment/aeAttachment.h"
#include "controller/aeController.h"
#include "link/aeLink.h"
#include "locomotion/aeAnimatorLocomotion.h"
#include "wakeboard/aeWakeboard.h"
#include "rule/aeRule.h"
#include "../gui/aeWindowMain.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRule.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleStateSnapshot.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeAnimator
/////////////////////

// Constructor, destructor
////////////////////////////

aeAnimator::aeAnimator( aeWindowMain &windowMain ) :
igdeEditableEntity( &windowMain.GetEnvironment() ),
pWindowMain( windowMain )
{
	deEngine * engine = GetEngine();
	
	pEngWorld = NULL;
	
	pEngLight = NULL;
	pEngComponent = NULL;
	pEngAnimator = NULL;
	pEngAnimatorInstance = NULL;
	pEngCollider = NULL;
	
	pLocomotion = NULL;
	pWakeboard = NULL;
	pSubAnimator = NULL;
	pTestingSubAnimator = NULL;
	pResetState = false;
	
	pCamera = NULL;
	
	pActiveController = NULL;
	pActiveLink = NULL;
	pActiveRule = NULL;
	
	pAttachments = NULL;
	pAttachmentCount = 0;
	pAttachmentSize = 0;
	pActiveAttachment = NULL;
	
	pPaused = false;
	pPlaySpeed = 1.0f;
	pTimeStep = 0.05f;
	
	pDDBones = NULL;
	pDDSBones = NULL;
	pDDSBoneCount = 0;
	pDDSBoneSize = 1.0f;
	
	pSky = NULL;
	pEnvObject = NULL;
	
	pNotifiers = NULL;
	pNotifierCount = 0;
	pNotifierSize = 0;
	
	try{
		SetFilePath( "new.deanimator" );
		
		pCreateWorld(); // creates animator
		pCreateCamera();
		pCreateCollider();
		
		pEngAnimatorInstance = engine->GetAnimatorInstanceManager()->CreateAnimatorInstance();
		pEngAnimatorInstance->SetAnimator( pEngAnimator );
		
		pLocomotion = new aeAnimatorLocomotion( this );
		pWakeboard = new aeWakeboard( this );
		
		// create sky
		pSky = new igdeWSky( windowMain.GetEnvironment() );
		pSky->SetGDDefaultSky();
		pSky->SetWorld( pEngWorld );
		
		// create the environment wrapper object
		pEnvObject = new igdeWObject( windowMain.GetEnvironment() );
		pEnvObject->SetWorld( pEngWorld );
		pEnvObject->SetPosition( decDVector( 0.0, 0.0, 0.0 ) );
		
		decLayerMask layerMask;
		layerMask.SetBit( eclTerrain );
		layerMask.SetBit( eclElements );
		layerMask.SetBit( eclAI );
		layerMask.SetBit( eclGround );
		pEnvObject->SetCollisionFilter( decCollisionFilter( layerMask ) );
		pEnvObject->SetCollisionFilterFallback( decCollisionFilter( layerMask ) );
		
		pEnvObject->SetGDClassName( "IGDETestTerrain" );
		
		// create debug drawers
		pDDBones = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDDBones->SetXRay( true );
		pDDBones->SetVisible( false );
		pEngWorld->AddDebugDrawer( pDDBones );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	SetSaved( false );
	SetChanged( false );
}

aeAnimator::~aeAnimator(){
	pCleanUp();
}



// Management
///////////////

void aeAnimator::Dispose(){
	RemoveAllAttachments();
	
	GetUndoSystem()->RemoveAll();
	RemoveAllRules();
	RemoveAllLinks();
	RemoveAllControllers();
}

void aeAnimator::Reset(){
	GetUndoSystem()->RemoveAll();
}

void aeAnimator::SetDisplayModelPath( const char *path ){
	if( pDisplayModelPath == path ){
		return;
	}
	
	pDisplayModelPath = path;
	
	pUpdateComponent();
	NotifyModelChanged();
}

void aeAnimator::SetDisplaySkinPath( const char *path ){
	if( pDisplaySkinPath == path ){
		return;
	}
	
	pDisplaySkinPath = path;
	
	pUpdateComponent();
	NotifyModelChanged();
}

void aeAnimator::SetDisplayRigPath( const char *path ){
	if( pDisplayRigPath == path ){
		return;
	}
	
	pDisplayRigPath = path;
	
	pUpdateComponent();
	NotifyModelChanged();
}

void aeAnimator::SetRigPath( const char *path ){
	if( pRigPath == path ){
		return;
	}
	
	pRigPath = path;
	
	pUpdateComponent();
	NotifyRigChanged();
}

void aeAnimator::SetAnimationPath( const char *path ){
	if( pAnimationPath == path ){
		return;
	}
	
	pAnimationPath = path;
	
	pUpdateAnimator();
	pCamera->SetBone( "" );
	NotifyAnimationChanged();
}

bool aeAnimator::GetShowBones() const{
	return pDDBones->GetVisible();
}

void aeAnimator::SetShowBones( bool showBones ){
	if( showBones != pDDBones->GetVisible() ){
		pDDBones->SetVisible( showBones );
		NotifyViewChanged();
	}
}

void aeAnimator::SetDDBoneSize( float size ){
	size = decMath::max( size, 0.01f );
	if( fabsf( size - pDDSBoneSize ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pDDSBoneSize = size;
	pUpdateDDSBones();
	NotifyViewChanged();
}



// Engine Specific
////////////////////

void aeAnimator::UpdateWorld( float elapsed ){
	const float controllerElapsed = elapsed * pPlaySpeed;
	const float realTimeElapsed = elapsed * pPlaySpeed;
	int i;
	
	// update the locomotion testing
	pLocomotion->Update( realTimeElapsed );
	
	// update the controllers which are linked to the elapsed time
	if( ! pPaused ){
		const int count = pControllers.GetCount();
		for( i=0; i<count; i++ ){
			pControllers.GetAt( i )->UpdateValue( controllerElapsed );
		}
	}
	
	// reset the animation states if required
	if( pEngComponent && pResetState ){
		const int boneCount = pEngComponent->GetBoneCount();
		
		for( i=0; i<boneCount; i++ ){
			deComponentBone &bone = pEngComponent->GetBoneAt( i );
			bone.SetPosition( decVector() );
			bone.SetRotation( decQuaternion() );
			bone.SetScale( decVector( 1.0f, 1.0f, 1.0f ) );
		}
		
		pEngComponent->InvalidateBones();
	}
	
	// apply the testing sub animator
	if( pTestingSubAnimator->GetEngineAnimator() ){
		pTestingSubAnimator->CopyControllers( *pEngAnimatorInstance );
		pTestingSubAnimator->Apply();
	}
	
	// update the animation
	pEngAnimatorInstance->Apply();
	
	// do physics
	pEngWorld->ProcessPhysics( realTimeElapsed );
	
	// post physics updates the locomotion testing
	pLocomotion->PostPhysics();
	
	// update environment object
	pEnvObject->Update( realTimeElapsed );
	
	// adjust the position
	if( pLocomotion->GetEnabled() ){
		pEngCollider->SetPosition( pLocomotion->GetPosition() );
		pEngCollider->SetOrientation( pLocomotion->GetOrientationQuaternion() );
	}
	
	// feet on ground
	pLocomotion->FeetOnGround();
	
	// post updates
	pLocomotion->PostUpdate();
	
	// update attachments
	if( ! pPaused ){
		for( i=0; i<pAttachmentCount; i++ ){
			pAttachments[ i ]->Update( realTimeElapsed );
		}
	}
	
	// update debug drawers
	pUpdateDDSBones();
	
	// update the scene
	pEngWorld->Update( realTimeElapsed );
	
	// update the camera
	pCamera->Update();
}



// Editing
////////////

void aeAnimator::SetPaused( bool paused ){
	if( paused != pPaused ){
		pPaused = paused;
		
		NotifyPlaybackChanged();
	}
}

void aeAnimator::SetPlaySpeed( float playSpeed ){
	if( fabsf( playSpeed - pPlaySpeed ) > 1e-5f ){
		pPlaySpeed = playSpeed;
		
		NotifyPlaybackChanged();
	}
}

void aeAnimator::SetTimeStep( float timeStep ){
	if( fabsf( timeStep - pTimeStep ) > 1e-5f ){
		pTimeStep = timeStep;
		
		NotifyPlaybackChanged();
	}
}



void aeAnimator::SetResetState( bool resetState ){
	if( resetState != pResetState ){
		pResetState = resetState;
		RebuildRules();
		NotifyViewChanged();
	}
}



void aeAnimator::AttachmentsForceUpdate(){
	pEngCollider->AttachmentsForceUpdate();
}

void aeAnimator::ResetSimulation(){
	// reset the locomotion simulation. this resets also the position and orientation
	// of the collider but not the velocities. these are though anyways not used
	pLocomotion->Reset();
	NotifyLocomotionChanged();
	
	// reset the controllers and apply the animator to get a clean state
	ResetControllers();
	pEngAnimatorInstance->Apply();
	
	// reset collider position which also updates attachments
	pEngCollider->SetPosition( decDVector() );
	pEngCollider->SetOrientation( decQuaternion() );
	
	// force update of all attachments to get a clean state there too
	pEngCollider->AttachmentsForceUpdate();
	
	// reset the physics states of all attachments. this has to be done after the
	// attachments have been updated by the physics module as this can changes states
	AttachmentsResetPhysics();
}



void aeAnimator::SetPathAttachmentConfig( const char *path ){
	pPathAttConfig = path;
}



// Controllers
////////////////

void aeAnimator::AddController( aeController *controller ){
	pControllers.Add( controller );
	controller->SetAnimator( this );
	NotifyControllerStructureChanged();
}

void aeAnimator::InsertControllerAt( aeController *controller, int index ){
	pControllers.Insert( controller, index );
	controller->SetAnimator( this );
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void aeAnimator::MoveControllerTo( aeController *controller, int index ){
	pControllers.Move( controller, index );
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void aeAnimator::RemoveController( aeController *controller ){
	if( ! pControllers.Has( controller ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pActiveController == controller ){
		if( pControllers.GetCount() > 1 ){
			if( pControllers.GetAt( 0 ) == controller ){
				SetActiveController( pControllers.GetAt( 1 ) );
				
			}else{
				SetActiveController( pControllers.GetAt( 0 ) );
			}
			
		}else{
			SetActiveController( NULL );
		}
	}
	
	controller->SetAnimator( NULL );
	pControllers.Remove( controller );
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void aeAnimator::RemoveAllControllers(){
	SetActiveController( NULL );
	
	const int count = pControllers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pControllers.GetAt( i )->SetAnimator( NULL );
	}
	pControllers.RemoveAll();
	
	pUpdateLinks();
	NotifyControllerStructureChanged();
}

void aeAnimator::SetActiveController( aeController *controller ){
	if( controller == pActiveController ){
		return;
	}
	
	pActiveController = controller;
	NotifyActiveControllerChanged();
}

void aeAnimator::ResetControllers(){
	const int count = pControllers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pControllers.GetAt( i )->ResetValue();
	}
}

void aeAnimator::ResetControllersWith( int locomotionAttribute ){
	const int count = pControllers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( pControllers.GetAt( i )->GetLocomotionAttribute() == locomotionAttribute ){
			pControllers.GetAt( i )->ResetValue();
		}
	}
}

void aeAnimator::InverseControllersWith( int locomotionAttribute ){
	const int count = pControllers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( pControllers.GetAt( i )->GetLocomotionAttribute() == locomotionAttribute ){
			pControllers.GetAt( i )->InverseValue();
		}
	}
}

void aeAnimator::IncrementControllersWith( int locomotionAttribute, float incrementBy ){
	const int count = pControllers.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		if( pControllers.GetAt( i )->GetLocomotionAttribute() == locomotionAttribute ){
			pControllers.GetAt( i )->IncrementCurrentValue( incrementBy );
		}
	}
}



// Links
//////////

void aeAnimator::AddLink( aeLink *link ){
	pLinks.Add( link );
	link->SetAnimator( this );
	NotifyLinkStructureChanged();
}

void aeAnimator::RemoveLink( aeLink *link ){
	if( ! pLinks.Has( link ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pActiveLink == link ){
		if( pLinks.GetCount() > 1 ){
			if( pLinks.GetAt( 0 ) == link ){
				SetActiveLink( pLinks.GetAt( 1 ) );
				
			}else{
				SetActiveLink( pLinks.GetAt( 0 ) );
			}
			
		}else{
			SetActiveLink( NULL );
		}
	}
	
	link->SetAnimator( NULL );
	pLinks.Remove( link );
	
	RebuildRules();
	NotifyLinkStructureChanged();
}

void aeAnimator::RemoveAllLinks(){
	SetActiveLink( NULL );
	
	const int ruleCount = pRules.GetCount();
	int i;
	for( i=0; i<ruleCount; i++ ){
		pRules.GetAt( i )->RemoveLinksFromAllTargets();
	}
	
	const int count = pLinks.GetCount();
	for( i=0; i<count; i++ ){
		pLinks.GetAt( i )->SetAnimator( NULL );
	}
	pLinks.RemoveAll();
	
	RebuildRules();
	NotifyLinkStructureChanged();
}

void aeAnimator::SetActiveLink( aeLink *link ){
	if( link == pActiveLink ){
		return;
	}
	
	pActiveLink = link;
	NotifyActiveLinkChanged();
}

int aeAnimator::CountLinkUsage( aeLink *link ) const{
	const int ruleCount = pRules.GetCount();
	int i, usageCount = 0;
	
	for( i=0; i<ruleCount; i++ ){
		usageCount += pRules.GetAt( i )->CountLinkUsage( link );
	}
	
	return usageCount;
}



// Rules
//////////

void aeAnimator::AddRule( aeRule *rule ){
	pRules.Add( rule );
	rule->SetAnimator( this );
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::InsertRuleAt( aeRule *rule, int index ){
	pRules.Insert( rule, index );
	rule->SetAnimator( this );
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::MoveRuleTo( aeRule *rule, int index ){
	pRules.Move( rule, index );
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::RemoveRule( aeRule *rule ){
	if( ! pRules.Has( rule ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pActiveRule == rule ){
		if( pRules.GetCount() > 1 ){
			if( pRules.GetAt( 0 ) == rule ){
				SetActiveRule( pRules.GetAt( 1 ) );
				
			}else{
				SetActiveRule( pRules.GetAt( 0 ) );
			}
			
		}else{
			SetActiveRule( NULL );
		}
	}
	
	rule->SetAnimator( NULL );
	pRules.Remove( rule );
	
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::RemoveAllRules(){
	SetActiveRule( NULL );
	
	const int count = pRules.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pRules.GetAt( i )->SetAnimator( NULL );
	}
	pRules.RemoveAll();
	
	RebuildRules();
	NotifyRuleStructureChanged();
}

void aeAnimator::SetActiveRule( aeRule *rule ){
	if( rule == pActiveRule ){
		return;
	}
	
	pActiveRule = rule;
	NotifyActiveRuleChanged();
}

void aeAnimator::RebuildRules(){
	const int count = pRules.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRules.GetAt( i )->SetEngineRule( NULL );
	}
	
	if( ! pEngAnimator ){
		return;
	}
	
	pEngAnimator->RemoveAllRules();
	
	deObjectReference engRule;
	if( pResetState ){
		engRule.TakeOver( new deAnimatorRuleStateSnapshot );
		( ( deAnimatorRuleStateSnapshot* )( deObject* )engRule )->SetUseLastState( true );
		pEngAnimator->AddRule( ( deAnimatorRule* )( deObject* )engRule );
	}
	
	for( i=0; i<count; i++ ){
		engRule.TakeOver( pRules.GetAt( i )->CreateEngineRule() );
		pEngAnimator->AddRule( ( deAnimatorRule* )( deObject* )engRule );
		pRules.GetAt( i )->SetEngineRule( ( deAnimatorRule* )( deObject* )engRule );
	}
}



// Bone Management
////////////////////

void aeAnimator::SetListBones( const decStringSet &bones ){
	if( bones == pListBones ){
		return;
	}
	
	pListBones = bones;
	
	if( pEngAnimator ){
		pEngAnimator->GetListBones() = bones;
		pEngAnimator->NotifyBonesChanged();
	}
	
	NotifyAnimatorChanged();
}

void aeAnimator::AddBone( const char *bone ){
	if( ! bone ) DETHROW( deeInvalidParam );
	
	if( ! pListBones.Has( bone ) ){
		pListBones.Add( bone );
		
		if( pEngAnimator ){
			pEngAnimator->GetListBones().Add( bone );
			pEngAnimator->NotifyBonesChanged();
		}
		
		NotifyAnimatorChanged();
	}
}

void aeAnimator::RemoveBone( const char *bone ){
	if( pListBones.Has( bone ) ){
		pListBones.Remove( bone );
		
		if( pEngAnimator ){
			pEngAnimator->GetListBones().Remove( bone );
			pEngAnimator->NotifyBonesChanged();
		}
		
		NotifyAnimatorChanged();
	}
}

void aeAnimator::RemoveAllBones(){
	if( pListBones.GetCount() > 0 ){
		pListBones.RemoveAll();
		
		if( pEngAnimator ){
			pEngAnimator->GetListBones().RemoveAll();
			pEngAnimator->NotifyBonesChanged();
		}
		
		NotifyAnimatorChanged();
	}
}



// Attachments
////////////////

aeAttachment *aeAnimator::GetAttachmentAt( int index ) const{
	if( index < 0 || index >= pAttachmentCount ) DETHROW( deeInvalidParam );
	
	return pAttachments[ index ];
}

aeAttachment *aeAnimator::GetAttachmentNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pAttachmentCount; i++ ){
		if( pAttachments[ i ]->GetName().Equals( name ) ){
			return pAttachments[ i ];
		}
	}
	
	return NULL;
}

int aeAnimator::IndexOfAttachment( aeAttachment *attachment ) const{
	if( ! attachment ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pAttachmentCount; i++ ){
		if( attachment == pAttachments[ i ] ){
			return i;
		}
	}
	
	return -1;
}

bool aeAnimator::HasAttachment( aeAttachment *attachment ) const{
	return IndexOfAttachment( attachment ) != -1;
}

bool aeAnimator::HasAttachmentNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pAttachmentCount; i++ ){
		if( pAttachments[ i ]->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void aeAnimator::AddAttachment( aeAttachment *attachment ){
	if( HasAttachment( attachment ) ) DETHROW( deeInvalidParam );
	
	if( pAttachmentCount == pAttachmentSize ){
		int newSize = pAttachmentSize * 3 / 2 + 1;
		aeAttachment **newArray = new aeAttachment*[ newSize ];
		if( pAttachments ){
			memcpy( newArray, pAttachments, sizeof( aeAttachment* ) * pAttachmentSize );
			delete [] pAttachments;
		}
		pAttachments = newArray;
		pAttachmentSize = newSize;
	}
	
	pAttachments[ pAttachmentCount ] = attachment;
	pAttachmentCount++;
	
	attachment->AddReference();
	attachment->SetAnimator( this );
	
	NotifyAttachmentStructureChanged();
}

void aeAnimator::RemoveAttachment( aeAttachment *attachment ){
	int i, index = IndexOfAttachment( attachment );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	if( attachment == pActiveAttachment ) SetActiveAttachment( NULL );
	
	for( i=index+1; i<pAttachmentCount; i++ ){
		pAttachments[ i - 1 ] = pAttachments[ i ];
	}
	pAttachmentCount--;
	
	attachment->SetAnimator( NULL );
	attachment->FreeReference();
	
	NotifyAttachmentStructureChanged();
}

void aeAnimator::RemoveAllAttachments(){
	SetActiveAttachment( NULL );
	
	while( pAttachmentCount > 0 ){
		pAttachmentCount--;
		pAttachments[ pAttachmentCount ]->SetAnimator( NULL );
		pAttachments[ pAttachmentCount ]->FreeReference();
	}
	
	NotifyAttachmentStructureChanged();
}

void aeAnimator::SetActiveAttachment( aeAttachment *attachment ){
	if( attachment != pActiveAttachment ){
		pActiveAttachment = attachment;
		
		NotifyActiveAttachmentChanged();
	}
}

void aeAnimator::AttachAttachments(){
	int i;
	
	for( i=0; i<pAttachmentCount; i++ ){
		pAttachments[ i ]->AttachCollider();
	}
}

void aeAnimator::DetachAttachments(){
	int i;
	
	for( i=0; i<pAttachmentCount; i++ ){
		pAttachments[ i ]->DetachCollider();
	}
}

void aeAnimator::AttachmentsResetPhysics(){
	int i;
	
	for( i=0; i<pAttachmentCount; i++ ){
		pAttachments[ i ]->ResetPhysics();
	}
}



// Notifiers
//////////////

aeAnimatorNotifier *aeAnimator::GetNotifierAt( int index ) const{
	if( index < 0 || index >= pNotifierCount ) DETHROW( deeInvalidParam );
	
	return pNotifiers[ index ];
}

int aeAnimator::IndexOfNotifier( aeAnimatorNotifier *notifier ) const{
	if( ! notifier ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pNotifierCount; i++ ){
		if( notifier == pNotifiers[ i ] ) return i;
	}
	
	return -1;
}

bool aeAnimator::HasNotifier( aeAnimatorNotifier *notifier ) const{
	if( ! notifier ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pNotifierCount; i++ ){
		if( notifier == pNotifiers[ i ] ) return true;
	}
	
	return false;
}

void aeAnimator::AddNotifier( aeAnimatorNotifier *notifier ){
	if( HasNotifier( notifier ) ) DETHROW( deeInvalidParam );
	
	if( pNotifierCount == pNotifierSize ){
		int newSize = pNotifierSize * 3 / 2 + 1;
		aeAnimatorNotifier **newArray = new aeAnimatorNotifier*[ newSize ];
		if( pNotifiers ){
			memcpy( newArray, pNotifiers, sizeof( aeAnimatorNotifier* ) * pNotifierSize );
			delete [] pNotifiers;
		}
		pNotifiers = newArray;
		pNotifierSize = newSize;
	}
	
	pNotifiers[ pNotifierCount ] = notifier;
	pNotifierCount++;
	
	notifier->AddReference();
}

void aeAnimator::RemoveNotifier( aeAnimatorNotifier *notifier ){
	int i, index = IndexOfNotifier( notifier );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pNotifierCount; i++ ){
		pNotifiers[ i - 1 ] = pNotifiers[ i ];
	}
	pNotifierCount--;
	
	notifier->FreeReference();
}

void aeAnimator::RemoveAllNotifiers(){
	while( pNotifierCount > 0 ){
		pNotifierCount--;
		pNotifiers[ pNotifierCount ]->FreeReference();
	}
}


void aeAnimator::NotifyStateChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->StateChanged( this );
	}
}

void aeAnimator::NotifyUndoChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->UndoChanged( this );
	}
}

void aeAnimator::NotifyAnimatorChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->AnimatorChanged( this );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifyViewChanged(){
	int i;
	
	for( i=0; i<pNotifierCount; i++ ){
		pNotifiers[ i ]->ViewChanged( this );
	}
}

void aeAnimator::NotifyModelChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->ModelChanged( this );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifySkyChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->SkyChanged( this );
	}
}

void aeAnimator::NotifyEnvObjectChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->EnvObjectChanged( this );
	}
}

void aeAnimator::NotifyRigChanged(){
	int i;
	for( i=0; i<pNotifierCount; i++ ){
		pNotifiers[ i ]->RigChanged( this );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifyAnimationChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->AnimationChanged( this );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifyPlaybackChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->PlaybackChanged( this );
	}
}

void aeAnimator::NotifyLocomotionChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->LocomotionChanged( this );
	}
}



void aeAnimator::NotifyActiveControllerChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->ActiveControllerChanged( this, pActiveController );
	}
}

void aeAnimator::NotifyControllerChanged( aeController *controller ){
	if( ! controller ) DETHROW( deeInvalidParam );
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->ControllerChanged( this, controller );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifyControllerNameChanged( aeController *controller ){
	if( ! controller ){
		DETHROW( deeInvalidParam );
	}
	int i;
	for( i=0; i<pNotifierCount; i++ ){
		pNotifiers[ i ]->ControllerNameChanged( this, controller );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifyControllerValueChanged( aeController *controller ){
	if( ! controller ) DETHROW( deeInvalidParam );
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->ControllerValueChanged( this, controller );
	}
}

void aeAnimator::NotifyControllerStructureChanged(){
	int n;
	
	pUpdateEngineControllers();
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->ControllerStructureChanged( this );
	}
	
	SetChanged( true );
}



void aeAnimator::NotifyActiveLinkChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->ActiveLinkChanged( this, pActiveLink );
	}
}

void aeAnimator::NotifyLinkChanged( aeLink *link ){
	if( ! link ) DETHROW( deeInvalidParam );
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->LinkChanged( this, link );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifyLinkNameChanged( aeLink *link ){
	if( ! link ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pNotifierCount; i++ ){
		pNotifiers[ i ]->LinkNameChanged( this, link );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifyLinkStructureChanged(){
	int n;
	
	pUpdateLinks();
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->LinkStructureChanged( this );
	}
	
	SetChanged( true );
}



void aeAnimator::NotifyActiveRuleChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->ActiveRuleChanged( this, pActiveRule );
	}
}

void aeAnimator::NotifyRuleChanged( aeRule *rule ){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->RuleChanged( this, rule );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifyRuleNameChanged( aeRule *rule ){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->RuleNameChanged( this, rule );
	}
	
	SetChanged( true );
}

void aeAnimator::NotifyRuleStructureChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->RuleStructureChanged( this );
	}
	
	SetChanged( true );
}



void aeAnimator::NotifyActiveAttachmentChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->ActiveAttachmentChanged( this, pActiveAttachment );
	}
}

void aeAnimator::NotifyAttachmentChanged( aeAttachment *attachment ){
	if( ! attachment ) DETHROW( deeInvalidParam );
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->AttachmentChanged( this, attachment );
	}
}

void aeAnimator::NotifyAttachmentStructureChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->AttachmentStructureChanged( this );
	}
}



void aeAnimator::NotifyCameraChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->CameraChanged( this );
	}
}

void aeAnimator::NotifyCameraViewChanged(){
	int n;
	
	for( n=0; n<pNotifierCount; n++ ){
		pNotifiers[ n ]->CameraViewChanged( this );
	}
}



// Private Functions
//////////////////////

void aeAnimator::pCleanUp(){
	if( pDDSBones ){
		delete [] pDDSBones;
	}
	
	RemoveAllNotifiers();
	if( pNotifiers ){
		delete [] pNotifiers;
	}
	
	if( pEnvObject ){
		delete pEnvObject;
	}
	if( pSky ){
		delete pSky;
	}
	
	if( pCamera ){
		delete pCamera;
	}
	
	RemoveAllAttachments();
	if( pAttachments ){
		delete [] pAttachments;
	}
	
	RemoveAllRules();
	RemoveAllLinks();
	RemoveAllControllers();
	
	if( pTestingSubAnimator ){
		delete pTestingSubAnimator;
	}
	if( pSubAnimator ){
		delete pSubAnimator;
	}
	if( pWakeboard ){
		delete pWakeboard;
	}
	if( pLocomotion ){
		delete pLocomotion;
	}
	
	if( pEngAnimatorInstance ){
		pEngAnimatorInstance->SetAnimator( NULL );
		pEngAnimatorInstance->SetComponent( NULL );
		pEngAnimatorInstance->FreeReference();
	}
	if( pEngAnimator ){
		pEngAnimator->SetRig( NULL );
		pEngAnimator->FreeReference();
	}
	
	if( pEngWorld ){
		if( pDDBones ){
			pEngWorld->RemoveDebugDrawer( pDDBones );
			pDDBones->FreeReference();
		}
		
		if( pEngComponent ){
			pEngWorld->RemoveComponent( pEngComponent );
			pEngComponent->FreeReference();
		}
		
		if( pEngCollider ){
			pEngCollider->SetComponent( NULL );
			pEngWorld->RemoveCollider( pEngCollider );
			pEngCollider->FreeReference();
		}
		
		if( pEngLight ){
			pEngWorld->RemoveLight( pEngLight );
			pEngLight->FreeReference();
		}
		
		pEngWorld->FreeReference();
	}
}



void aeAnimator::pCreateWorld(){
	deEngine * const engine = GetEngine();
	
	// create world
	pEngWorld = engine->GetWorldManager()->CreateWorld();
	pEngWorld->SetGravity( decVector( 0.0f, -9.81f, 0.0f ) );
	pEngWorld->SetDisableLights( false );
	
	pEngWorld->SetAmbientLight( decColor( 0.0f, 0.0f, 0.0f ) );
	
	// create animator
	pEngAnimator = engine->GetAnimatorManager()->CreateAnimator();
	
	// create sub animator
	pSubAnimator = new aeSubAnimator( engine );
	
	// create testing sub animator
	pTestingSubAnimator = new aeSubAnimator( engine );
}

void aeAnimator::pCreateCamera(){
	pCamera = new aeCamera( this, GetEngine() );
	
	pCamera->SetEngineWorld( pEngWorld );
	
	pCamera->Reset();
}

void aeAnimator::pCreateCollider(){
	pEngCollider = GetEngine()->GetColliderManager()->CreateColliderComponent();
	
	pEngCollider->SetResponseType( deCollider::ertKinematic );
	pEngCollider->SetUseLocalGravity( true );
	pEngCollider->SetMass( 50.0f );
	pEngCollider->SetEnabled( false );
	
	decCollisionFilter filter( pEngCollider->GetCollisionFilter() );
	filter.GetCategory().SetBit( aeAnimator::eclElements );
	filter.GetFilter().SetBit( aeAnimator::eclElements );
	pEngCollider->SetCollisionFilter( filter );
	
	pEngWorld->AddCollider( pEngCollider );
}

void aeAnimator::pUpdateComponent(){
	deEngine * const engine = GetEngine();
	deModel *displayModel = NULL;
	deSkin *displaySkin = NULL;
	deRig *displayRig = NULL;
	
	// detach all colliders
	DetachAttachments();
	
	// disable collider
	pEngCollider->SetComponent( NULL );
	pEngCollider->SetEnabled( false );
	
	// try to load the model, skin and rig if possible
	try{
		if( ! pDisplayModelPath.IsEmpty() ){
			displayModel = engine->GetModelManager()->LoadModel(
				pDisplayModelPath.GetString(), GetDirectoryPath() );
		}
		
		if( ! pDisplaySkinPath.IsEmpty() ){
			displaySkin = engine->GetSkinManager()->LoadSkin(
				pDisplaySkinPath.GetString(), GetDirectoryPath() );
		}
		
		if( ! pDisplayRigPath.IsEmpty() ){
			displayRig = engine->GetRigManager()->LoadRig(
				pDisplayRigPath.GetString(), GetDirectoryPath() );
		}
		
		if( pRigPath.IsEmpty() ){
			pEngRig = NULL;
			
		}else{
			pEngRig.TakeOver( engine->GetRigManager()->LoadRig( pRigPath, GetDirectoryPath() ) );
		}
		
	}catch( const deException &e ){
		GetLogger()->LogException( LOGSOURCE, e );
	}
	
	// protect the loaded parts
	try{
		// if the skin is missing use the default one
		if( ! displaySkin ){
			displaySkin = GetGameDefinition()->GetDefaultSkin();
			displaySkin->AddReference();
		}
		
		// reset the animator
		pEngAnimatorInstance->SetComponent( NULL ); // otherwise the animator is not reset
		
		// update the component with the model and skin
		if( displayModel && displaySkin ){
			if( pEngComponent ){
				pEngComponent->SetModelAndSkin( displayModel, displaySkin );
				
			}else{
				pEngComponent = engine->GetComponentManager()->CreateComponent( displayModel, displaySkin );
				pEngWorld->AddComponent( pEngComponent );
				
				pEngCollider->AddAttachment( new deColliderAttachment( pEngComponent ) );
			}
			
		}else if( pEngComponent ){
			deColliderAttachment * const attachment = pEngCollider->GetAttachmentWith( pEngComponent );
			if( attachment ){
				pEngCollider->RemoveAttachment( attachment );
			}
			
			pEngWorld->RemoveComponent( pEngComponent );
			pEngComponent->FreeReference();
			pEngComponent = NULL;
		}
		
		// set the rig if the component exists
		if( pEngComponent ){
			pEngComponent->SetRig( displayRig );
			pEngComponent->SetVisible( true );
			pEngComponent->SetPosition( decDVector() );
			pEngComponent->SetOrientation( decQuaternion() );
		}
		
		// set animator rig
		pEngAnimator->SetRig( pEngRig );
		
		// free the reference we hold
		if( displayRig ){
			displayRig->FreeReference();
			displayRig = NULL;
		}
		if( displayModel ){
			displayModel->FreeReference();
			displayModel = NULL;
		}
		if( displaySkin ){
			displaySkin->FreeReference();
			displaySkin = NULL;
		}
		
	}catch( const deException & ){
		if( displayModel ){
			displayModel->FreeReference();
		}
		if( displaySkin ){
			displaySkin->FreeReference();
		}
		if( displayRig ){
			displayRig->FreeReference();
		}
		throw;
	}
	
	// update the collider
	pEngCollider->SetComponent( pEngComponent );
	pEngCollider->SetEnabled( pEngComponent != NULL );
	
	// update the animator
	pEngAnimatorInstance->SetComponent( pEngComponent );
	pUpdateAnimator();
	
	// notify rules
	pAnimCompChanged();
	
	// attach colliders
	AttachAttachments();
}

void aeAnimator::pUpdateAnimator(){
	deAnimation *animation = NULL;
	
	// try to load the animation if possible
	try{
		if( ! pAnimationPath.IsEmpty() ){
			animation = GetEngine()->GetAnimationManager()->LoadAnimation(
				pAnimationPath.GetString(), GetDirectoryPath() );
		}
		
	}catch( const deException &e ){
		GetLogger()->LogException( LOGSOURCE, e );
	}
	
	// protect the loaded parts
	try{
		// set animation
		pEngAnimator->SetAnimation( animation );
		
		// free the reference we hold
		if( animation ) animation->FreeReference();
		
		// update sub animators
		pSubAnimator->SetComponentAndAnimation( pEngComponent, animation );
		pTestingSubAnimator->SetComponentAndAnimation( pEngComponent, animation );
		
	}catch( const deException & ){
		if( animation ) animation->FreeReference();
		throw;
	}
	
	// notify rules
	pAnimCompChanged();
}

void aeAnimator::pUpdateLinks(){
	const int count = pLinks.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pLinks.GetAt( i )->UpdateController();
	}
	RebuildRules();
}

void aeAnimator::pAnimCompChanged(){
	const int count = pRules.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pRules.GetAt( i )->UpdateCompAnim();
	}
}

void aeAnimator::pUpdateEngineControllers(){
	deAnimatorController *engController = NULL;
	int i;
	
	// set all engine controller indices in our controllers to -1
	const int count = pControllers.GetCount();
	for( i=0; i<count; i++ ){
		pControllers.GetAt( i )->SetEngineControllerIndex( -1 );
	}
	
	// remove the animator from the animator instance
	pEngAnimatorInstance->SetAnimator( NULL );
	
	// remove all controllers
	pEngAnimator->RemoveAllControllers();
	
	// add an engine controller for each controller we have
	try{
		for( i=0; i<count; i++ ){
			engController = new deAnimatorController;
			pEngAnimator->AddController( engController );
			engController = NULL;
		}
		
	}catch( const deException & ){
		if( engController ){
			delete engController;
		}
		
		throw;
	}
	
	// assign the animator to the animator instance. this creates the controllers
	// inside the animator instance
	pEngAnimatorInstance->SetAnimator( pEngAnimator );
	
	// now assign the matching engine controller indices to our controllers
	for( i=0; i<count; i++ ){
		pControllers.GetAt( i )->SetEngineControllerIndex( i );
	}
	
	// links have to be updated now
	pUpdateLinks();
}

void aeAnimator::pUpdateDDSBones(){
	decDMatrix matrix;
	int boneCount = 0;
	int i;
	
	if( pEngComponent ){
		boneCount = pEngComponent->GetBoneCount();
		pEngComponent->PrepareBones();
		matrix = decDMatrix( pEngComponent->GetMatrix() ).Normalized();
	}
	
	if( boneCount != pDDSBoneCount ){
		if( pDDSBones ){
			delete [] pDDSBones;
			pDDSBones = NULL;
			pDDSBoneCount = 0;
		}
		
		if( boneCount > 0 ){
			pDDSBones = new igdeWCoordSysArrows[ boneCount ];
			
			for( pDDSBoneCount=0; pDDSBoneCount<boneCount; pDDSBoneCount++ ){
				pDDSBones[ pDDSBoneCount ].SetParentDebugDrawer( pDDBones );
			}
		}
	}
	
	pDDBones->SetPosition( matrix.GetPosition() );
	pDDBones->SetOrientation( matrix.ToQuaternion() );
	
	for( i=0; i<pDDSBoneCount; i++ ){
		if( i < boneCount && pEngComponent ){
			const decMatrix boneMatrix = pEngComponent->GetBoneAt( i ).GetMatrix().Normalized();
			pDDSBones[ i ].SetPosition( boneMatrix.GetPosition() );
			pDDSBones[ i ].SetOrientation( boneMatrix.ToQuaternion() );
			pDDSBones[ i ].SetScale( decVector( pDDSBoneSize, pDDSBoneSize, pDDSBoneSize ) );
			pDDSBones[ i ].SetVisible( true );
			
		}else{
			pDDSBones[ i ].SetVisible( false );
		}
	}
}
