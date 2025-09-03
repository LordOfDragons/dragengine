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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "meObject.h"
#include "meObjectLink.h"
#include "meObjectSnapPoint.h"
#include "texture/meObjectTexture.h"
#include "../decal/meDecal.h"
#include "../meWorld.h"
#include "../meCamera.h"
#include "../pathfinding/mePathFindTest.h"
#include "../meWorldGuiParameters.h"
#include "../idgroup/meMapIDGroup.h"
#include "../idgroup/meIDGroup.h"
#include "../../collisions/meCLInvalidateDecals.h"
#include "../../configuration/meConfiguration.h"
#include "../../gui/meWindowMain.h"
#include "../../utils/meHelpers.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassInherit.h>
#include <deigde/gamedefinition/class/igdeGDClassManager.h>
#include <deigde/gamedefinition/class/igdeGDCamera.h>
#include <deigde/gamedefinition/class/component/igdeGDCComponent.h>
#include <deigde/gamedefinition/class/component/igdeGDCCTexture.h>
#include <deigde/gamedefinition/class/envmapprobe/igdeGDCEnvMapProbe.h>
#include <deigde/gamedefinition/class/navspace/igdeGDCNavigationSpace.h>
#include <deigde/gamedefinition/class/speaker/igdeGDCSpeaker.h>
#include <deigde/gamedefinition/class/light/igdeGDCLight.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>
#include <deigde/gui/wrapper/igdeWObject.h>
#include <deigde/gui/wrapper/object/igdeWOSOVisitor.h>
#include <deigde/gui/wrapper/object/igdeWOSOLight.h>
#include <deigde/gui/wrapper/object/igdeWOSONavigationSpace.h>
#include <deigde/gui/wrapper/object/igdeWOSONavigationBlocker.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWCoordSysArrows.h>
#include <deigde/triggersystem/igdeTriggerTarget.h>
#include <deigde/triggersystem/igdeTriggerExpression.h>
#include <deigde/codec/igdeCodecPropertyString.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/engine/igdeEngineController.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animator/deAnimatorManager.h>
#include <dragengine/resources/animator/deAnimatorInstance.h>
#include <dragengine/resources/animator/deAnimatorInstanceManager.h>
#include <dragengine/resources/animator/deAnimatorLink.h>
#include <dragengine/resources/animator/controller/deAnimatorController.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleAnimation.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleVisitorIdentify.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationManager.h>
#include <dragengine/resources/animation/deAnimationMove.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderComponent.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentManager.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/component/deComponentBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMeshManager.h>
#include <dragengine/resources/probe/deEnvMapProbeManager.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/resources/skin/dynamic/renderables/deDSRenderableColor.h>
#include <dragengine/resources/light/deLight.h>
#include <dragengine/resources/light/deLightManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/probe/deEnvMapProbe.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSpeakerManager.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// class cWOAsyncFinished
///////////////////////////

meObject::cWOAsyncFinished::cWOAsyncFinished(meObject &object) : pObject(object){
}

void meObject::cWOAsyncFinished::LoadFinished(igdeWObject&, bool){
	pObject.WOAsyncFinished();
}

void meObject::cWOAsyncFinished::AnyContentVisibleChanged(igdeWObject &wrapper){
	pObject.WOAnyContentVisibleChanged();
}

void meObject::cWOAsyncFinished::ExtendsChanged(igdeWObject &wrapper){
	pObject.WOExtendsChanged();
}



// class cWOTexture
/////////////////////

meObject::cWOTexture::cWOTexture( const deComponentTexture &componentTexture ) :
skin( componentTexture.GetSkin() ),
texture( componentTexture.GetTexture() ),
dynamicSkin( componentTexture.GetDynamicSkin() ),
texCoordTransform( componentTexture.GetTransform() ){
}



// Class meObject
///////////////////

// Constructor, destructor
////////////////////////////

