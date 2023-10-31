/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglComponent.h"
#include "deoglRComponent.h"
#include "deoglComponentTexture.h"
#include "deoglComponentLOD.h"
#include "../deGraphicOpenGl.h"
#include "../decal/deoglDecal.h"
#include "../decal/deoglRDecal.h"
#include "../model/deoglModel.h"
#include "../occlusiontest/mesh/deoglDynamicOcclusionMesh.h"
#include "../occlusiontest/mesh/deoglOcclusionMesh.h"
#include "../skin/deoglSkin.h"
#include "../skin/dynamic/deoglDynamicSkin.h"
#include "../skin/state/deoglSkinStateController.h"
#include "../world/deoglWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>



// Class deoglComponent
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglComponent::deoglComponent( deGraphicOpenGl &ogl, deComponent &component ) :
pOgl( ogl ),
pComponent( component ),
pRComponent( NULL ),

pParentWorld( NULL ),
pSkinStateController( NULL ),

pDynamicSkin( NULL ),

pLODs( NULL ),
pLODCount( 0 ),

pTextures( NULL ),
pTextureCount( 0 ),

pAccumUpdate( 0.0f ),

pDirtyComponent( true ),
pDirtySkinStateController( true ),
pDirtyLodTextures( true ),
pDirtyOctreeNode( true ),
pDirtyExtends( true ),
pDirtyMatrices( true ),
pDirtyDecals( true ),
pDirtyCullSphere( true ),
pDirtyRenderEnvMap( true ),
pDirtyResetStatic( true ),
pDirtyModel( true ),
pDirtySkin( true ),
pDirtyRig( true ),
pDirtyDynamicSkin( true ),
pDirtyOcclusionMesh( true ),
pDirtyOcclusionMeshBones( true ),
pDirtyRenderableMapping( true ),
pDirtyBoneMatrices( true ),
pDirtyLODErrorScaling( true ),
pDirtyMesh( true ),
pDirtySkinStateStates( true ),
pSkinStatePrepareRenderables( true ),
pDirtyStaticTexture( true ),
pNotifyTexturesChanged( false ),
pNotifyTUCChanged( false ),
pDirtySolid( true ),
pDirtyTextureUseSkin( true ),

pDynamicSkinRenderablesChanged( true ),
pDynamicSkinRequiresSync( true ),
pTextureDynamicSkinRenderablesChanged( true ),
pTextureDynamicSkinRequiresSync( true ),
pDecalRequiresSync( true ),
pRequiresUpdateEverySync( false ),

