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

#include "deoglDeferredRendering.h"
#include "deoglDRDepthMinMax.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../extensions/deoglExtensions.h"
#include "../../extensions/deoglExtResult.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglRestoreFramebuffer.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../texture/deoglTextureStageManager.h"
#include "../../texture/arraytexture/deoglArrayTexture.h"
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

enum eFBOMappingsDepth{
	/** No-Color with Depth1. */
	efbomdD1,
	/** No-Color with Depth2. */
	efbomdD2,
	/** \brief No-Color with Depth3. */
	efbomdD3,
	/** \brief No-Color with DepthXRay. */
	efbomdDXRay,
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
	/** Color without depth. */
	efbomdColorLum,
	/** Color with Depth1. */
	efbomdColorLumD1,
	/** Color with Depth2. */
	efbomdColorLumD2,
	/** Color, temporary1 without depth. */
	efbomdColorLumTemp2,
	/** Color, temporary1 with Depth1. */
	efbomdColorLumTemp2D1,
	/** Color, temporary1 with Depth2. */
	efbomdColorLumTemp2D2,
	/** Diffuse, Normal, Specularity, Color with Depth1. */
	efbomdMaterialColorD1,
	/** Diffuse, Normal, Specularity, Color with Depth2. */
	efbomdMaterialColorD2,
	/** Luminance. */
	efbomdLuminance,
	/** Number of FBOs used. */
	EFBOMD_COUNT
};

enum eFBOCopyDepth{
	efbocdDepth1Layer0,
	efbocdDepth1Layer1,
	efbocdDepth2Layer0,
	efbocdDepth2Layer1,
	efbocdDepth3Layer0,
	efbocdDepth3Layer1,
	efbocdDepthXRayLayer0,
	efbocdDepthXRayLayer1
};



