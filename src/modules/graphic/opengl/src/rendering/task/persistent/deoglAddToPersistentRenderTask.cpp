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

#include "deoglAddToPersistentRenderTask.h"
#include "deoglPersistentRenderTask.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "deoglPersistentRenderTaskPipeline.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskSubInstance.h"
#include "deoglPersistentRenderTaskOwner.h"
#include "../../../collidelist/deoglCollideListComponent.h"
#include "../../../component/deoglRComponent.h"
#include "../../../component/deoglRComponentTexture.h"
#include "../../../model/deoglModelLOD.h"
#include "../../../model/deoglRModel.h"
#include "../../../model/texture/deoglModelTexture.h"
#include "../../../renderthread/deoglRTShader.h"
#include "../../../renderthread/deoglRenderThread.h"
#include "../../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../../../skin/shader/deoglSkinShader.h"
#include "../../../skin/state/deoglSkinState.h"
#include "../../../skin/state/deoglSkinStateRenderable.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfigList.h"

#include <dragengine/common/exceptions.h>



// Class deoglAddToPersistentRenderTask
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglAddToPersistentRenderTask::deoglAddToPersistentRenderTask(
	deoglRenderThread &renderThread, deoglPersistentRenderTask &renderTask ) :
pRenderThread( renderThread  ),
pRenderTask( renderTask  ),

pSkinPipelineType( deoglSkinTexturePipelines::etGeometry ),

pSolid( false ),
pNoShadowNone( false ),
pNoNotReflected( false ),
pNoRendered( false ),
pOutline( false ),

pFilterXRay( false ),
pXRay( false ),

pFilterHoles( false ),
pWithHoles( false ),

pFilterDoubleSided( false ),
pDoubleSided( false ),
pForceDoubleSided( false ),

pFilterDecal( false ),
pDecal( false ),

pFilterCubeFace( -1 ),

pUseSpecialParamBlock( false ),

pEnforcePipeline( nullptr ),
pEnforceParamBlock( nullptr ){
}

deoglAddToPersistentRenderTask::~deoglAddToPersistentRenderTask(){
}



// Management
///////////////

void deoglAddToPersistentRenderTask::SetSkinPipelineType( deoglSkinTexturePipelines::eTypes type ){
	pSkinPipelineType = type;
}

void deoglAddToPersistentRenderTask::SetSkinPipelineModifier( int modifier ){
	pSkinPipelineModifier = modifier;
}

void deoglAddToPersistentRenderTask::SetSolid( bool solid ){
	pSolid = solid;
}

void deoglAddToPersistentRenderTask::SetNoNotReflected( bool noNotReflected ){
	pNoNotReflected = noNotReflected;
}

void deoglAddToPersistentRenderTask::SetNoRendered( bool noRendered ){
	pNoRendered = noRendered;
}

void deoglAddToPersistentRenderTask::SetOutline( bool outline ){
	pOutline = outline;
}

void deoglAddToPersistentRenderTask::SetFilterXRay( bool filterXRay ){
	pFilterXRay = filterXRay;
}

void deoglAddToPersistentRenderTask::SetXRay( bool xray ){
	pXRay = xray;
}

void deoglAddToPersistentRenderTask::SetNoShadowNone( bool noShadowNone ){
	pNoShadowNone = noShadowNone;
}

void deoglAddToPersistentRenderTask::SetFilterHoles( bool filterHoles ){
	pFilterHoles = filterHoles;
}

void deoglAddToPersistentRenderTask::SetWithHoles( bool withHoles ){
	pWithHoles = withHoles;
}

void deoglAddToPersistentRenderTask::SetFilterDoubleSided( bool filterDoubleSided ){
	pFilterDoubleSided = filterDoubleSided;
}
void deoglAddToPersistentRenderTask::SetDoubleSided( bool doubleSided ){
	pDoubleSided = doubleSided;
}

void deoglAddToPersistentRenderTask::SetForceDoubleSided( bool doubleSided ){
	pForceDoubleSided = doubleSided;
}

void deoglAddToPersistentRenderTask::SetFilterDecal( bool filterDecal ){
	pFilterDecal = filterDecal;
}

void deoglAddToPersistentRenderTask::SetDecal( bool decal ){
	pDecal = decal;
}

