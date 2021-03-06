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

#include "deoglREffectDistortImage.h"
#include "../../rendering/defren/deoglDeferredRendering.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../renderthread/deoglRTTexture.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderDefines.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../texture/deoglRImage.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../delayedoperation/deoglDelayedDeletion.h"
#include "../../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPEffect{
	spePosTransform,
	speTCTransform,
	speDistortTransform
};

// Class deoglREffectDistortImage
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglREffectDistortImage::deoglREffectDistortImage( deoglRenderThread &renderThread ) :
deoglREffect( renderThread ),
pImage( NULL ),
pShader( NULL ){
	LEAK_CHECK_CREATE( renderThread, EffectDistortImage );
}

class deoglREffectDistortionImageDeletion : public deoglDelayedDeletion{
public:
	deoglShaderProgram *shader;
	
	deoglREffectDistortionImageDeletion() :
	shader( NULL ){
	}
	
	virtual ~deoglREffectDistortionImageDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread &renderThread ){
		if( shader ){
			shader->RemoveUsage();
		}
	}
};

deoglREffectDistortImage::~deoglREffectDistortImage(){
	LEAK_CHECK_FREE( GetRenderThread(), EffectDistortImage );
	if( pImage ){
		pImage->FreeReference();
	}
	
	// delayed deletion of opengl containing objects
	deoglREffectDistortionImageDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglREffectDistortionImageDeletion;
		delayedDeletion->shader = pShader;
		GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		GetRenderThread().GetLogger().LogException( e );
		// throw; -> otherwise terminate
	}
}



// Management
///////////////

void deoglREffectDistortImage::SetStrength( const decVector2 &strength ){
	pStrength = strength;
}

void deoglREffectDistortImage::SetImage( deoglRImage *image ){
	if( image == pImage ){
		return;
	}
	
	if( pImage ){
		pImage->FreeReference();
	}
	
	pImage = image;
	
	if( image ){
		image->AddReference();
	}
}



deoglShaderProgram *deoglREffectDistortImage::GetShader(){
	if( ! pShader ){
		deoglShaderManager &shaderManager = GetRenderThread().GetShader().GetShaderManager();
		
		deoglShaderSources * const sources = shaderManager.GetSourcesNamed( "Effect Distort Image" );
		if( ! sources ){
			DETHROW( deeInvalidParam );
		}
		
		pShader = shaderManager.GetProgramWith( sources, deoglShaderDefines() );
	}
	
	return pShader;
}

void deoglREffectDistortImage::PrepareForRender(){
	if( pImage ){
		pImage->PrepareForRender();
	}
}

void deoglREffectDistortImage::Render( deoglRenderPlan &plan ){
	if( ! pImage ){
		return;
	}
	
	deoglTexture * const texture = pImage->GetTexture();
	if( ! texture ){
		return;
	}
	
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRTShader &rtshader = renderThread.GetShader();
	
	//int width = defren.GetWidth();
	//int height = defren.GetHeight();
	//int realHeight = defren.GetRealHeight();
	float scaleU = pImage->GetScaleFactorU();
	float scaleV = pImage->GetScaleFactorV();
	float su = pStrength.x * defren.GetScalingU(); //( float )width;
	float sv = pStrength.y * defren.GetScalingV(); //( float )height;
	
	// swap render texture
	defren.SwapPostProcessTarget();
	defren.ActivatePostProcessFBO( false );
	tsmgr.EnableTexture( 0, *defren.GetPostProcessTexture(), *rtshader.GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
	tsmgr.EnableTexture( 1, *texture, *rtshader.GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
	
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// set shader program
	// transform image
	// [-1,1] * su/2 + su/2 = [0,su]
	// [-1,1] * sv/2 + sv/2 = [0,sv]
	// transform distort
	// [0,1] * stru*su*2 - stru*su = [-strz*su,stru*su]
	// [0,1] * -strv*sv*2 + strv*sv = [strv*sv,-strv*sv]
	deoglShaderProgram * const shaderProgram = GetShader();
	rtshader.ActivateShader( shaderProgram );
	deoglShaderCompiled &shader = *shaderProgram->GetCompiled();
	
	shader.SetParameterFloat( spePosTransform, 1.0f, 1.0f, 0.0f, 0.0f );
	shader.SetParameterFloat( speTCTransform, scaleU * 0.5f, scaleV * 0.5f, scaleU * 0.5f, scaleV * 0.5f );
	shader.SetParameterFloat( speDistortTransform, 2.0f * su, -2.0f * sv, -su, sv );
	
	// TODO correctly we need an explicite clamp in the shader here since the texture
	// can be larger than the area we tap into.
	
	defren.RenderFSQuadVAO();
}