pLLSyncWorld( this )
{
	try{
		pRComponent = new deoglRComponent( ogl.GetRenderThread() );
		pSkinStateController = new deoglSkinStateController;
		
		pCreateLODs();
		pCreateTextures();
		
		deDecal *decal = component.GetRootDecal();
		while( decal ){
			DecalAdded( decal );
			decal = decal->GetLLComponentNext();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglComponent::~deoglComponent(){
	pCleanUp();
}



// Management
///////////////

// #define HACK_TEST_CS 1

#ifdef HACK_TEST_CS
#include <dragengine/common/utils/decTimer.h>
int hackCSResCount = 0;
float hackCSResTime = 0;
int hackCSTexCount = 0;
float hackCSTexTime = 0;
int hackCSMiscCount = 0;
float hackCSMiscTime = 0;
int hackCSAccumCount = 0;
float hackCSAccumTime = 0;
int hackCSGeomCount = 0;
float hackCSGeomTime = 0;
int hackCSExtCount = 0;
float hackCSExtTime = 0;
int hackCSOctCount = 0;
float hackCSOctTime = 0;
int hackCSBoneMapCount = 0;
float hackCSBoneMapTime = 0;
int hackCSSpecialCount = 0;
float hackCSSpecialTime = 0;
#endif

void deoglComponent::SyncToRender(){
	#ifdef HACK_TEST_CS
	decTimer timer; timer.Reset();
	#endif
	
	// sync resources
	pSyncModel();
	pSyncSkin();
	if( pDirtyRig ){
		pRComponent->RigChanged();
		pDirtyRig = false;
	}
	pSyncDynamicSkin();
	pSyncOcclusionMesh();
	#ifdef HACK_TEST_CS
	hackCSResCount++; hackCSResTime += timer.GetElapsedTime();
	#endif
	
	// sync lods and textures
	pSyncLodTextures();
	#ifdef HACK_TEST_CS
	hackCSTexCount++; hackCSTexTime += timer.GetElapsedTime();
	#endif
	
	// dirty calls affecting other dirty calls
	if( pDirtyComponent ){
		pRComponent->SetVisible( pComponent.GetVisible() );
		pRComponent->SetMovementHint( pComponent.GetHintMovement() );
		pRComponent->SetLayerMask( pComponent.GetLayerMask() );
		pRComponent->SetGIImportance( pComponent.GetEnableGI() ? pComponent.GetHintGIImportance() + 1 : 0 );
		pDirtyComponent = false;
	}
	
	if( pDirtyCullSphere ){
		pRComponent->SetDirtyCulling();
		pDirtyCullSphere = false;
	}
	if( pDirtyRenderEnvMap ){
		pRComponent->InvalidateRenderEnvMap();
		pDirtyRenderEnvMap = false;
	}
	if( pDirtyResetStatic ){
		pRComponent->ResetRenderStatic();
		pDirtyResetStatic = false;
	}
	if( pDirtyRenderableMapping ){
		pRComponent->UpdateRenderableMapping();
		pDirtyRenderableMapping = false;
		
		// we have to do this here and not in DirtyRenderableMapping() because
		// DirtyRenderableMapping() can be called between the UpdateRenderableMapping()
		// call above and the NotifyTexturesChanged() call below. if this happens the
		// pNotifyTexturesChanged flag will be cleared below while the
		// pDirtyRenderableMapping is true. this causes pNotifyTexturesChanged to be
		// not called the next time UpdateRenderableMapping() above is called. this in
		// turn causes listeners to miss an update and working with old data
		pNotifyTexturesChanged = true;
	}
	if( pDirtyLODErrorScaling ){
		const decVector &scaling = pComponent.GetScaling();
		pRComponent->SetLODErrorScaling( decMath::max( scaling.x, scaling.y, scaling.z ) );
		pDirtyLODErrorScaling = false;
	}
	if( pDirtyMesh ){
		pRComponent->MeshChanged();
		pRComponent->UpdateVertexPositionSets( pComponent );
		pDirtyMesh = false;
	}
	#ifdef HACK_TEST_CS
	hackCSMiscCount++; hackCSMiscTime += timer.GetElapsedTime();
	#endif
	
	// sync skin state controller
	if( pDirtySkinStateController ){
		pSkinStateController->Init( *pRComponent->GetSkinState(), pRComponent->GetSkin(), pParentWorld );
		int i;
		for( i=0; i<pTextureCount; i++ ){
			pTextures[ i ]->InitSkinState();
		}
		
		pDirtySkinStateController = false;
	}
	
	// accumulated times
// 	if( pAccumUpdate > 0.0f ){
		// TODO add parameter to deComponent used to multiply the accum update with. this would allow
		//      to update the component skins at a different pace without needing to call handle
		//      skin update itself. doing it here is highly desirable
		const float skinUpdate = pAccumUpdate; // * pComponent.GetSkinUpdateFactor()
		
		pRComponent->Update( pAccumUpdate );
		
		pSkinStateController->AdvanceTime( skinUpdate );
		pSkinStateController->SyncToRender();
		
		int i;
		for( i=0; i<pTextureCount; i++ ){
			pTextures[ i ]->AdvanceTime( skinUpdate );
			pTextures[ i ]->SyncToRender();
		}
		
		pRComponent->UpdateSkin( skinUpdate );
		
		pAccumUpdate = 0.0f;
		pCheckRequiresUpdateEverySync();
// 	}
	#ifdef HACK_TEST_CS
	hackCSAccumCount++; hackCSAccumTime += timer.GetElapsedTime();
	#endif
	
	// sync skin state properties. has to come after pSkinStateController->SyncToRender()
	// and pRComponent->UpdateSkin()
	if( pDirtySkinStateStates ){
		pRComponent->InitSkinStateStates( pComponent );
		pDirtySkinStateStates = false;
	}
	
	if( pSkinStatePrepareRenderables ){
		pRComponent->DirtyPrepareSkinStateRenderables();
		pSkinStatePrepareRenderables = false;
	}
	
	pRComponent->UpdateSkinStateStates(); // has to be done better. only some need this
	
	if( pDirtySolid ){
		pRComponent->DirtySolid();
		pDirtySolid = false;
	}
	
	// octree, extends and matrices. order is important
	// - deoglRComponent::UpdateExtends() depends on pRComponent::UpdateBoneMatrices()
	if( pDirtyMatrices ){
		pRComponent->SetMatrix( decDMatrix::CreateWorld( pComponent.GetPosition(),
			pComponent.GetOrientation(), pComponent.GetScaling() ) );
		pDirtyMatrices = false;
		#ifdef HACK_TEST_CS
		hackCSGeomCount++; hackCSGeomTime += timer.GetElapsedTime();
		#endif
	}
	
	if( pDirtyBoneMatrices ){
		pComponent.PrepareBones();
		pRComponent->UpdateBoneMatrices( pComponent );
		pRComponent->UpdateSkinStateBones( pComponent );
		pDirtyBoneMatrices = false;
		#ifdef HACK_TEST_CS
		hackCSBoneMapCount++; hackCSBoneMapTime += timer.GetElapsedTime();
		#endif
	}
	
	if( pDirtyExtends ){
		pRComponent->UpdateExtends( pComponent );
		pDirtyExtends = false;
		#ifdef HACK_TEST_CS
		hackCSExtCount++; hackCSExtTime += timer.GetElapsedTime();
		#endif
	}
	
	if( pDirtyOctreeNode ){
		pRComponent->UpdateOctreeNode();
		pDirtyOctreeNode = false;
		#ifdef HACK_TEST_CS
		hackCSOctCount++; hackCSOctTime += timer.GetElapsedTime();
		#endif
	}
	
	if( pDirtyTextureUseSkin ){
		pDirtyTextureUseSkin = false;
		pRComponent->UpdateTexturesUseSkin();
	}
	
	pSyncTextureDynamicSkinRenderablesChanged(); // requires UpdateTexturesUseSkin()
	
	if( pDirtyStaticTexture ){
		pDirtyStaticTexture = false;
		pRComponent->UpdateStaticTextures();
	}
	if( pNotifyTexturesChanged ){
		pNotifyTexturesChanged = false;
		pRComponent->NotifyTexturesChanged();
	}
	if( pNotifyTUCChanged ){
		pNotifyTUCChanged = false;
		pRComponent->NotifyTUCChanged();
	}
	
	// decals
	pSyncDecals();
}



void deoglComponent::Update( float elapsed ){
	pAccumUpdate += elapsed;
	
	if( pRequiresUpdateEverySync ){
		pRequiresSync();
	}
}



void deoglComponent::SetParentWorld( deoglWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	int i;
	for( i=0; i<pTextureCount; i++ ){
		pTextures[ i ]->ClearSkinStateController();
	}
	if( pSkinStateController ){
		pSkinStateController->Clear();
	}
	
	pParentWorld = parentWorld;
	
	pDirtyOctreeNode = true;
	pDirtySkinStateController = true;
	pDirtyRenderEnvMap = true;
}



deoglComponentLOD &deoglComponent::GetLODAt( int index ) const{
	if( index < 0 || index >= pLODCount ){
		DETHROW( deeInvalidParam );
	}
	return *pLODs[ index ];
}



deoglComponentTexture &deoglComponent::GetTextureAt( int index ){
	if( index < 0 || index >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	return *pTextures[ index ];
}



void deoglComponent::DynamicSkinRequiresSync(){
	pDynamicSkinRequiresSync = true;
	pDirtyStaticTexture = true;
	pSkinStatePrepareRenderables = true;
	pNotifyTexturesChanged = true;
	pRequiresSync();
}

void deoglComponent::TextureDynamicSkinRenderableChanged(){
	// called by deoglComponentTexture upon receiving changed notification from dynamic skin
	pTextureDynamicSkinRenderablesChanged = true;
	pTextureDynamicSkinRequiresSync = true;
	pDirtyRenderableMapping = true;
	pDirtyStaticTexture = true;
	pNotifyTexturesChanged = true;
	pNotifyTUCChanged = true;
	pDirtySolid = true;
	pRequiresSync();
}

void deoglComponent::TextureDynamicSkinRequiresSync(){
	pTextureDynamicSkinRequiresSync = true;
	pNotifyTexturesChanged = true;
	pRequiresSync();
}

void deoglComponent::DirtyRenderableMapping(){
	pDirtyRenderableMapping = true;
	pRequiresSync();
}

void deoglComponent::DirtyTextureUseSkin(){
	pDirtyTextureUseSkin = true;
	pRequiresSync();
}

void deoglComponent::DirtyAllTexturesUpdateRenderableMappings(){
	int i;
	for( i=0; i<pTextureCount; i++ ){
		pTextures[ i ]->DirtyRenderableMapping();
	}
}

void deoglComponent::DecalRequiresSync(){
	pDecalRequiresSync = true;
	pRequiresSync();
}



// Dynamic skin listener
//////////////////////////

void deoglComponent::DynamicSkinDestroyed(){
	pDynamicSkin = NULL;
}

void deoglComponent::DynamicSkinRenderablesChanged(){
	pDynamicSkinRenderablesChanged = true;
	pDynamicSkinRequiresSync = true;
	pDirtyRenderableMapping = true;
	pDirtyStaticTexture = true;
	pSkinStatePrepareRenderables = true;
	pNotifyTexturesChanged = true;
	pNotifyTUCChanged = true;
	pDirtySolid = true;
	
	int i;
	for( i=0; i<pTextureCount; i++ ){
		pTextures[ i ]->SetDynamicSkinRenderablesChanged( true );
	}
	pTextureDynamicSkinRenderablesChanged = true;
	pTextureDynamicSkinRequiresSync = true;
	
	pRequiresSync();
}

void deoglComponent::DynamicSkinRenderableChanged( deoglDSRenderable& ){
	pDynamicSkinRenderablesChanged = true;
	pDynamicSkinRequiresSync = true;
	pDirtyStaticTexture = true;
	pSkinStatePrepareRenderables = true;
	pNotifyTexturesChanged = true;
	pNotifyTUCChanged = true;
	pDirtySolid = true;
	
	int i;
	for( i=0; i<pTextureCount; i++ ){
		pTextures[ i ]->SetDynamicSkinRenderablesChanged( true );
	}
	pTextureDynamicSkinRenderablesChanged = true;
	pTextureDynamicSkinRequiresSync = true;
	
	pRequiresSync();
}

void deoglComponent::DynamicSkinRenderableRequiresSync( deoglDSRenderable& ){
	pDynamicSkinRequiresSync = true;
	pSkinStatePrepareRenderables = true;
	pRequiresSync();
}



// Notifications
//////////////////

void deoglComponent::PositionChanged(){
	pDirtyExtends = true;
	pDirtyMatrices = true;
	pDirtyOctreeNode = true;
	pDirtyCullSphere = true;
	pDirtyRenderEnvMap = true;
	pDirtyResetStatic = true;
	
	pRequiresSync();
}

void deoglComponent::ScalingChanged(){
	pDirtyExtends = true;
	pDirtyMatrices = true;
	pDirtyOctreeNode = true;
	pDirtyCullSphere = true;
	pDirtyResetStatic = true;
	pDirtyLODErrorScaling = true;
	
	pRequiresSync();
}

void deoglComponent::OrientationChanged(){
	pDirtyExtends = true;
	pDirtyMatrices = true;
	pDirtyOctreeNode = true;
	pDirtyCullSphere = true;
	pDirtyResetStatic = true;
	
	pRequiresSync();
}

void deoglComponent::ModelChanged(){
	pDropLODs();
	pCreateLODs();
	pDropTextures();
	pCreateTextures();
	
	pDirtyModel = true;
	pDirtyLodTextures = true;
	pDirtyOcclusionMeshBones = true;
	pDirtyMesh = true;
	pDirtyBoneMatrices = true;
	
	pDirtyExtends = true;
	pDirtyOctreeNode = true;
	pDirtyCullSphere = true;
	pDirtyResetStatic = true;
	pDirtyStaticTexture = true;
	pDirtyTextureUseSkin = true;
	pNotifyTexturesChanged = true;
	pNotifyTUCChanged = true;
	pDirtySolid = true;
	
	pRequiresSync();
}

void deoglComponent::SkinChanged(){
	pDirtySkin = true;
	pDirtyResetStatic = true;
	pDirtySkinStateController = true;
	pDirtyRenderableMapping = true;
	pDirtySkinStateStates = true;
	pSkinStatePrepareRenderables = true;
	pDirtyStaticTexture = true;
	pDirtyTextureUseSkin = true;
	pNotifyTexturesChanged = true;
	pNotifyTUCChanged = true;
	pDirtySolid = true;
	DirtyAllTexturesUpdateRenderableMappings();
	
	pRequiresSync();
}

void deoglComponent::ModelAndSkinChanged(){
	pDropLODs();
	pCreateLODs();
	pDropTextures();
	pCreateTextures();
	
	pDirtyModel = true;
	pDirtySkin = true;
	pDirtyLodTextures = true;
	pDirtyOcclusionMeshBones = true;
	pDirtyMesh = true;
	pDirtyBoneMatrices = true;
	
	pDirtyExtends = true;
	pDirtyOctreeNode = true;
	pDirtyCullSphere = true;
	pDirtyResetStatic = true;
	pDirtySkinStateController = true;
	pDirtyRenderableMapping = true;
	pDirtySkinStateStates = true;
	pSkinStatePrepareRenderables = true;
	pDirtyStaticTexture = true;
	pDirtyTextureUseSkin = true;
	pNotifyTexturesChanged = true;
	pNotifyTUCChanged = true;
	pDirtySolid = true;
	DirtyAllTexturesUpdateRenderableMappings();
	
	pRequiresSync();
}

void deoglComponent::RigChanged(){
	pDirtyRig = true;
	
	pDirtyExtends = true;
	pDirtyOctreeNode = true;
	pDirtyCullSphere = true;
	pDirtyMatrices = true;
	pDirtyResetStatic = true;
	pDirtyMesh = true;
	pDirtyBoneMatrices = true;
	pDirtySkinStateStates = true;
	pSkinStatePrepareRenderables = true;
	
	pRequiresSync();
}

void deoglComponent::VisibilityChanged(){
	pDirtyComponent = true;
	pDirtyResetStatic = true;
	pDirtyOctreeNode = true;
	
	pRequiresSync();
}

void deoglComponent::ExtendsChanged(){
	pDirtyExtends = true;
	pDirtyOctreeNode = true;
	pDirtyCullSphere = true;
	
	pRequiresSync();
}

void deoglComponent::MeshDirty(){
	pDirtyExtends = true;
	pDirtyOctreeNode = true;
	pDirtyBoneMatrices = true;
	pDirtyResetStatic = true;
	pDirtyMesh = true;
	
	pRequiresSync();
}

void deoglComponent::OcclusionMeshChanged(){
	pDirtyOcclusionMesh = true;
	pDirtyOcclusionMeshBones = true;
	
	
	// update all touching lights since their light volume potentially changes. this works even with
	// lights storing only components in their reduced extends. if a component has a new occlusion
	// mesh (or a larger one) that did not have it before the light volume can at best reduce but
	// not expand. if on the other hand the component has now no occlusion mesh (or a smaller one)
	// than before the light volumes of the affected lights can potentially expand. this works since
	// lights visit the entire full extends upon rebuilding the light volume. this catches then also
	// component occlusion meshes not included yet in their list
	pDirtyOctreeNode = true;
	pDirtyResetStatic = true;
	
	pRequiresSync();
}

void deoglComponent::ParametersChanged(){
	pDirtyComponent = true;
	pDirtyResetStatic = true;
	
	pRequiresSync();
}



void deoglComponent::TextureChanged( int index, deComponentTexture &texture ){
	if( index < 0 || index >= pTextureCount ){
		DETHROW( deeInvalidParam );
	}
	
	pTextures[ index ]->TextureChanged( texture );
	
	pDirtyRenderableMapping = true;
	pDirtyResetStatic = true;
	pTextureDynamicSkinRequiresSync = true;
	pDirtySkinStateController = true;
	pDirtySkinStateStates = true;
	pSkinStatePrepareRenderables = true;
	pDirtyStaticTexture = true;
	pNotifyTexturesChanged = true;
	pNotifyTUCChanged = true; // actually only if skin or dynamic skin change but not transform
	pDirtySolid = true;
	
	if( pRComponent->GetMovementHint() == deComponent::emhStationary
	&& pRComponent->GetRenderMode() == deoglRComponent::ermStatic ){
		pRComponent->NotifySkiesUpdateStatic();
	}
	
	pRequiresSync();
}



void deoglComponent::DynamicSkinChanged(){
	if( pDynamicSkin ){
		pDynamicSkin->RemoveListener( this );
	}
	
	if( pComponent.GetDynamicSkin() ){
		pDynamicSkin = ( deoglDynamicSkin* )pComponent.GetDynamicSkin()->GetPeerGraphic();
		pDynamicSkin->AddListener( this );
		
	}else{
		pDynamicSkin = NULL;
	}
	
	pDirtyDynamicSkin = true;
	pDynamicSkinRenderablesChanged = true;
	pDynamicSkinRequiresSync = true;
	pDirtyResetStatic = true;
	pDirtyRenderableMapping = true;
	pDirtyStaticTexture = true;
	pSkinStatePrepareRenderables = true;
	pNotifyTexturesChanged = true;
	pNotifyTUCChanged = true;
	pDirtySolid = true;
	DirtyAllTexturesUpdateRenderableMappings();
	
	pRequiresSync();
}



void deoglComponent::LayerMaskChanged(){
	pDirtyComponent = true;
	
	pRequiresSync();
}



void deoglComponent::DecalAdded( deDecal *decal ){
	( ( deoglDecal* )decal->GetPeerGraphic() )->SetParentComponent( this );
	
	pDirtyDecals = true;
	pDecalRequiresSync = true;
	
	pRequiresSync();
}

void deoglComponent::DecalRemoved( deDecal *decal ){
	deoglDecal &oglDecal = *( ( deoglDecal* )decal->GetPeerGraphic() );
	oglDecal.SetParentComponent( NULL );
	oglDecal.GetRDecal()->SetComponentMarkedRemove( true );
	
	pDirtyDecals = true;
	pDecalRequiresSync = true;
	
	pRequiresSync();
}

void deoglComponent::AllDecalsRemoved(){
	deDecal *decal = pComponent.GetRootDecal();
	while( decal ){
		deoglDecal &oglDecal = *( ( deoglDecal* )decal->GetPeerGraphic() );
		oglDecal.SetParentComponent( NULL );
		oglDecal.GetRDecal()->SetComponentMarkedRemove( true );
		
		decal = decal->GetLLComponentNext();
	}
	
	pDirtyDecals = true;
	
	pRequiresSync();
}



int deoglComponent::IndexOfTextureClosestTo( const decVector &vector, float radius ){
#if 0
	const int closestFace = IndexOfFaceClosestTo( vector, radius );
	
	if( closestFace != -1 ){
		return ( pModel->GetLODAt( 0 ).GetFaces()[ closestFace ] ).GetTexture();
	}
#endif
	
	return -1;
}

#if 0
int deoglComponent::IndexOfFaceClosestTo( const decVector &vector, float radius ){
	if( pModel && pModel->GetLODCount() > 0 ){
		const decVector boxMin( vector.x - radius, vector.y - radius, vector.z - radius );
		const decVector boxMax( vector.x + radius, vector.y + radius, vector.z + radius );
		
		deoglModelLOD &modelLOD = pModel->GetLODAt( 0 );
		const deoglModelFace * const faces = modelLOD.GetFaces();
		const oglModelVertex * const vertices = modelLOD.GetVertices();
		const int faceCount = modelLOD.GetFaceCount();
		float closestDistanceSquared = 0.0f;
		int closestFace = -1;
		int i;
		
		if( pRenderMode == ermDynamic ){
			pLODs[ 0 ]->PreparePositions();
			const oglVector * const positions = pLODs[ 0 ]->GetPositions();
			
			//int a=0;
			for( i=0; i<faceCount; i++ ){
				const deoglModelFace &face = faces[ i ];
				const oglModelVertex &v1 = vertices[ face.GetVertex1() ];
				const oglModelVertex &v2 = vertices[ face.GetVertex2() ];
				const oglModelVertex &v3 = vertices[ face.GetVertex3() ];
				const oglVector &p1 = positions[ v1.position ];
				const oglVector &p2 = positions[ v2.position ];
				const oglVector &p3 = positions[ v3.position ];
				const decVector cp1( p1.x, p1.y, p1.z );
				const decVector cp2( p2.x, p2.y, p2.z );
				const decVector cp3( p3.x, p3.y, p3.z );
				
				const decVector faceMin = cp1.Smallest( cp2 ).Smallest( cp3 );
				const decVector faceMax = cp1.Largest( cp2 ).Largest( cp3 );
				if( ! ( boxMax >= faceMin && boxMin <= faceMax  ) ){
					continue;
				}
				//a++;
				
				const float distanceSquared = ( deoglCollisionDetection::ClosestPointOnTriangle( cp1, cp2, cp3, vector ) - vector ).LengthSquared();
				
				if( closestFace == -1 || distanceSquared < closestDistanceSquared ){
					closestDistanceSquared = distanceSquared;
					closestFace = i;
				}
			}
			//pOgl->LogInfoFormat( "IndexOfFaceClosestTo dynamic tested %i/%i", a, faceCount );
			
			if( closestFace != -1 && closestDistanceSquared > radius * radius ){
				closestFace = -1;
			}
			
		}else{
			/*
			const oglModelPosition * const positions = modelLOD.GetPositions();
			modelLOD.PrepareOctree();
			
			//int a=0;
			for( i=0; i<faceCount; i++ ){
				const deoglModelFace &face = faces[ i ];
				
				if( ! ( boxMax >= face.GetMinExtend() && boxMin <= face.GetMaxExtend() ) ){
					continue;
				}
				//a++;
				
				const oglModelVertex &v1 = vertices[ face.GetVertex1() ];
				const oglModelVertex &v2 = vertices[ face.GetVertex2() ];
				const oglModelVertex &v3 = vertices[ face.GetVertex3() ];
				const decVector &p1 = positions[ v1.position ].position;
				const decVector &p2 = positions[ v2.position ].position;
				const decVector &p3 = positions[ v3.position ].position;
				const float distanceSquared = ( deoglCollisionDetection::ClosestPointOnTriangle( p1, p2, p3, vector ) - vector ).LengthSquared();
				
				if( closestFace == -1 || distanceSquared < closestDistanceSquared ){
					closestDistanceSquared = distanceSquared;
					closestFace = i;
				}
			}
			//pOgl->LogInfoFormat( "IndexOfFaceClosestTo static tested %i/%i", a, faceCount );
			
			if( closestFace != -1 && closestDistanceSquared > radius * radius ){
				closestFace = -1;
			}
			*/
			
			closestFace = deoglMOVClosestFace( modelLOD ).FindClosestFace( vector, radius );
			//pOgl->LogInfoFormat( "IndexOfFaceClosestTo static tested %i/%i/%i (nodes %i)",
			//	movClosestFace_FacesTested, movClosestFace_TotalFaces, faceCount, movClosestFace_NodesTested );
		}
		
		return closestFace;
		
	}else{
		return -1;
	}
}

int deoglComponent::IndexOfVertexClosestTo( const decVector &vector ){
	if( pModel && pModel->GetLODCount() > 0 ){
		const deoglModelLOD &modelLOD = pModel->GetLODAt( 0 );
		const oglModelVertex * const vertices = modelLOD.GetVertices();
		const int vertexCount = modelLOD.GetVertexCount();
		float closestDistanceSquared = 0.0f;
		int closestVertex = -1;
		int i;
		
		if( pRenderMode == ermDynamic ){
			pLODs[ 0 ]->PreparePositions();
			const oglVector * const positions = pLODs[ 0 ]->GetPositions();
			
			for( i=0; i<vertexCount; i++ ){
				const oglVector &vertex = positions[ vertices[ i ].position ];
				const float distanceSquared = decVector( vertex.x - vector.x, vertex.y - vector.y, vertex.z - vector.z ).LengthSquared();
				
				if( closestVertex == -1 || distanceSquared < closestDistanceSquared ){
					closestDistanceSquared = distanceSquared;
					closestVertex = i;
				}
			}
			
		}else{
			const oglModelPosition * const positions = modelLOD.GetPositions();
			
			for( i=0; i<vertexCount; i++ ){
				const float distanceSquared = ( positions[ vertices[ i ].position ].position - vector ).LengthSquared();
				
				if( closestVertex == -1 || distanceSquared < closestDistanceSquared ){
					closestDistanceSquared = distanceSquared;
					closestVertex = i;
				}
			}
		}
		
		return closestVertex;
		
	}else{
		return -1;
	}
}
#endif



// Private Functions
//////////////////////

void deoglComponent::pCleanUp(){
	if( pSkinStateController ){
		delete pSkinStateController;
	}
	
	pDropTextures();
	pDropLODs();
	
	if( pRComponent ){
		pRComponent->FreeReference();
	}
	
	if( pDynamicSkin ){
		pDynamicSkin->RemoveListener( this );
	}
}



void deoglComponent::pDropLODs(){
	if( ! pLODs ){
		return;
	}
	
	while( pLODCount > 0 ){
		pLODCount--;
		delete pLODs[ pLODCount ];
	}
	delete [] pLODs;
	pLODs = NULL;
	pLODCount = 0;
}

void deoglComponent::pCreateLODs(){
	const deModel * const model = pComponent.GetModel();
	if( ! model || model->GetLODCount() == 0 ){
		return;
	}
	
	const int lodCount = model->GetLODCount();
	
	pLODs = new deoglComponentLOD*[ lodCount ];
	
	for( pLODCount=0; pLODCount<lodCount; pLODCount++ ){
		pLODs[ pLODCount ] = new deoglComponentLOD( *this, pLODCount );
	}
}

void deoglComponent::pDropTextures(){
	if( ! pTextures ){
		return;
	}
	
	while( pTextureCount > 0 ){
		pTextureCount--;
		delete pTextures[ pTextureCount ];
	}
	delete [] pTextures;
	pTextures = NULL;
	pTextureCount = 0;
}

void deoglComponent::pCreateTextures(){
	const deModel * const model = pComponent.GetModel();
	if( ! model || model->GetTextureCount() == 0 ){
		return;
	}
	
	const int textureCount = model->GetTextureCount();
	
	pTextures = new deoglComponentTexture*[ textureCount ];
	
	for( pTextureCount=0; pTextureCount<textureCount; pTextureCount++ ){
		pTextures[ pTextureCount ] = new deoglComponentTexture( *this, pTextureCount );
	}
}



void deoglComponent::pSyncModel(){
	if( ! pDirtyModel ){
		return;
	}
	
	if( pComponent.GetModel() ){
		pRComponent->SetModel( ( ( deoglModel* )pComponent.GetModel()->GetPeerGraphic() )->GetRModel() );
		
	}else{
		pRComponent->SetModel( NULL );
	}
	
	pDirtyModel = false;
}

void deoglComponent::pSyncSkin(){
	if( ! pDirtySkin ){
		return;
	}
	
	if( pComponent.GetSkin() ){
		pRComponent->SetSkin( ( ( deoglSkin* )pComponent.GetSkin()->GetPeerGraphic() )->GetRSkin() );
		
	}else{
		pRComponent->SetSkin( NULL );
	}
	
	pDirtySkin = false;
}

void deoglComponent::pSyncDynamicSkin(){
	if( pDirtyDynamicSkin ){
		pRComponent->SetDynamicSkin( *this, pDynamicSkin ? pDynamicSkin->GetRDynamicSkin() : NULL );
		pDirtyDynamicSkin = false;
	}
	
	if( pDynamicSkinRenderablesChanged ){
		pDynamicSkinRenderablesChanged = false;
		pRComponent->DynamicSkinRenderablesChanged();
	}
	
	if( pDynamicSkinRequiresSync ){
		pDynamicSkinRequiresSync = false;
		if( pDynamicSkin ){
			pDynamicSkin->SyncToRender();
		}
	}
	
	if( pTextureDynamicSkinRequiresSync ){
		pTextureDynamicSkinRequiresSync = false;
		
		int i;
		for( i=0; i<pTextureCount; i++ ){
			if( pTextures[ i ]->GetDynamicSkin() ){
				pTextures[ i ]->GetDynamicSkin()->SyncToRender();
			}
		}
	}
}

void deoglComponent::pSyncOcclusionMesh(){
	if( pDirtyOcclusionMesh ){
		if( pComponent.GetOcclusionMesh() ){
			pRComponent->SetOcclusionMesh( ( ( deoglOcclusionMesh* )
				pComponent.GetOcclusionMesh()->GetPeerGraphic() )->GetROcclusionMesh() );
			
		}else{
			pRComponent->SetOcclusionMesh( NULL );
		}
		
		pDirtyOcclusionMesh = false;
	}
	
	if( pDirtyOcclusionMeshBones ){
		if( pRComponent->GetDynamicOcclusionMesh() ){
			pRComponent->GetDynamicOcclusionMesh()->UpdateBoneMappings( pComponent );
		}
		
		pDirtyOcclusionMeshBones = false;
	}
}

void deoglComponent::pSyncLodTextures(){
	if( ! pDirtyLodTextures ){
		return;
	}
	
	int i;
	
	pRComponent->RemoveAllLODs();
	for( i=0; i<pLODCount; i++ ){
		pRComponent->AddLOD( pLODs[ i ]->GetRLOD() );
	}
	
	pRComponent->RemoveAllTextures();
	for( i=0; i<pTextureCount; i++ ){
		pRComponent->AddTexture( pTextures[ i ]->GetRTexture() );
	}
	
	pDirtyLodTextures = false;
}

void deoglComponent::pSyncDecals(){
	if( pDirtyDecals ){
		pDirtyDecals = false;
		pRComponent->SyncDecalReferences( pComponent );
	}
	
	if( pDecalRequiresSync ){
		pDecalRequiresSync = false;
		
		deDecal *decal = pComponent.GetRootDecal();
		while( decal ){
			( ( deoglDecal* )decal->GetPeerGraphic() )->SyncToRender();
			decal = decal->GetLLComponentNext();
		}
	}
}

void deoglComponent::pSyncTextureDynamicSkinRenderablesChanged(){
	if( ! pTextureDynamicSkinRenderablesChanged ){
		return;
	}
	
	pTextureDynamicSkinRenderablesChanged = false;
	
	int i;
	for( i=0; i<pTextureCount; i++ ){
		if( pTextures[ i ]->GetDynamicSkinRenderablesChanged() ){
			pTextures[ i ]->SetDynamicSkinRenderablesChanged( false );
			pRComponent->TextureDynamicSkinRenderablesChanged( *pTextures[ i ]->GetRTexture() );
		}
	}
}

void deoglComponent::pCheckRequiresUpdateEverySync(){
	// if render static is false a timer is running until render static is set to true.
	// this time runs only while Update() is called
	pRequiresUpdateEverySync = ! pRComponent->GetRenderStatic();
	
	if( pSkinStateController->RequiresSyncEveryFrameUpdate() ){
		pRequiresUpdateEverySync = true;
		pSkinStatePrepareRenderables |= pSkinStateController->RequiresPrepareRenderables();
	}
	
	int i;
	for( i=0; i<pTextureCount; i++ ){
		deoglSkinStateController &ssc = *pTextures[ i ]->GetSkinStateController();
		if( ssc.RequiresSyncEveryFrameUpdate() ){
			pRequiresUpdateEverySync = true;
			pSkinStatePrepareRenderables |= ssc.RequiresPrepareRenderables();
		}
	}
}



void deoglComponent::pRequiresSync(){
	if( ! pLLSyncWorld.GetList() && pParentWorld ){
		pParentWorld->AddSyncComponent( this );
	}
}