// Class deoglDeferredRendering
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDeferredRendering::deoglDeferredRendering( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pTextureLuminance( NULL ),
pMemUse( renderThread.GetMemoryManager().GetConsumption().deferredRendering )
// pTextureLuminanceNormal( NULL ),
// pTextureLuminanceDepth( NULL ),
// pFBOLuminance( NULL ),
// pFBOLuminanceNormal( NULL )
{
	struct sQuadPoint{
		GLfloat x, y;
		GLint layer;
	};
	struct sBillboardPoint{
		oglVector3 position, normal, tangent;
		oglVector2 texcoord;
	};
	
	sQuadPoint fsquad[ 36 ], *ptrFSQuad = fsquad;
	int i;
	
	for( i=0; i<6; i++ ){
		*( ptrFSQuad++ ) = sQuadPoint{ -1.0f,  1.0f, i };
		*( ptrFSQuad++ ) = sQuadPoint{  1.0f,  1.0f, i };
		*( ptrFSQuad++ ) = sQuadPoint{  1.0f, -1.0f, i };
		*( ptrFSQuad++ ) = sQuadPoint{ -1.0f, -1.0f, i };
		*( ptrFSQuad++ ) = sQuadPoint{ -1.0f,  1.0f, i };
		*( ptrFSQuad++ ) = sQuadPoint{  1.0f, -1.0f, i };
	};
	
	const oglVector3 billboardNormal = { 0.0f, 0.0f, 1.0f };
	const oglVector3 billboardTangent = { 1.0f, 0.0f, 0.0f };
	const sBillboardPoint billboard[ 6 ] = {
		{ { -1.0f,  1.0f, 0.0f }, billboardNormal, billboardTangent, { 0.0f, 0.0f } },
		{ {  1.0f,  1.0f, 0.0f }, billboardNormal, billboardTangent, { 1.0f, 0.0f } },
		{ {  1.0f, -1.0f, 0.0f }, billboardNormal, billboardTangent, { 1.0f, 1.0f } },
		{ { -1.0f, -1.0f, 0.0f }, billboardNormal, billboardTangent, { 0.0f, 1.0f } },
		{ { -1.0f,  1.0f, 0.0f }, billboardNormal, billboardTangent, { 0.0f, 0.0f } },
		{ {  1.0f, -1.0f, 0.0f }, billboardNormal, billboardTangent, { 1.0f, 1.0f } } };
	
	pWidth = 0;
	pHeight = 0;
	pLayerCount = 1;
	pRealWidth = 0;
	pRealHeight = 0;
	pRealLayerCount = 1;
	pScalingU = 1.0f;
	pScalingV = 1.0f;
	pPixelSizeU = 1.0f;
	pPixelSizeV = 1.0f;
	pFSQuadOffU = 0.0f;
	pFSQuadOffV = 0.0f;
	
	pUseInverseDepth = renderThread.GetConfiguration().GetUseInverseDepth();
	if( ! renderThread.GetCapabilities().GetFormats().GetUseFBOTex2DFormatFor( deoglCapsFmtSupport::eutfDepthF_Stencil )
	||  ! renderThread.GetCapabilities().GetFormats().GetUseFBOTex2DFormatFor( deoglCapsFmtSupport::eutfDepthF )
	||  ! renderThread.GetCapabilities().GetFormats().GetUseFBOTexCubeFormatFor( deoglCapsFmtSupport::eutfDepthF_Stencil )
	||  ! renderThread.GetCapabilities().GetFormats().GetUseFBOTexCubeFormatFor( deoglCapsFmtSupport::eutfDepthF )
	||  ! pglClipControl ){
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
	
	pTextureDepth1 = NULL;
	pTextureDepth2 = NULL;
	pTextureDepth3 = NULL;
	pTextureDepthXRay = nullptr;
	
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
	
	for( i=0; i<EFBOMD_COUNT; i++ ){
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
	
	for( i=0; i<8; i++ ){
		pFBOCopyDepth[ i ] = nullptr;
	}
	
	pDepthMinMax = NULL;
	
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
		OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 12, ( const GLvoid * )0 ) );
		OGL_CHECK( renderThread, pglEnableVertexAttribArray( 1 ) );
		OGL_CHECK( renderThread, pglVertexAttribIPointer( 1, 1, GL_INT, 12, ( const GLvoid * )8 ) );
		
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
		OGL_CHECK( renderThread, pglVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 44, ( const GLvoid * )0 ) );
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
		
		pVAOBillboard->EnsureRTSVAO();
		
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

void deoglDeferredRendering::Resize( int width, int height, int layerCount ){
	if( width < 1 || height < 1 || layerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	int textureWidth, textureHeight, textureLayerCount;
	
	// if the size is somewhere larger resize the textures
	if( width > pRealWidth || height > pRealHeight || layerCount > pRealLayerCount ){
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
		textureWidth = decMath::max( width, pWidth );
		textureHeight = decMath::max( height, pHeight );
		textureLayerCount = decMath::max( layerCount, pLayerCount );
		
		pRenderThread.GetLogger().LogInfoFormat( "DefRen: Resizing buffers to %dx%dx%d",
			textureWidth, textureHeight, textureLayerCount );
		
		// resize textures
		pTextureDepth1->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureDepth1->CreateTexture();
		pTextureDepth2->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureDepth2->CreateTexture();
		pTextureDepth3->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureDepth3->CreateTexture();
		pTextureDepthXRay->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureDepthXRay->CreateTexture();
		
		pTextureDiffuse->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureDiffuse->CreateTexture();
		pTextureNormal->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureNormal->CreateTexture();
		pTextureReflectivity->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureReflectivity->CreateTexture();
		pTextureRoughness->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureRoughness->CreateTexture();
		pTextureAOSolidity->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureAOSolidity->CreateTexture();
		pTextureSubSurface->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureSubSurface->CreateTexture();
		pTextureLuminance->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureLuminance->CreateTexture();
		
		pTextureTemporary1->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureTemporary1->CreateTexture();
		pTextureTemporary2->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureTemporary2->CreateTexture();
		pTextureTemporary3->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureTemporary3->CreateTexture();
		
		pTextureColor->SetSize( textureWidth, textureHeight, textureLayerCount );
		pTextureColor->CreateTexture();
		
		// recreate depth min-max texture
		if( deoglDRDepthMinMax::USAGE_VERSION != -1 ){
			int minMaxWidth, minMaxHeight;
			for( minMaxWidth=1; minMaxWidth<(textureWidth>>1); minMaxWidth<<=1 );
			for( minMaxHeight=1; minMaxHeight<(textureHeight>>1); minMaxHeight<<=1 );
			pDepthMinMax = new deoglDRDepthMinMax( pRenderThread, minMaxWidth, minMaxHeight, textureLayerCount, 11 );
		}
		
		// store the new texture size
		pRealWidth = textureWidth;
		pRealHeight = textureHeight;
		pRealLayerCount = textureLayerCount;
		
		pUpdateMemoryUsage();
		
		// create the fbos
		pCreateFBOs();
	}
	
	// store the new size and update the scalings
	pWidth = width;
	pHeight = height;
	pLayerCount = layerCount;
	
	pPixelSizeU = 1.0f / ( float )pRealWidth;
	pPixelSizeV = 1.0f / ( float )pRealHeight;
	pFSQuadOffU = 0.5f / ( float )pRealWidth;
	pFSQuadOffV = 0.5f / ( float )pRealHeight;
	pScalingU = ( float )width / ( float )pRealWidth;
	pScalingV = ( float )height / ( float )pRealHeight;
	pClampU = ( float )( width - 1 ) / ( float )pRealWidth;
	pClampV = ( float )( height - 1 ) / ( float )pRealHeight;
}

deoglArrayTexture *deoglDeferredRendering::GetDepthTexture1() const{
	return pModeDepth ? pTextureDepth1 : pTextureDepth2;
}

deoglArrayTexture *deoglDeferredRendering::GetDepthTexture2() const{
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
	// 
	// NOTE layer blitting is not supported. this has to be done manually
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const int copyFrom = pModeDepth ? efbocdDepth1Layer0 : efbocdDepth2Layer0;
	const int copyTo = pModeDepth ? efbocdDepth2Layer0 : efbocdDepth1Layer0;
	int i, mask = 0;
	
	if( copyDepth ){
		mask |= GL_DEPTH_BUFFER_BIT;
	}
	if( copyStencil ){
		mask |= GL_STENCIL_BUFFER_BIT;
	}
	
	for( i=0; i<pLayerCount; i++ ){
		OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, pFBOCopyDepth[ copyTo + i ]->GetFBO() ) );
		OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_READ_FRAMEBUFFER, pFBOCopyDepth[ copyFrom + i ]->GetFBO() ) );
		OGL_CHECK( pRenderThread, pglBlitFramebuffer( 0, 0, pWidth - 1, pHeight - 1, 0, 0, pWidth - 1, pHeight - 1, mask, GL_NEAREST ) );
	}
	
	OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, oldfbo->GetFBO() ) );
}

