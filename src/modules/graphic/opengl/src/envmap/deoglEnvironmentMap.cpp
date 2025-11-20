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
#include <string.h>
#include <stdlib.h>

#include "deoglEnvironmentMap.h"
#include "deoglEnvMapSlotManager.h"
#include "deoglEnvMapSlot.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../light/volume/deoglLightVolume.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../rendering/deoglRenderGeometry.h"
#include "../rendering/deoglRenderOcclusion.h"
#include "../rendering/deoglRenderReflection.h"
#include "../rendering/deoglRenderSky.h"
#include "../rendering/deoglRenderWorld.h"
#include "../rendering/light/deoglRenderGI.h"
#include "../rendering/light/deoglRenderLight.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../rendering/task/deoglAddToRenderTask.h"
#include "../rendering/task/deoglRenderTask.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../visibility/visitor/deoglVisCollectOccMeshes.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"
//#include "../visibility/convexhull/deoglConvexVisHullBuilder.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include "../utils/collision/deoglDCollisionBox.h"



// Definitions
////////////////

static const int vCubeFaces[] = {
	deoglCubeMap::efPositiveX, deoglCubeMap::efNegativeX,
	deoglCubeMap::efPositiveY, deoglCubeMap::efNegativeY,
	deoglCubeMap::efPositiveZ, deoglCubeMap::efNegativeZ
};

#define MAX_LAST_GILIGHT_UPDATE 100



// Class deoglEnvironmentMap
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglEnvironmentMap::deoglEnvironmentMap( deoglRenderThread &renderThread ) :
pRenderThread( renderThread )
{
	pWorld = NULL;
	pOctreeNode = NULL;
	
	pSkyOnly = true;
	
	pSize = pRenderThread.GetConfiguration().GetEnvMapSize();
	pIsFloat = true;
	
	pEnvMap = NULL;
	pEnvMapPosition = NULL;
	pEnvMapDiffuse = NULL;
	pEnvMapNormal = NULL;
	pEnvMapEmissive = NULL;
	pEnvMapEqui = NULL;
	pMaxMipMapLevel = 0;
	
	pLayerMask.SetBit( 1 );
	
	pInfluenceBoxScale.Set( 1.0f, 1.0f, 1.0f );
	pInfluencePriority = 0;
	pHasInfluenceBox = false,
	pHasReflectionBox = false;
	pReflectionMaskBoxMatrices = NULL;
	pReflectionMaskBoxMatrixCount = 0;
	
	pDirty = true;
	pDirtyInit = true;
	pDirtyOctreeNode = true;
	pReady = false;
	pMaterialReady = false;
	pNextUpdateFace = 0;
	pLastGILightUpdate = MAX_LAST_GILIGHT_UPDATE;
	
	pPlanUsageCount = 0;
	pDestroyIfUnused = true;
	
	pMarked = false;
	
	pSlotIndex = -1;
	
	pConvexVolumeList = NULL;
	pLightVolume = NULL;
	pDirtyConvexVolumeList = true;
	
	try{
		pConvexVolumeList = new decConvexVolumeList;
		pLightVolume = new deoglLightVolume( renderThread );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglEnvironmentMap::~deoglEnvironmentMap(){
	pCleanUp();
}



// Management
///////////////

void deoglEnvironmentMap::SetWorld( deoglRWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	pRemoveFromAllRenderPlans();
	
	pWorld = world;
	
	if( pOctreeNode ){
		pOctreeNode->RemoveEnvMap( this );
		pOctreeNode = NULL;
	}
	pDirtyOctreeNode = true;
	
	SetDirty( true );
}

void deoglEnvironmentMap::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		pDirtyOctreeNode = true;
		SetDirty( true );
		
		if( pWorld ){
			pWorld->InvalidateEnvMapLayout();
		}
	}
}



void deoglEnvironmentMap::UpdateOctreePosition(){
	if( pDirtyOctreeNode ){
		if( ! pSkyOnly && pWorld ){
			pWorld->GetOctree().InsertEnvMapIntoTree( this );
		}
		
		pDirtyOctreeNode = false;
		SetDirty( true );
	}
}

