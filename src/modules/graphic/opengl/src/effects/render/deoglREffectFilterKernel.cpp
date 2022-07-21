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

#include "deoglREffectFilterKernel.h"
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
	speOptions,
	speKernel1,
	speKernel2,
	speKernel3
};

enum eSPEffectDownSample{
	spedsQuadParams,
	spedsTCClamp
};

// Class deoglREffectFilterKernel
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglREffectFilterKernel::deoglREffectFilterKernel( deoglRenderThread &renderThread ) :
deoglREffect( renderThread ),
pKernel( NULL ),
pKernelRows( 0 ),
pKernelCols( 0 ),
pScale( 1.0f ),
pShader( NULL ),
pShaderDownsample( NULL )
{
	LEAK_CHECK_CREATE( renderThread, EffectFilterKernel );
}

deoglREffectFilterKernel::~deoglREffectFilterKernel(){
	LEAK_CHECK_FREE( GetRenderThread(), EffectFilterKernel );
	if( pKernel ){
		delete [] pKernel;
	}
	if( pShaderDownsample ){
		pShaderDownsample->RemoveUsage();
	}
	if( pShader ){
		pShader->RemoveUsage();
	}
}



// Management
///////////////

void deoglREffectFilterKernel::SetKernelSize( int rows, int cols ){
	if( rows < 1 || cols < 1 ){
		DETHROW( deeInvalidParam );
	}
	if( ( rows % 2 ) == 0 || ( cols % 2 ) == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( pKernel ){
		delete [] pKernel;
	}
	pKernel = new float[ rows * cols ];
	pKernelRows = rows;
	pKernelCols = cols;
}

float deoglREffectFilterKernel::GetKernelValueAt( int row, int col ) const{
	if( row < 0 || row >= pKernelRows ){
		DETHROW( deeInvalidParam );
	}
	if( col < 0 || col >= pKernelCols ){
		DETHROW( deeInvalidParam );
	}
	return pKernel[ row * pKernelCols + col ];
}

void deoglREffectFilterKernel::SetKernelValueAt( int row, int col, float value ){
	if( row < 0 || row >= pKernelRows ){
		DETHROW( deeInvalidParam );
	}
	if( col < 0 || col >= pKernelCols ){
		DETHROW( deeInvalidParam );
	}
	pKernel[ row * pKernelCols + col ] = value;
}

void deoglREffectFilterKernel::SetScale( float scale ){
	pScale = scale;
}



deoglShaderProgram *deoglREffectFilterKernel::GetShader(){
	if( ! pShader ){
		deoglShaderManager &shaderManager = GetRenderThread().GetShader().GetShaderManager();
		
		deoglShaderSources * const sources = shaderManager.GetSourcesNamed( "Effect Filter Kernel" );
		if( ! sources ){
			DETHROW( deeInvalidParam );
		}
		
		pShader = shaderManager.GetProgramWith( sources, deoglShaderDefines() );
	}
	
	return pShader;
}

deoglShaderProgram *deoglREffectFilterKernel::GetShaderDownsample(){
	if( ! pShaderDownsample ){
		deoglShaderManager &shaderManager = GetRenderThread().GetShader().GetShaderManager();
		
		deoglShaderSources * const sources = shaderManager.GetSourcesNamed( "Effect Filter Kernel DownSample" );
		if( ! sources ){
			DETHROW( deeInvalidParam );
		}
		
		pShaderDownsample = shaderManager.GetProgramWith( sources, deoglShaderDefines() );
	}
	
	return pShaderDownsample;
}

void deoglREffectFilterKernel::Render( deoglRenderPlan &plan ){
	deoglRenderThread &renderThread = GetRenderThread();
	deoglTextureStageManager &tsmgr = renderThread.GetTexture().GetStages();
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	deoglRTShader &rtshader = renderThread.GetShader();
	
	if( pKernelRows != 3 || pKernelCols != 3 ){
		return;
	}
	
	// opengl parameters
	OGL_CHECK( renderThread, glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE ) );
	OGL_CHECK( renderThread, glDepthMask( GL_FALSE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_DEPTH_TEST ) );
		
	OGL_CHECK( renderThread, glDisable( GL_CULL_FACE ) );
	
	OGL_CHECK( renderThread, glDisable( GL_BLEND ) );
	
	// source texture in case downsampling is required
	defren.SwapPostProcessTarget();
	deoglArrayTexture *sourceTexture = defren.GetPostProcessTexture();
	
	// down sample if required
	int height = defren.GetHeight();
	int width = defren.GetWidth();
	int downsampleCount = 0;
	float downsampleSize = pScale;
	while( downsampleSize > 1.5f ){
		downsampleCount++;
		downsampleSize *= 0.5f;
	}
	
	if( downsampleCount > 0 ){
		int i;
		
		deoglShaderProgram * const shaderProgramDownsample = GetShaderDownsample();
		rtshader.ActivateShader( shaderProgramDownsample );
		deoglShaderCompiled &shaderDownsample = *shaderProgramDownsample->GetCompiled();
		
		defren.SetShaderParamFSQuad( shaderDownsample, spedsQuadParams );
		
		for( i=0; i<downsampleCount; i++ ){
			tsmgr.EnableArrayTexture( 0, *sourceTexture, *rtshader.GetTexSamplerConfig( deoglRTShader::etscClampNearest ) );
			
			if( ( i % 2 ) == 0 ){
				defren.ActivateFBOReflectivity( false );
				sourceTexture = defren.GetTextureReflectivity();
				
			}else{
				defren.ActivateFBOTemporary1( false );
				sourceTexture = defren.GetTextureTemporary1();
			}
			
			shaderDownsample.SetParameterInt( spedsTCClamp, width - 1, height - 1 );
			
			if( width % 2 > 0 ){
				width = ( width >> 1 ) + 1;
				
			}else{
				width >>= 1;
			}
			if( width < 1 ){
				width = 1;
			}
			
			if( height % 2 > 0 ){
				height = ( height >> 1 ) + 1;
				
			}else{
				height >>= 1;
			}
			if( height < 1 ){
				height = 1;
			}
			
			OGL_CHECK( renderThread, glViewport( 0, 0, width, height ) );
			
			defren.RenderFSQuadVAO();
		}
		
		OGL_CHECK( renderThread, glViewport( 0, 0, defren.GetWidth(), defren.GetHeight() ) );
	}
	
	// set shader program
	defren.ActivatePostProcessFBO( false );
	tsmgr.EnableArrayTexture( 0, *sourceTexture, *rtshader.GetTexSamplerConfig( deoglRTShader::etscClampLinear ) );
	
	// set program
	deoglShaderProgram * const shaderProgram = GetShader();
	rtshader.ActivateShader( shaderProgram );
	deoglShaderCompiled &shader = *shaderProgram->GetCompiled();
	
	//defren.SetShaderParamFSQuad( shader, speQuadParams );
	defren.SetShaderParamFSQuad( shader, speQuadParams, width, height );
	shader.SetParameterFloat( speOptions, ( float )pKernelRows, ( float )pKernelCols,
		defren.GetPixelSizeU() * downsampleSize/*pScale*/, defren.GetPixelSizeV() * downsampleSize/*pScale*/ );
	shader.SetParameterFloat( speKernel1, GetKernelValueAt( 0, 0 ), GetKernelValueAt( 0, 1 ), GetKernelValueAt( 0, 2 ) );
	shader.SetParameterFloat( speKernel2, GetKernelValueAt( 1, 0 ), GetKernelValueAt( 1, 1 ), GetKernelValueAt( 1, 2 ) );
	shader.SetParameterFloat( speKernel3, GetKernelValueAt( 2, 0 ), GetKernelValueAt( 2, 1 ), GetKernelValueAt( 2, 2 ) );
	
	defren.RenderFSQuadVAO();
}
