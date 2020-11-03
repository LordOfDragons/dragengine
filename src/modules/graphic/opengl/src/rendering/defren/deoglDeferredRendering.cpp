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

#include "deoglDeferredRendering.h"
#include "deoglDRDepthMinMax.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../extensions/deoglExtensions.h"
#include "../../extensions/deoglExtResult.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../texture/deoglRenderbuffer.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



/*
Texture Configurations

- Depth 1:                  depth/stencil [4]
- Depth 2:                  depth/stencil [4]
- Encoded Depth 1:          float rgba [8]
- Encoded Depth 2:          float rgba [8]
- Diffuse:                  float rgba [8]
- Normal:                   float rgba [8]
- Reflectivity:               float rgba [8]
- Temporary:                float rgba [8]
- Color:                    float rgba [8]
- Transparency Counter 1:   r [1]
- Transparency Counter 2:   r [1]
=> width * height * 50
=> 1680 x 1050 => 88.2 MB
=> 1024 x 768 => 39.3 MB

- Depth 1:                  depth/stencil [4]
- Depth 2:                  depth/stencil [4]
- Encoded Depth 1:          float rgba [8]
- Encoded Depth 2:          float rgba [8]
- Diffuse:                  rgba (diffuse.r, diffuse.g, diffuse.b, transparency) [4]
- Normal:                   rgb (normal.x, normal.y, normal.z) [3] {perhaps encoded normal rg [2]}
- Reflectivity:             rgb (reflectivity.r, reflectivity.g, reflectivity.b) [3] {perhaps rgb5 [2]}
- Roughness:                rgb (roughness, n/a, n/a) [3]
- AOSolidity:               rgb (ambientOcclusion, ssao, solidity) [3]
- Color:                    float rgba [8]
- Temporary1:               float rgb [6]
- Temporary2:               float rgb [6]
- Temporary3:               r [1]
=> width * height * 40
=> 1680 x 1050 => 70.5 MB (80%)
=> 1024 x 768 => 31.5 MB  (80%)

Tone mapping color key calculation (requires float):
	- Temporary1
	- Temporary2
Tone mapping bloom (requires float):
	- Temporary1
	- Temporary2
Transparency counting (requires int):
	- Diffuse
	- Reflectivity
Post-Processing before tone-mapping:
	- Color
	- Temporary1
Post-Processing after tone-mapping:
	- Diffuse
	- Reflectivity
Transparency Counter:
	- Diffuse
	- Reflectivity
Transparency:
	- Temporary1 (contains last layer color copy, requires RGB float)
Geometry Pass:
	- Temporary2 (contains reflection color, requires RGB float)
ScreenSpace AO:
	- Roughness.g (screen space rendering)
	- Temporary3 (temporary blur result)
	- Roughness.g (second blur result)


normal encoding for deferred rendering using http://en.wikipedia.org/wiki/Lambert_azimuthal_equal-area_projection :

// the slight offset of the 8.0 constants prevents div-by-zero for unlikely normals pointing straight away from the camera (0,0,1)
encoding( normal ):
	float f = sqrt( 8.0001 - 7.9999 * normal.z );
	return normal.xy / vec2( f ) + vec2( 0.5 );

decoding( encodedNormal ):
	vec2 fenc = encodedNormal * vec2( 4.0 ) - vec2( 2.0 );
	float f = dot( fenc, fenc );
	float g = sqrt( 1.0 - f * 0.25 );
	return vec3( fenc.xy * vec2( g ), f * 0.5 - 1.0 );



python testing:

def quantize( v, q ):
	return [ math.floor( x * float(q) ) / float(q) for x in v ]

def normalForZ( z ):
	return [ math.sqrt(1.0 - z*z), 0.0, z ]

def encodeNormal( normal, q ):
	f = math.sqrt( 8.0001 - 7.9999 * normal[2] )
	return quantize( [ x / f + 0.5 for x in normal[0:2] ], q )

def decodeNormal( enormal ):
	fenc = [ x * 4.0 - 2.0 for x in enormal ]
	f = fenc[0]*fenc[0] + fenc[1]*fenc[1]
	g = math.sqrt( 1.0 - f / 4.0 )
	return [ fenc[0]*g, fenc[1]*g, f*0.5 - 1.0 ]

def verror( v ):
	return math.sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] )

def test( count, q ):
	for i in range( count ):
		orgNormal = normalForZ( -1.0 + float( i ) / float( count ) * 2.0 )
		enormal = encodeNormal( orgNormal, q )
		decNormal = decodeNormal( enormal )
		diffNormal = [ math.fabs( decNormal[j] - orgNormal[j] ) for j in range( 3 ) ]
		print( '{:3d}: n1=({: .6f},{: .6f},{: .6f}) n2=({: .6f},{: .6f},{: .6f}) diff=({: .6f},{: .6f},{: .6f}) err={: .6f}'.format(
			i, orgNormal[0], orgNormal[1], orgNormal[2], decNormal[0], decNormal[1], decNormal[2],
			diffNormal[0], diffNormal[1], diffNormal[2], verror( diffNormal ) ) )

def test2( count, q ):
	for i in range( count ):
		orgNormal = normalForZ( -1.0 + float( i ) / float( count ) * 2.0 )
		enormal = quantize( [ x * 0.5 + 0.5 for x in orgNormal ], q )
		decNormal = [ x * 2.0 - 1.0 for x in enormal ]
		diffNormal = [ math.fabs( decNormal[j] - orgNormal[j] ) for j in range( 3 ) ]
		print( '{:3d}: n1=({: .6f},{: .6f},{: .6f}) n2=({: .6f},{: .6f},{: .6f}) diff=({: .6f},{: .6f},{: .6f}) err={: .6f}'.format(
			i, orgNormal[0], orgNormal[1], orgNormal[2], decNormal[0], decNormal[1], decNormal[2],
			diffNormal[0], diffNormal[1], diffNormal[2], verror( diffNormal ) ) )
*/



// Definitions
////////////////

enum eFBOMappingsEncodedDepth{
	/** Depth1 with RenBuf. */
	efbomedDepth1RB,
	/** Depth2 with RenBuf. */
	efbomedDepth2RB,
	/** Depth3 with RenBuf. */
	efbomedDepth3RB,
	/** Diffuse. */
	efbomedDiff,
	/** Diffuse with RenBuf. */
	efbomedDiffRB,
	/** Reflectivity. */
	efbomedRefl,
	/** Reflectivity with RenBuf. */
	efbomedReflRB,
	/** Roughness. */
	efbomedRough,
	/** Roughness with RenBuf. */
	efbomedRoughRB,
	/** AO-Solidity. */
	efbomedAOSolidity,
	/** Ao-Solidity with RenBuf. */
	efbomedAOSolidityRB,
	/** Temporary1 with RenBuf. */
	efbomedTemp1RB,
	/** Temporary2 with RenBuf. */
	efbomedTemp2RB,
	/** Temporary3 with RenBuf. */
	efbomedTemp3,
	/** Color with RenBuf. */
	efbomedColorRB,
	/** Depth1, Diffuse with RenBuf. */
	efbomedDepth1DiffRB,
	/** Depth2, Diffuse with RenBuf. */
	efbomedDepth2DiffRB,
	/** Depth1, Temporary1 with RenBuf. */
	efbomedDepth1Temp1RB,
	/** Depth2, Temporary1 with RenBuf. */
	efbomedDepth2Temp1RB,
	/** Color, Depth1 with RenBuf. */
	efbomedColorDepth1RB,
	/** Color, Depth2 with RenBuf. */
	efbomedColorDepth2RB,
	/** Color, tmeporary2 with RenBuf. */
	efbomedColorTemp2RB,
	/** Diffuse, Normal, Specularity, Color with RenBuf. */
	efbomedMaterialColorRB,
	/** Diffuse, Normal, Specularity, Temporary1 with RenBuf. */
	EFBOMED_COUNT
};