meObject::meObject( igdeEnvironment *environment ) :
pColliderOwner( this ),
pWOAsyncFinished( *this )
{
	if( ! environment ){
		DETHROW( deeInvalidParam );
	}
	
	deEngine * const engine = environment->GetEngineController()->GetEngine();
	
	pEnvironment = environment;
	pWorld = nullptr;
	
	pDebugDrawer = nullptr;
	pDDSObject = nullptr;
	pDDSLightAoE = nullptr;
	pDDSOcclusionMesh = nullptr;
	pDDSObjectShapes = nullptr;
	pDDSCoordSysArrows = nullptr;
	
	pWObject = nullptr;
	pEngComponentBroken = nullptr;
	pColDetCollider = nullptr;
	pCamera = nullptr;
	
	pRange = 10.0f; //pGetRangeFor( 1.0f, 2.0f, 0.01f );
	
	pTextures = nullptr;
	pTextureCount = 0;
	pTextureSize = 0;
	pActiveTexture = nullptr;
	
	pDecals = nullptr;
	pDecalCount = 0;
	pDecalSize = 0;
	
	pClassDef = nullptr;
	pSelected = false;
	pActive = false;
	pVisible = true;
	pShowMissingTextures = false;
	pSize.Set( 0.5f, 0.5f, 0.5f );
	pScaling.Set( 1.0f, 1.0f, 1.0f );
	pAttachedTo = nullptr;
	
	try{
		pWObject = new igdeWObject( *environment );
		
		// collision filter
		decLayerMask collisionCategory;
		collisionCategory.SetBit( meWorld::eclmObjects );
		
		decLayerMask collisionFilter;
		collisionFilter.SetBit( meWorld::eclmParticles );
		collisionFilter.SetBit( meWorld::eclmEditing );
		
		pWObject->SetCollisionFilter( decCollisionFilter( collisionCategory, collisionFilter ) );
		
		// particles collision filter
		collisionCategory = decLayerMask();
		collisionCategory.SetBit( meWorld::eclmParticles );
		
		collisionFilter = decLayerMask();
		collisionFilter.SetBit( meWorld::eclmObjects );
		collisionFilter.SetBit( meWorld::eclmHeightTerrains );
		collisionFilter.SetBit( meWorld::eclmPropFields );
		collisionFilter.SetBit( meWorld::eclmForceField );
		
		pWObject->SetCollisionFilterParticles( decCollisionFilter( collisionCategory, collisionFilter ) );
		
		// collision filter force field
		collisionCategory = decLayerMask();
		collisionCategory.SetBit( meWorld::eclmForceField );
		
		collisionFilter = decLayerMask();
		collisionFilter.SetBit( meWorld::eclmPropFields );
		collisionFilter.SetBit( meWorld::eclmParticles );
		
		pWObject->SetCollisionFilterForceFields( decCollisionFilter( collisionCategory, collisionFilter ) );
		
		// collision filter fallback and selection
		collisionCategory = decLayerMask();
		collisionCategory.SetBit( meWorld::eclmObjects );
		
		collisionFilter = decLayerMask();
		collisionFilter.SetBit( meWorld::eclmEditing );
		
		pWObject->SetCollisionFilterFallback( decCollisionFilter( collisionCategory, collisionFilter ) );
		// pWObject->SetCollisionFilterInteract( decCollisionFilter( collisionCategory, collisionFilter ) );
		
		// other parameters
		pWObject->SetRenderEnvMapMask( 1 << meWorld::elmEnvMapProbes );
		pWObject->SetAudioLayerMask( 1 << meWorld::elmAudio );
		
		pColDetCollider = engine->GetColliderManager()->CreateColliderVolume();
		pColDetCollider->SetEnabled( true );
		pColDetCollider->SetResponseType( deCollider::ertKinematic );
		pColDetCollider->SetUseLocalGravity( true );
		
		pWObject->SetColliderUserPointer( &pColliderOwner );
		pWObject->SetAsyncLoadFinished( &pWOAsyncFinished );
		
		// create debug drawer and shapes
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay( true );
		
		pDDSObject = new igdeWDebugDrawerShape;
		pDDSObject->SetVisible( false );
		pDDSObject->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSLightAoE = new igdeWDebugDrawerShape;
		pDDSLightAoE->SetVisible( false );
		pDDSLightAoE->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSOcclusionMesh = new igdeWDebugDrawerShape;
		pDDSOcclusionMesh->SetVisible( false );
		pDDSOcclusionMesh->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSObjectShapes = new igdeWDebugDrawerShape;
		pDDSObjectShapes->SetVisible( false );
		pDDSObjectShapes->SetEdgeColor( decColor( 0.35f, 0.35f, 0.35f, 1.0f ) );
		pDDSObjectShapes->SetFillColor( decColor( 0.35f, 0.35f, 0.35f, 0.1f ) );
		pDDSObjectShapes->SetParentDebugDrawer( pDebugDrawer );
		
		pDDSCoordSysArrows = new igdeWCoordSysArrows;
		pDDSCoordSysArrows->SetVisible( false );
		pDDSCoordSysArrows->SetArrowLength( 0.2f );
		pDDSCoordSysArrows->SetArrowSize( pDDSCoordSysArrows->GetArrowLength() * 0.05f );
		pDDSCoordSysArrows->SetParentDebugDrawer( pDebugDrawer );
		
		SetClassName( "Unknown" );
		
		pUpdateDDSColors();
		pUpdateShapes();
		pUpdateOutline();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meObject::~meObject(){
	pCleanUp();
}



// Management
///////////////

void meObject::Dispose(){
	RemoveAllTextures();
	RemoveAllLinks();
	pRemoveAllSnapPoints();
	
	SetAttachedTo( NULL );
	while( pAttachedObjectsList.GetCount() > 0 ){
		( ( meObject* )pAttachedObjectsList.GetAt(
			pAttachedObjectsList.GetCount() - 1 ) )->SetAttachedTo( NULL );
	}
}

void meObject::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	SetAttachedTo( NULL );
	pRemoveAllSnapPoints();
	
	if( pWorld ){
		pWorld->GetEngineWorld()->RemoveDebugDrawer( pDebugDrawer );
		if( pEngComponentBroken ){
			pWorld->GetEngineWorld()->RemoveComponent( pEngComponentBroken );
		}
	}
	
	if( pCamera ){
		pCamera->SetWorld( world );
	}
	
	DecrementIDGroupIDUsage();
	
	pWorld = world;
	
	UpdateIDGroupList();
	IncrementIDGroupIDUsage();
	
	if( world ){
		if( pEngComponentBroken ){
			world->GetEngineWorld()->AddComponent( pEngComponentBroken );
		}
		world->GetEngineWorld()->AddDebugDrawer( pDebugDrawer );
		
		pWObject->SetWorld( world->GetEngineWorld() ); // can trigger WOAsyncFinished in the future
		pWObject->SetTriggerTable( &world->GetTriggerTable() ); // can trigger WOAsyncFinished in the future
		
	}else{
		pWObject->SetWorld( NULL );
		pWObject->SetTriggerTable( NULL );
	}
	
	int i;
	for( i=0; i<pDecalCount; i++ ){
		pDecals[ i ]->SetWorld( world );
	}
	
	pUpdateComponent();
	pUpdateCamera();
	UpdateTriggerTargets();
	pCreateSnapPoints();
	pUpdateDDSNavSpaces();
	UpdateDDSObjectShapes();
	OnActiveCameraChanged();
	
	ShowStateChanged();
}

void meObject::SetSelected( bool selected ){
	if( selected == pSelected ){
		return;
	}
	
	pSelected = selected;
	
	pUpdateDDSColors();
	pUpdateOutline();
}

void meObject::SetActive( bool active ){
	if( active == pActive ){
		return;
	}
	
	pActive = active;
	
	pUpdateDDSColors();
	UpdateDDSObjectShapes();
	pUpdateOutline();
	CheckLinks();
}

void meObject::SetVisible( bool visible ){
	if( visible == pVisible ){
		return;
	}
	
	pVisible = visible;
	
	pWObject->SetVisible( visible );
	pUpdateComponent();
	pUpdateDDSColors();
	pUpdateOutline();
}

void meObject::SetShowMissingTextures( bool showMissingTextures ){
	if( showMissingTextures == pShowMissingTextures ){
		return;
	}
	
	pShowMissingTextures = showMissingTextures;
	
	pUpdateComponent();
	
	if( pWorld ){
		pWorld->NotifyObjectChanged( this );
	}
}

igdeGDClass::eScaleModes meObject::GetScaleMode() const{
	return pClassDef ? pClassDef->GetScaleMode() : igdeGDClass::esmFree;
}

bool meObject::IsGhost() const{
	if( pClassDef ){
		return pClassDef->GetIsGhost();
	}
	return false;
}


void meObject::OnGameDefinitionChanged(){
	igdeGDClass * const classDef = pEnvironment->GetGameProject()->
		GetGameDefinition()->GetClassManager()->GetNamed( pClassName );
	
	if( pWObject->GetGDClass() != classDef ){
		pWObject->SetGDClass( classDef ); // can trigger WOAsyncFinished in the future
		
		pUpdateDDSCoordSysArrowsLength();
		pUpdateCamera();
		UpdateTriggerTargets();
		UpdateNavPathTest();
		UpdateIDGroupList();
	}
	
	if( classDef == pClassDef ){
		return;
	}
	
	pRemoveAllSnapPoints();
	DecrementIDGroupIDUsage();
	
	pClassDef = classDef;
	
	UpdateIDGroupList();
	IncrementIDGroupIDUsage();
	pCreateSnapPoints();
}

void meObject::OnActiveCameraChanged(){
	const meCamera * const camera = pWorld && pWorld->GetActiveCamera() ? pWorld->GetActiveCamera() : nullptr;
	pWObject->SetCamera( camera ? camera->GetEngineCamera() : nullptr );
	SetVisible( ! camera || camera != pCamera );
}



void meObject::SetClassName( const char *className ){
	if( ! className ){
		DETHROW( deeInvalidParam );
	}
	
	pClassName = className;
	
	if( pWorld ){
		pWorld->SetChanged( true );
	}
	
	OnGameDefinitionChanged();
}

void meObject::SetPosition( const decDVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	meCLInvalidateDecals::Helper invalidateDecals( pWorld );
	invalidateDecals.Collect( *pWObject );
	
	pPosition = position;
	
	pDebugDrawer->SetPosition( position );
	pWObject->SetPosition( position );
	if( pEngComponentBroken ){
		pEngComponentBroken->SetPosition( position );
	}
	if( pColDetCollider ){
		pColDetCollider->SetPosition( position );
	}
	
	if( pWorld ) pWorld->SetChanged( true );
	pRepositionLinks();
	pRepositionCamera();
	UpdateNavPathTest();
	
	invalidateDecals.Collect( *pWObject );
	invalidateDecals.InvalidateDecals();
	
	pNotifyDecalsAboutChange();
}

void meObject::SetSize( const decVector &size ){
	if( size.IsEqualTo( pSize ) ){
		return;
	}
	
	meCLInvalidateDecals::Helper invalidateDecals( pWorld );
	invalidateDecals.Collect( *pWObject );
	
	pSize = decVector( 1e-5f, 1e-5f, 1e-5f ).Largest( size );
	
	if( pWorld ){
		pWorld->SetChanged( true );
	}
	
	pScalingFromSize();
	pUpdateDDSCoordSysArrowsLength();
	
	pUpdateShapes();
	pRepositionDDSNavSpaces();
	UpdateNavPathTest();
	
	invalidateDecals.Collect( *pWObject );
	invalidateDecals.InvalidateDecals();
	
	pNotifyDecalsAboutChange();
}

void meObject::SetScaling( const decVector &scaling ){
	if( scaling.IsEqualTo( pScaling ) ){
		return;
	}
	
	meCLInvalidateDecals::Helper invalidateDecals( pWorld );
	invalidateDecals.Collect( *pWObject );
	
	pScaling = decVector( 1e-5f, 1e-5f, 1e-5f ).Largest( scaling );
	
	if( pWorld ){
		pWorld->SetChanged( true );
	}
	
	pSizeFromScaling();
	pUpdateDDSCoordSysArrowsLength();
	
	pWObject->SetScaling( pScaling );
	pUpdateShapes();
	pRepositionDDSNavSpaces();
	UpdateNavPathTest();
	
	invalidateDecals.Collect( *pWObject );
	invalidateDecals.InvalidateDecals();
	
	pNotifyDecalsAboutChange();
}

void meObject::SetSizeAndScaling( const decVector &size, const decVector &scaling ){
	if( size.IsEqualTo( pSize ) && scaling.IsEqualTo( pScaling ) ){
		return;
	}
	
	meCLInvalidateDecals::Helper invalidateDecals( pWorld );
	invalidateDecals.Collect( *pWObject );
	
	pSize = decVector( 1e-5f, 1e-5f, 1e-5f ).Largest( size );
	pScaling = decVector( 1e-5f, 1e-5f, 1e-5f ).Largest( scaling );
	
	if( pWorld ){
		pWorld->SetChanged( true );
	}
	
	pUpdateDDSCoordSysArrowsLength();
	
	pUpdateShapes();
	pRepositionDDSNavSpaces();
	UpdateNavPathTest();
	
	invalidateDecals.Collect( *pWObject );
	invalidateDecals.InvalidateDecals();
	
	pNotifyDecalsAboutChange();
}

void meObject::SetRotation( const decVector &rotation ){
	if( rotation.IsEqualTo( pRotation ) ){
		return;
	}
	
	const decQuaternion orientation = decQuaternion::CreateFromEuler( rotation * DEG2RAD );
	
	meCLInvalidateDecals::Helper invalidateDecals( pWorld );
	invalidateDecals.Collect( *pWObject );
	
	pRotation = rotation;
	
	if( pWorld ) pWorld->SetChanged( true );
	
	pDebugDrawer->SetOrientation( orientation );
	pWObject->SetOrientation( orientation );
	if( pEngComponentBroken ){
		pEngComponentBroken->SetOrientation( orientation );
	}
	if( pColDetCollider ){
		pColDetCollider->SetOrientation( orientation );
	}
	
	pRepositionCamera();
	UpdateNavPathTest();
	
	invalidateDecals.Collect( *pWObject );
	invalidateDecals.InvalidateDecals();
	
	pNotifyDecalsAboutChange();
}

void meObject::SetID( const decUniqueID &id ){
	if( id == pID ){
		return;
	}
	
	pID = id;
	
	if( pWorld ){
		pWorld->SetChanged( true );
	}
}



void meObject::UpdateDDSObjectShapes(){
	pDDSObjectShapes->RemoveAllShapes();
	if(!pWorld){
		return;
	}
	
	const decStringList keys(pProperties.GetKeys());
	const int keyCount = keys.GetCount();
	igdeCodecPropertyString codec;
	int i;
	
	for(i=0; i<keyCount; i++){
		const decString &key = keys.GetAt(i);
		if(!IsPropertyShapeOrShapeList(key)){
			continue;
		}
		
		if(pActive && pActiveProperty == key){
			continue;
		}
		
		decShapeList shapeList;
		codec.DecodeShapeList(pProperties.GetAt(key), shapeList);
		
		const int shapeCount = shapeList.GetCount();
		decShape *shape = nullptr;
		int j;
		
		try{
			for(j=0; j<shapeCount; j++){
				shape = shapeList.GetAt(j)->Copy();
				pDDSObjectShapes->AddShape(shape);
				shape = nullptr;
			}
			
		}catch(const deException &){
			if(shape){
				delete shape;
			}
			throw;
		}
	}
}



void meObject::SetAttachedTo( meObject *object ){
	if( object == pAttachedTo ){
		return;
	}
	
	if( pAttachedTo ){
		meObjectLink * const link = pAttachedTo->GetLinkTo( this );
		if( link ){
			pAttachedTo->RemoveLink( link );
		}
		
		pAttachedTo->GetAttachedObjectsList().Remove( this );
		pAttachedTo->FreeReference();
	}
	
	meObject * const oldObject = pAttachedTo;
	pAttachedTo = object;
	
	if( object ){
		object->AddReference();
		object->GetAttachedObjectsList().Add( this );
	}
	
	if( pWorld ){
		pWorld->SetChanged( true );
	}
	
	if( oldObject ){
		oldObject->CheckLinks();
	}
	if( object ){
		object->CheckLinks();
	}
}

void meObject::SetAttachedToID( const char *id ){
	pAttachedToID = id;
}



void meObject::UpdateNavPathTest(){
	bool affectsPath = false;
	
	if( pClassDef && pWorld ){
		affectsPath |= meHelpers::FindFirstNavigationSpace( *pClassDef )
			|| meHelpers::FindFirstNavigationBlocker( *pClassDef );
	}
	
	if( affectsPath ){
		pWorld->GetPathFindTest()->Invalidate();
	}
}

decDMatrix meObject::GetObjectMatrix(){
	return decDMatrix::CreateSRT( pScaling, pRotation * DEG2RAD, pPosition );
}

decDMatrix meObject::GetInverseObjectMatrix(){
	return GetObjectMatrix().QuickInvert();
}



void meObject::DecrementIDGroupIDUsage(){
	if( ! pClassDef ){
		return;
	}
	
	// object properties
	const int mapCount = pMapIDGroup.GetCount();
	int i;
	
	for( i=0; i<mapCount; i++ ){
		const meMapIDGroup &map = *( ( meMapIDGroup* )pMapIDGroup.GetAt( i ) );
		const decString name( map.GetPropertyPrefix() + map.GetProperty()->GetName() );
		
		if( ! pProperties.Has( name ) ){
			continue;
		}
		
		const decString &value = pProperties.GetAt( name );
		if( value.IsEmpty() ){
			continue; // ignore empty identifier
		}
		
		//printf( "Object %p Class '%s' IDGroup %s Remove '%s' (%i)\n", this, pClassDef->GetName().GetString(),
		//	map.GetGroup()->GetName().GetString(), value, map.GetGroup()->GetUsageCountFor( value ) );
		map.GetGroup()->Remove( value );
	}
	
	// texture properties
	const int texMapCount = pTexMapIDGroup.GetCount();
	int j;
	
	for( i=0; i<texMapCount; i++ ){
		const meMapIDGroup &map = *( ( meMapIDGroup* )pTexMapIDGroup.GetAt( i ) );
		const decString name( map.GetPropertyPrefix() + map.GetProperty()->GetName() );
		
		for( j=0; j<pTextureCount; j++ ){
			const meObjectTexture &texture = *pTextures[ j ];
			const decStringDictionary properties = texture.GetProperties();
			const decString *value;
			
			if( ! properties.GetAt( name, &value ) ){
				continue;
			}
			
			if( value->IsEmpty() ){
				continue; // ignore empty identifier
			}
			
			//printf( "Object %p Class '%s' Texture '%s' IDGroup %s Remove '%s' (%i)\n", this,
			//	pClassDef->GetName().GetString(), texture.GetName().GetString(),
			//	map.GetGroup()->GetName().GetString(), value->GetString(),
			//	map.GetGroup()->GetUsageCountFor( value->GetString() ) );
			map.GetGroup()->Remove( value->GetString() );
		}
	}
}

void meObject::IncrementIDGroupIDUsage(){
	if( ! pClassDef ){
		return;
	}
	
	// object properties
	const int mapCount = pMapIDGroup.GetCount();
	int i;
	
	for( i=0; i<mapCount; i++ ){
		const meMapIDGroup &map = *( ( meMapIDGroup* )pMapIDGroup.GetAt( i ) );
		const decString name( map.GetPropertyPrefix() + map.GetProperty()->GetName() );
		const decString *value = nullptr;
		
		if( ! pProperties.GetAt( name, &value ) ){
			continue;
		}
		
		if( value->IsEmpty() ){
			continue; // ignore empty identifier
		}
		
		map.GetGroup()->Add( value->GetString() );
		// printf( "Object %p Class '%s' IDGroup %s Add '%s' (%i)\n", this, pClassDef->GetName().GetString(),
			// map.GetGroup()->GetName().GetString(), value->GetString(), map.GetGroup()->GetUsageCountFor( *value ) );
	}
	
	// texture properties
	const int texMapCount = pTexMapIDGroup.GetCount();
	int j;
	
	for( i=0; i<texMapCount; i++ ){
		const meMapIDGroup &map = *( ( meMapIDGroup* )pTexMapIDGroup.GetAt( i ) );
		const decString name( map.GetPropertyPrefix() + map.GetProperty()->GetName() );
		
		for( j=0; j<pTextureCount; j++ ){
			const meObjectTexture &texture = *pTextures[ j ];
			const decStringDictionary properties = texture.GetProperties();
			const decString *value;
			
			if( ! properties.GetAt( name, &value ) ){
				continue;
			}
			
			if( value->IsEmpty() ){
				continue; // ignore empty identifier
			}
			
			//printf( "Object %p Class '%s' Texture '%s' IDGroup %s Add '%s' (%i)\n", this,
			//	pClassDef->GetName().GetString(), texture.GetName().GetString(),
			//	map.GetGroup()->GetName().GetString(), value->GetString(),
			//	map.GetGroup()->GetUsageCountFor( value->GetString() ) );
			map.GetGroup()->Add( value->GetString() );
		}
	}
}

void meObject::UpdateIDGroupList(){
	pMapIDGroup.RemoveAll();
	pTexMapIDGroup.RemoveAll();
	
	if( pClassDef && pWorld ){
		pUpdateIDGroupList( *pClassDef, decString() );
	}
}



void meObject::ShowStateChanged(){
	if( pWorld ){
		const bool visible = pShowStateIsVisible();
		
		const meWorldGuiParameters &guiParams = pWorld->GetGuiParameters();
		const meWorldGuiParameters::eElementModes elementMode = guiParams.GetElementMode();
		const bool modeObjShape = elementMode == meWorldGuiParameters::eemObjectShape;
		const bool modeNavSpace = elementMode == meWorldGuiParameters::eemNavSpace;
		
		pDDSObject->SetVisible(visible);
		pDDSLightAoE->SetVisible(false); // visible && pLight
		pDDSOcclusionMesh->SetVisible(guiParams.GetShowOcclusionMeshes()
			|| (visible && guiParams.GetShowOcclusionMeshesSelected()));
		pDDSObjectShapes->SetVisible((modeObjShape && pSelected && pVisible)
			|| guiParams.GetShowShapes()
			|| (visible && guiParams.GetShowShapesSelected()));
		pDDSListNavSpaces.SetVisibleAll(modeNavSpace
			|| guiParams.GetShowNavigationSpaces()
			|| (visible && guiParams.GetShowNavigationSpacesSelected()));
		pDDSCoordSysArrows->SetVisible(visible);
	}
	
	int i;
	for( i=0; i<pDecalCount; i++ ){
		pDecals[ i ]->ShowStateChanged();
	}
}

void meObject::WOAsyncFinished(){
	// store WObject textures to properly apply missing texture
	pWOTextures.RemoveAll();
	
	const deComponent * const component = pWObject->GetComponent();
	if( component ){
		const int textureCount = component->GetTextureCount();
		int i;
		for( i=0; i<textureCount; i++ ){
			pWOTextures.Add( deObject::Ref::New( new cWOTexture( component->GetTextureAt( i ) ) ) );
		}
	}
	
	// continue updating
	pSizeFromScaling();
	pUpdateComponent();
	pUpdateShapes();
	pUpdateShapeOcclusionMesh();
	pUpdateDDSNavSpaces();
	pRepositionDDSNavSpaces();
	
	// update cut-off distance for display purpose
	class cWOSOLightVisitor : public igdeWOSOVisitor{
	public:
		float range;
		
		cWOSOLightVisitor() : range( 0.0f ){ }
		
		virtual void VisitLight( igdeWOSOLight &light ){
			if( ! light.GetLight() ){
				return;
			}
			// pGetCutOffDistFor( pLight->GetHalfIntensityDistance(), pLight->GetRange(), 0.01f );
			range = decMath::max( range, light.GetLight()->GetRange() );
		}
	} wosoLightVisitor;
	pWObject->VisitSubObjects( wosoLightVisitor );
	
	if( wosoLightVisitor.range > 0.0f ){
		if( fabsf( wosoLightVisitor.range - pRange ) > 0.001f ){
			pRange = wosoLightVisitor.range;
			pUpdateShapeLight();
			pUpdateDDSColors();
			pUpdateOutline();
		}
	}
	
	// comes last to ensure all visibilities are correct
	ShowStateChanged();
	
	if( pWorld ){
		meCLInvalidateDecals invalidateDecals(*pWorld);
		invalidateDecals.Collect(*pWObject);
		invalidateDecals.InvalidateDecals();
	}
}

void meObject::WOAnyContentVisibleChanged(){
	pUpdateBrokenComponent();
}

void meObject::WOExtendsChanged(){
	pUpdateShapes();
}



// Links
//////////

int meObject::GetLinkCount() const{
	return pLinks.GetCount();
}

meObjectLink *meObject::GetLinkAt( int index ) const{
	return ( meObjectLink* )pLinks.GetAt( index );
}

meObjectLink *meObject::GetLinkTo( meObject *target ) const{
	if( ! target ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		meObjectLink * const link = ( meObjectLink* )pLinks.GetAt( i );
		
		if( target == link->GetTarget() ){
			return link;
		}
	}
	
	return NULL;
}

bool meObject::HasLinkTo( meObject *target ) const{
	return GetLinkTo( target ) != NULL;
}

bool meObject::HasLink( meObjectLink *link ) const{
	return pLinks.Has( link );
}

int meObject::IndexOfLink( meObjectLink *link ) const{
	return pLinks.IndexOf( link );
}

void meObject::AddLink( meObjectLink *link ){
	pLinks.Add( link );
	
	if( pWorld ){
		link->SetWorld( pWorld );
		
	}else{
		link->SetWorld( NULL );
	}
}

void meObject::RemoveLink( meObjectLink *link ){
	const int index = pLinks.IndexOf( link );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	link->SetWorld( NULL );
	pLinks.RemoveFrom( index );
}

void meObject::RemoveAllLinks(){
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( meObjectLink* )pLinks.GetAt( i ) )->SetWorld( NULL );
	}
	pLinks.RemoveAll();
}

