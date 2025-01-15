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

#include "deoglLightPipelines.h"

#include "../shader/deoglLightShaderManager.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTChoices.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglLightPipelines
//////////////////////////////

static const deoglDebugNamesEnum::sEntry vDebugNamesTypesEntries[] = {
	{ deoglLightPipelines::etNoShadow, "etNoShadow" },
	{ deoglLightPipelines::etAmbient, "etAmbient" },
	{ deoglLightPipelines::etSolid1, "etSolid1" },
	{ deoglLightPipelines::etSolid1Transp1, "etSolid1Transp1" },
	{ deoglLightPipelines::etSolid2, "etSolid2" },
	{ deoglLightPipelines::etSolid2Transp1, "etSolid2Transp1" },
	{ deoglLightPipelines::etSolid2Transp2, "etSolid2Transp2" },
	{ deoglLightPipelines::etLumSolid1, "etLumSolid1" },
	{ deoglLightPipelines::etLumSolid2, "etLumSolid2" },
	{ deoglLightPipelines::etGIRayNoShadow, "etGIRayNoShadow" },
	{ deoglLightPipelines::etGIRaySolid1, "etGIRaySolid1" },
	{ deoglLightPipelines::etGIRaySolid2, "etGIRaySolid2" },
	deoglDebugNamesEnum::EndOfList
};

const deoglDebugNamesEnum deoglLightPipelines::DebugNamesTypes(
	"deoglLightPipelines::eTypes", vDebugNamesTypesEntries );

static const deoglDebugNamesEnum::sEntry vDebugNamesModifiersEntries[] = {
	{ deoglLightPipelines::emNoAmbient, "emNoAmbient" },
	{ deoglLightPipelines::emStereo, "emStereo" },
	{ deoglLightPipelines::emTransparent, "emTransparent" },
	{ deoglLightPipelines::emFlipCullFace, "emFlipCullFace" },
	{ deoglLightPipelines::emCameraInside, "emCameraInside" },
	deoglDebugNamesEnum::EndOfList
};

const deoglDebugNamesEnumSet deoglLightPipelines::DebugNamesModifiers(
	"deoglLightPipelines::eModifiers", vDebugNamesModifiersEntries );


// Class deoglLightPipelines::cPipelineGetShaderListener
/////////////////////////////////////////////////////////////////

deoglLightPipelines::cPipelineGetShaderListener::cPipelineGetShaderListener(
	deoglBatchedShaderLoading &batched, deoglLightPipelines &lightPipelines,
	eTypes type, int modifier, deoglLightPipeline::Ref &pipeline,
	const deoglPipelineConfiguration &config) :
pBatched(batched),
pLightPipelines(lightPipelines),
pType(type),
pModifier(modifier),
pConfig(config),
pPipeline(pipeline)
{
	batched.AddPendingCompile();
}

void deoglLightPipelines::cPipelineGetShaderListener::GetShaderFinished(deoglLightShader *shader){
	if(shader){
		pConfig.SetShader(shader->GetShader());
		pPipeline.TakeOver(new deoglLightPipeline(pBatched.GetRenderThread().
			GetPipelineManager().GetWith(pConfig), shader));
		
	}else{
		pBatched.GetRenderThread().GetLogger().LogErrorFormat("%s::pCreatePipelines(%s, %s)",
				pLightPipelines.GetDebugName(), DebugNamesTypes.EntryName(pType).GetString(),
				DebugNamesModifiers.SetName(pModifier).GetString());
	}
	
	pBatched.Loaded();
	pBatched.FinishCompile(shader != nullptr);
}


// Constructor, destructor
////////////////////////////