enum eFBOMappingsDepth{
	/** No-Color with Depth1. */
	efbomdD1,
	/** No-Color with Depth2. */
	efbomdD2,
	/** \brief No-Color with Depth3. */
	efbomdD3,
	/** Diffuse. */
	efbomdDiff,
	/** Diffuse with Depth1. */
	efbomdDiffD1,
	/** Diffuse with Depth2. */
	efbomdDiffD2,
	/** Reflectivity. */
	efbomdRefl,
	/** Reflectivity with Depth1. */
	efbomdReflD1,
	/** Reflectivity with Depth2. */
	efbomdReflD2,
	/** Roughness. */
	efbomdRough,
	/** Roughness with Depth1. */
	efbomdRoughD1,
	/** Roughness with Depth2. */
	efbomdRoughD2,
	/** Ao-Solidity. */
	efbomdAOSolidity,
	/** AO-Solidity with Depth1. */
	efbomdAOSolidityD1,
	/** AO-Solidity with Depth2. */
	efbomdAOSolidityD2,
	/** Temporary1 without depth. */
	efbomdTemp1,
	/** Temporary1 with Depth1. */
	efbomdTemp1D1,
	/** Temporary1 with Depth2. */
	efbomdTemp1D2,
	/** Temporary2 without depth. */
	efbomdTemp2,
	/** Temporary2 with Depth1. */
	efbomdTemp2D1,
	/** Temporary2 with Depth2. */
	efbomdTemp2D2,
	/** Temporary3 without depth. */
	efbomdTemp3,
	/** Color without depth. */
	efbomdColor,
	/** Color with Depth1. */
	efbomdColorD1,
	/** Color with Depth2. */
	efbomdColorD2,
	/** Color, temporary1 without depth. */
	efbomdColorTemp2,
	/** Color, temporary1 with Depth1. */
	efbomdColorTemp2D1,
	/** Color, temporary1 with Depth2. */
	efbomdColorTemp2D2,
	/** Diffuse, Normal, Specularity, Color with Depth1. */
	efbomdMaterialColorD1,
	/** Diffuse, Normal, Specularity, Color with Depth2. */
	efbomdMaterialColorD2,
	/** Number of FBOs used. */
	EFBOMD_COUNT
};

#define FBO_COUNT 30



