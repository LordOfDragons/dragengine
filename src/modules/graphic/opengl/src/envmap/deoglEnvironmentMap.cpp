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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "deoglEnvironmentMap.h"
#include "deoglEnvMapSlotManager.h"
#include "deoglEnvMapSlot.h"
#include "../capabilities/deoglCapabilities.h"
#include "../configuration/deoglConfiguration.h"
#include "../debug/deoglDebugSaveTexture.h"
#include "../delayedoperation/deoglDelayedDeletion.h"
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
	pEnvMapDepth = NULL;
	pEnvMapEqui = NULL;
	pEnvMapEquiDepth = NULL;
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
	pNextUpdateFace = 0;
	
	pPlanUsageCount = 0;
	pDestroyIfUnused = true;
	
	pMarked = false;
	
	pSlotIndex = -1;
	
	pConvexVolumeList = NULL;
	pLightVolume = NULL;
	pDirtyConvexVolumeList = true;
	pDirtyCubeMapHardLimit = true;
	pDirtyCubeMapSoftLimit = true;
	pCubeMapHardLimit = NULL;
	pCubeMapSoftLimit = NULL;
	pCubeMapLimitScale = 1.0f;
	
	try{
		pConvexVolumeList = new decConvexVolumeList;
		pLightVolume = new deoglLightVolume( renderThread );
		
		pCubeMapHardLimit = new deoglCubeMap( renderThread );
		pCubeMapHardLimit->SetSize( 128 );
		pCubeMapHardLimit->SetFBODepth16Format();
		
		pCubeMapSoftLimit = new deoglCubeMap( renderThread );
		pCubeMapSoftLimit->SetSize( 128 );
		pCubeMapSoftLimit->SetFBODepth16Format();
		
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
	pDirtyCubeMapHardLimit = true;
	pDirtyCubeMapSoftLimit = true;
	
	if( dirty ){
		pNextUpdateFace = 0;
	}
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



class deoglEnvironmentMapDeletion : public deoglDelayedDeletion{
public:
	deoglCubeMap *envMap;
	deoglCubeMap *envMapDepth;
	deoglTexture *envMapEqui;
	deoglTexture *envMapEquiDepth;
	deoglCubeMap *cubeMapHardLimit;
	deoglCubeMap *cubeMapSoftLimit;
	
	deoglEnvironmentMapDeletion() :
	envMap( NULL ),
	envMapDepth( NULL ),
	envMapEqui( NULL ),
	envMapEquiDepth( NULL ),
	cubeMapHardLimit( NULL ),
	cubeMapSoftLimit( NULL ){
	}
	
	virtual ~deoglEnvironmentMapDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread &renderThread ){
		if( cubeMapSoftLimit ){
			delete cubeMapSoftLimit;
		}
		if( cubeMapHardLimit ){
			delete cubeMapHardLimit;
		}
		if( envMapEqui ){
			delete envMapEqui;
		}
		if( envMapEquiDepth ){
			delete envMapEquiDepth;
		}
		if( envMap ){
			delete envMap;
		}
		if( envMapDepth ){
			delete envMapDepth;
		}
	}
};