void deoglAddToPersistentRenderTask::SetFilterCubeFace( int cubeFace ){
	pFilterCubeFace = cubeFace;
}

void deoglAddToPersistentRenderTask::SetUseSpecialParamBlock( bool use ){
	pUseSpecialParamBlock = use;
}

void deoglAddToPersistentRenderTask::SetEnforcePipeline( const deoglPipeline *pipeline ){
	pEnforcePipeline = pipeline;
}

void deoglAddToPersistentRenderTask::SetEnforceParamBlock( const deoglSPBlockUBO *block ){
	pEnforceParamBlock = block;
}

void deoglAddToPersistentRenderTask::AddComponent( deoglPersistentRenderTaskOwner &owner,
const deoglCollideListComponent &clcomponent ){
	deoglRComponent &component = *clcomponent.GetComponent();
	if( ! component.GetParentWorld() || ! component.GetModel() ){
		return;
	}
	if( pFilterCubeFace != -1 && ! clcomponent.GetCubeFaceMaskAt( pFilterCubeFace ) ){
		return;
	}
	
	const int lodLevel = clcomponent.GetLODLevel();
	const int specialFlags = clcomponent.GetSpecialFlags();
	const deoglRModel &model = *component.GetModel();
	const deoglModelLOD &modelLOD = model.GetLODAt( lodLevel );
	const int textureCount = modelLOD.GetTextureCount();
	int i;
	
	for( i=0; i<textureCount; i++ ){
		const deoglModelTexture &texture = modelLOD.GetTextureAt( i );
		if( texture.GetFaceCount() > 0 ){
			AddComponentFaces( owner, component, i, texture.GetFirstFace(), texture.GetFaceCount(), lodLevel, specialFlags );
		}
	}
}

void deoglAddToPersistentRenderTask::AddComponentFaces( deoglPersistentRenderTaskOwner &owner,
const deoglRComponent &component, int texture, int lodLevel, int specialFlags ){
	if( ! component.GetModel() ){
		return;
	}
	
	const deoglModelTexture &t = component.GetModel()->GetLODAt( lodLevel ).GetTextureAt( texture );
	if( t.GetFaceCount() > 0 ){
		AddComponentFaces( owner, component, texture, t.GetFirstFace(), t.GetFaceCount(), lodLevel, specialFlags );
	}
}

void deoglAddToPersistentRenderTask::AddComponentFaces( deoglPersistentRenderTaskOwner &owner,
const deoglRComponent &component, int texture, int firstFace, int faceCount, int lodLevel, int specialFlags ){
	const deoglRComponentTexture &componentTexture = component.GetTextureAt( texture );
	if( componentTexture.GetIsRendered() ){
		return;
	}
	
	const deoglSkinTexture * const skinTexture = componentTexture.GetUseSkinTexture();
	if( ! skinTexture ){
		return;
	}
	if( pFilterReject( skinTexture ) ){
		return;
	}
	
	const bool doubleSided = componentTexture.GetUseDoubleSided();
	if( pFilterDoubleSided && pDoubleSided != doubleSided ){
		return;
	}
	if( pFilterDecal && pDecal != componentTexture.GetUseDecal() ){
		return;
	}
	
	// obtain render task vao and add faces
	deoglVAO * const vao = component.GetVAO( lodLevel );
	if( ! vao ){
		return;
	}
	
	deoglSkinTexturePipelinesList::ePipelineTypes pipelinesType;
	
	if( componentTexture.GetUseDecal() ){
		pipelinesType = deoglSkinTexturePipelinesList::eptDecal;
		
	}else{
		pipelinesType = deoglSkinTexturePipelinesList::eptComponent;
	}
	
	int pipelineModifier = pSkinPipelineModifier;
	if( doubleSided || pForceDoubleSided ){
		pipelineModifier |= deoglSkinTexturePipelines::emDoubleSided;
	}
	
	deoglPersistentRenderTaskVAO * const rtvao = pGetTaskVAO(
		pipelinesType, pSkinPipelineType, pipelineModifier,
		*skinTexture, componentTexture.GetTUCForPipelineType ( pSkinPipelineType ), vao );
	
	const deoglSharedSPBElement &spbElement = *componentTexture.GetSharedSPBElement();
	const deoglSharedSPBRTIGroup &group = componentTexture.GetSharedSPBRTIGroup( lodLevel );
	
	deoglPersistentRenderTaskInstance *rti = rtvao->GetInstanceWith( &group );
	if( ! rti ){
		rti = rtvao->AddInstance( &spbElement.GetSPB(), &group );
		rti->SetFirstPoint( component.GetPointOffset( lodLevel ) );
		rti->SetFirstIndex( component.GetIndexOffset( lodLevel ) + firstFace * 3 );
		rti->SetIndexCount( faceCount * 3 );
	}
	
	owner.AddSubInstance( rti->AddSubInstance( spbElement.GetIndex(), specialFlags ) );
}



