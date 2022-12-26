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
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglLightPipelines
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightPipelines::deoglLightPipelines() :
pPrepared( false ){
}

deoglLightPipelines::~deoglLightPipelines(){
}



// Management
///////////////

const deoglLightPipeline *deoglLightPipelines::GetWith( eTypes type, int modifiers ) const{
	return pPipelines[ type ][ modifiers ];
}

const deoglLightPipeline &deoglLightPipelines::GetWithRef( deoglLightPipelines::eTypes type, int modifiers ) const{
	const deoglLightPipeline * const pipeline = pPipelines[ type ][ modifiers ];
	DEASSERT_NOTNULL( pipeline );
	return *pipeline;
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
	config.EnableDynamicStencilTest();
	config.EnableBlendAdd();
}

void deoglLightPipelines::pBasePipelineConfigGI( deoglPipelineConfiguration &config ){
	config.Reset();
	config.SetMasks( true, true, true, true, false );
	config.SetEnableScissorTest( true );
	config.EnableBlendAdd();
}

void deoglLightPipelines::pCreatePipelines( deoglRenderThread &renderThread,
deoglPipelineConfiguration &pipconf, deoglLightShaderConfig &shaconf,
deoglLightPipelines::eTypes type, int modifierMask ){
	deoglLightShaderManager &shaderManager = renderThread.GetShader().GetLightShaderManager();
	deoglPipelineManager &pipelineManager = renderThread.GetPipelineManager();
	const bool renderStereoVSLayer = renderThread.GetChoices().GetRenderStereoVSLayer();
	const bool texSha1Amb = shaconf.GetTextureShadow1Ambient();
	const bool texSha2Amb = shaconf.GetTextureShadow2Ambient();
	const GLenum cullFace = pipconf.GetCullFace();
	const GLenum flipCullFace = cullFace == GL_FRONT ? GL_BACK : GL_FRONT;
	
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
		
		// create shader and pipeline
		const deoglLightShader::Ref shader( deoglLightShader::Ref::New( shaderManager.GetShaderWith( shaconf ) ) );
		pipconf.SetShader( shader->GetShader() );
		
		pPipelines[ type ][ modifier ].TakeOver( new deoglLightPipeline( pipelineManager.GetWith( pipconf ), shader ) );
	}
}