bool meObject::CanLinkTo( meObject *object ) const{
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
	
	// no link with yourself
	if( object == this ){
		return false;
	}
	
	// link due to attaching
	if( pAttachedTo == object ){
		return true;
	}
	
	// no link has been found
	return false;
}

void meObject::UpdateTriggerTargets(){
	if( pWorld ){
		pWorld->GetTriggerTable().RemoveUnused();
		pWorld->NotifyTriggerTableChanged();
	}
}

void meObject::CheckLinks(){
	int i;
	
	// if we are not active or there is no game definition class remove all links
	if( ! pActive || ! pClassDef ){
		for( i=0; i<pLinks.GetCount(); i++ ){
			meObjectLink * const link = ( meObjectLink* )pLinks.GetAt( i );
			meObject *object = nullptr;
			
			if( link->GetAnchor() == this ){
				object = link->GetTarget();
				
			}else{
				object = link->GetAnchor();
			}
			
			if( object->HasLink( link ) ){
				object->RemoveLink( link );
			}
		}
		
		RemoveAllLinks();
		return;
	}
	
	// look for links with other objects. currently we look only in the current world.
	if( ! pWorld ){
		return;
	}
	
	const meObjectList &objects = pWorld->GetObjects();
	const int objectCount = objects.GetCount();
	for( i=0; i<objectCount; i++ ){
		meObject * const object = objects.GetAt( i );
		if( object == this ){
			continue;
		}
		
		// if we are the anchor make sure the link starts with us
		const bool isAnchor = CanLinkTo( object );
		
		if( isAnchor || object->CanLinkTo( this ) ){
			meObjectLink *link = GetLinkTo( object );
			
			if( link ){
				if( ! object->HasLink( link ) ){
					object->AddLink( link );
				}
				
			}else{
				link = object->GetLinkTo( this );
				
				if( link ){
					if( ! HasLink( link ) ){
						AddLink( link );
					}
					
				}else{
					link = nullptr;
					try{
						// create the link in the proper direction
						if( isAnchor ){
							link = new meObjectLink( pEnvironment, this, object );
							
						}else{
							link = new meObjectLink( pEnvironment, object, this );
						}
						if( ! link ){
							DETHROW( deeOutOfMemory );
						}
						
						// add the link and give up the reference we hold
						AddLink( link );
						object->AddLink( link );
						link->FreeReference();
						
					}catch( const deException & ){
						if( link ){
							link->FreeReference();
						}
						throw;
					}
				}
			}
			
		// otherwise make sure there is no link shown
		}else{
			meObjectLink * const link = GetLinkTo( object );
			
			if( link ){
				if( object->HasLink( link ) ){
					object->RemoveLink( link );
				}
				
				RemoveLink( link ); // remove last otherwise the link could be dangling all of a sudden
			}
		}
	}
}