// Class deoglDeferredRendering
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDeferredRendering::deoglDeferredRendering( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pTextureLuminance( NULL ),
pTextureLuminanceNormal( NULL ),
pTextureLuminanceDepth( NULL ),
pFBOLuminance( NULL ),
pFBOLuminanceNormal( NULL )
{
	const GLfloat fsquad[ 12 ] = {
		-1.0f,  1.0f,
		 1.0f,  1.0f,
		 1.0f, -1.0f,
		-1.0f, -1.0f,
		-1.0f,  1.0f, // for 2-triangle only
		 1.0f, -1.0f  // for 2-triangle only
	};
	const GLfloat billboard[ 66 ] = {
		-1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f, // for 2-triangle only
		 1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f  // for 2-triangle only
	};
	int i;
	
	pWidth = 0;
	pHeight = 0;
	pRealWidth = 0;
	pRealHeight = 0;
	pScalingU = 1.0f;
	pScalingV = 1.0f;
	pPixelSizeU = 1.0f;
	pPixelSizeV = 1.0f;
	pFSQuadOffU = 0.0f;
	pFSQuadOffV = 0.0f;
	
	pUseEncodedDepth = renderThread.GetConfiguration().GetDefRenEncDepth();
	
	pUseInverseDepth = renderThread.GetConfiguration().GetUseInverseDepth();
	if( ! renderThread.GetCapabilities().GetFormats().GetUseFBOTex2DFormatFor( deoglCapsFmtSupport::eutfDepthF_Stencil )
	||  ! renderThread.GetCapabilities().GetFormats().GetUseFBOTex2DFormatFor( deoglCapsFmtSupport::eutfDepthF )
	||  ! renderThread.GetCapabilities().GetFormats().GetUseFBOTexCubeFormatFor( deoglCapsFmtSupport::eutfDepthF_Stencil )
	||  ! renderThread.GetCapabilities().GetFormats().GetUseFBOTexCubeFormatFor( deoglCapsFmtSupport::eutfDepthF )
	||  ! pglClipControl
	||  pUseEncodedDepth ){
		pUseInverseDepth = false; // not supported
	}
	
	pUseFadeOutRange = pUseInverseDepth;
	
	renderThread.GetLogger().LogInfoFormat( "DefRen: Use Inverse Depth = %s", pUseInverseDepth ? "on" : "off" );
	
	if( pUseInverseDepth ){
		pDepthCompareFuncRegular = GL_GEQUAL;
		pDepthCompareFuncReversed = GL_LEQUAL;
		pClearDepthValueRegular = ( GLfloat )0.0;
		pClearDepthValueReversed = ( GLfloat )1.0;
		
	}else{
		pDepthCompareFuncRegular = GL_LEQUAL;
		pDepthCompareFuncReversed = GL_GEQUAL;
		pClearDepthValueRegular = ( GLfloat )1.0;
		pClearDepthValueReversed = ( GLfloat )0.0;
	}
	
	pRenderbuffer = NULL;
	pTextureDepth1 = NULL;
	pTextureDepth2 = NULL;
	pTextureDepth3 = NULL;
	
	pTextureDiffuse = NULL;
	pTextureNormal = NULL;
	pTextureReflectivity = NULL;
	pTextureRoughness = NULL;
	pTextureAOSolidity = NULL;
	pTextureSubSurface = NULL;
	pTextureTemporary1 = NULL;
	pTextureTemporary2 = NULL;
	pTextureTemporary3 = NULL;
	pTextureColor = NULL;
	
	for( i=0; i<FBO_COUNT; i++ ){
		pFBOs[ i ] = NULL;
	}
	pFBOMipMapDepth1 = NULL;
	pFBOMipMapDepth2 = NULL;
	pFBOMipMapTemporary1 = NULL;
	pFBOMipMapTemporary2 = NULL;
	pFBOMipMapDepthCount = 0;
	pFBOMipMapCount = 0;
	pModeDepth = true;
	pModePostProcess = true;
	
	pDepthMinMax = NULL;
	
	pMemoryUsageGPU = 0;
	pMemoryUsageGPUTexture = 0;
	pMemoryUsageGPURenBuf = 0;
	
	pVBOFullScreenQuad = 0;
	pVBOBillboard = 0;
	pVAOFullScreenQuad = NULL;
	pVAOBillboard = NULL;
	
	try{
		pCreateTextures();
		
		// full screen quad vao
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBOFullScreenQuad ) );
		if( ! pVBOFullScreenQuad ){
			DETHROW( deeOutOfMemory );
		}
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOFullScreenQuad ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( fsquad ), ( const GLvoid * )&fsquad, GL_STATIC_DRAW ) );
		
		pVAOFullScreenQuad = new deoglVAO( renderThread );
		OGL_CHECK( renderThread, pglBindVertexArray( pVAOFullScreenQuad->GetVAO() ) );
		
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 0 ) );
		OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, ( const GLvoid * )0 ) );
		
		// billboard vao
		OGL_CHECK( renderThread, pglGenBuffers( 1, &pVBOBillboard ) );
		if( ! pVBOBillboard ){
			DETHROW( deeOutOfMemory );
		}
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOBillboard ) );
		OGL_CHECK( renderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( billboard ), ( const GLvoid * )&billboard, GL_STATIC_DRAW ) );
		
		pVAOBillboard = new deoglVAO( renderThread );
		OGL_CHECK( renderThread, pglBindVertexArray( pVAOBillboard->GetVAO() ) );
		
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 0 ) );
		OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 44, ( const GLvoid * )0 ) );
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 1 ) );
		OGL_CHECK( renderThread, pglVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 44, ( const GLvoid * )12 ) );
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 2 ) );
		OGL_CHECK( renderThread, pglVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 44, ( const GLvoid * )12 ) );
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 3 ) );
		OGL_CHECK( renderThread, pglVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, 44, ( const GLvoid * )24 ) );
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 4 ) );
		OGL_CHECK( renderThread, pglVertexAttribPointer( 4, 2, GL_FLOAT, GL_FALSE, 44, ( const GLvoid * )36 ) );
		
		OGL_CHECK( renderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		OGL_CHECK( renderThread, pglBindVertexArray( 0 ) );
		
		// enfore a minimum size
		Resize( 64, 64 );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDeferredRendering::~deoglDeferredRendering(){
	pCleanUp();
}



// Management
///////////////

void deoglDeferredRendering::Resize( int width, int height ){
	if( width < 1 || height < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	int textureWidth, textureHeight;
	int neededWidth, neededHeight;
	
	// if the size is somewhere larger resize the textures
	if( width > pRealWidth || height > pRealHeight ){
		// NOTE for the case we ever want to resize down to consume memory in the future a minimum
		//      size of 64 has ot be kept for certain render code to still function properly
		
		// destroy the old depth min-max since we create a new one
		if( pDepthMinMax ){
			delete pDepthMinMax;
			pDepthMinMax = NULL;
		}
		
		// detach everything from the framebuffer
		pDestroyFBOs();
		
		pRenderThread.GetFramebuffer().Activate( NULL );
		
		// determine the larger sizes for each direction
		if( width > pWidth ){
			neededWidth = width;
		}else{
			neededWidth = pWidth;
		}
		if( height > pHeight ){
			neededHeight = height;
		}else{
			neededHeight = pHeight;
		}
		
		if( pRenderThread.GetConfiguration().GetDefRenUsePOTs() ){
			for( textureWidth=8; textureWidth<neededWidth; textureWidth<<=1 );
			for( textureHeight=8; textureHeight<neededHeight; textureHeight<<=1 );
			
		}else{
			textureWidth = neededWidth;
			textureHeight = neededHeight;
		}
		
		pRenderThread.GetLogger().LogInfoFormat( "DefRen: Resizing buffers to %dx%d", textureWidth, textureHeight );
		
		// resize renderbuffer
		pRenderbuffer->SetSize( textureWidth, textureHeight );
		pRenderbuffer->CreateBuffer();
		
		// resize textures
		pTextureDepth1->SetSize( textureWidth, textureHeight );
		pTextureDepth1->CreateTexture();
		pTextureDepth2->SetSize( textureWidth, textureHeight );
		pTextureDepth2->CreateTexture();
		pTextureDepth3->SetSize( textureWidth, textureHeight );
		pTextureDepth3->CreateTexture();
		
		pTextureDiffuse->SetSize( textureWidth, textureHeight );
		pTextureDiffuse->CreateTexture();
		pTextureNormal->SetSize( textureWidth, textureHeight );
		pTextureNormal->CreateTexture();
		pTextureReflectivity->SetSize( textureWidth, textureHeight );
		pTextureReflectivity->CreateTexture();
		pTextureRoughness->SetSize( textureWidth, textureHeight );
		pTextureRoughness->CreateTexture();
		pTextureAOSolidity->SetSize( textureWidth, textureHeight );
		pTextureAOSolidity->CreateTexture();
		pTextureSubSurface->SetSize( textureWidth, textureHeight );
		pTextureSubSurface->CreateTexture();
		
		pTextureTemporary1->SetSize( textureWidth, textureHeight );
		pTextureTemporary1->CreateTexture();
		pTextureTemporary2->SetSize( textureWidth, textureHeight );
		pTextureTemporary2->CreateTexture();
		pTextureTemporary3->SetSize( textureWidth, textureHeight );
		pTextureTemporary3->CreateTexture();
		
		pTextureColor->SetSize( textureWidth, textureHeight );
		pTextureColor->CreateTexture();
		
		// recreate depth min-max texture
		if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
			int minMaxWidth, minMaxHeight;
			for( minMaxWidth=1; minMaxWidth<(textureWidth>>1); minMaxWidth<<=1 );
			for( minMaxHeight=1; minMaxHeight<(textureHeight>>1); minMaxHeight<<=1 );
			pDepthMinMax = new deoglDRDepthMinMax( pRenderThread, minMaxWidth, minMaxHeight, 11 );
		}
		
		// store the new texture size
		pRealWidth = textureWidth;
		pRealHeight = textureHeight;
		
		pUpdateMemoryUsage();
		
		// create the fbos
		pCreateFBOs();
	}
	
	// store the new size and update the scalings
	pWidth = width;
	pHeight = height;
	
	pPixelSizeU = 1.0f / ( float )pRealWidth;
	pPixelSizeV = 1.0f / ( float )pRealHeight;
	pFSQuadOffU = 0.5f / ( float )pRealWidth;
	pFSQuadOffV = 0.5f / ( float )pRealHeight;
	pScalingU = ( float )width / ( float )pRealWidth;
	pScalingV = ( float )height / ( float )pRealHeight;
	pClampU = ( float )( width - 1 ) / ( float )pRealWidth;
	pClampV = ( float )( height - 1 ) / ( float )pRealHeight;
}

void deoglDeferredRendering::ForceResize(){
	const int width = pWidth;
	const int height = pHeight;
	
	pWidth = 0;
	pHeight = 0;
	pRealWidth = 0;
	pRealHeight = 0;
	
	Resize( width, height );
}

deoglTexture *deoglDeferredRendering::GetDepthTexture1() const{
	return pModeDepth ? pTextureDepth1 : pTextureDepth2;
}

deoglTexture *deoglDeferredRendering::GetDepthTexture2() const{
	return pModeDepth ? pTextureDepth2 : pTextureDepth1;
}

void deoglDeferredRendering::SwapDepthTextures(){
	pModeDepth = ! pModeDepth;
}

void deoglDeferredRendering::CopyFirstDepthToSecond( bool copyDepth, bool copyStencil ){
	if( ! copyDepth && ! copyStencil ){
		return;
	}
	
	// to copy the depth we have to assign two framebuffers to GL_READ_FRAMEBUFFER and GL_DRAW_FRAMEBUFFER.
	// the current frame buffer has been set using GL_FRAMEBUFFER. this implicitely sets GL_READ_FRAMEBUFFER
	// and GL_DRAW_FRAMEBUFFER to the same framebuffer. we have thus to only alter the GL_DRAW_FRAMEBUFFER
	// to another framebuffer containing the second depth buffer and back to the previous framebuffer once
	// we are done with the copy.
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	int mask = 0, restoreBindTarget;
	
	if( pUseEncodedDepth ){
		// the depth and stencil is reused between the first and second framebuffer. we have to only copy
		// the depth texture in the first attachment slot. this requires though also setting the
		// GL_READ_FRAMEBUFFER to make sure no wrong color copies can happen (the specs are totally
		// vague about all this).
		
		if( ! copyDepth ){
			return;
		}
		
		if( pModeDepth ){
			OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_READ_FRAMEBUFFER, pFBOs[ efbomedDepth1RB ]->GetFBO() ) );
			OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, pFBOs[ efbomedDepth2RB ]->GetFBO() ) );
			
		}else{
			OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_READ_FRAMEBUFFER, pFBOs[ efbomedDepth2RB ]->GetFBO() ) );
			OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, pFBOs[ efbomedDepth1RB ]->GetFBO() ) );
		}
		
		mask = GL_COLOR_BUFFER_BIT;
		restoreBindTarget = GL_FRAMEBUFFER;
		
	}else{
		if( pModeDepth ){
			OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, pFBOs[ efbomdD2 ]->GetFBO() ) );
			
		}else{
			OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, pFBOs[ efbomdD1 ]->GetFBO() ) );
		}
		
		if( copyDepth ){
			mask |= GL_DEPTH_BUFFER_BIT;
		}
		if( copyStencil ){
			mask |= GL_STENCIL_BUFFER_BIT;
		}
		restoreBindTarget = GL_DRAW_FRAMEBUFFER;
	}
	
	OGL_CHECK( pRenderThread, pglBlitFramebuffer( 0, 0, pWidth - 1, pHeight - 1, 0, 0, pWidth - 1, pHeight - 1, mask, GL_NEAREST ) );
	OGL_CHECK( pRenderThread, pglBindFramebuffer( restoreBindTarget, oldfbo->GetFBO() ) );
}

void deoglDeferredRendering::CopyFirstDepthToThirdDepth( bool copyDepth, bool copyStencil ){
	if( ! copyDepth && ! copyStencil ){
		return;
	}
	
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	int mask = 0, restoreBindTarget;
	
	if( pUseEncodedDepth ){
		if( ! copyDepth ){
			return;
		}
		
		if( pModeDepth ){
			OGL_CHECK( pRenderThread, pglBindFramebuffer(
				GL_READ_FRAMEBUFFER, pFBOs[ efbomedDepth1RB ]->GetFBO() ) );
			OGL_CHECK( pRenderThread, pglBindFramebuffer(
				GL_DRAW_FRAMEBUFFER, pFBOs[ efbomedDepth3RB ]->GetFBO() ) );
			
		}else{
			OGL_CHECK( pRenderThread, pglBindFramebuffer(
				GL_READ_FRAMEBUFFER, pFBOs[ efbomedDepth2RB ]->GetFBO() ) );
			OGL_CHECK( pRenderThread, pglBindFramebuffer(
				GL_DRAW_FRAMEBUFFER, pFBOs[ efbomedDepth3RB ]->GetFBO() ) );
		}
		
		mask = GL_COLOR_BUFFER_BIT;
		restoreBindTarget = GL_FRAMEBUFFER;
		
	}else{
		OGL_CHECK( pRenderThread, pglBindFramebuffer(
			GL_DRAW_FRAMEBUFFER, pFBOs[ efbomdD3 ]->GetFBO() ) );
		
		if( copyDepth ){
			mask |= GL_DEPTH_BUFFER_BIT;
		}
		if( copyStencil ){
			mask |= GL_STENCIL_BUFFER_BIT;
		}
		restoreBindTarget = GL_DRAW_FRAMEBUFFER;
	}
	
	OGL_CHECK( pRenderThread, pglBlitFramebuffer( 0, 0, pWidth - 1, pHeight - 1,
		0, 0, pWidth - 1, pHeight - 1, mask, GL_NEAREST ) );
	OGL_CHECK( pRenderThread, pglBindFramebuffer( restoreBindTarget, oldfbo->GetFBO() ) );
}