deoglLightPipelines::deoglLightPipelines( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pPrepared( false ){
}

deoglLightPipelines::~deoglLightPipelines(){
}



// Management
///////////////

const deoglLightPipeline *deoglLightPipelines::GetWith( eTypes type, int modifiers ) const{
	return pPipelines[ type ][ modifiers ];
}

const deoglLightPipeline &deoglLightPipelines::GetWithRef( eTypes type, int modifiers ) const{
	try{
		return pPipelines[ type ][ modifiers ];
		
	}catch( ... ){
		deoglRTLogger &l = pRenderThread.GetLogger();
		l.LogErrorFormat( "%s::GetWithRef(%s, %s)",
			GetDebugName(), DebugNamesTypes.EntryName( type ).GetString(),
			DebugNamesModifiers.SetName( modifiers ).GetString() );
		int i;
		for( i=0; i<ModifiersPerType; i++ ){
			l.LogErrorFormat( "- %s: %s", DebugNamesModifiers.SetName( i ).GetString(),
				( deoglLightPipeline* )pPipelines[ type ][ i ] ? "Present" : "Absent" );
		}
		throw;
	}
}

void deoglLightPipelines::Prepare(deoglBatchedShaderLoading &batched){
	if( pPrepared ){
		return;
	}
	
	pPreparePipelines(batched);
	if(!batched.TimedOut()){
		pPrepared = true;
	}
}



// Protected Functions
////////////////////////

void deoglLightPipelines::pBasePipelineConfig(deoglRenderThread &renderThread,
deoglPipelineConfiguration &config){
	config.Reset();
	config.SetMasks( true, true, true, true, false );
	config.SetEnableScissorTest( true );
	config.SetClipControl( renderThread.GetChoices().GetUseInverseDepth() );
	config.EnableDepthTest( renderThread.GetChoices().GetDepthCompareFuncRegular() );
	config.EnableCulling( false );
	config.EnableDynamicStencilTest();
	config.EnableBlendAdd();
}

void deoglLightPipelines::pBasePipelineConfigGI(deoglPipelineConfiguration &config){
	config.Reset();
	config.SetMasks( true, true, true, true, false );
	config.EnableBlendAdd();
}

void deoglLightPipelines::pCreatePipelines(deoglRenderThread &renderThread,
const deoglPipelineConfiguration &basePipelineConfig,
const deoglLightShaderConfig &baseShaderConfig, deoglLightPipelines::eTypes type,
int modifierMask, deoglBatchedShaderLoading &batched){
	deoglLightShaderManager &shaderManager = renderThread.GetShader().GetLightShaderManager();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	const bool renderStereoVSLayer = renderThread.GetChoices().GetRenderStereoVSLayer();
	deoglPipelineConfiguration pipconf( basePipelineConfig );
	deoglLightShaderConfig shaconf( baseShaderConfig );
	const bool texSha1Amb = baseShaderConfig.GetTextureShadow1Ambient();
	const bool texSha2Amb = baseShaderConfig.GetTextureShadow2Ambient();
	const GLenum cullFace = basePipelineConfig.GetCullFace();
	const GLenum flipCullFace = cullFace == GL_FRONT ? GL_BACK : GL_FRONT;
	const GLenum depthFunc = basePipelineConfig.GetDepthFunc();
	
	int modifier;
	
	for( modifier=0; modifier<ModifiersPerType; modifier++ ){
		// skip unsupported modifiers
		if( ( modifier & modifierMask ) != modifier ){
			continue;
		}
		
		if(!batched.CanLoad()){
			return;
		}
		
		// no ambient
		if( modifier & emNoAmbient ){
			shaconf.SetTextureShadow1Ambient( false );
			shaconf.SetTextureShadow2Ambient( false );
			
		}else{
			shaconf.SetTextureShadow1Ambient( texSha1Amb );
			shaconf.SetTextureShadow2Ambient( texSha2Amb );
		}
		
		// stereo
		if( modifier & emStereo ){
			if( renderStereoVSLayer ){
				shaconf.SetVSRenderStereo( true );
				
			}else{
				shaconf.SetGSRenderStereo( true );
			}
			
		}else{
			shaconf.SetVSRenderStereo( false );
			shaconf.SetGSRenderStereo( false );
		}
		
		// transparent
		if( modifier & emTransparent ){
			pipconf.EnableBlendTranspAdd();
			
		}else{
			pipconf.EnableBlendAdd();
		}
		
		// cull mode
		pipconf.SetCullFace( modifier & emFlipCullFace ? flipCullFace : cullFace );
		
		// camera inside
		if( modifier & emCameraInside ){
			pipconf.SetDepthFunc( GL_ALWAYS );
			pipconf.SetDepthClamp( true );
			
		}else{
			pipconf.SetDepthFunc( depthFunc );
			pipconf.SetDepthClamp( false );
		}
		
		// create shader and pipeline
		if(batched.GetAsyncCompile()){
			shaderManager.GetShaderWithAsync(shaconf, new cPipelineGetShaderListener(
				batched, *this, type, modifier, pPipelines[type][modifier], pipconf));
			
		}else{
			deoglLightShader *shader;
			
			try{
				shader = shaderManager.GetShaderWith(shaconf);
				
			}catch(...){
				pRenderThread.GetLogger().LogErrorFormat("%s::pCreatePipelines(%s, %s)",
					GetDebugName(), DebugNamesTypes.EntryName(type).GetString(),
					DebugNamesModifiers.SetName(modifier).GetString());
				batched.Loaded();
				throw;
			}
			
			pipconf.SetShader(shader->GetShader());
			
			pPipelines[type][modifier].TakeOver(new deoglLightPipeline(
				pipelineManager.GetWith(pipconf), shader));
			
			batched.Loaded();
		}
	}
}
