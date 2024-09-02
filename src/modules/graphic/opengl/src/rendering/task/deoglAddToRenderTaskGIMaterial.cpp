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

#include "deoglAddToRenderTaskGIMaterial.h"
#include "deoglRenderTask.h"
#include "deoglRenderTaskPipeline.h"
#include "deoglRenderTaskTexture.h"
#include "deoglRenderTaskVAO.h"
#include "deoglRenderTaskInstance.h"
#include "../defren/deoglDeferredRendering.h"
#include "../../collidelist/deoglCollideList.h"
#include "../../collidelist/deoglCollideListComponent.h"
#include "../../component/deoglRComponent.h"
#include "../../component/deoglRComponentLOD.h"
#include "../../component/deoglRComponentTexture.h"
#include "../../gi/deoglGI.h"
#include "../../gi/deoglGIMaterials.h"
#include "../../model/deoglModelLOD.h"
#include "../../model/deoglRModel.h"
#include "../../model/texture/deoglModelTexture.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/deoglRSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../skin/dynamic/renderables/render/deoglRDSRenderable.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../skin/state/deoglSkinStateRenderable.h"
#include "../../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglAddToRenderTaskGIMaterial
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglAddToRenderTaskGIMaterial::deoglAddToRenderTaskGIMaterial(
	deoglRenderThread &renderThread, deoglRenderTask &renderTask ) :
pRenderThread( renderThread  ),
pRenderTask( renderTask  )
{
	Reset();
}

deoglAddToRenderTaskGIMaterial::~deoglAddToRenderTaskGIMaterial(){
}



// Management
///////////////

void deoglAddToRenderTaskGIMaterial::SetSkinPipelineType( deoglSkinTexturePipelines::eTypes type ){
	pSkinPipelineType = type;
}

void deoglAddToRenderTaskGIMaterial::Reset(){
	pSkinPipelineType = deoglSkinTexturePipelines::etGIMaterial;
}

deoglRenderTaskTexture *deoglAddToRenderTaskGIMaterial::AddComponentTexture( deoglRComponentLOD &lod, int texture ){
	deoglRComponentTexture &componentTexture = lod.GetComponent().GetTextureAt( texture );
	if( componentTexture.GetIsRendered() ){
		// NOTE for this to work we have to recast the ray into the direction of the direct
		//      rendering camera. this is though quite complex to achieve fast in a ray
		//      tracer. for this reason this is skipped
		return NULL;
	}
	
	deoglSkinTexture * const skinTexture = componentTexture.GetUseSkinTexture();
	if( ! skinTexture ){
		return NULL;
	}
	
	if( pFilterReject( skinTexture ) ){
		return NULL;
	}
	
	// obtain render task texture
	return pGetTaskTexture( skinTexture, componentTexture.GetTUCForPipelineType( pSkinPipelineType ) );
}



// Private Functions
//////////////////////

bool deoglAddToRenderTaskGIMaterial::pFilterReject( const deoglSkinTexture *skinTexture ) const{
	if( ! skinTexture ){
		return true;
	}
	if( /*skinTexture->GetHasSolidity() ||*/ skinTexture->GetHasTransparency() ){
		return true;
	}
	return false;
}

deoglRenderTaskTexture *deoglAddToRenderTaskGIMaterial::pGetTaskTexture(
deoglSkinTexture *skinTexture, deoglTexUnitsConfig *tuc ){
	// retrieve the shader and texture units configuration to use
	const deoglPipeline *pipeline = nullptr;
	
	if( ! pipeline ){
		pipeline = skinTexture->GetPipelines().GetAt( deoglSkinTexturePipelinesList::eptComponent ).
			GetWithRef( pSkinPipelineType ).GetPipeline();
	}
	
	DEASSERT_NOTNULL( pipeline )
	
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	// obtain render task texture
	deoglRenderTaskPipeline &rtpipeline = *pRenderTask.AddPipeline( pipeline );
	deoglRenderTaskTexture &rttexture = *rtpipeline.AddTexture( tuc->GetRTSTexture() );
	
	if( tuc->GetMaterialIndex() == -1 ){
		pRenderThread.GetGI().GetMaterials().AddTUC( tuc );
	}
	
	return &rttexture;
}