void deoglDeferredRendering::SetUseEncodedDepth( bool useEncodedDepth ){
	/*
	if( useEncodedDepth != pUseEncodedDepth ){
		pUseEncodedDepth = useEncodedDepth;
		
		if( useEncodedDepth ){
			pRenderbuffer->SetDepthFormat( true );
			
		}else{
			pRenderbuffer->SetStencilFormat();
		}
		
		if( useEncodedDepth ){
			pTextureDepth1->SetFBOFormat( 8, 4, true );
			pTextureDepth2->SetFBOFormat( 8, 4, true );
			
		}else{
			pTextureDepth1->SetDepthFormat( false );
			pTextureDepth2->SetDepthFormat( false );
		}
		
		if( pRealWidth > 0 && pRealHeight > 0 ){
			pRenderbuffer->CreateBuffer();
			pTextureDepth1->CreateTexture();
			pTextureDepth2->CreateTexture();
		}
	}
	*/
	
	// using texture-depth + renbuf-stencil is not supported
}

decDMatrix deoglDeferredRendering::CreateProjectionDMatrix( int width, int height,
float fov, float fovRatio, float znear, float zfar ) const{
	if( width < 1 || height < 1 || fov <= 0.0f || fov >= PI || fovRatio <= 0.0f || znear <= 0.0f || znear >= zfar ){
		DETHROW( deeInvalidParam );
	}
	
	// infinite projective matrix. works for both the inverse depth case and the fallback
	// non-inverse depth case. for fallback it is slightly better than non-infinite thus
	// the same infinite projection matrix can be used for both cases
	const double aspectRatio = ( double )width / ( double )height;
	decDMatrix m;
	
	m.a11 = 1.0 / tan( fov * 0.5 );
	m.a12 = 0.0;
	m.a13 = 0.0;
	m.a14 = 0.0;
	
	m.a21 = 0.0;
	m.a22 = aspectRatio / tan( fov * fovRatio * 0.5 );
	m.a23 = 0.0;
	m.a24 = 0.0;
	
	m.a31 = 0.0;
	m.a32 = 0.0;
	
	if( pUseInverseDepth ){
		// due to inverse depth changing z-clamping
		m.a33 = 0.0;
		m.a34 = znear;
		
	}else{
		m.a33 = 1.0;
		m.a34 = -2.0 * znear;
	}
	
	m.a41 = 0.0;
	m.a42 = 0.0;
	m.a43 = 1.0;
	m.a44 = 0.0;
	
	return m;
	
	/*
	// non-infinite projective matrix
	const double aspectRatio = ( double )width / ( double )height;
	const double denominator = 1.0 / ( double )( zfar - znear );
	decDMatrix m;
	
	m.a11 = 1.0 / tan( fov * 0.5 );
	m.a12 = 0.0;
	m.a13 = 0.0;
	m.a14 = 0.0;
	m.a21 = 0.0;
	m.a22 = aspectRatio / tan( fov * fovRatio * 0.5 );
	m.a23 = 0.0;
	m.a24 = 0.0;
	m.a31 = 0.0;
	m.a32 = 0.0;
	m.a33 = ( zfar + znear ) * denominator;
	m.a34 = -2.0 * zfar * znear * denominator;
	m.a41 = 0.0;
	m.a42 = 0.0;
	m.a43 = 1.0;
	m.a44 = 0.0;
	
	return m;
	*/
}

decDMatrix deoglDeferredRendering::CreateFrustumDMatrix( int width, int height,
float fov, float fovRatio, float znear, float zfar ) const{
	// frustum matrix is always non-infinite otherwise SetFrustum calls fail
	const double aspectRatio = ( double )width / ( double )height;
	const double denominator = 1.0 / ( double )( zfar - znear );
	decDMatrix m;
	
	m.a11 = 1.0 / tan( fov * 0.5 );
	m.a12 = 0.0;
	m.a13 = 0.0;
	m.a14 = 0.0;
	m.a21 = 0.0;
	m.a22 = aspectRatio / tan( fov * fovRatio * 0.5 );
	m.a23 = 0.0;
	m.a24 = 0.0;
	m.a31 = 0.0;
	m.a32 = 0.0;
	m.a33 = ( zfar + znear ) * denominator;
	m.a34 = -2.0 * zfar * znear * denominator;
	m.a41 = 0.0;
	m.a42 = 0.0;
	m.a43 = 1.0;
	m.a44 = 0.0;
	
	return m;
}



void deoglDeferredRendering::InitPostProcessTarget(){
	pModePostProcess = true;
}

void deoglDeferredRendering::SwapPostProcessTarget(){
	pModePostProcess = ! pModePostProcess;
}

deoglTexture *deoglDeferredRendering::GetPostProcessTexture() const{
	if( pModePostProcess ){
		return pTextureColor;
		
	}else{
		return pTextureTemporary2;
	}
}



void deoglDeferredRendering::ActivateFBODepth(){
	if( pUseEncodedDepth ){
		if( pModeDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedDepth1RB ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedDepth2RB ] );
		}
		
	}else{
		if( pModeDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdD1 ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdD2 ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBODepthLevel( int level ){
	if( level < 0 || level > pFBOMipMapDepthCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( level == 0 ){
		ActivateFBODepth();
		
	}else{
		if( pModeDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOMipMapDepth1[ level - 1 ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOMipMapDepth2[ level - 1 ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBODepth3(){
	if( pUseEncodedDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedDepth3RB ] );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdD3 ] );
	}
}

void deoglDeferredRendering::ActivatePostProcessFBO( bool withDepth ){
	if( pModePostProcess ){
		ActivateFBOTemporary2( withDepth );
		
	}else{
		ActivateFBOColor( withDepth );
	}
}

void deoglDeferredRendering::ActivateFBODiffuse( bool withDepth ){
	if( pUseEncodedDepth ){
		if( withDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedDiffRB ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedDiff ] );
		}
		
	}else{
		if( withDepth ){
			if( pModeDepth ){
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdDiffD1 ] );
				
			}else{
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdDiffD2 ] );
			}
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdDiff ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBOReflectivity( bool withDepth ){
	if( pUseEncodedDepth ){
		if( withDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedReflRB ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedRefl ] );
		}
		
	}else{
		if( withDepth ){
			if( pModeDepth ){
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdReflD1 ] );
				
			}else{
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdReflD2 ] );
			}
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdRefl ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBORoughness( bool withDepth ){
	if( pUseEncodedDepth ){
		if( withDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedRoughRB ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedRough ] );
		}
		
	}else{
		if( withDepth ){
			if( pModeDepth ){
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdRoughD1 ] );
				
			}else{
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdRoughD2 ] );
			}
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdRough ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBOAOSolidity( bool withDepth ){
	if( pUseEncodedDepth ){
		if( withDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedAOSolidityRB ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedAOSolidity ] );
		}
		
	}else{
		if( withDepth ){
			if( pModeDepth ){
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdAOSolidityD1 ] );
				
			}else{
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdAOSolidityD2 ] );
			}
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdAOSolidity ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBOTemporary1( bool withDepth ){
	if( pUseEncodedDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedTemp1RB ] );
		
	}else{
		if( withDepth ){
			if( pModeDepth ){
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp1D1 ] );
				
			}else{
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp1D2 ] );
			}
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp1 ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBOTemporary1Level( int level ){
	if( level < 0 || level > pFBOMipMapCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( level == 0 ){
		ActivateFBOTemporary1( false );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOMipMapTemporary1[ level - 1 ] );
	}
}