void meObject::Update( float elapsed ){
	pWObject->Update( elapsed );
}



// Textures
/////////////

meObjectTexture *meObject::GetTextureAt( int index ) const{
	if( index < 0 || index >= pTextureCount ) DETHROW( deeInvalidParam );
	
	return pTextures[ index ];
}

meObjectTexture *meObject::GetTextureNamed( const char *name ) const{
	int i;
	
	for( i=0; i<pTextureCount; i++ ){
		if( pTextures[ i ]->GetName() == name ){
			return pTextures[ i ];
		}
	}
	
	return NULL;
}

bool meObject::HasTextureNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pTextureCount; i++ ){
		if( strcmp( name, pTextures[ i ]->GetName() ) == 0 ){
			return true;
		}
	}
	
	return false;
}

int meObject::IndexOfTexture( meObjectTexture *texture ) const{
	if( ! texture ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pTextureCount; i++ ){
		if( texture == pTextures[ i ] ) return i;
	}
	
	return -1;
}

int meObject::IndexOfTextureNamed( const char *name ) const{
	if( ! name ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pTextureCount; i++ ){
		if( strcmp( name, pTextures[ i ]->GetName() ) == 0 ){
			return i;
		}
	}
	
	return -1;
}

bool meObject::HasTexture( meObjectTexture *texture ) const{
	if( ! texture ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pTextureCount; i++ ){
		if( texture == pTextures[ i ] ) return true;
	}
	
	return false;
}

void meObject::AddTexture( meObjectTexture *texture ){
	if( ! texture || HasTextureNamed( texture->GetName() ) ) DETHROW( deeInvalidParam );
	
	if( pTextureCount == pTextureSize ){
		int newSize = pTextureCount * 3 / 2 + 1;
		meObjectTexture **newArray = new meObjectTexture*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pTextures ){
			memcpy( newArray, pTextures, sizeof( meObjectTexture* ) * pTextureSize );
			delete [] pTextures;
		}
		pTextures = newArray;
		pTextureSize = newSize;
	}
	
	pTextures[ pTextureCount ] = texture;
	pTextureCount++;
	
	texture->AddReference();
	texture->SetObject( this );
	
	pUpdateComponent();
	
	if( pWorld ){
		pWorld->NotifyObjectTextureCountChanged( this );
		pWorld->SetChanged( true );
	}
}

void meObject::RemoveTexture( meObjectTexture *texture ){
	const int index = IndexOfTexture( texture );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( texture == pActiveTexture ){
		SetActiveTexture( NULL );
	}
	
	int i;
	for( i=index+1; i<pTextureCount; i++ ){
		pTextures[ i - 1 ] = pTextures[ i ];
	}
	pTextureCount--;
	
	texture->SetObject( NULL );
	texture->FreeReference();
	
	pWObject->ResetComponentTextures();
	pUpdateComponent();
	
	if( pWorld ){
		pWorld->NotifyObjectTextureCountChanged( this );
		pWorld->SetChanged( true );
	}
}

void meObject::RemoveAllTextures(){
	SetActiveTexture( NULL );
	
	while( pTextureCount > 0 ){
		pTextureCount--;
		
		pTextures[ pTextureCount ]->SetObject( NULL );
		pTextures[ pTextureCount ]->FreeReference();
	}
	
	// this is a problem here. on clean up it makes no sense to recreate textures in the component.
	// we need a way to prevent any kind of updates during clean up which are no more needed at that time
	pWObject->ResetComponentTextures();
	UpdateComponentTextures();
	
	if( pWorld ){
		pWorld->NotifyObjectTextureCountChanged( this );
		pWorld->SetChanged( true );
	}
}

void meObject::SetActiveTexture( meObjectTexture *texture ){
	if( texture != pActiveTexture ){
		pActiveTexture = texture;
		
		if( pWorld ){
			pWorld->NotifyObjectActiveTextureChanged( this );
		}
	}
}

void meObject::UpdateComponentTextures(){
	deComponent * const component = pWObject->GetComponent();
	if( ! component || ! component->GetModel() ){
		return;
	}
	
	deModel * const engModel = component->GetModel();
	deSkin * const engSkin = component->GetSkin();
	const int textureCount = engModel->GetTextureCount();
	int i;
	
	// deSkin * const defaultSkin = pEnvironment->GetGameProject()->GetGameDefinition()->GetDefaultSkin();
	deSkin * const defaultSkin = pEnvironment->GetStockSkin( igdeEnvironment::essTestMap );
	const int defaultTexture = 0;
	
	for( i=0; i<textureCount; i++ ){
		deComponentTexture &componentTexture = component->GetTextureAt( i );
		const decString &textureName = engModel->GetTextureAt( i )->GetName();
		meObjectTexture * const texture = GetTextureNamed( textureName );
		
		const cWOTexture &wotexture = *( ( cWOTexture* )pWOTextures.GetAt( i ) );
		deSkin *useSkin = wotexture.skin;
		int useTexture = wotexture.texture;
		deDynamicSkin *useDynamicSkin = wotexture.dynamicSkin;
		decTexMatrix2 texCoordTransform = wotexture.texCoordTransform;
		
		if( texture ){
			if( texture->GetEngineSkin() ){
				useSkin = texture->GetEngineSkin();
				useTexture = 0;
			}
			
			decVector2 texCoordOffset = texture->GetTexCoordOffset();
			decVector2 texCoordScale = texture->GetTexCoordScaling();
			float texCoordRotation = texture->GetTexCoordRotation();
			
			if( texCoordScale.x == 0.0f ){
				texCoordScale.x = 1.0f;
			}
			if( texCoordScale.y == 0.0f ){
				texCoordScale.y = 1.0f;
			}
			texCoordTransform = decTexMatrix2::CreateScale( texCoordScale ) *
				decTexMatrix2::CreateRotation( texCoordRotation * DEG2RAD ) *
				decTexMatrix2::CreateTranslation( texCoordOffset );
			
			useDynamicSkin = texture->GetDynamicSkin();
		}
		
		if( ! useSkin && pShowMissingTextures && ( ! engSkin || engSkin->IndexOfTextureNamed( textureName ) == -1 ) ){
			useSkin = defaultSkin;
			useTexture = defaultTexture;
		}
		
		if( useSkin != componentTexture.GetSkin()
		|| useTexture != componentTexture.GetTexture()
		|| useDynamicSkin != componentTexture.GetDynamicSkin()
		|| ! texCoordTransform.IsEqualTo( componentTexture.GetTransform() ) ){
			componentTexture.SetSkin( useSkin );
			componentTexture.SetTexture( useTexture );
			componentTexture.SetTransform( texCoordTransform );
			componentTexture.SetDynamicSkin( useDynamicSkin );
			component->NotifyTextureChanged( i );
		}
	}
}