void deoglEnvironmentMap::Destroy(){
	// this can be called from the main thread. we need to use delayed-deletion here.
	if( pSlotIndex != -1 ){
		pRenderThread.GetEnvMapSlotManager().GetSlotAt( pSlotIndex ).Clear();
		pSlotIndex = -1;
	}
	
	SetDirty( true );
	pDirtyInit = true;
	pReady = false;
	
	// delayed deletion of opengl containing objects
	deoglEnvironmentMapDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglEnvironmentMapDeletion;
		
		delayedDeletion->cubeMapSoftLimit = pCubeMapSoftLimit;
		pCubeMapSoftLimit = NULL;
		
		delayedDeletion->cubeMapHardLimit = pCubeMapHardLimit;
		pCubeMapHardLimit = NULL;
		
		delayedDeletion->envMapEqui = pEnvMapEqui;
		pEnvMapEqui = NULL;
		
		delayedDeletion->envMapEquiDepth = pEnvMapEquiDepth;
		pEnvMapEquiDepth = NULL;
		
		delayedDeletion->envMap = pEnvMap;
		pEnvMap = NULL;
		
		delayedDeletion->envMapDepth = pEnvMapDepth;
		pEnvMapDepth = NULL;
		
		pRenderThread.GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pRenderThread.GetLogger().LogException( e );
		throw;
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
	
	if( ! pEnvMap ){
		pEnvMap = new deoglCubeMap( pRenderThread );
		pEnvMap->SetFBOFormat( 4, pIsFloat ); // color + mask
		//pEnvMap->SetFBOFormat( 3, pIsFloat );
		//pEnvMap->SetFormatFBOByNumber( deoglCapsFmtSupport::eutfRG11B10F );
		pEnvMap->SetMipMapped( true );
	}
	pEnvMap->SetSize( pSize );
	pEnvMap->CreateCubeMap();
	
	/*
	if( ! pEnvMapDepth ){
		pEnvMapDepth = new deoglCubeMap( pRenderThread );
		pEnvMapDepth->SetDepthFormat();
	}
	pEnvMapDepth->SetSize( pSize );
	pEnvMapDepth->CreateCubeMap();
	*/
	
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
		
		/*
		if( ! pEnvMapEquiDepth ){
			pEnvMapEquiDepth = new deoglTexture( pRenderThread );
			pEnvMapEquiDepth->SetDepthFormat( false );
		}
		pEnvMapEquiDepth->SetSize( pSize * 4, pSize * 2 );
		pEnvMapEquiDepth->CreateTexture();
		*/
	}
	
	pDirtyInit = false;
	
	UpdateConvexVolumeList();
}