// Private Functions
//////////////////////

bool deoglAddToPersistentRenderTask::pFilterReject( const deoglSkinTexture *skinTexture ) const{
	if( ! skinTexture ){
		return true;
	}
	if( pOutline ){
		if( ! skinTexture->GetHasOutline() ){
			return true;
		}
		if( pSolid != skinTexture->GetIsOutlineSolid() ){
			return true;
		}
		
	}else{
		if( pSolid != skinTexture->GetSolid() ){
			return true;
		}
	}
	
	if( pFilterRejectNoSolid( skinTexture ) ){
		return true;
	}
	return false;
}

bool deoglAddToPersistentRenderTask::pFilterRejectNoSolid( const deoglSkinTexture *skinTexture ) const{
	if( ! skinTexture ){
		return true;
	}
	if( pFilterXRay && pXRay != skinTexture->GetXRay() ){
		return true;
	}
	if( pFilterHoles && pWithHoles != skinTexture->GetHasHoles() ){
		return true;
	}
	if( pNoRendered && skinTexture->GetRendered() ){
		return true;
	}
	if( pNoShadowNone && skinTexture->GetShadowNone() ){
		return true;
	}
	if( pNoNotReflected && ! skinTexture->GetReflected() ){
		return true;
	}
	return false;
}

deoglPersistentRenderTaskVAO *deoglAddToPersistentRenderTask::pGetTaskVAO(
deoglSkinTexturePipelinesList::ePipelineTypes pipelinesType,
deoglSkinTexturePipelines::eTypes pipelineType, int pipelineModifier,
const deoglSkinTexture &skinTexture, const deoglTexUnitsConfig *tuc, const deoglVAO *vao ) const{
	// retrieve the shader and texture units configuration to use
	const deoglPipeline *pipeline = pEnforcePipeline;
	int spbInstanceIndexBase = -1, drawIDOffset = -1;
	
	if( ! pipeline ){
		const deoglSkinTexturePipeline * const skinPipeline = skinTexture.GetPipelines().
			GetAt( pipelinesType ).GetWith( pipelineType, pipelineModifier );
		if( skinPipeline ){
			pipeline = skinPipeline->GetPipeline();
			spbInstanceIndexBase = skinPipeline->GetShader()->GetTargetSPBInstanceIndexBase();
			drawIDOffset = skinPipeline->GetShader()->GetTargetDrawIDOffset();
		}
	}
	
	DEASSERT_NOTNULL( pipeline )
	
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	deoglPersistentRenderTaskPipeline *rtpipeline = pRenderTask.GetPipelineWith( pipeline );
	if( ! rtpipeline ){
		rtpipeline = pRenderTask.AddPipeline( pipeline );
		rtpipeline->SetParameterBlock( NULL );
		rtpipeline->SetSPBInstanceIndexBase( spbInstanceIndexBase );
		rtpipeline->SetDrawIDOffset( drawIDOffset );
	}
	
	deoglPersistentRenderTaskTexture *rttexture = rtpipeline->GetTextureWith( tuc );
	if( ! rttexture ){
		rttexture = rtpipeline->AddTexture( tuc );
		rttexture->SetParameterBlock( skinTexture.GetSharedSPBElement()->GetSPB().GetParameterBlock() );
	}
	
	deoglPersistentRenderTaskVAO *rtvao = rttexture->GetVAOWith( vao );
	if( ! rtvao ){
		rtvao = rttexture->AddVAO( vao );
	}
	
	return rtvao;
}