bool meObject::IsComponentBroken() const{
	return !pWObject->IsAnyContentVisible();
#if 0
	// if the component is missing it is broken
	const deComponent * const component = pWObject->GetComponent();
	if( ! component ){
		return false;
	}
	
	// if the model is missing the component is broken
	const deModel * const engModel = component->GetModel();
	if( ! engModel ){
		return false;
	}
	
	// if the component has no textures it is broken
	const int textureCount = engModel->GetTextureCount();
	if( textureCount == 0 ){
		return false;
	}
	
	
	// if the component is set to show invalid textures the component is not broken
	if( pShowMissingTextures ){
		return false;
	}
	
	// find a texture that is mapped and visible. if found the component is not broken
	const deSkin * const engSkin = component->GetSkin();
	int i;
	
	for( i=0; i<textureCount; i++ ){
		const deComponentTexture &componentTexture = component->GetTextureAt( i );
		const deSkin * const engCompTexSkin = componentTexture.GetSkin();
		
		// check if the component texture is overwritten with a valid texture
		if( engCompTexSkin ){
			const int compTexTex = componentTexture.GetTexture();
			if( compTexTex >= 0 && compTexTex < engCompTexSkin->GetTextureCount() ){
				// texture has a skin assigned and the texture number is inside valid boundaries.
				// the user can see at least this texture
				return false;
			}
		}
		
		// check if the component texture matches the component skin
		if( engSkin && engSkin->IndexOfTextureNamed( engModel->GetTextureAt( i )->GetName() ) != -1 ){
			return false;
		}
	}
	
	// no component texture is visible to the user. the component is broken
	return true;
#endif
}

void meObject::GetTextureNameList( decStringList &list ) const{
	int i;
	
	list.RemoveAll();
	
	for( i=0; i<pTextureCount; i++ ){
		list.Add( pTextures[ i ]->GetName() );
	}
}

void meObject::GetModelTextureNameList( decStringList &list ) const{
	const deComponent * const component = pWObject->GetComponent();
	
	list.RemoveAll();
	
	if( component && component->GetModel() ){
		const deModel &engModel = *component->GetModel();
		const int textureCount = engModel.GetTextureCount();
		int i;
		
		for( i=0; i<textureCount; i++ ){
			list.Add( engModel.GetTextureAt( i )->GetName() );
		}
	}
}



// Properties
///////////////

void meObject::SetProperty( const char *key, const char *value ){
	const decString *checkString = nullptr;
	if( pProperties.GetAt( key, &checkString ) && *checkString == value ){
		return;
	}
	
	bool activeChanged = false;
	
	DecrementIDGroupIDUsage();
	
	pProperties.SetAt( key, value );
	
	if( pActiveProperty.IsEmpty() ){
		pActiveProperty = key;
		activeChanged = true;
	}
	
	IncrementIDGroupIDUsage();
	pUpdateProperties();
	pUpdateCamera();
	
	if( pClassDef ){
		if( pClassDef->HasNavSpaceLinkedProperty( key ) ){
			pUpdateDDSNavSpaces();
		}
		if( pClassDef->HasNavSpaceLinkedProperty( key )
		|| pClassDef->HasNavBlockerLinkedProperty( key ) ){
			UpdateNavPathTest();
		}
	}
	
	if( pWorld ){
		pWorld->SetChanged( true );
		pWorld->NotifyObjectPropertiesChanged( this );
		
		if( activeChanged ){
			pWorld->NotifyObjectActivePropertyChanged( this );
		}
	}
}

void meObject::SetProperties( const decStringDictionary &properties ){
	DecrementIDGroupIDUsage();
	
	pProperties = properties;
	
	if( pProperties.GetCount() == 0 ){
		pActiveProperty = "";
		
	}else{
		pActiveProperty = pProperties.GetKeys().GetAt( 0 );
	}
	
	IncrementIDGroupIDUsage();
	pUpdateProperties();
	pUpdateDDSNavSpaces();
	UpdateNavPathTest();
	pUpdateCamera();
	
	if( pWorld ){
		pWorld->SetChanged( true );
		pWorld->NotifyObjectPropertiesChanged( this );
		pWorld->NotifyObjectActivePropertyChanged( this );
	}
}

void meObject::RemoveProperty( const char *key ){
	if( ! pProperties.Has( key ) ){
		return;
	}
	
	bool activeChanged = false;
	
	DecrementIDGroupIDUsage();
	
	pProperties.Remove( key );
	
	if( pActiveProperty == key ){
		if( pProperties.GetCount() == 0 ){
			pActiveProperty = "";
			
		}else{
			pActiveProperty = pProperties.GetKeys().GetAt( 0 );
		}
		activeChanged = true;
	}
	
	IncrementIDGroupIDUsage();
	pUpdateProperties();
	pUpdateCamera();
	
	if( pClassDef ){
		if( pClassDef->HasNavSpaceLinkedProperty( key ) ){
			pUpdateDDSNavSpaces();
		}
		if( pClassDef->HasNavSpaceLinkedProperty( key )
		|| pClassDef->HasNavBlockerLinkedProperty( key ) ){
			UpdateNavPathTest();
		}
	}
	
	if( pWorld ){
		pWorld->SetChanged( true );
		pWorld->NotifyObjectPropertiesChanged( this );
		
		if( activeChanged ){
			pWorld->NotifyObjectActivePropertyChanged( this );
		}
	}
}

void meObject::RemoveAllProperties(){
	if( pProperties.GetCount() == 0 ){
		return;
	}
	
	DecrementIDGroupIDUsage();
	
	pProperties.RemoveAll();
	pActiveProperty = "";
	
	pUpdateProperties();
	pUpdateDDSNavSpaces();
	UpdateNavPathTest();
	pUpdateCamera();
	
	if( pWorld ){
		pWorld->SetChanged( true );
		pWorld->NotifyObjectPropertiesChanged( this );
		pWorld->NotifyObjectActivePropertyChanged( this );
	}
}

void meObject::SetActiveProperty( const char *property ){
	if( pActiveProperty == property ){
		return;
	}
	
	pActiveProperty = property;
	
	UpdateDDSObjectShapes();
	
	if( pWorld ){
		pWorld->NotifyObjectActivePropertyChanged( this );
	}
}



bool meObject::IsPropertyShape( const char *property ) const{
	if( ! property ){
		DETHROW( deeInvalidParam );
	}
	
	const igdeGDProperty * const gdproperty = pClassDef ? pClassDef->GetPropertyNamed( property ) : NULL;
	if( gdproperty ){
		return gdproperty->GetType() == igdeGDProperty::eptShape;
	}
	
	return false;
}

bool meObject::IsPropertyShapeList( const char *property ) const{
	if( ! property ){
		DETHROW( deeInvalidParam );
	}
	
	const igdeGDProperty * const gdproperty = pClassDef ? pClassDef->GetPropertyNamed( property ) : NULL;
	if( gdproperty ){
		return gdproperty->GetType() == igdeGDProperty::eptShapeList;
	}
	
	return false;
}

bool meObject::IsPropertyShapeOrShapeList( const char *property ) const{
	if( ! property ){
		DETHROW( deeInvalidParam );
	}
	
	const igdeGDProperty * const gdproperty = pClassDef ? pClassDef->GetPropertyNamed( property ) : NULL;
	if( gdproperty ){
		const int propertyType = gdproperty->GetType();
		return propertyType == igdeGDProperty::eptShapeList || propertyType == igdeGDProperty::eptShape;
	}
	
	return false;
}



// Decals
///////////

meDecal *meObject::GetDecalAt(int index) const{
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index < pDecalCount)
	
	return pDecals[index];
}

int meObject::IndexOfDecal(meDecal *decal) const{
	if(!decal){
		return -1;
	}
	
	int i;
	for(i=0; i<pDecalCount; i++){
		if(decal == pDecals[i]){
			return i;
		}
	}
	return -1;
}

bool meObject::HasDecal(meDecal *decal) const{
	if(!decal){
		return false;
	}
	
	int i;
	for(i=0; i<pDecalCount; i++){
		if(decal == pDecals[i]){
			return true;
		}
	}
	return false;
}

void meObject::AddDecal( meDecal *decal ){
	DEASSERT_FALSE(HasDecal(decal))
	
	if( pDecalCount == pDecalSize ){
		int newSize = pDecalCount * 3 / 2 + 1;
		meDecal **newArray = new meDecal*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pDecals ){
			memcpy( newArray, pDecals, sizeof( meDecal* ) * pDecalSize );
			delete [] pDecals;
		}
		pDecals = newArray;
		pDecalSize = newSize;
	}
	
	pDecals[ pDecalCount ] = decal;
	pDecalCount++;
	
	decal->AddReference();
	decal->SetParentObject( this );
	decal->SetWorld( pWorld );
	
	if( pWorld ) pWorld->SetChanged( true );
}