void deoglEnvironmentMap::Update( deoglRenderPlan &parentPlan ){
	if( ! pDirty ){
		return;
	}
	if( ! pWorld ){
		return;
	}
	if( pWorld->GetEnvMapUpdateCount() == 0 ){
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
		//pEnvMapDepth->CreateCubeMap();
		// TODO we need now a layer mask to select what sky instances are rendered into the
		//      the env map. the default here chooses all but if somebody uses layer masking
		//      this is potentially wrong.
		//      
		//      find a solution for this
		//      
		pRenderThread.GetRenderers().GetSky().RenderSkyIntoEnvMap( *pWorld, decLayerMask(), *this );
		if( pRenderThread.GetRenderers().GetReflection().GetUseEquiEnvMap() ){
			pRenderThread.GetRenderers().GetReflection().ConvertCubeMap2EquiMap( *pEnvMap, pEnvMapEqui );
			// TODO same for depth
		}
		/*
		decString text;
		int i;
		
		text.Format( "envmap_%p_X%.2f_Y%.2f_Z%.2f_cubemap", this, pPosition.x, pPosition.y, pPosition.z );
		int textLen = text.GetLength();
		for( i=0; i<textLen; i++ ) if( text.GetAt( i ) == '.' ) text.SetAt( i, 'p' );
		pRenderThread.GetDebug().GetDebugSaveTexture().SaveCubeMap( *pEnvMap, text.GetString(), false, 0, false );
		
		text.Format( "envmap_%p_X%.2f_Y%.2f_Z%.2f_equimap", this, pPosition.x, pPosition.y, pPosition.z );
		textLen = text.GetLength();
		for( i=0; i<textLen; i++ ) if( text.GetAt( i ) == '.' ) text.SetAt( i, 'p' );
		pRenderThread.GetDebug().GetDebugSaveTexture().SaveTexture( *pEnvMapEqui, text.GetString(), false );
		*/
		if( pRenderThread.GetRenderers().GetReflection().GetUseEquiEnvMap() ){
			pEnvMap->DestroyCubeMap();
			//pEnvMapDepth->DestroyCubeMap();
		}
		
		pReady = true;
		
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
	
	plan.SetViewport( 0, 0, pSize, pSize );
	plan.SetUpscaleSize( pSize, pSize );
	plan.SetUseUpscaling( false );
	plan.SetCameraParameters( DEG2RAD * 90.0f, 1.0f, 0.01f, 500.0f );
	plan.SetForceShadowMapSize( pSize ); //>> 1 );
// 	plan.SetIgnoreStaticComponents( true );
	plan.SetNoReflections( true );
	
	plan.SetUseLayerMask( true );
	plan.SetLayerMask( pLayerMask );
	
	// use the parent plan gi state but without modifying it. allows to use GI with
	// no extra cost and witout messing up parent GI state
	plan.SetUseConstGIState( parentPlan.GetRenderGIState() );
	plan.SetUseGIState( plan.GetUseConstGIState() != NULL );
	
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
	//pEnvMapDepth->CreateCubeMap();
	
	if( cmf == 0 ){
		// initial clear and mip maps creation. this is required or else it won't work.
		// better would be here using a placeholder environment map until the true
		// environment map has finished rendering. but for the time being this init
		// is done. cleared to black for the time being. would be better to clear to
		// the average ambient lighting at the envmap location once known
		deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
		
		OGL_CHECK( pRenderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
		
		// we can not use a shared framebuffer here from GetManager().GetFBOWithResolution()
		// because various rendering parts use shared framebuffer too. if the same framebuffer
		// is selected it is used at the same time by the environment map rendering and other
		// rendering. this can lead to problems
		deoglFramebuffer &fbo = pRenderThread.GetFramebuffer().GetEnvMap();
		pRenderThread.GetFramebuffer().Activate( &fbo );
		
		fbo.DetachAllImages();
		
		const GLfloat clearColor[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
		int j;
		
		for( j=0; j<6; j++ ){
			fbo.AttachColorCubeMapFace( 0, pEnvMap, vCubeFaces[ j ] );
			fbo.Verify();
			OGL_CHECK( pRenderThread, pglClearBufferfv( GL_COLOR, 0, &clearColor[ 0 ] ) );
		}
		
		pRenderThread.GetFramebuffer().Activate( oldfbo );
		/* only required with shared framebuffer
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		*/
		
		pEnvMap->CreateMipMaps();
	}
	
	try{
		deoglFramebuffer &fbo = pRenderThread.GetFramebuffer().GetEnvMap();
		pRenderThread.GetFramebuffer().Activate( &fbo );
		
		fbo.DetachAllImages();
		
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		plan.SetFBOTarget( &fbo );
		plan.SetUpsideDown( false );
		
		timer.Reset();
		
		//for( cmf=0; cmf<6; cmf++ ){
			deoglCubeMap::CreateMatrixForFace( matrixCamera, pPosition, vCubeFaces[ cmf ] );
			plan.SetCameraMatrix( matrixCamera );
			
			plan.PrepareRender( NULL );
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
				/* only required with shared framebuffer
				if( fbo ){
					fbo->DecreaseUsageCount();
				}
				*/
				plan.SetFBOTarget( NULL );
				pNextUpdateFace = 6;
				pDirty = false;
				return;
			}
			
			defren.Resize( pSize, pSize );
			
			pRenderThread.GetFramebuffer().Activate( &fbo );
			//fbo->AttachDepthCubeMap( pEnvMapDepth, vCubeFaces[ cmf ] );
			fbo.AttachColorCubeMapFace( 0, pEnvMap, vCubeFaces[ cmf ] );
			fbo.Verify();
			
			plan.Render();
			pRenderThread.GetRenderers().GetWorld().RenderFinalizeFBO( plan );
			pRenderThread.GetRenderers().GetReflection().RenderEnvMapMask( plan, *this, vCubeFaces[ cmf ] );
			renderTime[ cmf ] = ( int )( timer.GetElapsedTime() * 1000000.0 );
			renderTime[ 7 ] += renderTime[ cmf ];
		//}
		
		plan.SetFBOTarget( NULL );
		
// 		pRenderThread.GetFramebuffer().Activate( oldfbo );
		pRenderThread.GetFramebuffer().Activate( NULL );
		/* only required with shared framebuffer
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		*/
		
		pNextUpdateFace++;
		if( pNextUpdateFace == 6 ){
			/*
			decString text;
			text.Format( "envmap_%p", this );
			pRenderThread.GetDebug().GetDebugSaveTexture().SaveCubeMap( *pEnvMap, text.GetString(), false, false );
			*/
			pEnvMap->CreateMipMaps();
			
			if( pRenderThread.GetRenderers().GetReflection().GetUseEquiEnvMap() ){
				pRenderThread.GetRenderers().GetReflection().ConvertCubeMap2EquiMap( *pEnvMap, pEnvMapEqui );
				// TODO same for depth
			}
			if( false ){
				deoglDebugSaveTexture &dst = pRenderThread.GetDebug().GetDebugSaveTexture();
				decString text;
				int i;
				
				if( pEnvMap ){
					text.Format( "envmap_X%.2f_Y%.2f_Z%.2f_cubemap", pPosition.x, pPosition.y, pPosition.z );
					int textLen = text.GetLength();
					for( i=0; i<textLen; i++ ) if( text.GetAt( i ) == '.' ) text.SetAt( i, 'p' );
					dst.SaveCubeMapConversion( *pEnvMap, text.GetString(), false,
						deoglDebugSaveTexture::ecColorLinearToneMapsRGB );
				}
				if( pEnvMapDepth ){
					text.Format( "envmap_X%.2f_Y%.2f_Z%.2f_cubemap_depth", pPosition.x, pPosition.y, pPosition.z );
					int textLen = text.GetLength();
					for( i=0; i<textLen; i++ ) if( text.GetAt( i ) == '.' ) text.SetAt( i, 'p' );
					dst.SaveCubeMapConversion( *pEnvMapDepth, text.GetString(), false,
						deoglDebugSaveTexture::ecDepthBuffer );
				}
				
				if( pEnvMapEqui ){
					text.Format( "envmap_X%.2f_Y%.2f_Z%.2f_equimap", pPosition.x, pPosition.y, pPosition.z );
					int textLen = text.GetLength();
					for( i=0; i<textLen; i++ ) if( text.GetAt( i ) == '.' ) text.SetAt( i, 'p' );
					dst.SaveTextureConversion( *pEnvMapEqui, text.GetString(),
						deoglDebugSaveTexture::ecColorLinearToneMapsRGB );
				}
				if( pEnvMapEquiDepth ){
					text.Format( "envmap_X%.2f_Y%.2f_Z%.2f_equimap_depth", pPosition.x, pPosition.y, pPosition.z );
					int textLen = text.GetLength();
					for( i=0; i<textLen; i++ ) if( text.GetAt( i ) == '.' ) text.SetAt( i, 'p' );
					dst.SaveTextureConversion( *pEnvMapEquiDepth, text.GetString(),
						deoglDebugSaveTexture::ecDepthBuffer );
				}
			}
			
			if( pRenderThread.GetRenderers().GetReflection().GetUseEquiEnvMap() ){
				pEnvMap->DestroyCubeMap();
				//pEnvMapDepth->DestroyCubeMap();
			}
			
			if( pSlotIndex != -1 ){
				pRenderThread.GetEnvMapSlotManager().NotifyEnvMapChanged( pSlotIndex );
			}
			
			pReady = true;
		}
		
		renderTime[ 6 ] = ( int )( timer.GetElapsedTime() * 1000000.0 );
		renderTime[ 7 ] += renderTime[ 6 ];
		if( pRenderThread.GetConfiguration().GetDebugPrintSkyUpdate() ){
			pRenderThread.GetLogger().LogInfoFormat(
				"time: x+(%iys) x-(%iys) y+(%iys) y-(%iys) z+(%iys) z-(%iys) M(%iys) T(%iys)\n",
				renderTime[ 0 ], renderTime[ 1 ], renderTime[ 2 ], renderTime[ 3 ],
				renderTime[ 4 ], renderTime[ 5 ], renderTime[ 6 ], renderTime[ 7 ] );
		}
		
		/*
		if( pEnvMap ){
			decString filename;
			filename.Format( "envmap/render-%g-%g-%g_%d", pPosition.x, pPosition.y, pPosition.z, pNextUpdateFace );
			pRenderThread.GetDebug().GetDebugSaveTexture().SaveCubeMapConversion(
				*pEnvMap, filename, true, deoglDebugSaveTexture::ecColorLinearToneMapsRGB );
		}
		*/
		
	}catch( const deException &e ){
		pRenderThread.GetLogger().LogException( e );
// 		pRenderThread.GetFramebuffer().Activate( oldfbo );
		pRenderThread.GetFramebuffer().Activate( NULL );
		/* only required with shared framebuffer
		if( fbo ){
			fbo->DecreaseUsageCount();
		}
		*/
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
	
	pCubeMapLimitScale = maxSize;
	pDirtyCubeMapHardLimit = true;
	pDirtyCubeMapSoftLimit = true;
	
	pDirtyConvexVolumeList = false;
	if( pRenderThread.GetConfiguration().GetDebugPrintSkyUpdate() ){
		pRenderThread.GetLogger().LogInfoFormat( "UpdateConvexVolumeList (%g,%g,%g): %iys\n",
			pPosition.x, pPosition.y, pPosition.z, ( int )( timer.GetElapsedTime() * 1000000.0 ) );
	}
}

void deoglEnvironmentMap::PrepareCubeMapHardLimit(){
	if( ! pDirtyCubeMapHardLimit ){
		return;
	}
	
	deoglCollideList collideList;
	deoglDCollisionBox colbox;
	decTimer timer;
	
	timer.Reset();
	
	colbox.SetCenter( pPosition );
	colbox.SetHalfSize( decDVector( ( double )pCubeMapLimitScale, ( double )pCubeMapLimitScale, ( double )pCubeMapLimitScale ) );
	collideList.AddComponentsColliding( pWorld->GetOctree(), &colbox );
	
	pCubeMapHardLimit->CreateCubeMap();
	
	pRenderThread.GetRenderers().GetOcclusion().RenderOcclusionCubeMap( collideList, pCubeMapHardLimit, pPosition, 0.01f, pCubeMapLimitScale );
	
	pRenderThread.GetLogger().LogInfoFormat( "PrepareCubeMapHardLimit (%g,%g,%g): %iys\n",
		pPosition.x, pPosition.y, pPosition.z, ( int )( timer.GetElapsedTime() * 1000000.0 ) );
	if( false ){
		decString filename;
		filename.Format( "envmap_limit_hard_X%.2f_Y%.2f_Z%.2f", pPosition.x, pPosition.y, pPosition.z );
		const int filenameLen = filename.GetLength();
		int i;
		for( i=0; i<filenameLen; i++ ){
			if( filename.GetAt( i ) == '.' ){
				filename.SetAt( i, ',' );
			}
		}
		pRenderThread.GetDebug().GetDebugSaveTexture().SaveDepthCubeMap( *pCubeMapHardLimit, filename.GetString(), true );
	}
	
	pDirtyCubeMapHardLimit = false;
}

void deoglEnvironmentMap::PrepareCubeMapSoftLimit(){
	if( ! pDirtyCubeMapSoftLimit ){
		return;
	}
	
	pDirtyCubeMapSoftLimit = false;
}



void deoglEnvironmentMap::SkyChanged(){
	SetDirty( true );
}

void deoglEnvironmentMap::LightChanged( deoglRLight *light ){
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
