/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

void deoglLightPipelines::Prepare(){
	if( ! pPrepared ){
		pPrepared = true;
		pPreparePipelines();
	}
}



// Protected Functions
////////////////////////

void deoglLightPipelines::pBasePipelineConfig( deoglRenderThread &renderThread,
deoglPipelineConfiguration &config ){
	config.Reset();
	config.SetMasks( true, true, true, true, false );
	config.SetEnableScissorTest( true );
	config.SetClipControl( renderThread.GetChoices().GetUseInverseDepth() );
	config.EnableDepthTest( renderThread.GetChoices().GetDepthCompareFuncRegular() );
	config.EnableCulling( false );
	config.EnableDynamicStencilTest();
	config.EnableBlendAdd();
}

void deoglLightPipelines::pBasePipelineConfigGI( deoglPipelineConfiguration &config ){
	config.Reset();
	config.SetMasks( true, true, true, true, false );
	config.EnableBlendAdd();
}

void deoglLightPipelines::pCreatePipelines( deoglRenderThread &renderThread,
const deoglPipelineConfiguration &basePipelineConfig,
const deoglLightShaderConfig &baseShaderConfig, deoglLightPipelines::eTypes type, int modifierMask ){
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
		const deoglLightShader::Ref shader(
			deoglLightShader::Ref::New( shaderManager.GetShaderWith( shaconf ) ) );
		
		try{
			shader->EnsureShaderExists();
			
		}catch( ... ){
			pRenderThread.GetLogger().LogErrorFormat( "%s::pCreatePipelines(%s, %s)",
				GetDebugName(), DebugNamesTypes.EntryName( type ).GetString(),
				DebugNamesModifiers.SetName( modifier ).GetString() );
			throw;
		}
		
		pipconf.SetShader( shader->GetShader() );
		
		pPipelines[ type ][ modifier ].TakeOver(
			new deoglLightPipeline( pipelineManager.GetWith( pipconf ), shader ) );
	}
}