void deoglDeferredRendering::CopyFirstDepthToThirdDepth( bool copyDepth, bool copyStencil ){
	if( ! copyDepth && ! copyStencil ){
		return;
	}
	
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const int copyFrom = pModeDepth ? efbocdDepth1Layer0 : efbocdDepth2Layer0;
	int i, mask = 0;
	
	if( copyDepth ){
		mask |= GL_DEPTH_BUFFER_BIT;
	}
	if( copyStencil ){
		mask |= GL_STENCIL_BUFFER_BIT;
	}
	
	for( i=0; i<pLayerCount; i++ ){
		OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, pFBOCopyDepth[ efbocdDepth3Layer0 + i ]->GetFBO() ) );
		OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_READ_FRAMEBUFFER, pFBOCopyDepth[ copyFrom + i ]->GetFBO() ) );
		
		OGL_CHECK( pRenderThread, pglBlitFramebuffer( 0, 0, pWidth - 1, pHeight - 1,
			0, 0, pWidth - 1, pHeight - 1, mask, GL_NEAREST ) );
	}
	
	OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, oldfbo->GetFBO() ) );
}

void deoglDeferredRendering::CopyFirstDepthToXRayDepth( bool copyDepth, bool copyStencil ){
	// WARNING! this is not working because glBlitFramebuffer is NOT synchronized
	//          with rendering which means shaders using the XRay depth texture
	//          will NOT see the blitted content. who designed such a mess?!
	if( ! copyDepth && ! copyStencil ){
		return;
	}
	
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	const int copyFrom = pModeDepth ? efbocdDepth1Layer0 : efbocdDepth2Layer0;
	int i, mask = 0;
	
	if( copyDepth ){
		mask |= GL_DEPTH_BUFFER_BIT;
	}
	if( copyStencil ){
		mask |= GL_STENCIL_BUFFER_BIT;
	}
	
	OGL_CHECK( pRenderThread, glDisable( GL_SCISSOR_TEST ) );
	
	for( i=0; i<pLayerCount; i++ ){
		OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, pFBOCopyDepth[ efbocdDepthXRayLayer0 + i ]->GetFBO() ) );
		OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_READ_FRAMEBUFFER, pFBOCopyDepth[ copyFrom + i ]->GetFBO() ) );
		
		OGL_CHECK( pRenderThread, pglBlitFramebuffer( 0, 0, pWidth - 1, pHeight - 1,
			0, 0, pWidth - 1, pHeight - 1, mask, GL_NEAREST ) );
	}
	
	OGL_CHECK( pRenderThread, pglBindFramebuffer( GL_DRAW_FRAMEBUFFER, oldfbo->GetFBO() ) );
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

deoglArrayTexture *deoglDeferredRendering::GetPostProcessTexture() const{
	if( pModePostProcess ){
		return pTextureColor;
		
	}else{
		return pTextureTemporary2;
	}
}



void deoglDeferredRendering::ActivateFBODepth(){
	if( pModeDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdD1 ] );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdD2 ] );
	}
}

void deoglDeferredRendering::ActivateFBODepthLevel( int level ){
	if( level < 0 || level > pFBOMipMapDepthCount ){
		DETHROW( deeInvalidParam );
	}
	
	if( level == 0 ){
		ActivateFBODepth();
		
	}else{
		pRenderThread.GetFramebuffer().Activate( ( pModeDepth ? pFBOMipMapDepth1 : pFBOMipMapDepth2 )[ level - 1 ] );
	}
}

void deoglDeferredRendering::ActivateFBODepth3(){
	pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdD3 ] );
}

void deoglDeferredRendering::ActivateFBODepthXRay(){
	pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdDXRay ] );
}

void deoglDeferredRendering::ActivatePostProcessFBO( bool withDepth ){
	if( pModePostProcess ){
		ActivateFBOTemporary2( withDepth );
		
	}else{
		ActivateFBOColor( withDepth, false );
	}
}

void deoglDeferredRendering::ActivateFBODiffuse( bool withDepth ){
	if( withDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdDiffD1 : efbomdDiffD2 ] );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdDiff ] );
	}
}

void deoglDeferredRendering::ActivateFBOReflectivity( bool withDepth ){
	if( withDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdReflD1 : efbomdReflD2 ] );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdRefl ] );
	}
}

void deoglDeferredRendering::ActivateFBORoughness( bool withDepth ){
	if( withDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdRoughD1 : efbomdRoughD2 ] );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdRough ] );
	}
}

void deoglDeferredRendering::ActivateFBOAOSolidity( bool withDepth ){
	if( withDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdAOSolidityD1 : efbomdAOSolidityD2 ] );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdAOSolidity ] );
	}
}