void deoglDeferredRendering::ActivateFBOTemporary2( bool withDepth ){
	if( pUseEncodedDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedTemp2RB ] );
		
	}else{
		if( withDepth ){
			if( pModeDepth ){
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp2D1 ] );
				
			}else{
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp2D2 ] );
			}
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp2 ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBOTemporary2Level( int level ){
	if( level < 0 || level > pFBOMipMapCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( level == 0 ){
		ActivateFBOTemporary2( false );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOMipMapTemporary2[ level - 1 ] );
	}
}

void deoglDeferredRendering::ActivateFBOTemporary3(){
	if( pUseEncodedDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedTemp3 ] );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp3 ] );
	}
}

void deoglDeferredRendering::ActivateFBOColor( bool withDepth ){
	if( pUseEncodedDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedColorRB ] );
		
	}else{
		if( withDepth ){
			if( pModeDepth ){
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdColorD1 ] );
				
			}else{
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdColorD2 ] );
			}
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdColor ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBODepthDiffuse(){
	if( pUseEncodedDepth ){
		if( pModeDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedDepth1DiffRB ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedDepth2DiffRB ] );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void deoglDeferredRendering::ActivateFBODepthTemp1(){
	if( pUseEncodedDepth ){
		if( pModeDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedDepth1Temp1RB ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedDepth2Temp1RB ] );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void deoglDeferredRendering::ActivateFBOColorDepth(){
	if( pUseEncodedDepth ){
		if( pModeDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedColorDepth1RB ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedColorDepth2RB ] );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void deoglDeferredRendering::ActivateFBOColorTemp2( bool withDepth ){
	if( pUseEncodedDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedColorTemp2RB ] );
		
	}else{
		if( withDepth ){
			if( pModeDepth ){
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdColorTemp2D1 ] );
				
			}else{
				pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdColorTemp2D2 ] );
			}
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdColorTemp2 ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBOMaterialColor(){
	if( pUseEncodedDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomedMaterialColorRB ] );
		
	}else{
		if( pModeDepth ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdMaterialColorD1 ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdMaterialColorD2 ] );
		}
	}
}

void deoglDeferredRendering::ActivateFBOLuminance(){
	pRenderThread.GetFramebuffer().Activate( pFBOLuminance );
}

void deoglDeferredRendering::ActivateFBOLuminanceNormal(){
	pRenderThread.GetFramebuffer().Activate( pFBOLuminanceNormal );
}



void deoglDeferredRendering::RenderFSQuadVAO(){
	OGL_CHECK( pRenderThread, pglBindVertexArray( pVAOFullScreenQuad->GetVAO() ) );
	OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
}



void deoglDeferredRendering::SetShaderViewport( deoglShaderCompiled &shader, int parameter, bool normalized ){
	if( normalized ){
		shader.SetParameterFloat( parameter, 0.0f, 0.0f, pClampU, pClampV );
		
	}else{
		shader.SetParameterFloat( parameter, 0.0f, 0.0f, ( float )( pWidth - 1 ), ( float )( pHeight - 1 ) );
	}
}

void deoglDeferredRendering::SetShaderViewport( deoglSPBlockUBO &paramBlock, int parameter, bool normalized ){
	if( normalized ){
		paramBlock.SetParameterDataVec4( parameter, 0.0f, 0.0f, pClampU, pClampV );
		
	}else{
		paramBlock.SetParameterDataVec4( parameter, 0.0f, 0.0f, ( float )( pWidth - 1 ), ( float )( pHeight - 1 ) );
	}
}

void deoglDeferredRendering::SetShaderParamFSQuad( deoglShaderCompiled &shader, int parameter ) const{
	// ( -1,  1 ) => ( 0, sv )
	// (  1,  1 ) => ( su, sv )
	// (  1, -1 ) => ( su, 0 )
	// ( -1, -1 ) => ( 0, 0 )
	// 
	// tc.s = x * (su/2) + (su/2)
	// tc.t = y * (sv/2) + (sv/2)
	const float hsu = pScalingU * 0.5f;
	const float hsv = pScalingV * 0.5f;
	
	shader.SetParameterFloat( parameter, hsu, hsv, hsu, hsv );
}

void deoglDeferredRendering::SetShaderParamFSQuad( deoglShaderCompiled &shader, int parameter, int width, int height ) const{
	// ( -1,  1 ) => ( 0, sv )
	// (  1,  1 ) => ( su, sv )
	// (  1, -1 ) => ( su, 0 )
	// ( -1, -1 ) => ( 0, 0 )
	//
	// tc.s = x * (su/2) + (su/2)
	// tc.t = y * (sv/2) + (sv/2)
	const float hsu = pPixelSizeU * ( float )width * 0.5f;
	const float hsv = pPixelSizeV * ( float )height * 0.5f;
	
	shader.SetParameterFloat( parameter, hsu, hsv, hsu, hsv );
}

void deoglDeferredRendering::SetShaderParamFSQuad( deoglShaderCompiled &shader, int parameter, float x1, float y1, float x2, float y2 ) const{
	// ( -1,  1 ) => ( x1, y2 )
	// (  1,  1 ) => ( x2, y2 )
	// (  1, -1 ) => ( x2, y1 )
	// ( -1, -1 ) => ( x1, y1 )
	// 
	// tc.s = (x1+x2)/2 + (x2-x1)/2 * s
	// tc.t = (y1+y2)/2 + (y2-y1)/2 * t
	// 
	// tc.s = s * ((x2-x1)/2) + ((x1+x2)/2)
	// tc.t = t * ((y2-y1)/2) + ((y1+y2)/2)
	const float scaleS = pPixelSizeU * ( x2 - x1 ) * 0.5f;
	const float scaleT = pPixelSizeV * ( y2 - y1 ) * 0.5f;
	const float offsetS = pPixelSizeU * ( x1 + x2 ) * 0.5f;
	const float offsetT = pPixelSizeV * ( y1 + y2 ) * 0.5f;
	
	shader.SetParameterFloat( parameter, scaleS, scaleT, offsetS, offsetT );
}

void deoglDeferredRendering::SetShaderParamFSQuadUpsideDown( deoglShaderCompiled &shader, int parameter ) const{
	// ( -1,  1 ) => ( 0, 0 )
	// (  1,  1 ) => ( su, 0 )
	// (  1, -1 ) => ( su, sv )
	// ( -1, -1 ) => ( 0, sv )
	//
	// tc.s = x * (su/2) + (su/2)
	// tc.t = y * (-sv/2) + (sv/2)
	const float hsu = pScalingU * 0.5f;
	const float hsv = pScalingV * 0.5f;
	
	shader.SetParameterFloat( parameter, hsu, -hsv, hsu, hsv );
}

void deoglDeferredRendering::SetShaderParamFSQuad( deoglSPBlockUBO &paramBlock, int parameter ) const{
	// ( -1,  1 ) => ( 0, sv )
	// (  1,  1 ) => ( su, sv )
	// (  1, -1 ) => ( su, 0 )
	// ( -1, -1 ) => ( 0, 0 )
	// 
	// tc.s = x * (su/2) + (su/2)
	// tc.t = y * (sv/2) + (sv/2)
	const float hsu = pScalingU * 0.5f;
	const float hsv = pScalingV * 0.5f;
	
	paramBlock.SetParameterDataVec4( parameter, hsu, hsv, hsu, hsv );
}

void deoglDeferredRendering::SetShaderParamSCToTC( deoglShaderCompiled &shader, int parameter ) const{
	// (0,0) => (0,0)
	// (w,0) => (su,0)
	// (w,h) => (su,sv)
	// (0,h) => (0,sv)
	//
	// tc.s = x * (su/w) + 0
	// tc.t = y * (sv/h) + 0
	shader.SetParameterFloat( parameter, pScalingU / ( float )pWidth, pScalingV / ( float )pHeight, 0.0f, 0.0f );
}

void deoglDeferredRendering::SetShaderParamSCToFSTC( deoglShaderCompiled &shader, int parameter ) const{
	// (0,0) => (0,0)
	// (w,0) => (1,0)
	// (w,h) => (1,1)
	// (0,h) => (0,1)
	//
	// tc.s = x * (1/w) + 0
	// tc.t = y * (1/h) + 0
	shader.SetParameterFloat( parameter, 1.0f / ( float )pWidth, 1.0f / ( float )pHeight );
}

