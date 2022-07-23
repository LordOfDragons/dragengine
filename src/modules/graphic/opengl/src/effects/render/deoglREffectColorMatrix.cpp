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

#include "deoglREffectColorMatrix.h"
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
#include "../../texture/deoglTextureStageManager.h"
#include "../../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

enum eSPEffect{
	speQuadParams,
	speColorMatrix,
	speColorOffset
};

// Class deoglREffectColorMatrix
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglREffectColorMatrix::deoglREffectColorMatrix( deoglRenderThread &renderThread ) :
deoglREffect( renderThread )
{
	LEAK_CHECK_CREATE( renderThread, EffectColorMatrix );
}

deoglREffectColorMatrix::~deoglREffectColorMatrix(){
	LEAK_CHECK_FREE( GetRenderThread(), EffectColorMatrix );
}



// Management
///////////////

void deoglREffectColorMatrix::SetColorMatrix( const decColorMatrix &colorMatrix ){
	pColorMatrix = colorMatrix;
}



deoglShaderProgram *deoglREffectColorMatrix::GetShader(){
	if( ! pShader ){
		deoglShaderManager &shaderManager = GetRenderThread().GetShader().GetShaderManager();
		
		deoglShaderSources * const sources = shaderManager.GetSourcesNamed( "Effect Color Matrix" );
		if( ! sources ){
			DETHROW( deeInvalidParam );
		}
		
		pShader = shaderManager.GetProgramWith( sources, deoglShaderDefines() );
	}
	
	return pShader;
}

void deoglREffectColorMatrix::Render( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRTShader &rtshader = renderThread.GetShader();
	decColorMatrix colorMatrix;
	
	// gamma correct
	/*
	// no good. the matrix values can be as well color values which should
	// be gamma corrected but on the other side they can also be real
	// matrix values which should not be gamma corrected. another problem
	// is that negative values are valid but gamma correction on negative
	// values does not work. so right now we do not do gamma correction as
	// we assume the matrix is a value only matrix. a better solution has
	// to be found.
	
	paramMatrix.a11 = powf( colorMatrix.a11, OGL_RENDER_GAMMA );
	paramMatrix.a12 = powf( colorMatrix.a12, OGL_RENDER_GAMMA );
	paramMatrix.a13 = powf( colorMatrix.a13, OGL_RENDER_GAMMA );
	paramOffset.x = powf( colorMatrix.a15, OGL_RENDER_GAMMA );
	
	paramMatrix.a21 = powf( colorMatrix.a21, OGL_RENDER_GAMMA );
	paramMatrix.a22 = powf( colorMatrix.a22, OGL_RENDER_GAMMA );
	paramMatrix.a23 = powf( colorMatrix.a23, OGL_RENDER_GAMMA );
	paramOffset.y = powf( colorMatrix.a25, OGL_RENDER_GAMMA );
	
	paramMatrix.a31 = powf( colorMatrix.a31, OGL_RENDER_GAMMA );
	paramMatrix.a32 = powf( colorMatrix.a32, OGL_RENDER_GAMMA );
	paramMatrix.a33 = powf( colorMatrix.a33, OGL_RENDER_GAMMA );
	paramOffset.z = powf( colorMatrix.a35, OGL_RENDER_GAMMA );
	
	paramMatrix.a41 = powf( colorMatrix.a41, OGL_RENDER_GAMMA );
	paramMatrix.a42 = powf( colorMatrix.a42, OGL_RENDER_GAMMA );
	paramMatrix.a43 = powf( colorMatrix.a43, OGL_RENDER_GAMMA );
	*/
	
	colorMatrix = pColorMatrix;
	
	// set attachments
	defren.SwapPostProcessTarget();
	defren.ActivatePostProcessFBO( false );
	tsmgr.EnableArrayTexture( 0, *defren.GetPostProcessTexture(),
		*rtshader.GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
	
	// set states
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// set program
	deoglShaderProgram * const shaderProgram = GetShader();
	rtshader.ActivateShader( shaderProgram );
	deoglShaderCompiled &shader = *shaderProgram->GetCompiled();
	
	defren.SetShaderParamFSQuad( shader, speQuadParams );
	shader.SetParameterColorMatrix5x4( speColorMatrix, speColorOffset, colorMatrix );
	
	defren.RenderFSQuadVAO();
}