void deoglEnvironmentMap::SetOctreeNode( deoglWorldOctree *node ){
	pOctreeNode = node;
}



void deoglEnvironmentMap::SetSkyOnly( bool skyOnly ){
	if( ! pRenderThread.GetConfiguration().GetEnvMapEnable() ){
		skyOnly = true; // nvidia problem
	}
	
	if( skyOnly != pSkyOnly ){
		pSkyOnly = skyOnly;
		SetDirty( true );
		pDirtyInit = true;
	}
}



void deoglEnvironmentMap::SetSize( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( size != pSize ){
		pSize = size;
		
		pMaxMipMapLevel = 0;
		while( size > 1 ){
			pMaxMipMapLevel++;
			size >>= 1;
		}
		
		SetDirty( true );
		pDirtyInit = true;
	}
}

void deoglEnvironmentMap::SetIsFloat( bool isFloat ){
	if( isFloat != pIsFloat ){
		pIsFloat = isFloat;
		
		SetDirty( true );
		pDirtyInit = true;
	}
}



void deoglEnvironmentMap::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



void deoglEnvironmentMap::SetMatrixInfluenceBox( const decDMatrix &matrix ){
	pMatrixInfluenceBox = matrix;
	SetDirty( true );
}

void deoglEnvironmentMap::SetInfluenceBoxBorderFactor( const decVector &factor ){
	pInfluenceBoxBorderFactor = factor;
	SetDirty( true );
}

void deoglEnvironmentMap::SetInfluenceBoxScale( const decVector &scale ){
	pInfluenceBoxScale = scale;
	SetDirty( true );
}

void deoglEnvironmentMap::SetHasInfluenceBox( bool hasInfluenceBox ){
	pHasInfluenceBox = hasInfluenceBox;
	SetDirty( true );
}

void deoglEnvironmentMap::SetInfluencePriority( int priority ){
	pInfluencePriority = priority;
	//SetDirty( true );
}



void deoglEnvironmentMap::SetMatrixReflectionBox( const decDMatrix &matrix ){
	pMatrixReflectionBox = matrix;
	SetDirty( true );
}

void deoglEnvironmentMap::SetHasReflectionBox( bool hasReflectionBox ){
	pHasReflectionBox = hasReflectionBox;
	SetDirty( true );
}



const decDMatrix& deoglEnvironmentMap::GetReflectionMaskBoxMatrixAt( int index ) const{
	if( index < 0 || index >= pReflectionMaskBoxMatrixCount ){
		DETHROW( deeInvalidParam );
	}
	return pReflectionMaskBoxMatrices[ index ];
}

void deoglEnvironmentMap::RemoveAllReflectionMaskBoxMatrices(){
	if( pReflectionMaskBoxMatrices ){
		delete [] pReflectionMaskBoxMatrices;
		pReflectionMaskBoxMatrices = NULL;
		pReflectionMaskBoxMatrixCount = 0;
		
		SetDirty( true );
	}
}

void deoglEnvironmentMap::AddReflectionMaskBoxMatrix( const decDMatrix &matrix ){
	decDMatrix *newArray = new decDMatrix[ pReflectionMaskBoxMatrixCount + 1 ];
	
	if( pReflectionMaskBoxMatrices ){
		memcpy( newArray, pReflectionMaskBoxMatrices, sizeof( decDMatrix ) * pReflectionMaskBoxMatrixCount );
		delete [] pReflectionMaskBoxMatrices;
	}
	pReflectionMaskBoxMatrices = newArray;
	
	pReflectionMaskBoxMatrices[ pReflectionMaskBoxMatrixCount++ ] = matrix;
	
	SetDirty( true );
}



void deoglEnvironmentMap::SetDirty( bool dirty ){
	pDirty = dirty;
	pDirtyConvexVolumeList = true;
	
	if( dirty ){
		pNextUpdateFace = 0;
	}
}

int deoglEnvironmentMap::IsLastGILightUpdateAtMax() const{
	return pLastGILightUpdate == MAX_LAST_GILIGHT_UPDATE;
}

void deoglEnvironmentMap::IncLastGILightUpdate(){
	pLastGILightUpdate = decMath::min( pLastGILightUpdate + 1, MAX_LAST_GILIGHT_UPDATE );
}

