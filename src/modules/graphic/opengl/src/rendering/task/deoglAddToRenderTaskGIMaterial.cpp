/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
#include <stdlib.h>
#include <string.h>

#include "deoglAddToRenderTaskGIMaterial.h"
#include "deoglRenderTask.h"
#include "deoglRenderTaskShader.h"
#include "deoglRenderTaskTexture.h"
#include "deoglRenderTaskVAO.h"
#include "deoglRenderTaskInstance.h"
#include "deoglRenderTaskInstanceGroup.h"
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

void deoglAddToRenderTaskGIMaterial::SetSkinShaderType( deoglSkinTexture::eShaderTypes shaderType ){
	pSkinShaderType = shaderType;
}

void deoglAddToRenderTaskGIMaterial::Reset(){
	pSkinShaderType = deoglSkinTexture::estComponentGIMaterial;
}

deoglRenderTaskTexture *deoglAddToRenderTaskGIMaterial::AddComponentTexture( deoglRComponentLOD &lod, int texture ){
	deoglRComponentTexture &componentTexture = lod.GetComponent().GetTextureAt( texture );
	deoglSkinTexture * const skinTexture = componentTexture.GetUseSkinTexture();
	if( ! skinTexture ){
		return NULL;
	}
	
	if( pFilterReject( skinTexture ) ){
		return NULL;
	}
	
	// hack style test for a camera renderable
	deoglSkinChannel *skinChannel = skinTexture->GetChannelAt( deoglSkinChannel::ectColor );
	deoglSkinState * const useSkinState = componentTexture.GetUseSkinState();
	
	if( skinChannel && useSkinState ){
		const deoglRDynamicSkin *dynamicSkin = lod.GetComponent().GetDynamicSkin();
		const int skinRenderable = skinChannel->GetRenderable();
		
		if( skinRenderable >= 0 && skinRenderable < useSkinState->GetRenderableCount() && dynamicSkin ){
			const deoglSkinStateRenderable &skinStateRenderable = *useSkinState->GetRenderableAt( skinRenderable );
			
			if( skinStateRenderable.GetHostRenderable() != -1 ){
				if( dynamicSkin->GetRenderableAt( skinStateRenderable.GetHostRenderable() )->GetRenderPlan() ){
					return NULL;
				}
			}
		}
	}
	
	// obtain render task texture
	return pGetTaskTexture( skinTexture, componentTexture.GetTUCForShaderType( pSkinShaderType ) );
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
	deoglShaderProgram *shader = NULL;
	int spbInstanceIndexBase = -1;
	
	if( ! shader ){
		deoglSkinShader * const skinShader = skinTexture->GetShaderFor( pSkinShaderType );
		if( skinShader ){
			shader = skinShader->GetShader();
			spbInstanceIndexBase = skinShader->GetTargetSPBInstanceIndexBase();
		}
	}
	
	if( ! shader ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	// obtain render task shader
	deoglRenderTaskShader *renderTaskShader;
	if( shader->GetRenderTaskTrackingNumber() == pRenderTask.GetTrackingNumber() ){
		renderTaskShader = shader->GetRenderTaskShader();
		
	}else{
		renderTaskShader = pRenderTask.AddShader( shader );
		renderTaskShader->SetParameterBlock( NULL );
		renderTaskShader->SetSPBInstanceIndexBase( spbInstanceIndexBase );
		shader->SetRenderTaskShader( renderTaskShader );
		shader->SetRenderTaskTrackingNumber( pRenderTask.GetTrackingNumber() );
	}
	
	// obtain render task texture
	if( tuc->GetRenderTaskTrackingNumber() != pRenderTask.GetTrackingNumber() ){
		pRenderTask.AddTUC( tuc );
	}
	deoglRenderTaskTexture *renderTaskTexture =
		renderTaskShader->GetTextureForIndex( tuc->GetRenderTaskTUCIndex() );
	if( ! renderTaskTexture ){
		renderTaskTexture = pRenderTask.TextureFromPool();
		renderTaskTexture->SetTUC( tuc );
		renderTaskTexture->SetParameterBlock( skinTexture->GetParameterBlock() );
		renderTaskTexture->SetTexture( skinTexture );
		renderTaskShader->AddTexture( renderTaskTexture );
	}
	
	if( tuc->GetMaterialIndex() == -1 ){
		pRenderThread.GetGI().GetMaterials().AddTUC( tuc );
	}
	
	return renderTaskTexture;
}