void deoglDeferredRendering::ActivateFBOTemporary1( bool withDepth ){
	if( withDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdTemp1D1 : efbomdTemp1D2 ] );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp1 ] );
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
	if( withDepth ){
		pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdTemp2D1 : efbomdTemp2D2 ] );
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp2 ] );
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
	pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdTemp3 ] );
}

void deoglDeferredRendering::ActivateFBOColor( bool withDepth, bool withLuminance ){
	if( withDepth ){
		if( withLuminance ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdColorLumD1 : efbomdColorLumD2 ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdColorD1 : efbomdColorD2 ] );
		}
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ withLuminance ? efbomdColorLum : efbomdColor ] );
	}
}

void deoglDeferredRendering::ActivateFBOColorTemp2( bool withDepth, bool withLuminance ){
	if( withDepth ){
		if( withLuminance ){
			pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdColorLumTemp2D1 : efbomdColorLumTemp2D2 ] );
			
		}else{
			pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdColorTemp2D1 : efbomdColorTemp2D2 ] );
		}
		
	}else{
		pRenderThread.GetFramebuffer().Activate( pFBOs[ withLuminance ? efbomdColorLumTemp2 : efbomdColorTemp2 ] );
	}
}

void deoglDeferredRendering::ActivateFBOMaterialColor(){
	pRenderThread.GetFramebuffer().Activate( pFBOs[ pModeDepth ? efbomdMaterialColorD1 : efbomdMaterialColorD2 ] );
}

void deoglDeferredRendering::ActivateFBOLuminance(){
	pRenderThread.GetFramebuffer().Activate( pFBOs[ efbomdLuminance ] );
}



void deoglDeferredRendering::RenderFSQuadVAO(){
	OGL_CHECK( pRenderThread, pglBindVertexArray( pVAOFullScreenQuad->GetVAO() ) );
	OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLE_FAN, 0, 4 ) );
	OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
}

void deoglDeferredRendering::RenderFSQuadVAOStereo(){
	OGL_CHECK( pRenderThread, pglBindVertexArray( pVAOFullScreenQuad->GetVAO() ) );
	OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLES, 0, 12 ) );
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

void deoglDeferredRendering::SetShaderParamFSQuadUpsideDown( deoglShaderCompiled &shader, int parameter,
int width, int height ) const{
	// ( -1,  1 ) => ( 0, 0 )
	// (  1,  1 ) => ( su, 0 )
	// (  1, -1 ) => ( su, sv )
	// ( -1, -1 ) => ( 0, sv )
	//
	// tc.s = x * (su/2) + (su/2)
	// tc.t = y * (-sv/2) + (sv/2)
	const float hsu = pPixelSizeU * ( float )width * 0.5f;
	const float hsv = pPixelSizeV * ( float )height * 0.5f;
	
	shader.SetParameterFloat( parameter, hsu, -hsv, hsu, hsv );
}