void deoglDeferredRendering::SetShaderParamSCToCSP( deoglShaderCompiled &shader, int parameter ) const{
	// (0,0) => (-1,-1)
	// (w,0) => (1,-1)
	// (w,h) => (1,1)
	// (0,h) => (-1,1)
	//
	// tc.s = x * (2/w) + (-1 + 1/2pix)
	// tc.t = y * (2/h) + (-1 + 1/2pix)
	// 
	// half pixel offset to map into center of pixel for best results
	shader.SetParameterFloat( parameter, 2.0f / ( float )pWidth, 2.0f / ( float )pHeight,
		0.5f / ( float )pWidth - 1.0f, 0.5f / ( float )pHeight - 1.0f );
}



// private functions
//////////////////////

void deoglDeferredRendering::pCleanUp(){
	if( pDepthMinMax ){
		delete pDepthMinMax;
	}
	pDestroyFBOs();
	
	if( pVAOBillboard ){
		delete pVAOBillboard;
	}
	if( pVBOBillboard ){
		pglDeleteBuffers( 1, &pVBOBillboard );
	}
	if( pVAOFullScreenQuad ){
		delete pVAOFullScreenQuad;
	}
	if( pVBOFullScreenQuad ){
		pglDeleteBuffers( 1, &pVBOFullScreenQuad );
	}
	
	if( pTextureLuminanceDepth ){
		delete pTextureLuminanceDepth;
	}
	if( pTextureLuminanceNormal ){
		delete pTextureLuminanceNormal;
	}
	if( pTextureLuminance ){
		delete pTextureLuminance;
	}
	
	if( pTextureColor ){
		delete pTextureColor;
	}
	if( pTextureTemporary3 ){
		delete pTextureTemporary3;
	}
	if( pTextureTemporary2 ){
		delete pTextureTemporary2;
	}
	if( pTextureTemporary1 ){
		delete pTextureTemporary1;
	}
	if( pTextureSubSurface ){
		delete pTextureSubSurface;
	}
	if( pTextureAOSolidity ){
		delete pTextureAOSolidity;
	}
	if( pTextureRoughness ){
		delete pTextureRoughness;
	}
	if( pTextureReflectivity ){
		delete pTextureReflectivity;
	}
	if( pTextureNormal ){
		delete pTextureNormal;
	}
	if( pTextureDiffuse ){
		delete pTextureDiffuse;
	}
	
	if( pTextureDepth3 ){
		delete pTextureDepth3;
	}
	if( pTextureDepth2 ){
		delete pTextureDepth2;
	}
	if( pTextureDepth1 ){
		delete pTextureDepth1;
	}
	if( pRenderbuffer ){
		delete pRenderbuffer;
	}
}

void deoglDeferredRendering::pCreateTextures(){
	// create the renderbuffer
	pRenderbuffer = new deoglRenderbuffer( pRenderThread );
	if( pUseEncodedDepth ){
		pRenderbuffer->SetDepthFormat( true );
		
	}else{
		//pRenderbuffer->SetStencilFormat();
	}
	
	// create depth textures
	pTextureDepth1 = new deoglTexture( pRenderThread );
	pTextureDepth1->SetMipMapped( true );
	pTextureDepth2 = new deoglTexture( pRenderThread );
	pTextureDepth2->SetMipMapped( true );
	pTextureDepth3 = new deoglTexture( pRenderThread );
	pTextureDepth3->SetMipMapped( true );
	
	if( pUseEncodedDepth ){
		pTextureDepth1->SetFBOFormat( 4, true );
		pTextureDepth2->SetFBOFormat( 4, true );
		pTextureDepth3->SetFBOFormat( 4, true );
		
	}else{
		pTextureDepth1->SetDepthFormat( true, pUseInverseDepth );
		pTextureDepth2->SetDepthFormat( true, pUseInverseDepth );
		pTextureDepth3->SetDepthFormat( true, pUseInverseDepth );
	}
	
	// create diffuse texture
	pTextureDiffuse = new deoglTexture( pRenderThread );
	pTextureDiffuse->SetFBOFormat( 4, false ); //4, true );
	
	// create normal texture
	// RGB8 gives strong jumping artifacts during the separate reflection pass due to errors
	// in the normals. RGB10A2 reduces the jumping artifacts but they are still faintly
	// visibile. With RGB16 the jumping artifacts are not visible anymore. RB11B11F has even
	// worse jumping artifacts.
	pTextureNormal = new deoglTexture( pRenderThread );
	//pTextureNormal->SetFBOFormat( 3, false ); //4, true );
	pTextureNormal->SetFBOFormat( 3, true ); //4, true );
	//pTextureNormal->SetFormatFromCaps( deoglCapsFmtSupport::eutfRGB10A2 );
	
	// create reflectivity texture
	pTextureReflectivity = new deoglTexture( pRenderThread );
	pTextureReflectivity->SetFBOFormat( 3, false ); //4, true );
	
	// create roughness texture
	pTextureRoughness = new deoglTexture( pRenderThread );
	pTextureRoughness->SetFBOFormat( 3, false );
	
	// create ao-solidity texture
	pTextureAOSolidity = new deoglTexture( pRenderThread );
	pTextureAOSolidity->SetFBOFormat( 3, false );
	
	// create sub-surface texture
	pTextureSubSurface = new deoglTexture( pRenderThread );
	pTextureSubSurface->SetFBOFormat( 3, true );
	
	// create temporary textures
	pTextureTemporary1 = new deoglTexture( pRenderThread );
	//pTextureTemporary1->SetMipMapped( true );
	pTextureTemporary1->SetFBOFormat( 3, true );
	
	pTextureTemporary2 = new deoglTexture( pRenderThread );
	//pTextureTemporary2->SetMipMapped( true );
	pTextureTemporary2->SetFBOFormat( 3, true );
	
	pTextureTemporary3 = new deoglTexture( pRenderThread );
	pTextureTemporary3->SetFBOFormat( 1, false );
	
	// create color texture
	pTextureColor = new deoglTexture( pRenderThread );
	pTextureColor->SetFBOFormat( 4, true );
	//pTextureColor->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGBA32F ); // only for special debugging
	
	// luminance textures
	pTextureLuminance = new deoglTexture( pRenderThread );
	pTextureLuminance->SetFBOFormat( 3, true ); // fbo same format requirement. only 1 component used
	pTextureLuminance->SetSize( 128, 64 );
	pTextureLuminance->CreateTexture();
	
	pTextureLuminanceNormal = new deoglTexture( pRenderThread );
	pTextureLuminanceNormal->SetFBOFormat( 3, true );
	pTextureLuminanceNormal->SetSize( 128, 64 );
	pTextureLuminanceNormal->CreateTexture();
	
	pTextureLuminanceDepth = new deoglTexture( pRenderThread );
	pTextureLuminanceDepth->SetDepthFormat( true, pUseInverseDepth );
	pTextureLuminanceDepth->SetSize( 128, 64 );
	pTextureLuminanceDepth->CreateTexture();
}

void deoglDeferredRendering::pUpdateMemoryUsage(){
	pMemoryUsageGPURenBuf = 0;
	pMemoryUsageGPUTexture = 0;
	
	pMemoryUsageGPURenBuf += pRenderbuffer->GetMemoryUsageGPU();
	
	pMemoryUsageGPUTexture += pTextureDepth1->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureDepth2->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureDepth3->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureDiffuse->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureNormal->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureReflectivity->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureRoughness->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureAOSolidity->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureSubSurface->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureColor->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureTemporary1->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureTemporary2->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureTemporary3->GetMemoryUsageGPU();
	
	if( deoglDRDepthMinMax::USAGE_VERSION == 0 ){
		pMemoryUsageGPUTexture += pDepthMinMax->GetTexture()->GetMemoryUsageGPU();
		
	}else if( deoglDRDepthMinMax::USAGE_VERSION == 1 ){
		pMemoryUsageGPUTexture += pDepthMinMax->GetTextureMin()->GetMemoryUsageGPU();
		pMemoryUsageGPUTexture += pDepthMinMax->GetTextureMax()->GetMemoryUsageGPU();
		
	}else if( deoglDRDepthMinMax::USAGE_VERSION == 2 ){
		pMemoryUsageGPUTexture += pDepthMinMax->GetTexture()->GetMemoryUsageGPU();
	}
	
	pMemoryUsageGPUTexture += pTextureLuminance->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureLuminanceNormal->GetMemoryUsageGPU();
	pMemoryUsageGPUTexture += pTextureLuminanceDepth->GetMemoryUsageGPU();
	
	pMemoryUsageGPU = pMemoryUsageGPUTexture + pMemoryUsageGPURenBuf;
	
	pRenderThread.GetLogger().LogInfoFormat( "DefRen: Memory Consumption (%d x %d)", pRealWidth, pRealHeight );
	pRenderThread.GetLogger().LogInfoFormat( "- Render Buffer: %d", pMemoryUsageGPURenBuf );
	pRenderThread.GetLogger().LogInfoFormat( "- Texture: %d", pMemoryUsageGPUTexture );
	pRenderThread.GetLogger().LogInfoFormat( "- Total: %d", pMemoryUsageGPU );
}

