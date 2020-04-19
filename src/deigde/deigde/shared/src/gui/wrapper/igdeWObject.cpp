/* 
 * Drag[en]gine IGDE
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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "igdeWObject.h"
#include "object/igdeWOSubObject.h"
#include "object/igdeWOSOBillboard.h"
#include "object/igdeWOSOLight.h"
#include "object/igdeWOSOSpeaker.h"
#include "object/igdeWOSOParticleEmitter.h"
#include "object/igdeWOSOForceField.h"
#include "object/igdeWOSOEnvMapProbe.h"
#include "object/igdeWOSONavigationSpace.h"
#include "object/igdeWOSONavigationBlocker.h"
#include "object/igdeWOSOComponent.h"
#include "../../codec/igdeCodecPropertyString.h"
#include "../../environment/igdeEnvironment.h"
#include "../../engine/igdeEngineController.h"
#include "../../gamedefinition/igdeGameDefinition.h"
#include "../../gamedefinition/class/igdeGDClass.h"
#include "../../gamedefinition/class/igdeGDClassInherit.h"
#include "../../gamedefinition/class/igdeGDClassManager.h"
#include "../../gamedefinition/class/billboard/igdeGDCBillboard.h"
#include "../../gamedefinition/class/component/igdeGDCCTexture.h"
#include "../../gamedefinition/class/component/igdeGDCComponent.h"
#include "../../gamedefinition/class/envmapprobe/igdeGDCEnvMapProbe.h"
#include "../../gamedefinition/class/light/igdeGDCLight.h"
#include "../../gamedefinition/class/speaker/igdeGDCSpeaker.h"
#include "../../gamedefinition/class/navspace/igdeGDCNavigationSpace.h"
#include "../../gamedefinition/class/navblocker/igdeGDCNavigationBlocker.h"
#include "../../gamedefinition/property/igdeGDProperty.h"
#include "../../gameproject/igdeGameProject.h"
#include "../../resourceloader/igdeResourceLoaderListener.h"
#include "../../resourceloader/igdeResourceLoaderTask.h"
#include "../../triggersystem/igdeTriggerTargetList.h"
#include "../../triggersystem/igdeTriggerExpressionParser.h"
#include "../../triggersystem/igdeTriggerExpression.h"
#include "../../triggersystem/igdeTriggerListener.h"
#include "../../triggersystem/igdeTriggerTarget.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decObjectDictionary.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileReaderReference.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderAttachment.h>
#include <dragengine/resources/collider/deColliderBone.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Definitions
////////////////

#define LOGSOURCE "IGDE"


// Collider Listener
//////////////////////

class igdeWObjectColliderListener : public deBaseScriptingCollider{
private:
	igdeWObject *pObject;
	
public:
	igdeWObjectColliderListener( igdeWObject *object ){
		pObject = object;
	}
	
	virtual ~igdeWObjectColliderListener(){
	}
	
	virtual void CollisionResponse( deCollider*, deCollisionInfo* ){
	}
	
	virtual bool CanHitCollider( deCollider*, deCollider* ){
		return true;
	}
	
	virtual void ColliderChanged( deCollider* ){
		pObject->OnColliderChanged();
	}
};



// Trigger listener
/////////////////////

class igdeWObjectTriggerListener : public igdeTriggerListener{
	igdeWObject &pWrapper;
	
protected:
	virtual ~igdeWObjectTriggerListener(){
	}
	
public:
	igdeWObjectTriggerListener( igdeWObject &wrapper ) : pWrapper( wrapper ){
	}
	
	virtual void TriggerTargetChanged( igdeTriggerTarget* ){
		pWrapper.UpdateTriggerableProperties();
	}
};



// Asynchronous load finished listener
////////////////////////////////////////

igdeWObject::cAsyncLoadFinished::cAsyncLoadFinished(){
}

igdeWObject::cAsyncLoadFinished::~cAsyncLoadFinished(){
}



// Class igdeWObject
//////////////////////

// Constructor, destructor
////////////////////////////

igdeWObject::igdeWObject( igdeEnvironment &environment ) :
pEnvironment( environment ),
pScaling( 1.0f, 1.0f, 1.0f ),
pRenderLayerMask( 0x1 ),
pRenderEnvMapMask( 0x1 ),
pAudioLayerMask( 0x2 ),
pDynamicCollider( false ),
pVisible( true ),
pPartiallyHidden( false ),
pListenerCollider( NULL ),
pTriggerTable( NULL ),
pHasBoxExtends( false ),
pDirtyExtends( false ),
pDirtyFallbackColliderShape( false ),
pAsyncLoadFinished( NULL ),
pAsyncLoadCounter( 0 )
{
	try{
		pTriggerListener.TakeOver( new igdeWObjectTriggerListener( *this ) );
		
		// create collider
		pColliderFallback.TakeOver( environment.GetEngineController()->GetEngine()
			->GetColliderManager()->CreateColliderVolume() );
		pColliderFallback->SetEnabled( true );
		pColliderFallback->SetResponseType( deCollider::ertStatic );
		pColliderFallback->SetUseLocalGravity( true );
		decShapeList shapeList;
		shapeList.Add( new decShapeBox( decVector( 0.1f, 0.1f, 0.1f ) ) );
		( ( deColliderVolume& )( deCollider& )pColliderFallback ).SetShapes( shapeList );
		pColliderFallback->SetMass( 5.0f );
		
		pListenerCollider = new igdeWObjectColliderListener( this );
		environment.SetColliderDelegee( pColliderFallback, pListenerCollider );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

igdeWObject::~igdeWObject(){
	pCleanUp();
}



// Management
///////////////

void igdeWObject::SetWorld( deWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	pDestroySubObjects();
	DetachCollider();
	if( pWorld ){
		pWorld->RemoveCollider( pColliderFallback );
	}
	
	pWorld = world;
	
	if( world ){
		world->AddCollider( pColliderFallback );
	}
	
	pUpdateVisiblity();
	
	if( world ){
		pCreateSubObjects();
	}
}

void igdeWObject::SetCamera( deCamera *camera ){
	if( camera == pCamera ){
		return;
	}
	
	pCamera = camera;
	
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->CameraChanged();
	}
}

void igdeWObject::SetGDClass( igdeGDClass *gdClass ){
	if( pGDClass == gdClass ){
		return;
	}
	
	pDestroySubObjects();
	
	pGDClass = gdClass;
	
	pDirtyExtends = true;
	pDirtyFallbackColliderShape = true;
	
	pUpdateVisiblity();
	pUpdateColliderResponseType();
	pUpdateColliderShapes();
	
	UpdateTriggerTargets();
	
	if( pWorld ){
		pCreateSubObjects();
	}
}

void igdeWObject::SetGDClassName( const char *gdClassName ){
	SetGDClass( pEnvironment.GetGameProject()->GetGameDefinition()->GetClassManager()->GetNamed( gdClassName ) );
}

void igdeWObject::SetTriggerTable( igdeTriggerTargetList *triggerTable ){
	if( triggerTable == pTriggerTable ){
		return;
	}
	
	pTriggerTable = triggerTable;
	UpdateTriggerTargets();
	
	pSubObjectsInitTriggers();
	pSubObjectsUpdateTriggers();
}



void igdeWObject::SetPosition( const decDVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	pColliderFallback->SetPosition( position );
	pSubObjectsUpdateGeometry();
}

void igdeWObject::SetOrientation( const decQuaternion &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	
	pColliderFallback->SetOrientation( orientation );
	pSubObjectsUpdateGeometry();
}

void igdeWObject::SetScaling( const decVector &scaling ){
	if( scaling.IsEqualTo( pScaling ) ){
		return;
	}
	
	pScaling = scaling.Largest( decVector( 1e-5f, 1e-5f, 1e-5f ) );
	pDirtyFallbackColliderShape = true;
	
	pSubObjectsUpdateGeometry();
	pUpdateColliderShapes();
}



void igdeWObject::SetVisible( bool visible ){
	if( visible == pVisible ){
		return;
	}
	
	pVisible = visible;
	pUpdateVisiblity();
}

void igdeWObject::SetPartiallyHidden( bool partiallyHidden ){
	if( partiallyHidden == pPartiallyHidden ){
		return;
	}
	
	pPartiallyHidden = partiallyHidden;
	pUpdateVisiblity();
}



void igdeWObject::SetRenderLayerMask( int mask ){
	if( mask == pRenderLayerMask ){
		return;
	}
	
	pRenderLayerMask = mask;
	pSubObjectsUpdateLayerMasks();
}

void igdeWObject::SetRenderEnvMapMask( int mask ){
	if( mask == pRenderEnvMapMask ){
		return;
	}
	
	pRenderEnvMapMask = mask;
	pSubObjectsUpdateLayerMasks();
}

void igdeWObject::SetAudioLayerMask( int mask ){
	if( mask == pAudioLayerMask ){
		return;
	}
	
	pAudioLayerMask = mask;
	pSubObjectsUpdateLayerMasks();
}

const decVector &igdeWObject::GetBoxMinExtend(){
	pPrepareExtends();
	return pBoxMinExtend;
}

const decVector &igdeWObject::GetBoxMaxExtend(){
	pPrepareExtends();
	return pBoxMaxExtend;
}

bool igdeWObject::GetHasBoxExtends(){
	pPrepareExtends();
	return pHasBoxExtends;
}

void igdeWObject::SetCollisionFilter( const decCollisionFilter &collisionFilter ){
	if( collisionFilter == pCollisionFilter ){
		return;
	}
	
	pCollisionFilter = collisionFilter;
	pSubObjectsUpdateCollisionFilter();
}

void igdeWObject::SetCollisionFilterParticles( const decCollisionFilter &collisionFilter ){
	if( collisionFilter == pCollisionFilterParticles ){
		return;
	}
	
	pCollisionFilterParticles = collisionFilter;
	pSubObjectsUpdateCollisionFilter();
}

void igdeWObject::SetCollisionFilterForceFields( const decCollisionFilter &collisionFilter ){
	if( collisionFilter == pCollisionFilterForceField ){
		return;
	}
	
	pCollisionFilterForceField = collisionFilter;
	pSubObjectsUpdateCollisionFilter();
}

void igdeWObject::SetCollisionFilterFallback( const decCollisionFilter &collisionFilter ){
	if( collisionFilter == pCollisionFilterFallback ){
		return;
	}
	
	pCollisionFilterFallback = collisionFilter;
	
	pColliderFallback->SetCollisionFilter( pCollisionFilterFallback );
	pSubObjectsUpdateCollisionFilter();
}

void igdeWObject::SetDynamicCollider( bool dynamic ){
	if( dynamic == pDynamicCollider ){
		return;
	}
	
	pDynamicCollider = dynamic;
	pUpdateColliderResponseType();
}



void igdeWObject::Update( float elapsed ){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->Update( elapsed );
	}
	
	pUpdateColliderShapes();
}

void igdeWObject::ResetComponentTextures(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->ResetComponentTextures();
	}
}

void igdeWObject::ResetPhysics(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->ResetPhysics();
	}
}

void igdeWObject::VisitSubObjects( igdeWOSOVisitor &visitor ){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->Visit( visitor );
	}
}

void igdeWObject::SetAsyncLoadFinished( cAsyncLoadFinished *listener ){
	pAsyncLoadFinished = listener;
}

void igdeWObject::OnGameDefinitionChanged(){
	if( pGDClass ){
		SetGDClassName( pGDClass->GetName() );
	}
}



// Properties
///////////////

void igdeWObject::SetProperty( const char *key, const char *value ){
	pProperties.SetAt( key, value );
	pUpdateProperties();
}

void igdeWObject::SetPropertiesFrom( const decStringDictionary &list ){
	pProperties = list;
	pUpdateProperties();
}

void igdeWObject::RemoveProperty( const char *key ){
	if( pProperties.Has( key ) ){
		pProperties.Remove( key );
		pUpdateProperties();
	}
}

void igdeWObject::RemoveAllProperties(){
	if( pProperties.GetCount() > 0 ){
		pProperties.RemoveAll();
		pUpdateProperties();
	}
}



bool igdeWObject::IsAttachedCollider() const{
	return pParentCollider;
}

void igdeWObject::AttachColliderRig( deColliderComponent *parentCollider ){
	if( ! parentCollider ){
		DETHROW( deeInvalidParam );
	}
	
	DetachCollider();
	
	deColliderAttachment *attachment = NULL;
	try{
		if( pColliderComponent ){
			attachment = new deColliderAttachment( pColliderComponent );
			attachment->SetAttachType( deColliderAttachment::eatRig );
			parentCollider->AddAttachment( attachment );
			attachment = NULL;
		}
		
		attachment = new deColliderAttachment( pColliderFallback );
		attachment->SetAttachType( deColliderAttachment::eatStatic );
		parentCollider->AddAttachment( attachment );
		
	}catch( const deException &e ){
		if( attachment ){
			delete attachment;
		}
		throw;
	}
	
	pParentCollider = parentCollider;
}

void igdeWObject::AttachColliderBone( deColliderComponent *parentCollider, const char *bone,
const decVector &position, const decQuaternion &orientation ){
	if( ! parentCollider || ! bone ){
		DETHROW( deeInvalidParam );
	}
	
	DetachCollider();
	
	if( strlen( bone ) > 0 ){
		deColliderAttachment *attachment = NULL;
		try{
			if( pColliderComponent ){
				attachment = new deColliderAttachment( pColliderComponent );
				attachment->SetAttachType( deColliderAttachment::eatBone );
				attachment->SetTrackBone( bone );
				attachment->SetPosition( position );
				attachment->SetOrientation( orientation );
				parentCollider->AddAttachment( attachment );
				attachment = NULL;
			}
			
			attachment = new deColliderAttachment( pColliderFallback );
			attachment->SetAttachType( deColliderAttachment::eatBone );
			attachment->SetTrackBone( bone );
			attachment->SetPosition( position );
			attachment->SetOrientation( orientation );
			parentCollider->AddAttachment( attachment );
			
		}catch( const deException &e ){
			if( attachment ){
				delete attachment;
			}
			throw;
		}
		
		pAttachToBone = bone;
	}
	
	pParentCollider = parentCollider;
}

void igdeWObject::DetachCollider(){
	if( ! pParentCollider ){
		return;
	}
	
	deColliderAttachment *attachment = pParentCollider->GetAttachmentWith( pColliderFallback );
	if( attachment ){
		pParentCollider->RemoveAttachment( attachment );
	}
	
	if( pColliderComponent ){
		attachment = pParentCollider->GetAttachmentWith( pColliderComponent );
		if( attachment ){
			pParentCollider->RemoveAttachment( attachment );
		}
	}
	
	pParentCollider = NULL;
	pAttachToBone.Empty();
	
	// reset position and orientation otherwise our state and the collider state are out of sync
	pColliderFallback->SetPosition( pPosition );
	pColliderFallback->SetOrientation( pOrientation );
	
	pSubObjectsUpdateGeometry();
}

deCollider *igdeWObject::GetCollider() const{
	return pColliderComponent ? pColliderComponent : pColliderFallback;
}

deComponent *igdeWObject::GetComponent() const{
	return pColliderComponent ? ( ( deColliderComponent& )( deCollider& )pColliderComponent ).GetComponent() : NULL;
}

void igdeWObject::SetColliderUserPointer( void *userPointer ){
	if( pColliderComponent ){
		pEnvironment.SetColliderUserPointer( pColliderComponent, userPointer );
	}
	pEnvironment.SetColliderUserPointer( pColliderFallback, userPointer );
}

void igdeWObject::OnColliderChanged(){
}



void igdeWObject::UpdateTriggerTargets(){
	pListTriggerTarget.RemoveAll();
	if( ! pTriggerTable || ! pGDClass ){
		return;
	}
	
	const decStringList &keys = pProperties.GetKeys();
	const int keyCount = keys.GetCount();
	int i;
	
	for( i=0; i<keyCount; i++ ){
		const decString &key = keys.GetAt( i );
		const decString &value = pProperties.GetAt( key );
		if( value.IsEmpty() ){
			continue;
		}
		
		igdeGDProperty * const gdproperty = pGDClass->GetPropertyNamed( key );
		if( gdproperty && gdproperty->GetType() == igdeGDProperty::eptTriggerTarget ){
			pListTriggerTarget.Add( pTriggerTable->GetNamedAddIfMissing( value ) );
		}
	}
}

void igdeWObject::UpdateTriggerableProperties(){
	pSubObjectsUpdateTriggers();
}



void igdeWObject::SubObjectFinishedLoading( igdeWOSubObject&, bool ){
	pAsyncLoadCounter--;
	pCheckAsyncLoadFinished();
}

void igdeWObject::SubObjectExtendsDirty(){
	pDirtyExtends = true;
	pDirtyFallbackColliderShape = true;
}

void igdeWObject::SetInteractCollider( deColliderComponent *collider ){
	if( pColliderComponent ){
		pEnvironment.SetColliderDelegee( pColliderComponent, NULL );
		pEnvironment.SetColliderUserPointer( pColliderComponent, NULL );
		pColliderComponent = NULL;
		pColliderFallback->SetEnabled( pVisible && ! pPartiallyHidden );
	}
	
	if( ! collider ){
		return;
	}
	
	pColliderFallback->SetEnabled( false );
	pColliderComponent = collider;
	pEnvironment.SetColliderDelegee( collider, pListenerCollider );
	pEnvironment.SetColliderUserPointer( collider, pEnvironment.GetColliderUserPointer( pColliderFallback ) );
}



// Private Functions
//////////////////////

void igdeWObject::pCleanUp(){
	DetachCollider();
	pDestroySubObjects();
	   SetWorld( NULL );
	
	if( pColliderFallback ){
		pEnvironment.SetColliderDelegee( pColliderFallback, NULL );
		pColliderFallback = NULL;
	}
	if( pListenerCollider ){
		delete pListenerCollider;
	}
}



void igdeWObject::pUpdateVisiblity(){
	const bool partiallyVisible = ( pVisible && ! pPartiallyHidden );
	pColliderFallback->SetEnabled( ! pColliderComponent && partiallyVisible );
	
	pSubObjectsUpdateVisibility();
}



void igdeWObject::pCreateSubObjects(){
	pAsyncLoadCounter = 1;
	
	if( pGDClass && pWorld ){
		pCreateSubObjects( "", pGDClass );
	}
	
	pAsyncLoadCounter--;
	pCheckAsyncLoadFinished();
}

void igdeWObject::pCreateSubObjects( const decString &prefix, const igdeGDClass &gdclass ){
	deObjectReference refSubObject;
	int i;
	
	// components
	const igdeGDCComponentList &components = gdclass.GetComponentList();
	const int componentCount = components.GetCount();
	for( i=0; i<componentCount; i++ ){
		pAsyncLoadCounter++;
		try{
			refSubObject.TakeOver( new igdeWOSOComponent( *this, *components.GetAt( i ), prefix ) );
			pSubObjects.Add( refSubObject );
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	// billboards
	const igdeGDCBillboardList &billboards = gdclass.GetBillboardList();
	const int billboardCount = billboards.GetCount();
	for( i=0; i<billboardCount; i++ ){
		pAsyncLoadCounter++;
		try{
			refSubObject.TakeOver( new igdeWOSOBillboard( *this, *billboards.GetAt( i ), prefix ) );
			pSubObjects.Add( refSubObject );
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	// lights
	const igdeGDCLightList &lights = gdclass.GetLightList();
	const int lightCount = lights.GetCount();
	for( i=0; i<lightCount; i++ ){
		pAsyncLoadCounter++;
		try{
			refSubObject.TakeOver( new igdeWOSOLight( *this, *lights.GetAt( i ), prefix ) );
			pSubObjects.Add( refSubObject );
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	// speakers
	const igdeGDCSpeakerList &speakers = gdclass.GetSpeakerList();
	const int speakerCount = speakers.GetCount();
	for( i=0; i<speakerCount; i++ ){
		pAsyncLoadCounter++;
		try{
			refSubObject.TakeOver( new igdeWOSOSpeaker( *this, *speakers.GetAt( i ), prefix ) );
			pSubObjects.Add( refSubObject );
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	// particle emitters
	const igdeGDCParticleEmitterList &particleEmitters = gdclass.GetParticleEmitterList();
	const int particleEmitterCount = particleEmitters.GetCount();
	for( i=0; i<particleEmitterCount; i++ ){
		pAsyncLoadCounter++;
		try{
			refSubObject.TakeOver( new igdeWOSOParticleEmitter( *this, *particleEmitters.GetAt( i ), prefix ) );
			pSubObjects.Add( refSubObject );
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	// force fields
	const igdeGDCForceFieldList &forceFields = gdclass.GetForceFieldList();
	const int forceFieldCount = forceFields.GetCount();
	for( i=0; i<forceFieldCount; i++ ){
		pAsyncLoadCounter++;
		try{
			refSubObject.TakeOver( new igdeWOSOForceField( *this, *forceFields.GetAt( i ), prefix ) );
			pSubObjects.Add( refSubObject );
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	// envMapProbes
	const igdeGDCEnvMapProbeList &envMapProbes = gdclass.GetEnvironmentMapProbeList();
	const int envMapProbeCount = envMapProbes.GetCount();
	for( i=0; i<envMapProbeCount; i++ ){
		pAsyncLoadCounter++;
		try{
			refSubObject.TakeOver( new igdeWOSOEnvMapProbe( *this, *envMapProbes.GetAt( i ), prefix ) );
			pSubObjects.Add( refSubObject );
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	// navigation spaces
	const igdeGDCNavigationSpaceList &navigationSpaces = gdclass.GetNavigationSpaceList();
	const int navigationSpaceCount = navigationSpaces.GetCount();
	for( i=0; i<navigationSpaceCount; i++ ){
		pAsyncLoadCounter++;
		try{
			refSubObject.TakeOver( new igdeWOSONavigationSpace( *this, *navigationSpaces.GetAt( i ), prefix ) );
			pSubObjects.Add( refSubObject );
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	// navigation spaces
	const igdeGDCNavigationBlockerList &navigationBlockers = gdclass.GetNavigationBlockerList();
	const int navigationBlockerCount = navigationBlockers.GetCount();
	for( i=0; i<navigationBlockerCount; i++ ){
		pAsyncLoadCounter++;
		try{
			refSubObject.TakeOver( new igdeWOSONavigationBlocker( *this, *navigationBlockers.GetAt( i ), prefix ) );
			pSubObjects.Add( refSubObject );
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	// inherited classes
	const int inheritCount = gdclass.GetInheritClassCount();
	for( i=0; i<inheritCount; i++ ){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt( i );
		if( inherit.GetClass() ){
			pCreateSubObjects( inherit.GetPropertyPrefix() + prefix, *inherit.GetClass() );
		}
	}
}

void igdeWObject::pDestroySubObjects(){
	pSubObjects.RemoveAll();
}

void igdeWObject::pSubObjectsReattachToColliders(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->ReattachToColliders();
	}
}

void igdeWObject::pSubObjectsInitTriggers(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->InitTriggers();
	}
}

void igdeWObject::pSubObjectsUpdateGeometry(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->UpdateGeometry();
	}
}

void igdeWObject::pSubObjectsUpdateTriggers(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->UpdateTriggers();
	}
}

void igdeWObject::pSubObjectsUpdateVisibility(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->UpdateVisibility();
	}
}

void igdeWObject::pSubObjectsUpdateLayerMasks(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->UpdateLayerMasks();
	}
}

void igdeWObject::pSubObjectsUpdateCollisionFilter(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->UpdateCollisionFilter();
	}
}

void igdeWObject::pSubObjectsAllFinishedLoading(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->OnAllSubObjectsFinishedLoading();
	}
}

void igdeWObject::pCheckAsyncLoadFinished(){
	if( pAsyncLoadCounter != 0 ){
		return;
	}
	
	pSubObjectsAllFinishedLoading();
	pSubObjectsUpdateTriggers();
	pSubObjectsReattachToColliders();
	
	if( pAsyncLoadFinished ){
		pAsyncLoadFinished->LoadFinished( *this, true );
	}
}

void igdeWObject::pUpdateProperties(){
	UpdateTriggerTargets();
	
	if( ! pWorld ){
		return;
	}
	
	pAsyncLoadCounter = 1;
	
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pAsyncLoadCounter++;
		try{
			( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->UpdateParameters();
			
		}catch( const deException &e ){
			pEnvironment.GetLogger()->LogException( LOGSOURCE, e );
			pAsyncLoadCounter--;
		}
	}
	
	pAsyncLoadCounter--;
	pCheckAsyncLoadFinished();
	
	pSubObjectsInitTriggers();
	pSubObjectsUpdateTriggers();
}

void igdeWObject::pUpdateColliderResponseType(){
	const int count = pSubObjects.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( igdeWOSubObject* )pSubObjects.GetAt( i ) )->UpdateColliderResponseType();
	}
	
	deCollider::eResponseType responseType;
	if( pColliderComponent ){
		responseType = pColliderComponent->GetResponseType();
		
	}else if( pDynamicCollider ){
		responseType = deCollider::ertDynamic;
		
	}else{
		responseType = deCollider::ertKinematic;
	}
	
	pColliderFallback->SetResponseType( responseType );
	pColliderFallback->SetUseLocalGravity( responseType != deCollider::ertDynamic );
}

void igdeWObject::pUpdateColliderShapes(){
	if( ! pDirtyFallbackColliderShape ){
		return;
	}
	
	pDirtyFallbackColliderShape = false;
	
	const int count = pSubObjects.GetCount();
	decShapeList shapeList;
	int i;
	for( i=0; i<count; i++ ){
		const igdeWOSubObject &subObject = *( ( igdeWOSubObject* )pSubObjects.GetAt( i ) );
		if( ! subObject.HasBoxExtends() ){
			continue;
		}
		
		const decVector &boxMinExtend = subObject.GetBoxMinExtends();
		const decVector &boxMaxExtend = subObject.GetBoxMaxExtends();
		decVector boxSize( ( boxMaxExtend - boxMinExtend ) * 0.5f );
		decVector boxCenter( ( boxMinExtend + boxMaxExtend ) * 0.5f );
		
		boxSize.x *= pScaling.x;
		boxSize.y *= pScaling.y;
		boxSize.z *= pScaling.z;
		boxCenter.x *= pScaling.x;
		boxCenter.y *= pScaling.y;
		boxCenter.z *= pScaling.z;
		
		shapeList.Add( new decShapeBox( boxSize, boxCenter ) );
	}
	
	if( shapeList.GetCount() == 0 ){
		// ensure the user can still select the object
		shapeList.Add( new decShapeBox( decVector( 0.1f, 0.1f, 0.1f ) ) );
	}
	
	( ( deColliderVolume& )( deCollider& )pColliderFallback ).SetShapes( shapeList );
}

void igdeWObject::pPrepareExtends(){
	if( ! pDirtyExtends ){
		return;
	}
	
	const int count = pSubObjects.GetCount();
	int i;
	
	pHasBoxExtends = false;
	pBoxMinExtend.SetZero();
	pBoxMaxExtend.SetZero();
	
	for( i=0; i<count; i++ ){
		const igdeWOSubObject &subObject = *( ( igdeWOSubObject* )pSubObjects.GetAt( i ) );
		if( ! subObject.HasBoxExtends() ){
			continue;
		}
		
		if( pHasBoxExtends ){
			pBoxMinExtend.SetSmallest( subObject.GetBoxMinExtends() );
			pBoxMaxExtend.SetLargest( subObject.GetBoxMaxExtends() );
			
		}else{
			pBoxMinExtend = subObject.GetBoxMinExtends();
			pBoxMaxExtend = subObject.GetBoxMaxExtends();
			pHasBoxExtends = true;
		}
	}
	
	if( pHasBoxExtends ){
		const decVector boxSize( pBoxMaxExtend - pBoxMinExtend );
		if( boxSize.x < 0.001f ){
			pBoxMinExtend.x -= 0.0005f;
			pBoxMaxExtend.x += 0.0005f;
		}
		if( boxSize.y < 0.001f ){
			pBoxMinExtend.y -= 0.0005f;
			pBoxMaxExtend.y += 0.0005f;
		}
		if( boxSize.z < 0.001f ){
			pBoxMinExtend.z -= 0.0005f;
			pBoxMaxExtend.z += 0.0005f;
		}
	}
	
	pDirtyExtends = false;
}