void deoglEnvironmentMap::SetMaxLastGILightUpdate(){
	pLastGILightUpdate = MAX_LAST_GILIGHT_UPDATE;
}

void deoglEnvironmentMap::ResetLastGILightUpdate(){
	pLastGILightUpdate = 0;
}



void deoglEnvironmentMap::AddPlanUsage(){
	pPlanUsageCount++;
}

void deoglEnvironmentMap::RemovePlanUsage(){
	if( pPlanUsageCount == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pPlanUsageCount--;
	
	if( pPlanUsageCount == 0 && pDestroyIfUnused ){
		Destroy();
	}
}



void deoglEnvironmentMap::Destroy(){
	// this can be called from the main thread
	if( pSlotIndex != -1 ){
		pRenderThread.GetEnvMapSlotManager().GetSlotAt( pSlotIndex ).Clear();
		pSlotIndex = -1;
	}
	
	SetDirty( true );
	pDirtyInit = true;
	pReady = false;
	pMaterialReady = false;
	pLastGILightUpdate = MAX_LAST_GILIGHT_UPDATE;
	
	if( pEnvMapEqui ){
		delete pEnvMapEqui;
		pEnvMapEqui = nullptr;
	}
	if( pEnvMap ){
		delete pEnvMap;
		pEnvMap = nullptr;
	}
	if( pEnvMapPosition ){
		delete pEnvMapPosition;
		pEnvMapPosition = nullptr;
	}
	if( pEnvMapDiffuse ){
		delete pEnvMapDiffuse;
		pEnvMapDiffuse = nullptr;
	}
	if( pEnvMapNormal ){
		delete pEnvMapNormal;
		pEnvMapNormal = nullptr;
	}
	if( pEnvMapEmissive ){
		delete pEnvMapEmissive;
		pEnvMapEmissive = nullptr;
	}
}

void deoglEnvironmentMap::SetDestroyIfUnused( bool destroyIfUnused ){
	pDestroyIfUnused = destroyIfUnused;
	
	if( destroyIfUnused && pPlanUsageCount == 0 ){
		Destroy();
	}
}



void deoglEnvironmentMap::SetMarked( bool marked ){
	pMarked = marked;
}



void deoglEnvironmentMap::SetSlotIndex( int slotIndex ){
	pSlotIndex = slotIndex;
}



void deoglEnvironmentMap::PrepareForRender(){
	if( ! pDirtyInit ){
		UpdateConvexVolumeList();
		return;
	}
	
	// create env map fully prepared
	if( ! pEnvMap ){
		pEnvMap = new deoglCubeMap( pRenderThread );
		pEnvMap->SetFBOFormat( 4, pIsFloat ); // color + mask
		//pEnvMap->SetFBOFormat( 3, pIsFloat );
		//pEnvMap->SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG11B10F );
		pEnvMap->SetMipMapped( true );
	}
	pEnvMap->SetSize( pSize );
	pEnvMap->CreateCubeMap();
	
	deoglFramebuffer &fbo = pRenderThread.GetFramebuffer().GetEnvMap();
	const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	pRenderThread.GetFramebuffer().Activate( &fbo );
	fbo.DetachAllImages();
	fbo.AttachColorCubeMap( 0, pEnvMap );
	fbo.Verify();
	OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
	
	pEnvMap->CreateMipMaps();
	
	// create material env maps
	if( ! pEnvMapPosition ){
		pEnvMapPosition = new deoglArrayTexture( pRenderThread );
		pEnvMapPosition->SetFBOFormat( 3, true );
	}
	pEnvMapPosition->SetSize( pSize, pSize, 6 );
	pEnvMapPosition->CreateTexture();
	
	if( ! pEnvMapDiffuse ){
		pEnvMapDiffuse = new deoglArrayTexture( pRenderThread );
		pEnvMapDiffuse->SetFBOFormat( 3, false );
	}
	pEnvMapDiffuse->SetSize( pSize, pSize, 6 );
	pEnvMapDiffuse->CreateTexture();
	
	if( ! pEnvMapNormal ){
		pEnvMapNormal = new deoglArrayTexture( pRenderThread );
		pEnvMapNormal->SetFBOFormat( 3, false );
	}
	pEnvMapNormal->SetSize( pSize, pSize, 6 );
	pEnvMapNormal->CreateTexture();
	
	if( ! pEnvMapEmissive ){
		pEnvMapEmissive = new deoglArrayTexture( pRenderThread );
		pEnvMapEmissive->SetFormat( pEnvMap->GetFormat() );
	}
	pEnvMapEmissive->SetSize( pSize, pSize, 6 );
	pEnvMapEmissive->CreateTexture();
	
	// WARNING
	// initing the color produces hick-ups sometimes that are ugly. this is most probably due to the automatic
	// mip-map generation taking place in this case. for the time being this does not seem to be a problem.
	// better would be though to add a new call to clear the entire texture to a given color preventing
	// creating mip-maps with the hardware.
	
	// init the color. this is required as otherwise the cubemap is considered invalid messing up rendering ugly
//	deoglPixelBuffer pixelbuffer( deoglPixelBuffer::epfFloat4, pSize, pSize, 6 );
//	pixelbuffer.SetToFloatColor( 0.0f, 0.0f, 0.0f, 1.0f );
//	pEnvMap->SetPixels( pixelbuffer );
	
	// create the initial equi env map
	if( pRenderThread.GetRenderers().GetReflection().GetUseEquiEnvMap() ){
		if( ! pEnvMapEqui ){
			pEnvMapEqui = new deoglTexture( pRenderThread );
			pEnvMapEqui->SetFBOFormat( 4, pIsFloat ); // color + mask
			//pEnvMapEqui->SetFBOFormat( 3, pIsFloat );
			//pEnvMapEqui->SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG11B10F );
			pEnvMapEqui->SetMipMapped( true );
		}
		pEnvMapEqui->SetSize( pSize * 4, pSize * 2 );
		pEnvMapEqui->CreateTexture();
	//	deoglPixelBuffer pbEquiMap( deoglPixelBuffer::epfFloat4, pEnvMapEqui->GetWidth(), pEnvMapEqui->GetHeight(), 1 );
	//	pbEquiMap.SetToFloatColor( 0.0f, 0.0f, 0.0f, 1.0f );
	//	pEnvMapEqui->SetPixels( pbEquiMap );
	}
	
	pDirtyInit = false;
	
	UpdateConvexVolumeList();
}

void deoglEnvironmentMap::Update( deoglRenderPlan &parentPlan ){
	if( ! pDirty || ! pWorld || pWorld->GetEnvMapUpdateCount() == 0 ){
		return;
	}
	
	pWorld->DecEnvMapUpdateCount();
	pDirty = false;
	
	if( pSkyOnly ){
		if( pRenderThread.GetConfiguration().GetDebugPrintSkyUpdate() ){
			pRenderThread.GetLogger().LogInfoFormat( "EnvMap Update: %p position=(%f,%f,%f) size=%i",
				this, pPosition.x, pPosition.y, pPosition.z, pSize );
		}
		
		pEnvMap->CreateCubeMap();
		pEnvMapPosition->DestroyTexture();
		pEnvMapDiffuse->DestroyTexture();
		pEnvMapNormal->DestroyTexture();
		pEnvMapEmissive->DestroyTexture();
		
		pRenderThread.GetRenderers().GetSky().RenderSkyIntoEnvMap( *pWorld, pLayerMask, *this );
		
		// copy to equi-rect if required
		if( pRenderThread.GetRenderers().GetReflection().GetUseEquiEnvMap() ){
			pRenderThread.GetRenderers().GetReflection().ConvertCubeMap2EquiMap( *pEnvMap, pEnvMapEqui );
			
			// destroy cube map textures
			pEnvMap->DestroyCubeMap();
			
		}else{
			pEnvMap->CreateMipMaps();
		}
		
		pReady = true;
		pMaterialReady = false;
		pLastGILightUpdate = MAX_LAST_GILIGHT_UPDATE;
		
	}else{
		if( pRenderThread.GetConfiguration().GetDebugPrintSkyUpdate() ){
			pRenderThread.GetLogger().LogInfoFormat( "EnvMap Update: %p position=(%f,%f,%f) size=%i face=%i",
				this, pPosition.x, pPosition.y, pPosition.z, pSize, pNextUpdateFace );
		}
		RenderEnvCubeMap( parentPlan );
	}
}

void deoglEnvironmentMap::RenderEnvCubeMap( deoglRenderPlan &parentPlan ){
	int renderTime[ 8 ] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	decTimer timer;
	
	const deoglDebugTraceGroup debugTrace( pRenderThread, "EnvironmentMap.RenderEnvCubeMap" );
	deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
	// dont do this. active framebuffer could be an env-map one and vanish while rendering
	// takes place resulting in segfaults
// 	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	deoglRenderPlan &plan = *pWorld->GetEnvMapRenderPlan();
	decDMatrix matrixCamera;
	int cmf;
	
	// NOTE re-entrant problem. if two or more env-maps are updated in a chain
	//      the same shared render plan is used resulting in all kinds of
	//      nasty problems. to solve this more than one shared render plan
	//      has to exist.
	
	plan.SetViewport( pSize, pSize );
	plan.SetUpscaleSize( pSize, pSize );
	plan.SetUseUpscaling( false );
	plan.SetCameraParameters( DEG2RAD * 90.0f, 1.0f, 0.01f, 500.0f );
// 	plan.SetIgnoreStaticComponents( true );
	plan.SetNoReflections( true );
	
	plan.SetUseLayerMask( true );
	plan.SetLayerMask( pLayerMask );
	
	plan.SetLodMaxPixelError( 2 );
	plan.SetLodLevelOffset( 1 );
	
	// use the parent plan gi state but without modifying it. allows to use GI with
	// no extra cost and witout messing up parent GI state
	plan.SetUseConstGIState( NULL );//parentPlan.GetRenderGIState() );
	plan.SetUseGIState( plan.GetUseConstGIState() != NULL );
	
	plan.SetNoAmbientLight( true );
	
	// TODO we need to find a way to figure out what adapted intensity to use here.
	//      in the case of regular rendering the lower camera intensity is used which
	//      is fine enough for the first image rendered until the last average scene
	//      intensity is known. in the case of environment maps though we have neither
	//      a last average scene intensity nor a camera with intensity boundaries.
	//      
	//      one solution would be to examine all lights affecting the environment
	//      map and to calculate the incoming light intensity similar to lumimeters.
	//      this would require though testing against occlusion meshes or shadow maps
	//      to not use a too high intensity.
	//      
	//      another solution would be to look at cameras in the world finding the one
	//      closest to the environment map. this way we could use the last known average
	//      intensity of a camera recently rendered. this also has potential problems
	//      but it would base the decision on somewhat usful data.
	//      
	//      all in all there is no simple solution to this problem. using right now
	//      the constant scene key should ensure guessing not too high
	plan.SetCameraAdaptedIntensity( 1.0f );
	
	//plan.SetDebugTiming( true );
	
	matrixCamera.SetCamera( pPosition, decDVector( 0.0, 0.0, 1.0 ), decDVector( 0.0, 1.0, 0.0 ) );
	
	cmf = pNextUpdateFace;
	
	pEnvMap->CreateCubeMap();
	pEnvMapPosition->CreateTexture();
	pEnvMapDiffuse->CreateTexture();
	pEnvMapNormal->CreateTexture();
	pEnvMapEmissive->CreateTexture();
	
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	
	if( cmf == 0 ){
		// initial clear and mip maps creation. this is required or else it won't work.
		// better would be here using a placeholder environment map until the true
		// environment map has finished rendering. but for the time being this init
		// is done. cleared to black for the time being. would be better to clear to
		// the average ambient lighting at the envmap location once known
		
		// we can not use a shared framebuffer here from GetManager().GetFBOWithResolution()
		// because various rendering parts use shared framebuffer too. if the same framebuffer
		// is selected it is used at the same time by the environment map rendering and other
		// rendering. this can lead to problems
		pRenderThread.GetRenderers().GetWorld().GetPipelineClearBuffers()->Activate();
		
		deoglFramebuffer &fbo = pRenderThread.GetFramebuffer().GetEnvMap();
		pRenderThread.GetFramebuffer().Activate( &fbo );
		fbo.DetachAllImages();
		fbo.AttachColorArrayTexture( 0, pEnvMapEmissive );
		fbo.Verify();
		
		const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		
		deoglFramebuffer &fboMaterial = pRenderThread.GetFramebuffer().GetEnvMapMaterial();
		const GLfloat clearPosition[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		const GLfloat clearDiffuse[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f };
		const GLfloat clearNormal[ 4 ] = { 0.0f, 0.0f, 0.0f, 0.0f }; // int-shifted
		
		pRenderThread.GetFramebuffer().Activate( &fboMaterial );
		fboMaterial.DetachAllImages();
		fboMaterial.AttachColorArrayTexture( 0, pEnvMapPosition );
		fboMaterial.AttachColorArrayTexture( 1, pEnvMapDiffuse );
		fboMaterial.AttachColorArrayTexture( 2, pEnvMapNormal );
		fboMaterial.Verify();
		
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearPosition[ 0 ] ) );
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 1, &clearDiffuse[ 0 ] ) );
		OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 2, &clearNormal[ 0 ] ) );
		
		pRenderThread.GetFramebuffer().Activate( oldfbo );
		
		pMaterialReady = false;
		pLastGILightUpdate = MAX_LAST_GILIGHT_UPDATE;
	}
	
	try{
		// prepare material fbo
		deoglFramebuffer &fboMaterial = pRenderThread.GetFramebuffer().GetEnvMapMaterial();
		const GLenum buffers[ 3 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		
		pRenderThread.GetFramebuffer().Activate( &fboMaterial );
		fboMaterial.DetachAllImages();
		OGL_CHECK( pRenderThread, pglDrawBuffers( 3, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		plan.SetFBOMaterial( &fboMaterial );
		
		// prepare fbo
		deoglFramebuffer &fbo = pRenderThread.GetFramebuffer().GetEnvMap();
		
		pRenderThread.GetFramebuffer().Activate( &fbo );
		fbo.DetachAllImages();
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		plan.SetFBOTarget( &fbo );
		plan.SetUpsideDown( false );
		
		// render
		timer.Reset();
		
		//for( cmf=0; cmf<6; cmf++ ){
			deoglCubeMap::CreateMatrixForFace( matrixCamera, pPosition, vCubeFaces[ cmf ] );
			plan.SetCameraMatrix( matrixCamera );
			plan.SetFBOMaterialMatrix( plan.GetInverseCameraMatrix() * decDMatrix::CreateTranslation( -pPosition ) );
			
			plan.PrepareRender( nullptr );
			// ^-- this can cause ourself to be marked for deletion. due to the render plan
			//     keeping a guard reference we do not die ending up with a segfault but the
			//     pEnvMap has been NULL-ed already. if pEnvMap is NULL we drop out since
			//     this env-map is not going to be used by anybody anymore.
			//     
			//     still this is a bad solution for the actual problem which is caused by
			//     PrepareRender. the problem is that PrepareRender also prepares the env
			//     map lists. the GetEnvMapUpdateCount call in deoglRWorld prevents calling
			//     into updating envmap again but it does not protect against manipulating
			//     the env-map list in the first place. and this manipulation is the source
			//     of the problem. the only working solution is to somehow prevent
			//     PrepareRender from touching env-map list at all. using the pEnvMap being
			//     NULL as indicator for a removed env-map is a workaround
			if( ! pEnvMap ){
				pRenderThread.GetFramebuffer().Activate( NULL );
				plan.SetFBOTarget( NULL );
				plan.SetFBOMaterial( NULL );
				pNextUpdateFace = 6;
				pDirty = false;
				return;
			}
			
			defren.Resize( pSize, pSize );
			
			pRenderThread.GetFramebuffer().Activate( &fboMaterial );
			fboMaterial.DetachAllImages();
			fboMaterial.AttachColorArrayTextureLayer( 0, pEnvMapPosition, cmf );
			fboMaterial.AttachColorArrayTextureLayer( 1, pEnvMapDiffuse, cmf );
			fboMaterial.AttachColorArrayTextureLayer( 2, pEnvMapNormal, cmf );
			fboMaterial.Verify();
			
			pRenderThread.GetFramebuffer().Activate( &fbo );
			fbo.DetachAllImages();
			fbo.AttachColorArrayTextureLayer( 0, pEnvMapEmissive, cmf );
			fbo.Verify();
			
			plan.Render();
			pRenderThread.GetRenderers().GetWorld().RenderFinalizeFBO( plan, false, false );
			pRenderThread.GetRenderers().GetReflection().RenderEnvMapMask( plan, *this, vCubeFaces[ cmf ] );
			renderTime[ cmf ] = ( int )( timer.GetElapsedTime() * 1000000.0 );
			renderTime[ 7 ] += renderTime[ cmf ];
		//}
		
		plan.SetFBOTarget( NULL );
		plan.SetFBOMaterial( NULL );
		
		pRenderThread.GetFramebuffer().Activate( oldfbo );
		
		pNextUpdateFace++;
		if( pNextUpdateFace == 6 ){
			pRenderThread.GetRenderers().GetReflection().CopyEnvMap( *pEnvMapEmissive, *pEnvMap );
			pEnvMap->CreateMipMaps();
			
			if( pRenderThread.GetRenderers().GetReflection().GetUseEquiEnvMap() ){
				pRenderThread.GetRenderers().GetReflection().ConvertCubeMap2EquiMap( *pEnvMap, pEnvMapEqui );
				
				pEnvMap->DestroyCubeMap();
				pEnvMapPosition->DestroyTexture();
				pEnvMapDiffuse->DestroyTexture();
				pEnvMapNormal->DestroyTexture();
				pEnvMapEmissive->DestroyTexture();
			}
			
			if( pSlotIndex != -1 ){
				pRenderThread.GetEnvMapSlotManager().NotifyEnvMapChanged( pSlotIndex );
			}
			
			pReady = true;
			pMaterialReady = true;
			pLastGILightUpdate = MAX_LAST_GILIGHT_UPDATE;
		}
		
		renderTime[ 6 ] = ( int )( timer.GetElapsedTime() * 1000000.0 );
		renderTime[ 7 ] += renderTime[ 6 ];
		if( pRenderThread.GetConfiguration().GetDebugPrintSkyUpdate() ){
			pRenderThread.GetLogger().LogInfoFormat(
				"time: x+(%iys) x-(%iys) y+(%iys) y-(%iys) z+(%iys) z-(%iys) M(%iys) T(%iys)\n",
				renderTime[ 0 ], renderTime[ 1 ], renderTime[ 2 ], renderTime[ 3 ],
				renderTime[ 4 ], renderTime[ 5 ], renderTime[ 6 ], renderTime[ 7 ] );
		}
		
	}catch( const deException &e ){
		pRenderThread.GetLogger().LogException( e );
		pRenderThread.GetFramebuffer().Activate( oldfbo );
		throw;
	}
	
	if( pNextUpdateFace < 6 ){
		pDirty = true;
	}
}



void deoglEnvironmentMap::UpdateConvexVolumeList(){
	if( ! pDirtyConvexVolumeList ){
		return;
	}
	
	decTimer timer;
	timer.Reset();
	const float maxSize = 25.0f;
	const double maxExtend = ( double )maxSize * 2.0 + 0.001; // for true version
	//const double maxExtend = ( double )maxSize + 0.001; // for false version
	
	const decDVector boxMinExtend = pPosition - decDVector( maxExtend, maxExtend, maxExtend );
	const decDVector boxMaxExtend = pPosition + decDVector( maxExtend, maxExtend, maxExtend );
	
	pConvexVolumeList->RemoveAllVolumes();
	pConvexVolumeList->SetToCube( decVector( maxSize, maxSize, maxSize ) );
	
	if( pWorld ){
		if( true ){
			const deoglEnvironmentMapList &list = pWorld->GetEnvMapList();
			const int count = list.GetCount();
			int i;
			
			for( i=0; i<count; i++ ){
				const deoglEnvironmentMap &envmap = *list.GetAt( i );
				const decDVector &position = envmap.GetPosition();
				
				if( position >= boxMinExtend && position <= boxMaxExtend && ! pPosition.IsEqualTo( position, 0.01 ) ){
					const decVector splitPosition = ( ( position - pPosition ) * 0.5 ).ToVector();
					const decVector splitNormal = -splitPosition.Normalized();
					
					pConvexVolumeList->SplitByPlane( splitNormal, splitPosition, true, NULL );
				}
			}
			
		}else{
#if 0
			deoglConvexVisHullBuilder builder;
			deoglVisCollectOccMeshes collect;
			deoglTriangleSorter &triangleSorter = *collect.GetTriangleSorter();
			float safeRadius, srtemp;
			
			collect.SetVisitExtends( boxMinExtend, boxMaxExtend );
			collect.SetMatrixInvHull( decDMatrix::CreateTranslation( -pPosition ) );
			pWorld->VisitRegion( boxMinExtend, boxMaxExtend, collect );
			
			safeRadius = ( float )( boxMaxExtend - pPosition ).Length();
			srtemp = ( float )( pPosition - boxMinExtend ).Length();
			if( srtemp > safeRadius ){
				safeRadius = srtemp;
			}
			
			if( triangleSorter.GetTriangleCount() > 0 ){
				triangleSorter.SortRadial( decVector() );
				builder.CropByTriangles( *pConvexVolumeList, triangleSorter, decVector(), safeRadius + 0.1f );
			}
			
			// cropping produces unfortunately a concave volume which we can't use. to solve this a convex hull has to
			// be calculated from the volume points. this is correctly done using a gift-wrap algorithm. for the time
			// being though we simply calculate an axis aligned bounding box and clip the volume against it. this
			// produces a result that is not as optimal as the gift-wrap but it's better than nothing for the time being
			if( true ){
			decDVector boundingBoxMinExtend, boundingBoxMaxExtend; // better env map extends
			
			builder.GetTransformedVolumeExtends( *pConvexVolumeList, decDMatrix::CreateTranslation( pPosition ),
				boundingBoxMinExtend, boundingBoxMaxExtend );
			
			pConvexVolumeList->SetToCube( ( boundingBoxMaxExtend - boundingBoxMinExtend ).ToVector() );
			pConvexVolumeList->Move( ( ( boundingBoxMinExtend + boundingBoxMaxExtend ) * 0.5 ).ToVector() );
			}
#endif
		}
	}
	
	pLightVolume->CreateFrom( *pConvexVolumeList );
	
	pDirtyConvexVolumeList = false;
	if( pRenderThread.GetConfiguration().GetDebugPrintSkyUpdate() ){
		pRenderThread.GetLogger().LogInfoFormat( "UpdateConvexVolumeList (%g,%g,%g): %iys\n",
			pPosition.x, pPosition.y, pPosition.z, ( int )( timer.GetElapsedTime() * 1000000.0 ) );
	}
}



void deoglEnvironmentMap::SkyChanged(){
	SetDirty( true );
}

void deoglEnvironmentMap::LightChanged( deoglRLight* ){
	SetDirty( true );
}



void deoglEnvironmentMap::PrepareQuickDispose(){
	pWorld = NULL;
	pOctreeNode = NULL;
	
	pComponentList.RemoveAll();
	pBillboardList.RemoveAll();
	pParticleEmitterInstanceList.RemoveAll();
	pRenderPlanList.RemoveAll();
}



// Private Functions
//////////////////////

void deoglEnvironmentMap::pCleanUp(){
	if( pReflectionMaskBoxMatrices ){
		delete [] pReflectionMaskBoxMatrices;
	}
	if( pLightVolume ){
		delete pLightVolume;
	}
	if( pConvexVolumeList ){
		delete pConvexVolumeList;
	}
	
	Destroy();
}

void deoglEnvironmentMap::pRemoveFromAllRenderPlans(){
	const int count = pRenderPlanList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRenderPlanList.GetAt( i )->RemoveEnvMap( this );
	}
	pRenderPlanList.RemoveAll();
	
	pPlanUsageCount = 0;
	if( pDestroyIfUnused ){
		Destroy();
	}
}