void meObject::InsertDecalAt( meDecal *decal, int index ){
	DEASSERT_FALSE(HasDecal(decal))
	DEASSERT_TRUE(index >= 0)
	DEASSERT_TRUE(index <= pDecalCount)
	
	int i;
	for( i=pDecalCount-1; i>index; i-- ){
		pDecals[ i ] = pDecals[ i - 1 ];
	}
	
	pDecals[ index ] = decal;
	pDecalCount++;
	
	decal->AddReference();
	decal->SetParentObject( this );
	decal->SetWorld( pWorld );
	
	if( pWorld ) pWorld->SetChanged( true );
}

void meObject::RemoveDecal( meDecal *decal ){
	int i, index = IndexOfDecal( decal );
	DEASSERT_TRUE(index != -1)
	
	for( i=index+1; i<pDecalCount; i++ ){
		pDecals[ i - 1 ] = pDecals[ i ];
	}
	pDecalCount--;
	
	decal->SetWorld( NULL );
	decal->SetParentObject( NULL );
	decal->FreeReference();
	
	if( pWorld ) pWorld->SetChanged( true );
}

void meObject::RemoveAllDecals(){
	while( pDecalCount > 0 ){
		pDecalCount--;
		
		pDecals[ pDecalCount ]->SetWorld( NULL );
		pDecals[ pDecalCount ]->SetParentObject( NULL );
		pDecals[ pDecalCount ]->FreeReference();
	}
	
	if( pWorld ) pWorld->SetChanged( true );
}

void meObject::MoveDecalTo( meDecal *decal, int index ){
	if( index < 0 || index > pDecalCount ) DETHROW( deeInvalidParam );
	int i, oldIndex = IndexOfDecal( decal );
	if( oldIndex == -1 ) DETHROW( deeInvalidParam );
	
	if( index < oldIndex ){
		for( i=oldIndex; i>index; i-- ){
			pDecals[ i ] = pDecals[ i - 1 ];
		}
		pDecals[ index ] = decal;
		
		if( pWorld ) pWorld->SetChanged( true );
		
	}else if( index > oldIndex ){
		for( i=oldIndex; i<index; i++ ){
			pDecals[ i ] = pDecals[ i + 1 ];
		}
		pDecals[ index ] = decal;
		
		if( pWorld ) pWorld->SetChanged( true );
	}
}



// Collision Detection
////////////////////////

void meObject::FindDecalsAt( const decVector &point, deDecalList *list ){
	const deComponent * const component = pWObject->GetComponent();
	
	if( component ){
		component->FindDecalsAt( point, *list );
	}
}

void meObject::FindDecalsTouching( decShape *shape, deDecalList *list ){
	const deComponent * const component = pWObject->GetComponent();
	
	if( component ){
		component->FindDecalsTouching( shape, *list );
	}
}



// Private Functions
//////////////////////

void meObject::pCleanUp(){
	SetWorld( NULL );
	
	pRemoveAllSnapPoints();
	
	RemoveAllDecals();
	if( pDecals ){
		delete [] pDecals;
	}
	
	RemoveAllLinks();
	
	RemoveAllTextures();
	if( pTextures ){
		delete [] pTextures;
	}
	
	if( pColDetCollider ){
		pColDetCollider->FreeReference();
	}
	if( pEngComponentBroken ){
		pEngComponentBroken->FreeReference();
	}
	if( pWObject ){
		delete pWObject;
	}
	
	if( pCamera ){
		delete pCamera;
	}
	
	if( pDDSCoordSysArrows ){
		delete pDDSCoordSysArrows;
	}
	if( pDDSObjectShapes ){
		delete pDDSObjectShapes;
	}
	if( pDDSOcclusionMesh ){
		delete pDDSOcclusionMesh;
	}
	if( pDDSLightAoE ){
		delete pDDSLightAoE;
	}
	if( pDDSObject ){
		delete pDDSObject;
	}
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
}

void meObject::pUpdateDDSColors(){
	if( pActive ){
		pDDSObject->SetEdgeColor( decColor( 1.0f, 0.5f, 0.0f, 1.0 ) );
		pDDSOcclusionMesh->SetEdgeColor( decColor( 0.75f, 0.75f, 0.0f, 1.0f ) );
		pDDSOcclusionMesh->SetFillColor( decColor( 0.75f, 0.75f, 0.0f, 0.01f ) );
		
	}else if( pSelected ){
		pDDSObject->SetEdgeColor( decColor( 1.0f, 0.0f, 0.0f, 1.0 ) );
		pDDSOcclusionMesh->SetEdgeColor( decColor( 0.75f, 0.5f, 0.0f, 1.0f ) );
		pDDSOcclusionMesh->SetFillColor( decColor( 0.75f, 0.5f, 0.0f, 0.01f ) );
		
	}else{
		pDDSOcclusionMesh->SetEdgeColor( decColor( 0.5f, 0.5f, 0.0f, 1.0f ) );
		pDDSOcclusionMesh->SetFillColor( decColor( 0.5f, 0.5f, 0.0f, 0.01f ) );
		/*pDDSObject->SetEdgeColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
		pDDSLightAoE->SetEdgeColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
		pDDSLightAoE->SetFillColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );*/
	}
	
	pDDSObject->SetFillColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
	
	pDDSLightAoE->SetEdgeColor( decColor( 0.75f, 0.75f, 0.0f, 1.0 ) );
	pDDSLightAoE->SetFillColor( decColor( 1.0f, 1.0f, 0.0f, 0.01f ) );
	
	ShowStateChanged();
}

void meObject::pUpdateDDSNavSpaces(){
	if( ! pWObject ){
		pDDSListNavSpaces.RemoveAll();
		return;
	}
	
	bool requiresReposition;
	{
	class cWOSONavSpaceVisitor : public igdeWOSOVisitor{
	public:
		deDebugDrawer * const debugDrawer;
		igdeWDebugDrawerShapeList &ddShapes;
		int count;
		bool requiresReposition;
		
		cWOSONavSpaceVisitor( deDebugDrawer *ddebugDrawer, igdeWDebugDrawerShapeList &dddShapes ) :
		debugDrawer( ddebugDrawer ), ddShapes( dddShapes ), count( 0 ), requiresReposition( false ){
		}
		
		virtual ~cWOSONavSpaceVisitor(){
			while( ddShapes.GetCount() > count ){
				ddShapes.Remove( ddShapes.GetAt( ddShapes.GetCount() - 1 ) );
			}
		}
		
		virtual void VisitNavigationSpace( igdeWOSONavigationSpace &navigationSpace ){
			if( ! navigationSpace.GetNavigationSpace() ){
				return;
			}
			
			// navigation space
			igdeWDebugDrawerShape *ddshape = nullptr;
			
			if( count < ddShapes.GetCount() ){
				ddshape = ddShapes.GetAt( count );
				
			}else{
				try{
					ddshape = new igdeWDebugDrawerShape;
					ddshape->SetParentDebugDrawer( debugDrawer );
					ddshape->SetEdgeColor( decColor( 0.5f, 0.5f, 0.5f, 1.0f ) );
					ddshape->SetFillColor( decColor( 0.5f, 0.5f, 0.5f, 0.1f ) );
					ddShapes.Add( ddshape );
					requiresReposition = true;
					
				}catch( const deException & ){
					if( ddshape ){
						delete ddshape;
					}
					throw;
				}
			}
			
			count++;
			ddshape->RemoveAllFaces();
			ddshape->RemoveAllShapes();
			ddshape->AddNavSpaceFaces( *navigationSpace.GetNavigationSpace() );
			
			// navigation blocker
			if( navigationSpace.GetNavigationSpace()->GetBlockerShapeList().GetCount() > 0 ){
				Blocker( navigationSpace.GetNavigationSpace()->GetBlockerShapeList() );
			}
		}
		
		virtual void VisitNavigationBlocker( igdeWOSONavigationBlocker &navigationBlocker ){
			if( navigationBlocker.GetNavigationBlocker()
			&& navigationBlocker.GetNavigationBlocker()->GetShapeList().GetCount() > 0 ){
				Blocker( navigationBlocker.GetNavigationBlocker()->GetShapeList() );
			}
		}
		
		void Blocker( const decShapeList &shapes ){
			igdeWDebugDrawerShape *ddshape = nullptr;
			
			if( count < ddShapes.GetCount() ){
				ddshape = ddShapes.GetAt( count );
				
			}else{
				try{
					ddshape = new igdeWDebugDrawerShape;
					ddshape->SetParentDebugDrawer( debugDrawer );
					ddshape->SetEdgeColor( decColor( 0.25f, 0.25f, 0.35f, 1.0f ) );
					ddshape->SetFillColor( decColor( 0.25f, 0.25f, 0.35f, 0.1f ) );
					ddShapes.Add( ddshape );
					requiresReposition = true;
					
				}catch( const deException & ){
					if( ddshape ){
						delete ddshape;
					}
					throw;
				}
			}
			
			count++;
			ddshape->RemoveAllFaces();
			ddshape->RemoveAllShapes();
			ddshape->AddShapes( shapes );
		}
	} wosoNavSpaceVisitor( pDebugDrawer, pDDSListNavSpaces );
	pWObject->VisitSubObjects( wosoNavSpaceVisitor );
	requiresReposition = wosoNavSpaceVisitor.requiresReposition;
	}
	
	if( requiresReposition ){
		pRepositionDDSNavSpaces();
	}
}

void meObject::pUpdateDDSCoordSysArrowsLength(){
	const float objSize = decMath::max( decMath::max( pSize.x, pSize.y ), pSize.z );
	pDDSCoordSysArrows->SetArrowLength( decMath::min( objSize * 0.25f, 1.0f ) );
	pDDSCoordSysArrows->SetArrowSize( pDDSCoordSysArrows->GetArrowLength() * 0.05f );
}