void deoglDeferredRendering::SetShaderParamFSQuadUpsideDown( deoglShaderCompiled &shader, int parameter,
float x1, float y1, float x2, float y2 ) const{
	// ( -1,  1 ) => ( x1, y1 )
	// (  1,  1 ) => ( x2, y1 )
	// (  1, -1 ) => ( x2, y2 )
	// ( -1, -1 ) => ( x1, y2 )
	// 
	// tc.s = (x1+x2)/2 + (x2-x1)/2 * s
	// tc.t = (y2+y1)/2 + (y1-y2)/2 * t
	// 
	// tc.s = s * ((x2-x1)/2) + ((x1+x2)/2)
	// tc.t = t * ((y1-y2)/2) + ((y2+y1)/2)
	const float scaleS = pPixelSizeU * ( x2 - x1 ) * 0.5f;
	const float scaleT = pPixelSizeV * ( y1 - y2 ) * 0.5f;
	const float offsetS = pPixelSizeU * ( x1 + x2 ) * 0.5f;
	const float offsetT = pPixelSizeV * ( y2 + y1 ) * 0.5f;
	
	shader.SetParameterFloat( parameter, scaleS, scaleT, offsetS, offsetT );
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
	if( pVAOFullScreenQuad ){
		delete pVAOFullScreenQuad;
	}
	
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLBuffer( pVBOBillboard );
	dops.DeleteOpenGLBuffer( pVBOFullScreenQuad );
	
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
	if( pTextureLuminance ){
		delete pTextureLuminance;
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
	
	if( pTextureDepthXRay ){
		delete pTextureDepthXRay;
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
}

void deoglDeferredRendering::pCreateTextures(){
	// create depth textures
	pTextureDepth1 = new deoglArrayTexture( pRenderThread );
	pTextureDepth1->SetMipMapped( true );
	pTextureDepth1->SetDepthFormat( true, pUseInverseDepth );
	pTextureDepth1->SetDebugObjectLabel( "DefRen.Depth1" );
	
	pTextureDepth2 = new deoglArrayTexture( pRenderThread );
	pTextureDepth2->SetMipMapped( true );
	pTextureDepth2->SetDepthFormat( true, pUseInverseDepth );
	pTextureDepth2->SetDebugObjectLabel( "DefRen.Depth2" );
	
	pTextureDepth3 = new deoglArrayTexture( pRenderThread );
	pTextureDepth3->SetMipMapped( true );
	pTextureDepth3->SetDepthFormat( true, pUseInverseDepth );
	pTextureDepth3->SetDebugObjectLabel( "DefRen.Depth3" );
	
	pTextureDepthXRay = new deoglArrayTexture( pRenderThread );
	pTextureDepthXRay->SetMipMapped( true );
	pTextureDepthXRay->SetDepthFormat( true, pUseInverseDepth );
	pTextureDepthXRay->SetDebugObjectLabel( "DefRen.DepthXRay" );
	
	// create diffuse texture
	pTextureDiffuse = new deoglArrayTexture( pRenderThread );
	pTextureDiffuse->SetFBOFormat( 4, false ); //4, true );
	pTextureDiffuse->SetDebugObjectLabel( "DefRen.Diffuse" );
	
	// create normal texture
	// RGB8 gives strong jumping artifacts during the separate reflection pass due to errors
	// in the normals. RGB10A2 reduces the jumping artifacts but they are still faintly
	// visibile. With RGB16 the jumping artifacts are not visible anymore. RB11B11F has even
	// worse jumping artifacts.
	pTextureNormal = new deoglArrayTexture( pRenderThread );
	//pTextureNormal->SetFBOFormat( 3, false ); //4, true );
	pTextureNormal->SetFBOFormat( 3, true ); //4, true );
	//pTextureNormal->SetFormatFromCaps( deoglCapsFmtSupport::eutfRGB10A2 );
	pTextureNormal->SetDebugObjectLabel( "DefRen.Normal" );
	
	// create reflectivity texture
	pTextureReflectivity = new deoglArrayTexture( pRenderThread );
	pTextureReflectivity->SetFBOFormat( 3, false ); //4, true );
	pTextureReflectivity->SetDebugObjectLabel( "DefRen.Reflectivity" );
	
	// create roughness texture
	pTextureRoughness = new deoglArrayTexture( pRenderThread );
	pTextureRoughness->SetFBOFormat( 3, false );
	pTextureRoughness->SetDebugObjectLabel( "DefRen.Roughness" );
	
	// create ao-solidity texture
	pTextureAOSolidity = new deoglArrayTexture( pRenderThread );
	pTextureAOSolidity->SetFBOFormat( 3, false );
	pTextureAOSolidity->SetDebugObjectLabel( "DefRen.AOSolidity" );
	
	// create sub-surface texture
	pTextureSubSurface = new deoglArrayTexture( pRenderThread );
	pTextureSubSurface->SetFBOFormat( 3, true );
	pTextureSubSurface->SetDebugObjectLabel( "DefRen.SubSurface" );
	
	// create temporary textures
	pTextureTemporary1 = new deoglArrayTexture( pRenderThread );
	//pTextureTemporary1->SetMipMapped( true );
	pTextureTemporary1->SetFBOFormat( 3, true );
	pTextureTemporary1->SetDebugObjectLabel( "DefRen.Temporary1" );
	
	pTextureTemporary2 = new deoglArrayTexture( pRenderThread );
	//pTextureTemporary2->SetMipMapped( true );
	pTextureTemporary2->SetFBOFormat( 3, true );
	pTextureTemporary2->SetDebugObjectLabel( "DefRen.Temporary2" );
	
	pTextureTemporary3 = new deoglArrayTexture( pRenderThread );
	pTextureTemporary3->SetFBOFormat( 1, false );
	pTextureTemporary3->SetDebugObjectLabel( "DefRen.Temporary3" );
	
	// create color texture
	pTextureColor = new deoglArrayTexture( pRenderThread );
	pTextureColor->SetFBOFormat( 4, true );
	//pTextureColor->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRGBA32F ); // only for special debugging
	pTextureColor->SetDebugObjectLabel( "DefRen.Color" );
	
	// create luminance texture
	pTextureLuminance = new deoglArrayTexture( pRenderThread );
	pTextureLuminance->SetFBOFormat( 1, true );
	pTextureLuminance->SetDebugObjectLabel( "DefRen.Luminance" );
	
	// luminance textures
	/*
	pTextureLuminance = new deoglArrayTexture( pRenderThread );
	pTextureLuminance->SetFBOFormat( 1, true );
	pTextureLuminance->SetSize( 128, 64 );
	pTextureLuminance->CreateTexture();
	
	pTextureLuminanceNormal = new deoglArrayTexture( pRenderThread );
	pTextureLuminanceNormal->SetFBOFormat( 3, true );
	pTextureLuminanceNormal->SetSize( 128, 64 );
	pTextureLuminanceNormal->CreateTexture();
	
	pTextureLuminanceDepth = new deoglArrayTexture( pRenderThread );
	pTextureLuminanceDepth->SetDepthFormat( true, pUseInverseDepth );
	pTextureLuminanceDepth->SetSize( 128, 64 );
	pTextureLuminanceDepth->CreateTexture();
	*/
}

void deoglDeferredRendering::pUpdateMemoryUsage(){
	pMemUse.Clear();
	
	pMemUse.texture += pTextureDepth1->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureDepth2->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureDepth3->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureDepthXRay->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureDiffuse->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureNormal->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureReflectivity->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureRoughness->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureAOSolidity->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureSubSurface->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureColor->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureLuminance->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureTemporary1->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureTemporary2->GetMemoryConsumption().Total();
	pMemUse.texture += pTextureTemporary3->GetMemoryConsumption().Total();
	
	if( deoglDRDepthMinMax::USAGE_VERSION == 0 ){
		pMemUse.texture += pDepthMinMax->GetTexture()->GetMemoryConsumption().Total();
		
	}else if( deoglDRDepthMinMax::USAGE_VERSION == 1 ){
		pMemUse.texture += pDepthMinMax->GetTextureMin()->GetMemoryConsumption().Total();
		pMemUse.texture += pDepthMinMax->GetTextureMax()->GetMemoryConsumption().Total();
		
	}else if( deoglDRDepthMinMax::USAGE_VERSION == 2 ){
		pMemUse.texture += pDepthMinMax->GetTexture()->GetMemoryConsumption().Total();
	}
	
	pRenderThread.GetLogger().LogInfoFormat( "DefRen: Memory Consumption (%d x %d)", pRealWidth, pRealHeight );
	pRenderThread.GetLogger().LogInfoFormat( "- Texture: %uMB", pMemUse.texture.GetConsumptionMB() );
	pRenderThread.GetLogger().LogInfoFormat( "- Total: %uMB", pMemUse.TotalMB() );
}

void deoglDeferredRendering::pCreateFBOs(){
	const deoglRestoreFramebuffer restoreFbo( pRenderThread );
	
	pCreateFBOTex( efbomdD1, "DefRen.Depth1", NULL, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdD2, "DefRen.Depth2", NULL, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdD3, "DefRen.Depth3", NULL, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth3 );
	pCreateFBOTex( efbomdDXRay, "DefRen.DepthXRay", NULL, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepthXRay );
	pCreateFBOTex( efbomdDiff, "DefRen.Diffuse", pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdDiffD1, "DefRen.DiffuseD1", pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdDiffD2, "DefRen.DiffuseD2", pTextureDiffuse, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdRefl, "DefRen.Reflectivity", pTextureReflectivity, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdReflD1, "DefRen.ReflectivityD1", pTextureReflectivity, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdReflD2, "DefRen.ReflectivityD2", pTextureReflectivity, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdRough, "DefRen.Roughness", pTextureRoughness, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdRoughD1, "DefRen.RoughnessD1", pTextureRoughness, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdRoughD2, "DefRen.RoughnessD2", pTextureRoughness, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdAOSolidity, "DefRen.AOSolidity", pTextureAOSolidity, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdAOSolidityD1, "DefRen.AOSolidityD1", pTextureAOSolidity, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdAOSolidityD2, "DefRen.AOSolidityD2", pTextureAOSolidity, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdTemp1, "DefRen.Temporary1", pTextureTemporary1, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdTemp1D1, "DefRen.Temporary1D1", pTextureTemporary1, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdTemp1D2, "DefRen.Temporary1D2", pTextureTemporary1, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdTemp2, "DefRen.Temporary2", pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdTemp2D1, "DefRen.Temporary2D1", pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdTemp2D2, "DefRen.Temporary2D2", pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdTemp3, "DefRen.Temporary3", pTextureTemporary3, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdColor, "DefRen.Color", pTextureColor, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdColorD1, "DefRen.ColorD1", pTextureColor, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdColorD2, "DefRen.ColorD2", pTextureColor, NULL, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdColorTemp2, "DefRen.ColorTemp2", pTextureColor, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdColorTemp2D1, "DefRen.ColorTemp2D1", pTextureColor, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdColorTemp2D2, "DefRen.ColorTemp2D2", pTextureColor, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdColorLum, "DefRen.ColorLum", pTextureColor, pTextureLuminance, NULL, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdColorLumD1, "DefRen.ColorLumD1", pTextureColor, pTextureLuminance, NULL, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdColorLumD2, "DefRen.ColorLumD2", pTextureColor, pTextureLuminance, NULL, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdColorLumTemp2, "DefRen.ColorLumTemp2", pTextureColor, pTextureLuminance, pTextureTemporary2, NULL, NULL, NULL, NULL, NULL );
	pCreateFBOTex( efbomdColorLumTemp2D1, "DefRen.ColorLumTemp2D1", pTextureColor, pTextureLuminance, pTextureTemporary2, NULL, NULL, NULL, NULL, pTextureDepth1 );
	pCreateFBOTex( efbomdColorLumTemp2D2, "DefRen.ColorLumTemp2D2", pTextureColor, pTextureLuminance, pTextureTemporary2, NULL, NULL, NULL, NULL, pTextureDepth2 );
	pCreateFBOTex( efbomdLuminance, "DefRen.Luminance", pTextureLuminance, NULL, NULL, NULL, NULL, NULL, NULL, NULL );
	
	if( pRenderThread.GetCapabilities().GetMaxDrawBuffers() >= 8 ){
		pCreateFBOTex( efbomdMaterialColorD1, "DefRen.MaterialColorD1", pTextureDiffuse, pTextureNormal,
			pTextureReflectivity, pTextureRoughness, pTextureAOSolidity,
			pTextureSubSurface, pTextureColor, pTextureDepth1 );
		pCreateFBOTex( efbomdMaterialColorD2, "DefRen.MaterialColorD2", pTextureDiffuse, pTextureNormal,
			pTextureReflectivity, pTextureRoughness, pTextureAOSolidity,
			pTextureSubSurface, pTextureColor, pTextureDepth2 );
		
	}else{
		pCreateFBOTex( efbomdMaterialColorD1, "DefRen.MaterialColorD1", pTextureDiffuse, pTextureNormal,
			pTextureReflectivity, pTextureColor, NULL, NULL, NULL, pTextureDepth1 );
		pCreateFBOTex( efbomdMaterialColorD2, "DefRen.MaterialColorD2", pTextureDiffuse, pTextureNormal,
			pTextureReflectivity, pTextureColor, NULL, NULL, NULL, pTextureDepth2 );
	}
	
	char debugName[ 31 ];
	
	// fbos for the mip map levels
	int fboMipMapCount = pTextureDepth1->GetRealMipMapLevelCount();
	
	if( fboMipMapCount > 0 ){
		int i;
		
		pFBOMipMapDepth1 = new deoglFramebuffer*[ fboMipMapCount ];
		pFBOMipMapDepth2 = new deoglFramebuffer*[ fboMipMapCount ];
		
		for( pFBOMipMapDepthCount=0; pFBOMipMapDepthCount<fboMipMapCount; pFBOMipMapDepthCount++ ){
			pFBOMipMapDepth1[ pFBOMipMapDepthCount ] = NULL;
			pFBOMipMapDepth2[ pFBOMipMapDepthCount ] = NULL;
		}
		
		for( i=0; i<fboMipMapCount; i++ ){
			try{
				pFBOMipMapDepth1[ i ] = new deoglFramebuffer( pRenderThread, false );
				pRenderThread.GetFramebuffer().Activate( pFBOMipMapDepth1[ i ] );
				pFBOMipMapDepth1[ i ]->AttachDepthArrayTextureLevel( pTextureDepth1, i + 1 );
				const GLenum buffers[ 1 ] = { GL_NONE };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
				pFBOMipMapDepth1[ i ]->Verify();
				sprintf( debugName, "DefRen.Depth1.MipMap%d", i );
				pFBOMipMapDepth1[ i ]->SetDebugObjectLabel( debugName );
				
			}catch( const deException &e ){
// 				deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBOs", __LINE__ );
// 				tracePoint.AddValue( "texture", "MipMapDepth1" );
// 				tracePoint.AddValueInt( "level", i + 1 );
				throw;
			}
			
			try{
				pFBOMipMapDepth2[ i ] = new deoglFramebuffer( pRenderThread, false );
				pRenderThread.GetFramebuffer().Activate( pFBOMipMapDepth2[ i ] );
				pFBOMipMapDepth2[ i ]->AttachDepthArrayTextureLevel( pTextureDepth2, i + 1 );
				const GLenum buffers[ 1 ] = { GL_NONE };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
				pFBOMipMapDepth2[ i ]->Verify();
				sprintf( debugName, "DefRen.Depth2.MipMap%d", i );
				pFBOMipMapDepth2[ i ]->SetDebugObjectLabel( debugName );
				
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
				pFBOMipMapTemporary1[ i ]->AttachColorArrayTextureLevel( 0, pTextureTemporary1, i + 1 );
				const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
				pFBOMipMapTemporary1[ i ]->Verify();
				sprintf( debugName, "DefRen.Temporary1.MipMap%d", i );
				pFBOMipMapTemporary1[ i ]->SetDebugObjectLabel( debugName );
				
			}catch( const deException &e ){
// 				deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBOs", __LINE__ );
// 				tracePoint.AddValue( "texture", "MipMapTemporary1" );
// 				tracePoint.AddValueInt( "level", i + 1 );
				throw;
			}
			
			try{
				pFBOMipMapTemporary2[ i ] = new deoglFramebuffer( pRenderThread, false );
				pRenderThread.GetFramebuffer().Activate( pFBOMipMapTemporary2[ i ] );
				pFBOMipMapTemporary2[ i ]->AttachColorArrayTextureLevel( 0, pTextureTemporary2, i + 1 );
				const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
				pFBOMipMapTemporary2[ i ]->Verify();
				sprintf( debugName, "DefRen.Temporary2.MipMap%d", i );
				pFBOMipMapTemporary2[ i ]->SetDebugObjectLabel( debugName );
				
			}catch( const deException &e ){
// 				deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBOs", __LINE__ );
// 				tracePoint.AddValue( "texture", "MipMapTemporary2" );
// 				tracePoint.AddValueInt( "level", i + 1 );
				throw;
			}
		}
	}
	
	// FBOs for copy depth
	deoglArrayTexture * const copyDepthTex[ 4 ] = { pTextureDepth1, pTextureDepth2, pTextureDepth3, pTextureDepthXRay };
	int i;
	
	for( i=0; i<8; i++ ){
		pFBOCopyDepth[ i ] = new deoglFramebuffer( pRenderThread, false );
		pRenderThread.GetFramebuffer().Activate( pFBOCopyDepth[ i ] );
		pFBOCopyDepth[ i ]->AttachDepthArrayTextureLayer( copyDepthTex[ i / 2 ], decMath::min( i % 2, pLayerCount - 1 ) );
		const GLenum buffersNone[ 1 ] = { GL_NONE };
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffersNone ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
		pFBOCopyDepth[ i ]->Verify();
		sprintf( debugName, "DefRen.CopyDepth.Layer%d", i );
		pFBOCopyDepth[ i ]->SetDebugObjectLabel( debugName );
	}
	
	// luminance fbo
	/*
	const GLenum buffers[ 2 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	
	pFBOLuminance = new deoglFramebuffer( pRenderThread, false );
	pRenderThread.GetFramebuffer().Activate( pFBOLuminance );
	pFBOLuminance->AttachDepthTexture( pTextureLuminanceDepth );
	pFBOLuminance->AttachColorArrayTexture( 0, pTextureLuminance );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOLuminance->Verify();
	
	pFBOLuminanceNormal = new deoglFramebuffer( pRenderThread, false );
	pRenderThread.GetFramebuffer().Activate( pFBOLuminanceNormal );
	pFBOLuminanceNormal->AttachDepthTexture( pTextureLuminanceDepth );
	pFBOLuminanceNormal->AttachColorArrayTexture( 0, pTextureLuminance );
	pFBOLuminanceNormal->AttachColorArrayTexture( 1, pTextureLuminanceNormal );
	OGL_CHECK( pRenderThread, pglDrawBuffers( 2, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	pFBOLuminanceNormal->Verify();
	*/
}

void deoglDeferredRendering::pCreateFBOTex( int index, const char *debugName,
deoglArrayTexture *texture1, deoglArrayTexture *texture2, deoglArrayTexture *texture3,
deoglArrayTexture *texture4, deoglArrayTexture *texture5, deoglArrayTexture *texture6,
deoglArrayTexture *texture7, deoglArrayTexture *depth ){
	try{
		pFBOs[ index ] = new deoglFramebuffer( pRenderThread, false );
		
		pRenderThread.GetFramebuffer().Activate( pFBOs[ index ] );
		
		if( depth ){
			pFBOs[ index ]->AttachDepthArrayTexture( depth );
			pFBOs[ index ]->AttachStencilArrayTexture( depth );
		}
		pFBOAttachColors( index, debugName, texture1, texture2, texture3, texture4, texture5, texture6, texture7 );
		
		pFBOs[ index ]->Verify();
		
	}catch( const deException &e ){
// 		deErrorTracePoint &tracePoint = *pOgl->AddErrorTracePoint( "deoglDeferredRendering::pCreateFBOTex", __LINE__ );
// 		tracePoint.AddValueInt( "index", index );
		throw;
	}
}

void deoglDeferredRendering::pFBOAttachColors( int index, const char *debugName,
deoglArrayTexture *texture1, deoglArrayTexture *texture2, deoglArrayTexture *texture3,
deoglArrayTexture *texture4, deoglArrayTexture *texture5, deoglArrayTexture *texture6,
deoglArrayTexture *texture7 ){
	const GLenum buffers[ 7 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
	
	if( texture1 ){
		pFBOs[ index ]->AttachColorArrayTexture( 0, texture1 );
		
		if( texture2 ){
			pFBOs[ index ]->AttachColorArrayTexture( 1, texture2 );
			
			if( texture3 ){
				pFBOs[ index ]->AttachColorArrayTexture( 2, texture3 );
				
				if( texture4 ){
					pFBOs[ index ]->AttachColorArrayTexture( 3, texture4 );
					
					if( texture5 ){
						pFBOs[ index ]->AttachColorArrayTexture( 4, texture5 );
						
						if( texture6 ){
							pFBOs[ index ]->AttachColorArrayTexture( 5, texture6 );
							
							if( texture7 ){
								pFBOs[ index ]->AttachColorArrayTexture( 6, texture7 );
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
	
	pFBOs[ index ]->SetDebugObjectLabel( debugName );
}

void deoglDeferredRendering::pDestroyFBOs(){
	int i;
	
	for( i=0; i<8; i++ ){
		if( pFBOCopyDepth[ i ] ){
			delete pFBOCopyDepth[ i ];
			pFBOCopyDepth[ i ] = nullptr;
		}
	}
	
	for( i=0; i<pFBOMipMapDepthCount; i++ ){
		if( pFBOMipMapDepth1[ i ] ){
			delete pFBOMipMapDepth1[ i ];
			pFBOMipMapDepth1[ i ] = NULL;
		}
		if( pFBOMipMapDepth2[ i ] ){
			delete pFBOMipMapDepth2[ i ];
			pFBOMipMapDepth2[ i ] = NULL;
		}
	}
	pFBOMipMapDepthCount = 0;
	if( pFBOMipMapDepth1 ){
		delete [] pFBOMipMapDepth1;
	}
	if( pFBOMipMapDepth2 ){
		delete [] pFBOMipMapDepth2;
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
	}
	pFBOMipMapCount = 0;
	if( pFBOMipMapTemporary1 ){
		delete [] pFBOMipMapTemporary1;
	}
	if( pFBOMipMapTemporary2 ){
		delete [] pFBOMipMapTemporary2;
	}
	
	for( i=0; i<EFBOMD_COUNT; i++ ){
		if( pFBOs[ i ] ){
			delete pFBOs[ i ];
			pFBOs[ i ] = NULL;
		}
	}
}