void deoglDeferredRendering::pCreateFBOs(){
	if( pUseEncodedDepth ){
		pCreateFBORenBuf( efbomedDepth1RB, pTextureDepth1, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedDepth2RB, pTextureDepth2, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedDepth3RB, pTextureDepth3, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedDiff, pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, NULL, false );
		pCreateFBORenBuf( efbomedDiffRB, pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedRefl, pTextureReflectivity, NULL, NULL, NULL, NULL, NULL, NULL, false );
		pCreateFBORenBuf( efbomedReflRB, pTextureReflectivity, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedRough, pTextureRoughness, NULL, NULL, NULL, NULL, NULL, NULL, false );
		pCreateFBORenBuf( efbomedRoughRB, pTextureRoughness, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedAOSolidity, pTextureAOSolidity, NULL, NULL, NULL, NULL, NULL, NULL, false );
		pCreateFBORenBuf( efbomedAOSolidityRB, pTextureAOSolidity, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedTemp1RB, pTextureTemporary1, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedTemp2RB, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedTemp3, pTextureTemporary3, NULL, NULL, NULL, NULL, NULL, NULL, false );
		pCreateFBORenBuf( efbomedColorRB, pTextureColor, NULL, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedDepth1DiffRB, pTextureDepth1, pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedDepth2DiffRB, pTextureDepth2, pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedDepth1Temp1RB, pTextureDepth1, pTextureTemporary1, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedDepth2Temp1RB, pTextureDepth2, pTextureTemporary1, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedColorDepth1RB, pTextureColor, pTextureDepth1, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedColorDepth2RB, pTextureColor, pTextureDepth2, NULL, NULL, NULL, NULL, NULL, true );
		pCreateFBORenBuf( efbomedColorTemp2RB, pTextureColor, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, true );
		
		if( pRenderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
			pCreateFBORenBuf( efbomedMaterialColorRB, pTextureDiffuse, pTextureNormal,
				pTextureReflectivity, pTextureRoughness, pTextureAOSolidity, pTextureSubSurface, pTextureColor, true );
			
		}else{
			pCreateFBORenBuf( efbomedMaterialColorRB, pTextureDiffuse, pTextureNormal,
				pTextureReflectivity, pTextureColor, NULL, NULL, NULL, true );
		}
		
	}else{
		pCreateFBOTex( efbomdD1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdD2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
		pCreateFBOTex( efbomdD3, NULL, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth3 );
		pCreateFBOTex( efbomdDiff, pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		pCreateFBOTex( efbomdDiffD1, pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdDiffD2, pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
		pCreateFBOTex( efbomdRefl, pTextureReflectivity, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		pCreateFBOTex( efbomdReflD1, pTextureReflectivity, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdReflD2, pTextureReflectivity, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
		pCreateFBOTex( efbomdRough, pTextureRoughness, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		pCreateFBOTex( efbomdRoughD1, pTextureRoughness, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdRoughD2, pTextureRoughness, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
		pCreateFBOTex( efbomdAOSolidity, pTextureAOSolidity, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		pCreateFBOTex( efbomdAOSolidityD1, pTextureAOSolidity, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdAOSolidityD2, pTextureAOSolidity, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
		pCreateFBOTex( efbomdTemp1, pTextureTemporary1, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		pCreateFBOTex( efbomdTemp1D1, pTextureTemporary1, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdTemp1D2, pTextureTemporary1, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
		pCreateFBOTex( efbomdTemp2, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		pCreateFBOTex( efbomdTemp2D1, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdTemp2D2, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
		pCreateFBOTex( efbomdTemp3, pTextureTemporary3, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		pCreateFBOTex( efbomdColor, pTextureColor, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
		pCreateFBOTex( efbomdColorD1, pTextureColor, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdColorD2, pTextureColor, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
		pCreateFBOTex( efbomdColorTemp2, pTextureColor, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, NULL );
		pCreateFBOTex( efbomdColorTemp2D1, pTextureColor, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdColorTemp2D2, pTextureColor, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
		
		if( pRenderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
			pCreateFBOTex( efbomdMaterialColorD1, pTextureDiffuse, pTextureNormal,
				pTextureReflectivity, pTextureRoughness, pTextureAOSolidity,
				pTextureSubSurface, pTextureColor, pTextureDepth1 );
			pCreateFBOTex( efbomdMaterialColorD2, pTextureDiffuse, pTextureNormal,
				pTextureReflectivity, pTextureRoughness, pTextureAOSolidity,
				pTextureSubSurface, pTextureColor, pTextureDepth2 );
			
		}else{
			pCreateFBOTex( efbomdMaterialColorD1, pTextureDiffuse, pTextureNormal,
				pTextureReflectivity, pTextureColor, NULL, NULL, NULL, pTextureDepth1 );
			pCreateFBOTex( efbomdMaterialColorD2, pTextureDiffuse, pTextureNormal,
				pTextureReflectivity, pTextureColor, NULL, NULL, NULL, pTextureDepth2 );
		}
	}
	
	// fbos for the mip map levels
	int fboMipMapCount = pTextureDepth1->GetRealMipMapLevelCount();
	
	if( fboMipMapCount > 0 ){
		int i;
		
		pFBOMipMapDepth1 = new deoglFramebuffer*[ fboMipMapCount ];
		pFBOMipMapDepth2 = new deoglFramebuffer*[ fboMipMapCount ];
		
		for( pFBOMipMapDepthCount=0; pFBOMipMapDepthCount<fboMipMapCount; pFBOMipMapDepthCount++ ){
			pFBOMipMapDepth1[ pFBOMipMapCount ] = NULL;
			pFBOMipMapDepth2[ pFBOMipMapCount ] = NULL;
		}
		
		for( i=0; i<fboMipMapCount; i++ ){
			try{
				pFBOMipMapDepth1[ i ] = new deoglFramebuffer( pRenderThread, false );
				pRenderThread.GetFramebuffer().Activate( pFBOMipMapDepth1[ i ] );
				pFBOMipMapDepth1[ i ]->AttachDepthTextureLevel( pTextureDepth1, i + 1 );
				const GLenum buffers[ 1 ] = { GL_NONE };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
				pFBOMipMapDepth1[ i ]->Verify();
				
			}catch( const deException &e ){
// 				deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBOs", __LINE__ );
// 				tracePoint.AddValue( "texture", "MipMapDepth1" );
// 				tracePoint.AddValueInt( "level", i + 1 );
				throw;
			}
			
			try{
				pFBOMipMapDepth2[ i ] = new deoglFramebuffer( pRenderThread, false );
				pRenderThread.GetFramebuffer().Activate( pFBOMipMapDepth2[ i ] );
				pFBOMipMapDepth2[ i ]->AttachDepthTextureLevel( pTextureDepth2, i + 1 );
				const GLenum buffers[ 1 ] = { GL_NONE };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
				pFBOMipMapDepth2[ i ]->Verify();
				
			}catch( const deException &e ){
// 				deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBOs", __LINE__ );
// 				tracePoint.AddValue( "texture", "MipMapDepth2" );
// 				tracePoint.AddValueInt( "level", i + 1 );
				throw;
			}
		}
	}
	
	// fbos for the mip map levels
	fboMipMapCount = 0; //pTextureTemporary1->GetRealMipMapLevelCount() - 1;
	
	if( fboMipMapCount > 0 ){
		int i;
		
		pFBOMipMapTemporary1 = new deoglFramebuffer*[ fboMipMapCount ];
		pFBOMipMapTemporary2 = new deoglFramebuffer*[ fboMipMapCount ];
		
		for( pFBOMipMapCount=0; pFBOMipMapCount<fboMipMapCount; pFBOMipMapCount++ ){
			pFBOMipMapTemporary1[ pFBOMipMapCount ] = NULL;
			pFBOMipMapTemporary2[ pFBOMipMapCount ] = NULL;
		}
		
		for( i=0; i<fboMipMapCount; i++ ){
			try{
				pFBOMipMapTemporary1[ i ] = new deoglFramebuffer( pRenderThread, false );
				pRenderThread.GetFramebuffer().Activate( pFBOMipMapTemporary1[ i ] );
				pFBOMipMapTemporary1[ i ]->AttachColorTextureLevel( 0, pTextureTemporary1, i + 1 );
				const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
				pFBOMipMapTemporary1[ i ]->Verify();
				
			}catch( const deException &e ){
// 				deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBOs", __LINE__ );
// 				tracePoint.AddValue( "texture", "MipMapTemporary1" );
// 				tracePoint.AddValueInt( "level", i + 1 );
				throw;
			}
			
			try{
				pFBOMipMapTemporary2[ i ] = new deoglFramebuffer( pRenderThread, false );
				pRenderThread.GetFramebuffer().Activate( pFBOMipMapTemporary2[ i ] );
				pFBOMipMapTemporary2[ i ]->AttachColorTextureLevel( 0, pTextureTemporary2, i + 1 );
				const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
				pFBOMipMapTemporary2[ i ]->Verify();
				
			}catch( const deException &e ){
// 				deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBOs", __LINE__ );
// 				tracePoint.AddValue( "texture", "MipMapTemporary2" );
// 				tracePoint.AddValueInt( "level", i + 1 );
				throw;
			}
		}
	}
	
	// luminance fbo
	const GLenum buffers[ 2 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	
	pFBOLuminance = new deoglFramebuffer( pRenderThread, false );
	pRenderThread.GetFramebuffer().Activate( pFBOLuminance );
	pFBOLuminance->AttachDepthTexture( pTextureLuminanceDepth );
	pFBOLuminance->AttachColorTexture( 0, pTextureLuminance );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOLuminance->Verify();
	
	pFBOLuminanceNormal = new deoglFramebuffer( pRenderThread, false );
	pRenderThread.GetFramebuffer().Activate( pFBOLuminanceNormal );
	pFBOLuminanceNormal->AttachDepthTexture( pTextureLuminanceDepth );
	pFBOLuminanceNormal->AttachColorTexture( 0, pTextureLuminance );
	pFBOLuminanceNormal->AttachColorTexture( 1, pTextureLuminanceNormal );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 2, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOLuminanceNormal->Verify();
}

void deoglDeferredRendering::pCreateFBOTex( int index, deoglTexture *texture1, deoglTexture *texture2,
deoglTexture *texture3, deoglTexture *texture4, deoglTexture *texture5, deoglTexture *texture6,
deoglTexture *texture7, deoglTexture *depth ){
	try{
		pFBOs[ index ] = new deoglFramebuffer( pRenderThread, false );
		
		pRenderThread.GetFramebuffer().Activate( pFBOs[ index ] );
		
		if( depth ){
			pFBOs[ index ]->AttachDepthTexture( depth );
			pFBOs[ index ]->AttachStencilTexture( depth );
			//pFBOs[ index ]->AttachStencilRenderbuffer( pRenderbuffer );
		}
		pFBOAttachColors( index, texture1, texture2, texture3, texture4, texture5, texture6, texture7 );
		
		pFBOs[ index ]->Verify();
		
	}catch( const deException &e ){
// 		deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBOTex", __LINE__ );
// 		tracePoint.AddValueInt( "index", index );
		throw;
	}
}

void deoglDeferredRendering::pCreateFBORenBuf( int index, deoglTexture *texture1, deoglTexture *texture2,
deoglTexture *texture3, deoglTexture *texture4, deoglTexture *texture5, deoglTexture *texture6,
deoglTexture *texture7, bool depth ){
	try{
		pFBOs[ index ] = new deoglFramebuffer( pRenderThread, false );
		
		pRenderThread.GetFramebuffer().Activate( pFBOs[ index ] );
		
		if( depth ){
			pFBOs[ index ]->AttachDepthRenderbuffer( pRenderbuffer );
			pFBOs[ index ]->AttachStencilRenderbuffer( pRenderbuffer );
		}
		pFBOAttachColors( index, texture1, texture2, texture3, texture4, texture5, texture6, texture7 );
		
		pFBOs[ index ]->Verify();
		
	}catch( const deException &e ){
// 		deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBORenBuf", __LINE__ );
// 		tracePoint.AddValueInt( "index", index );
		throw;
	}
}

void deoglDeferredRendering::pFBOAttachColors( int index, deoglTexture *texture1, deoglTexture *texture2,
deoglTexture *texture3, deoglTexture *texture4, deoglTexture *texture5, deoglTexture *texture6,
deoglTexture *texture7 ){
	const GLenum buffers[ 7 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
	
	if( texture1 ){
		pFBOs[ index ]->AttachColorTexture( 0, texture1 );
		
		if( texture2 ){
			pFBOs[ index ]->AttachColorTexture( 1, texture2 );
			
			if( texture3 ){
				pFBOs[ index ]->AttachColorTexture( 2, texture3 );
				
				if( texture4 ){
					pFBOs[ index ]->AttachColorTexture( 3, texture4 );
					
					if( texture5 ){
						pFBOs[ index ]->AttachColorTexture( 4, texture5 );
						
						if( texture6 ){
							pFBOs[ index ]->AttachColorTexture( 5, texture6 );
							
							if( texture7 ){
								pFBOs[ index ]->AttachColorTexture( 6, texture7 );
								OGL_CHECK( pRenderThread, pglDrawBuffers( 7, buffers ) );
								
							}else{
								OGL_CHECK( pRenderThread, pglDrawBuffers( 6, buffers ) );
							}
							
						}else{
							OGL_CHECK( pRenderThread, pglDrawBuffers( 5, buffers ) );
						}
						
					}else{
						OGL_CHECK( pRenderThread, pglDrawBuffers( 4, buffers ) );
					}
					
				}else{
					OGL_CHECK( pRenderThread, pglDrawBuffers( 3, buffers ) );
				}
				
			}else{
				OGL_CHECK( pRenderThread, pglDrawBuffers( 2, buffers ) );
			}
			
		}else{
			OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		}
		
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
	}else{
		const GLenum buffersNone[ 1 ] = { GL_NONE };
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffersNone ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
	}
}

void deoglDeferredRendering::pDestroyFBOs(){
	int i;
	
	if( pFBOLuminanceNormal ){
		delete pFBOLuminanceNormal;
		pFBOLuminanceNormal = NULL;
	}
	if( pFBOLuminance ){
		delete pFBOLuminance;
		pFBOLuminance = NULL;
	}
	
	for( i=0; i<pFBOMipMapCount; i++ ){
		if( pFBOMipMapTemporary1[ i ] ){
			delete pFBOMipMapTemporary1[ i ];
			pFBOMipMapTemporary1[ i ] = NULL;
		}
		if( pFBOMipMapTemporary2[ i ] ){
			delete pFBOMipMapTemporary2[ i ];
			pFBOMipMapTemporary2[ i ] = NULL;
		}
		
		if( pFBOMipMapDepth1[ i ] ){
			delete pFBOMipMapDepth1[ i ];
			pFBOMipMapDepth1[ i ] = NULL;
		}
		if( pFBOMipMapDepth2[ i ] ){
			delete pFBOMipMapDepth2[ i ];
			pFBOMipMapDepth2[ i ] = NULL;
		}
	}
	pFBOMipMapCount = 0;
	
	if( pFBOMipMapTemporary1 ){
		delete [] pFBOMipMapTemporary1;
		pFBOMipMapTemporary1 = NULL;
	}
	if( pFBOMipMapTemporary2 ){
		delete [] pFBOMipMapTemporary2;
		pFBOMipMapTemporary2 = NULL;
	}
	
	if( pFBOMipMapDepth1 ){
		delete [] pFBOMipMapDepth1;
		pFBOMipMapDepth1 = NULL;
	}
	if( pFBOMipMapDepth2 ){
		delete [] pFBOMipMapDepth2;
		pFBOMipMapDepth2 = NULL;
	}
	
	for( i=0; i<EFBOMD_COUNT; i++ ){
		if( pFBOs[ i ] ){
			delete pFBOs[ i ];
			pFBOs[ i ] = NULL;
		}
	}
}