void meObject::pRepositionDDSNavSpaces(){
	if(!pWObject || !pClassDef){
		return;
	}
	
	const igdeGDCNavigationSpaceList &gdcNavSpaceList = pClassDef->GetNavigationSpaceList();
	const int count = pDDSListNavSpaces.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( i >= gdcNavSpaceList.GetCount() ){
			break;
		}
		
		igdeWDebugDrawerShape &ddshape = *pDDSListNavSpaces.GetAt( i );
		
		if( pClassDef ){
			const igdeGDCNavigationSpace &gdNavSpace = *gdcNavSpaceList.GetAt( i );
			ddshape.SetPosition( gdNavSpace.GetPosition() );
			ddshape.SetOrientation( gdNavSpace.GetOrientation() );
			ddshape.SetScale( pScaling );
			
		}else{
			ddshape.SetPosition( decVector() );
			ddshape.SetOrientation( decQuaternion() );
			ddshape.SetScale( decVector( 1.0f, 1.0f, 1.0f ) );
		}
	}
}

void meObject::pUpdateOutline(){
	if( pActive ){
		pWObject->SetOutlineSkin( pEnvironment->GetStockSkin( igdeEnvironment::essEditOutline ) );
		pWObject->SetOutlineColor( decColor( 1.0f, 0.5f, 0.0f ) );
		
	}else if( pSelected ){
		pWObject->SetOutlineSkin( pEnvironment->GetStockSkin( igdeEnvironment::essEditOutline ) );
		pWObject->SetOutlineColor( decColor( 1.0f, 0.0f, 0.0f ) );
		
	}else{
		pWObject->SetOutlineSkin( nullptr );
	}
}

void meObject::pUpdateShapes(){
	decVector baseMinExtend, baseMaxExtend;
	if( pWObject->GetHasBoxExtends() ){
		baseMinExtend = pWObject->GetBoxMinExtend();
		baseMaxExtend = pWObject->GetBoxMaxExtend();
		
	}else{
		baseMinExtend.Set( -0.5f, -0.5f, -0.5f );
		baseMaxExtend.Set( 0.5f, 0.5f, 0.5f );
	}
	
	if( pClassDef && pClassDef->GetScaleMode() != igdeGDClass::esmFixed ){
		baseMinExtend.x *= pScaling.x;
		baseMinExtend.y *= pScaling.y;
		baseMinExtend.z *= pScaling.z;
		
		baseMaxExtend.x *= pScaling.x;
		baseMaxExtend.y *= pScaling.y;
		baseMaxExtend.z *= pScaling.z;
	}
	
	const decVector position( ( baseMinExtend + baseMaxExtend ) * 0.5f );
	const decVector halfExtends( ( baseMaxExtend - baseMinExtend ) * 0.5f );
	
	// update debug drawer shape
	pDDSObject->RemoveAllShapes();
	pDDSObject->AddBoxShape( halfExtends, position, decQuaternion() );
	
	pUpdateShapeLight();
	pUpdateShapeOcclusionMesh();
	
	// update collider shape
	if( pColDetCollider ){
		decShapeBox *box = nullptr;
		decShapeList shapeList;
		
		try{
			box = new decShapeBox( halfExtends, position );
			shapeList.Add( box );
			box = nullptr;
			
		}catch( const deException & ){
			if( box ) delete box;
			throw;
		}
		
		pColDetCollider->SetShapes( shapeList );
	}
}

void meObject::pUpdateShapeLight(){
	class cWOSOLightVisitor : public igdeWOSOVisitor{
		igdeWDebugDrawerShape &shape;
		
	public:
		cWOSOLightVisitor( igdeWDebugDrawerShape &sshape ) : shape( sshape ){
			shape.RemoveAllShapes();
			shape.RemoveAllFaces();
		}
		
		virtual void VisitLight( igdeWOSOLight &wosoLight ){
			if( ! wosoLight.GetLight() ){
				return;
			}
			
			const deLight &light = *wosoLight.GetLight();
			const float ratio = light.GetSpotRatio();
			const float angleX = light.GetSpotAngle();
			const float angleY = light.GetSpotAngle() * ratio;
			const float range = light.GetRange();
			
			switch( light.GetType() ){
			case deLight::eltPoint:
				shape.AddSphereShape( range, light.GetPosition() );
				break;
				
			case deLight::eltSpot:{
				const decMatrix matrix( decMatrix::CreateWorld(
					light.GetPosition(), light.GetOrientation() ) );
				
				shape.AddCylinderShape( range * 0.5f, 0.0f, range * tanf( angleX * 0.5f ),
					decVector2( 1.0f, ratio ), decVector2( 1.0f, ratio ),
					matrix.Transform( 0.0f, 0.0f, range * 0.5f ),
					decQuaternion::CreateFromEulerX( DEG2RAD * 90.0f ) * light.GetOrientation() );
				}break;
				
			case deLight::eltProjector:{
				const decMatrix matrix( decMatrix::CreateWorld(
					light.GetPosition(), light.GetOrientation() ) );
				const float height = range * tanf( angleY * 0.5f );
				const float width = range * tanf( angleX * 0.5f );
				deDebugDrawerShapeFace *face = nullptr;
				
				try{
					face = new deDebugDrawerShapeFace;
					face->AddVertex( decVector() );
					face->AddVertex( matrix.Transform( -width, height, range ) );
					face->AddVertex( matrix.Transform( width, height, range ) );
					face->CalculateNormal();
					shape.AddFace( face );
					face = nullptr;
					
					face = new deDebugDrawerShapeFace;
					face->AddVertex( decVector() );
					face->AddVertex( matrix.Transform( width, height, range ) );
					face->AddVertex( matrix.Transform( width, -height, range ) );
					face->CalculateNormal();
					shape.AddFace( face );
					face = nullptr;
					
					face = new deDebugDrawerShapeFace;
					face->AddVertex( decVector() );
					face->AddVertex( matrix.Transform( width, -height, range ) );
					face->AddVertex( matrix.Transform( -width, -height, range ) );
					face->CalculateNormal();
					shape.AddFace( face );
					face = nullptr;
					
					face = new deDebugDrawerShapeFace;
					face->AddVertex( decVector() );
					face->AddVertex( matrix.Transform( -width, -height, range ) );
					face->AddVertex( matrix.Transform( -width, height, range ) );
					face->CalculateNormal();
					shape.AddFace( face );
					face = nullptr;
					
					face = new deDebugDrawerShapeFace;
					face->AddVertex( matrix.Transform( width, height, range ) );
					face->AddVertex( matrix.Transform( -width, height, range ) );
					face->AddVertex( matrix.Transform( -width, -height, range ) );
					face->AddVertex( matrix.Transform( width, -height, range ) );
					face->CalculateNormal();
					shape.AddFace( face );
					
				}catch( const deException & ){
					if( face ){
						delete face;
					}
					throw;
				}
				
				}break;
			}
		}
	} wosoLightVisitor( *pDDSLightAoE );
	pWObject->VisitSubObjects( wosoLightVisitor );
}

void meObject::pUpdateShapeOcclusionMesh(){
	const deComponent * const component = pWObject->GetComponent();
	
	pDDSOcclusionMesh->RemoveAllFaces();
	
	if( component && component->GetOcclusionMesh() ){
		pDDSOcclusionMesh->AddOcclusionMeshFaces( *component->GetOcclusionMesh() );
	}
}

void meObject::pUpdateComponent(){
	UpdateComponentTextures();
	pUpdateBrokenComponent();
}

void meObject::pUpdateBrokenComponent(){
	// if the component is broken show a broken component replacement
	if( IsComponentBroken() ){
		if( ! pEngComponentBroken ){
			const igdeGameDefinition &gamedef = *pEnvironment->GetGameProject()->GetGameDefinition();
			pEngComponentBroken = pEnvironment->GetEngineController()->GetEngine()->
				GetComponentManager()->CreateComponent( gamedef.GetDefaultModel(), gamedef.GetDefaultSkin() );
			pEngComponentBroken->SetPosition( pWObject->GetPosition() );
			pEngComponentBroken->SetOrientation( pWObject->GetOrientation() );
			
			if( pWorld ){
				pWorld->GetEngineWorld()->AddComponent( pEngComponentBroken );
			}
		}
		
		bool componentVisible = pWObject->GetVisible();
		const igdeGDCComponent * const gdccomponent = meHelpers::FindFirstComponent( pWObject->GetGDClass() );
		if( gdccomponent && gdccomponent->GetPartialHide() ){
			componentVisible = ! pWObject->GetPartiallyHidden();
		}
		pEngComponentBroken->SetVisible( componentVisible );
		
	}else{
		if( ! pEngComponentBroken ){
			return;
		}
		
		if( pWorld ){
			pWorld->GetEngineWorld()->RemoveComponent( pEngComponentBroken );
		}
		pEngComponentBroken->FreeReference();
		pEngComponentBroken = nullptr;
	}
}

void meObject::pUpdateProperties(){
	pWObject->SetPropertiesFrom( pProperties ); // can trigger WOAsyncFinished in the future
	
	UpdateTriggerTargets();
	pUpdateShapes();
	UpdateDDSObjectShapes();
	CheckLinks();
}



void meObject::pUpdateCamera(){
	const igdeGDCamera *gdCamera = nullptr;
	if(pClassDef){
		gdCamera = meHelpers::FindFirstCamera(*pClassDef);
	}
	
	if(gdCamera){
		if(!pCamera && pWorld){
			pCamera = new meCamera(GetEnvironment()->GetEngineController()->GetEngine());
			pCamera->SetEnableGI(pWorld->GetWindowMain().GetConfiguration().GetEnableGI());
			pCamera->SetHostObject(this);
			pCamera->SetWorld(pWorld);
		}
		
	}else if(pCamera){
		delete pCamera;
		pCamera = nullptr;
	}
	
	if(!pCamera || !gdCamera){
		return;
	}
	
	pCamera->SetFov(gdCamera->GetFov());
	pCamera->SetFovRatio(gdCamera->GetFovRatio());
	pCamera->SetImageDistance(gdCamera->GetImageDistance());
	pCamera->SetViewDistance(gdCamera->GetViewDistance());
	
	pCheckCameraProps();
	pRepositionCamera();
}

