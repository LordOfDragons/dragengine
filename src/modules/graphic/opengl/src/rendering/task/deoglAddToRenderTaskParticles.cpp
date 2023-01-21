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
#include <stdlib.h>
#include <string.h>

#include "deoglAddToRenderTaskParticles.h"
#include "deoglRenderTaskParticles.h"
#include "deoglRenderTaskParticlesStep.h"
#include "../defren/deoglDeferredRendering.h"
#include "../task/shared/deoglRenderTaskSharedInstance.h"
#include "../task/shared/deoglRenderTaskSharedTexture.h"
#include "../task/shared/deoglRenderTaskSharedVAO.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../envmap/deoglEnvironmentMap.h"
#include "../../particle/deoglRParticleEmitter.h"
#include "../../particle/deoglRParticleEmitterInstance.h"
#include "../../particle/deoglRParticleEmitterInstanceType.h"
#include "../../particle/deoglRParticleEmitterType.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/shared/deoglSharedSPB.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../skin/channel/deoglSkinChannel.h"
#include "../../skin/combinedTexture/deoglCombinedTexture.h"
#include "../../skin/dynamic/deoglRDynamicSkin.h"
#include "../../skin/shader/deoglSkinShader.h"
#include "../../skin/state/deoglSkinState.h"
#include "../../skin/state/deoglSkinStateRenderable.h"
#include "../../target/deoglRenderTarget.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglAddToRenderTaskParticles
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglAddToRenderTaskParticles::deoglAddToRenderTaskParticles(
deoglRenderThread &renderThread, deoglRenderTaskParticles *renderTask ) :
pRenderThread( renderThread ),
pRenderTask( renderTask ),
pSkinPipelineType( deoglSkinTexturePipelines::etGeometry ),
pSkinPipelineModifier( 0 ){
}

deoglAddToRenderTaskParticles::~deoglAddToRenderTaskParticles(){
}



// Management
///////////////

void deoglAddToRenderTaskParticles::SetSkinPipelineType( deoglSkinTexturePipelines::eTypes type ){
	pSkinPipelineType = type;
}

void deoglAddToRenderTaskParticles::SetSkinPipelineModifier( int modifier ){
	pSkinPipelineModifier = modifier;
}



void deoglAddToRenderTaskParticles::AddParticles( const deoglParticleEmitterInstanceList &list ){
	const int count = list.GetCount();
	int e, p;
	
	for( e=0; e<count; e++ ){
		deoglRParticleEmitterInstance &instance = *list.GetAt( e );
		const deoglRParticleEmitterInstance::sParticle * const particles = instance.GetParticles();
		const int particleCount = instance.GetParticleCount();
		
		for( p=0; p<particleCount; p++ ){
			AddParticle( instance, particles + p );
		}
	}
}

void deoglAddToRenderTaskParticles::AddParticle( deoglRParticleEmitterInstance &emitterInstance,
const deoglRParticleEmitterInstance::sParticle *particle ){
	// fetch parameters
	deoglRParticleEmitterInstanceType &itype = emitterInstance.GetTypeAt( particle->type );
	const deoglRParticleEmitterType &etype = emitterInstance.GetEmitter()->GetTypeAt( particle->type );
	const bool simulationRibbon = ( etype.GetSimulationType() == deParticleEmitterType::estRibbon );
	const bool simulationBeam = ( etype.GetSimulationType() == deParticleEmitterType::estBeam );
	
	const deoglRSkin * const skin = itype.GetUseSkin();
	const deoglRDynamicSkin * const dynamicSkin = itype.GetDynamicSkin();
	const deoglSkinTexture * const skinTexture = itype.GetUseSkinTexture();
	
	// if there is no texture we can skip the geometry attached to it
	if( ! skinTexture ){
		return;
	}
	
	// if the emitter is ribbon or beam type and the particle is the first skip it
	if( ( simulationRibbon || simulationBeam ) && particle->ribbonLine[ 0 ] == 0
	&& particle->ribbonLine[ 1 ] == 0 ){
		return;
	}
	
	// retrieve the shader and texture units configuration to use
	const deoglSkinTexturePipeline &pipeline = itype.GetUseSkinPipelines().
		GetWithRef( pSkinPipelineType, pSkinPipelineModifier );
	
	const deoglTexUnitsConfig *tuc = itype.GetTUCForPipelineType ( pSkinPipelineType );
	if( ! tuc ){
		tuc = pRenderThread.GetShader().GetTexUnitsConfigList().GetEmptyNoUsage();
	}
	
	// determine if the last step has the same texture
	const deoglVAO * const vao = emitterInstance.GetVAO();
	deoglRenderTaskParticlesStep *rtps = nullptr;
	
	if( pRenderTask->GetStepCount() > 0 ){
		rtps = pRenderTask->GetStepAt( pRenderTask->GetStepCount() - 1 );
		
		if( rtps->GetVAO() != vao
		|| rtps->GetPipeline() != pipeline.GetPipeline()
		|| rtps->GetSkin() != skin
		|| rtps->GetDynamicSkin() != dynamicSkin ){
			rtps = nullptr;
		}
	}
	
	// if there exists no matching step create a new one
	if( ! rtps ){
		rtps = pRenderTask->AddStep();
		rtps->SetPipeline( pipeline.GetPipeline() );
		rtps->SetTUC( tuc );
		rtps->SetVAO( vao );
		rtps->SetSkin( skin );
		rtps->SetDynamicSkin( dynamicSkin );
		
		rtps->SetFirstIndex( emitterInstance.GetIBOUsedIndexCount() );
		
		if( simulationRibbon || simulationBeam ){
			rtps->SetPrimitiveType( GL_LINES_ADJACENCY );
			
		}else{
			rtps->SetPrimitiveType( GL_POINTS );
		}
		
		rtps->SetParameterBlockTexture( skinTexture->GetSharedSPBElement()->GetSPB().GetParameterBlock() );
		rtps->SetParameterBlockInstance( itype.GetParamBlock() );
	}
	
	// add an instance
	rtps->AddInstance().particle = particle;
	
	// update IBO
	if( simulationRibbon || simulationBeam ){
		emitterInstance.AddIBOEntries(
			particle->particle + particle->ribbonLine[ 0 ],
			particle->particle + particle->ribbonLine[ 1 ],
			particle->particle,
			particle->particle + particle->ribbonLine[ 2 ] );
		rtps->IncrementIndexCount( 4 );
		
	}else{
		emitterInstance.AddIBOEntry( particle->particle );
		rtps->IncrementIndexCount( 1 );
	}
}