void meObject::pCheckCameraProps(){
	if(!pCamera){
		return;
	}
	
	igdeGDCamera *gdCamera = nullptr;
	decString prefix;
	if(pClassDef){
		meHelpers::FindFirstCamera(*pClassDef, prefix, gdCamera);
	}
	if(!gdCamera){
		return;
	}
	
	decString value, defaultValue;
	const decString *pvalue;
	bool found;
	
	// name property
	const decString propName(prefix + gdCamera->GetPropName());
	
	if(pProperties.Has(propName)){
		value.Format("%s: %s", pClassDef->GetName().GetString(), pProperties.GetAt(propName).GetString());
		
	}else if(pClassDef->GetDefaultPropertyValue(propName, defaultValue)){
		value.Format("%s: %s", pClassDef->GetName().GetString(), defaultValue.GetString());
		
	}else{
		value.Format("%s: <unnamed>", pClassDef->GetName().GetString());
	}
	
	pCamera->SetName(value);
	
	// position/rotation property
	const decString propPosition(prefix + gdCamera->GetPropPosition());
	const decString propRotation(prefix + gdCamera->GetPropRotation());
	
	decQuaternion orientation(gdCamera->GetOrientation());
	decDVector position(gdCamera->GetPosition());
	igdeCodecPropertyString codec;
	
	if(!propPosition.IsEmpty()){
		found = pProperties.GetAt(propPosition, &pvalue);
		if(!found){
			found = pClassDef->GetDefaultPropertyValue(propPosition, value);
			if(found){
				pvalue = &value;
			}
		}
		if(found){
			try{
				codec.DecodeDVector(*pvalue, position);
				
			}catch(const deException &){
				position = gdCamera->GetPosition();
			}
		}
	}
	
	if(!propRotation.IsEmpty()){
		found = pProperties.GetAt(propRotation, &pvalue);
		if(!found){
			found = pClassDef->GetDefaultPropertyValue(propRotation, value);
			if(found){
				pvalue = &value;
			}
		}
		if(found){
			try{
				decDVector rotation;
				codec.DecodeDVector(*pvalue, rotation);
				orientation.SetFromEuler(rotation * DEG2RAD);
				
			}catch(const deException &){
				orientation = gdCamera->GetOrientation();
			}
		}
	}
	
	pCamera->SetHostMatrix(decDMatrix::CreateWorld(position, orientation));
}

void meObject::pRepositionCamera(){
	if(!pCamera){
		return;
	}
	
	const decDMatrix matrix = pCamera->GetHostMatrix().
		QuickMultiply(decDMatrix::CreateRT(pRotation * DEG2RAD, pPosition));
	
	pCamera->SetPosition(matrix.GetPosition());
	pCamera->SetOrientation(matrix.GetEulerAngles().ToVector() * RAD2DEG);
}

void meObject::pRepositionLinks(){
	const int count = pLinks.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( meObjectLink* )pLinks.GetAt( i ) )->ObjectsMoved();
	}
}



void meObject::pRemoveAllSnapPoints(){
	if( pSnapPoints.GetCount() == 0 ){
		return;
	}
	
	pSnapPoints.RemoveAll();
	
	pDebugDrawer->NotifyShapeLayoutChanged();
}

void meObject::pCreateSnapPoints(){
	if( ! pClassDef ){
		return;
	}
	
	const igdeGDCSnapPointList &list = pClassDef->GetSnapPointList();
	const int count = list.GetCount();
	if( count == 0 ){
		return;
	}
	
	meObjectSnapPoint *snapPoint = nullptr;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			snapPoint = new meObjectSnapPoint( this, list.GetAt( i ) );
			snapPoint->SetWorld( pWorld );
			pSnapPoints.Add( snapPoint );
			snapPoint->FreeReference();
			snapPoint = nullptr;
		}
		
	}catch( const deException & ){
		if( snapPoint ){
			snapPoint->FreeReference();
		}
		throw;
	}
	
	pDebugDrawer->NotifyShapeLayoutChanged();
	pColDetCollider->AttachmentsForceUpdate();
}



void meObject::pScalingFromSize(){
	if( pClassDef && pClassDef->GetScaleMode() != igdeGDClass::esmFixed ){
		decVector boxSize( 1.0f, 1.0f, 1.0f );
		if( pWObject->GetHasBoxExtends() ){
			boxSize = pWObject->GetBoxMaxExtend() - pWObject->GetBoxMinExtend();
		}
		pScaling.x = pSize.x / boxSize.x;
		pScaling.y = pSize.y / boxSize.y;
		pScaling.z = pSize.z / boxSize.z;
		
	}else{
		pScaling.Set( 1.0f, 1.0f, 1.0f );
	}
	
	pWObject->SetScaling( pScaling );
}

void meObject::pSizeFromScaling(){
	decVector boxSize( 1.0f, 1.0f, 1.0f );
	if( pWObject->GetHasBoxExtends() ){
		boxSize = pWObject->GetBoxMaxExtend() - pWObject->GetBoxMinExtend();
	}
	pSize.x = boxSize.x * pScaling.x;
	pSize.y = boxSize.y * pScaling.y;
	pSize.z = boxSize.z * pScaling.z;
}

float meObject::pGetRangeFor( float halfIntDist, float attExp, float targetIntensity ) const{
	float cutOffDist = 1000.0f;
	
	if( attExp > 0.1f ){
		cutOffDist = powf( ( 1.0f / targetIntensity ) - 1.0f, 1.0f / attExp ) * halfIntDist;
	}
	
	if( cutOffDist > 1000.0f ){
		return 1000.0f;
	}
	
	return cutOffDist;
}



void meObject::pNotifyDecalsAboutChange(){
	int i;
	
	for( i=0; i<pDecalCount; i++ ){
		pDecals[ i ]->NotifyParentChanged();
	}
}

bool meObject::pAnyGDClassHasAnyPartialVisOf( const igdeGDClass &gdclass, const igdeTagManager &tags ) const{
	if( gdclass.GetPartialHideTags().HasAnyOf( tags ) ){
		return true;
	}
	
	const int count = gdclass.GetInheritClassCount();
	int i;
	for( i=0; i<count; i++ ){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt( i );
		if( inherit.GetClass() && pAnyGDClassHasAnyPartialVisOf( *inherit.GetClass(), tags ) ){
			return true;
		}
	}
	
	return false;
}

void meObject::pUpdateIDGroupList( const igdeGDClass &gdclass, const decString &prefix ){
	meIDGroupList &groupList = pWorld->GetIDGroupList();
	
	// object properties
	const igdeGDPropertyList &gdProperties = gdclass.GetListProperties();
	const int gdPropertyCount = gdProperties.GetCount();
	int i;
	
	for( i=0; i<gdPropertyCount; i++ ){
		igdeGDProperty &gdProperty = *gdProperties.GetAt( i );
		
		if( gdProperty.GetType() != igdeGDProperty::eptIdentifier
		|| gdProperty.GetIdentifierGroup().IsEmpty() || ! gdProperty.GetIdentifierUsage() ){
			continue;
		}
		
		// printf( "Object %p Class '%s' Add IDGroup '%s'\n", this, pClassDef->GetName().GetString(),
		// 	gdProperty.GetIdentifierGroup().GetString() );
		pMapIDGroup.Add( meMapIDGroup::Ref::New( new meMapIDGroup( &gdProperty,
			groupList.GetOrAddNamed( gdProperty.GetIdentifierGroup() ), prefix ) ) );
	}
	
	// texture properties
	const igdeGDPropertyList &gdTexProperties = gdclass.GetTextureProperties();
	const int gdTexPropertyCount = gdTexProperties.GetCount();
	
	for( i=0; i<gdTexPropertyCount; i++ ){
		igdeGDProperty &gdProperty = *gdTexProperties.GetAt( i );
		
		if( gdProperty.GetType() != igdeGDProperty::eptIdentifier
		|| gdProperty.GetIdentifierGroup().IsEmpty() || ! gdProperty.GetIdentifierUsage() ){
			continue;
		}
		
		//printf( "Object %p Class '%s' Texture Add IDGroup '%s'\n", this,
		//	pClassDef->GetName().GetString(), gdProperty->GetIdentifierGroup().GetString() );
		pTexMapIDGroup.Add( meMapIDGroup::Ref::New( new meMapIDGroup( &gdProperty,
			groupList.GetOrAddNamed( gdProperty.GetIdentifierGroup() ), prefix ) ) );
	}
	
	// inherits
	const int inheritCount = gdclass.GetInheritClassCount();
	for( i=0; i<inheritCount; i++ ){
		const igdeGDClassInherit &inherit = *gdclass.GetInheritClassAt( i );
		if( inherit.GetClass() ){
			pUpdateIDGroupList( *inherit.GetClass(), prefix + inherit.GetPropertyPrefix() );
		}
	}
}

bool meObject::pShowStateIsVisible(){
	if(!pWorld){
		return false;
	}
	
	const meWorldGuiParameters &guiParams = pWorld->GetGuiParameters();
	const meWorldGuiParameters::eElementModes elementMode = guiParams.GetElementMode();
	const bool modeObj = elementMode == meWorldGuiParameters::eemObject;
	
	bool hiddenByTag = false;
	if(pClassDef){
		hiddenByTag = pClassDef->GetHideTags().HasAnyOf(guiParams.GetTagsHideClass());
		
		pWObject->SetPartiallyHidden(pAnyGDClassHasAnyPartialVisOf(
			*pClassDef, guiParams.GetTagsPartialHideClass()));
		pUpdateComponent();
	}
	
	return pVisible && (pActive || pSelected) && modeObj && !hiddenByTag;
}
