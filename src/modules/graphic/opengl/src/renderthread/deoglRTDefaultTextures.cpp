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

#include "deoglRTDefaultTextures.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTDefaultTextures
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTDefaultTextures::deoglRTDefaultTextures( deoglRenderThread &renderThread ) :
pColor( NULL ),
pTransparency( NULL ),
pNormal( NULL ),
pHeight( NULL ),
pReflectivity( NULL ),
pRoughness( NULL ),
pEmissivity( NULL ),
pAO( NULL ),
pSolidity( NULL ),
pEnvRoomMask( NULL ),
pNonPbrMetalness( nullptr ),
pShadowMap( NULL ),
pShadowMapInverseDepth( NULL ),
pShadowMapColor( NULL ),
pShadowCube( NULL ),
pShadowCubeInverseDepth( nullptr ),
pShadowCubeColor( NULL ),

pWeights( NULL ),
pMaskOpaque( NULL ),
pNoise2D( NULL ),
pEnvMap( NULL ),

pColorArray( NULL ),
pTransparencyArray( NULL ),
pNormalArray( NULL ),
pHeightArray( NULL ),
pReflectivityArray( NULL ),
pRoughnessArray( NULL ),
pEmissivityArray( NULL ),
pAOArray( NULL ),
pSolidityArray( NULL ),
pEnvRoomMaskArray( NULL ),
pNonPbrMetalnessArray( nullptr )
{
	try{
		pCreateDefaultTextures( renderThread );
		pCreateDefaultTexturesArray( renderThread );
		pCreateTextureMaskOpaque( renderThread );
		pCreateTextureNoise2D( renderThread );
		//pCreateWeightsTexture( renderThread );
		pCreateShadowTextures( renderThread );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRTDefaultTextures::~deoglRTDefaultTextures(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoglRTDefaultTextures::pCleanUp(){
	if( pNonPbrMetalnessArray ){
		delete pNonPbrMetalnessArray;
	}
	if( pEnvRoomMaskArray ){
		delete pEnvRoomMaskArray;
	}
	if( pSolidityArray ){
		delete pSolidityArray;
	}
	if( pAOArray ){
		delete pAOArray;
	}
	if( pEmissivityArray ){
		delete pEmissivityArray;
	}
	if( pRoughnessArray ){
		delete pRoughnessArray;
	}
	if( pReflectivityArray ){
		delete pReflectivityArray;
	}
	if( pHeightArray ){
		delete pHeightArray;
	}
	if( pNormalArray ){
		delete pNormalArray;
	}
	if( pTransparencyArray ){
		delete pTransparencyArray;
	}
	if( pColorArray ){
		delete pColorArray;
	}
	
	if( pEnvMap ){
		delete pEnvMap;
	}
	if( pNoise2D ){
		delete pNoise2D;
	}
	if( pMaskOpaque ){
		delete pMaskOpaque;
	}
	if( pWeights ){
		delete pWeights;
	}
	
	if( pShadowCubeColor ){
		delete pShadowCubeColor;
	}
	if( pShadowCube ){
		delete pShadowCube;
	}
	if( pShadowCubeInverseDepth ){
		delete pShadowCubeInverseDepth;
	}
	if( pShadowMapColor ){
		delete pShadowMapColor;
	}
	if( pShadowMap ){
		delete pShadowMap;
	}
	if( pShadowMapInverseDepth ){
		delete pShadowMapInverseDepth;
	}
	if( pNonPbrMetalness ){
		delete pNonPbrMetalness;
	}
	if( pEnvRoomMask ){
		delete pEnvRoomMask;
	}
	if( pSolidity ){
		delete pSolidity;
	}
	if( pAO ){
		delete pAO;
	}
	if( pEmissivity ){
		delete pEmissivity;
	}
	if( pRoughness ){
		delete pRoughness;
	}
	if( pReflectivity ){
		delete pReflectivity;
	}
	if( pHeight ){
		delete pHeight;
	}
	if( pNormal ){
		delete pNormal;
	}
	if( pTransparency ){
		delete pTransparency;
	}
	if( pColor ){
		delete pColor;
	}
}

void deoglRTDefaultTextures::pCreateDefaultTextures( deoglRenderThread &renderThread ){
	const deoglPixelBuffer::Ref pixelBuffer1( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfByte4, 1, 1, 1 ) ) );
	const deoglPixelBuffer::Ref pixelBuffer2( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfFloat4, 1, 1, 6 ) ) );
	
	// diffuse channel texture: color.rgb, alpha
	pixelBuffer1->SetToIntColor( 0, 0, 0, 255 );
	pColor = new deoglTexture( renderThread );
	pColor->SetSize( 1, 1 );
	pColor->SetMapingFormat( 4, false, false );
	pColor->SetPixels( pixelBuffer1 );
	
	// transparency channel texture: alpha, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 0, 0, 0 );
	pTransparency = new deoglTexture( renderThread );
	pTransparency->SetSize( 1, 1 );
	pTransparency->SetMapingFormat( 4, false, false );
	pTransparency->SetPixels( pixelBuffer1 );
	
	// normal channel texture: normal.rgb, normalDeviation
	pixelBuffer1->SetToIntColor( 128, 128, 255, 0 );
	pNormal = new deoglTexture( renderThread );
	pNormal->SetSize( 1, 1 );
	pNormal->SetMapingFormat( 4, false, false );
	pNormal->SetPixels( pixelBuffer1 );
	
	// height channel texture: height, cone, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 255, 0, 0 );
	pHeight = new deoglTexture( renderThread );
	pHeight->SetSize( 1, 1 );
	pHeight->SetMapingFormat( 2, false, false );
	pHeight->SetPixels( pixelBuffer1 );
	
	// reflectivity channel texture: reflectivity.rgb
	pixelBuffer1->SetToIntColor( 0, 0, 0, 0 );
	pReflectivity = new deoglTexture( renderThread );
	pReflectivity->SetSize( 1, 1 );
	pReflectivity->SetMapingFormat( 4, false, false );
	pReflectivity->SetPixels( pixelBuffer1 );
	
	// roughness channel texture: roughness
	pixelBuffer1->SetToIntColor( 255, 0, 0, 0 );
	pRoughness = new deoglTexture( renderThread );
	pRoughness->SetSize( 1, 1 );
	pRoughness->SetMapingFormat( 4, false, false );
	pRoughness->SetPixels( pixelBuffer1 );
	
	// emissivity channel texture: emissivity.rgb, n/a
	pixelBuffer1->SetToIntColor( 0, 0, 0, 0 );
	pEmissivity = new deoglTexture( renderThread );
	pEmissivity->SetSize( 1, 1 );
	pEmissivity->SetMapingFormat( 4, false, false );
	pEmissivity->SetPixels( pixelBuffer1 );
	
	// ambient occlusion channel texture: ambinetOcclusion, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 255, 255, 255 );
	pAO = new deoglTexture( renderThread );
	pAO->SetSize( 1, 1 );
	pAO->SetMapingFormat( 4, false, false );
	pAO->SetPixels( pixelBuffer1 );
	
	// solidity channel texture: solidity, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 255, 255, 255 );
	pSolidity = new deoglTexture( renderThread );
	pSolidity->SetSize( 1, 1 );
	pSolidity->SetMapingFormat( 4, false, false );
	pSolidity->SetPixels( pixelBuffer1 );
	
	// environment room mask channel texture: mask, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 0, 0, 0 );
	pEnvRoomMask = new deoglTexture( renderThread );
	pEnvRoomMask->SetSize( 1, 1 );
	pEnvRoomMask->SetMapingFormat( 4, false, false );
	pEnvRoomMask->SetPixels( pixelBuffer1 );
	
	// environment map: 50% gray for all pixels
	pixelBuffer2->SetToFloatColor( 0.5f, 0.5f, 0.5f, 1.0f );
	pEnvMap = new deoglCubeMap( renderThread );
	pEnvMap->SetSize( 1 );
	pEnvMap->SetMapingFormat( 3, true, false );
	pEnvMap->SetPixels( pixelBuffer2 );
	
	// non-pbr metalness: metalness, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 0, 0, 0, 0 );
	pNonPbrMetalness = new deoglTexture( renderThread );
	pNonPbrMetalness->SetSize( 1, 1 );
	pNonPbrMetalness->SetMapingFormat( 4, false, false );
	pNonPbrMetalness->SetPixels( pixelBuffer1 );
}

void deoglRTDefaultTextures::pCreateDefaultTexturesArray( deoglRenderThread &renderThread ){
	const deoglPixelBuffer::Ref pixelBuffer1( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfByte4, 1, 1, 1 ) ) );
	
	// diffuse channel texture: color.rgb, alpha
	pixelBuffer1->SetToIntColor( 0, 0, 0, 255 );
	pColorArray = new deoglArrayTexture( renderThread );
	pColorArray->SetSize( 1, 1, 1 );
	pColorArray->SetMapingFormat( 4, false, false );
	pColorArray->SetPixels( pixelBuffer1 );
	
	// transparency channel texture: alpha, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 0, 0, 0 );
	pTransparencyArray = new deoglArrayTexture( renderThread );
	pTransparencyArray->SetSize( 1, 1, 1 );
	pTransparencyArray->SetMapingFormat( 4, false, false );
	pTransparencyArray->SetPixels( pixelBuffer1 );
	
	// normal channel texture: normal.rgb, normalDeviation
	pixelBuffer1->SetToIntColor( 128, 128, 255, 0 );
	pNormalArray = new deoglArrayTexture( renderThread );
	pNormalArray->SetSize( 1, 1, 1 );
	pNormalArray->SetMapingFormat( 4, false, false );
	pNormalArray->SetPixels( pixelBuffer1 );
	
	// height channel texture: height, cone, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 255, 0, 0 );
	pHeightArray = new deoglArrayTexture( renderThread );
	pHeightArray->SetSize( 1, 1, 1 );
	pHeightArray->SetMapingFormat( 2, false, false );
	pHeightArray->SetPixels( pixelBuffer1 );
	
	// reflectivity channel texture: roughness
	pixelBuffer1->SetToIntColor( 0, 0, 0, 0 );
	pReflectivityArray = new deoglArrayTexture( renderThread );
	pReflectivityArray->SetSize( 1, 1, 1 );
	pReflectivityArray->SetMapingFormat( 4, false, false );
	pReflectivityArray->SetPixels( pixelBuffer1 );
	
	// roughness channel texture: roughness
	pixelBuffer1->SetToIntColor( 255, 0, 0, 0 );
	pRoughnessArray = new deoglArrayTexture( renderThread );
	pRoughnessArray->SetSize( 1, 1, 1 );
	pRoughnessArray->SetMapingFormat( 4, false, false );
	pRoughnessArray->SetPixels( pixelBuffer1 );
	
	// emissivity channel texture: emissivity.rgb, n/a
	pixelBuffer1->SetToIntColor( 0, 0, 0, 0 );
	pEmissivityArray = new deoglArrayTexture( renderThread );
	pEmissivityArray->SetSize( 1, 1, 1 );
	pEmissivityArray->SetMapingFormat( 4, false, false );
	pEmissivityArray->SetPixels( pixelBuffer1 );
	
	// ambient occlusion channel texture: ambinetOcclusion, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 255, 255, 255 );
	pAOArray = new deoglArrayTexture( renderThread );
	pAOArray->SetSize( 1, 1, 1 );
	pAOArray->SetMapingFormat( 4, false, false );
	pAOArray->SetPixels( pixelBuffer1 );
	
	// solidity channel texture: solidity, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 255, 255, 255 );
	pSolidityArray = new deoglArrayTexture( renderThread );
	pSolidityArray->SetSize( 1, 1, 1 );
	pSolidityArray->SetMapingFormat( 4, false, false );
	pSolidityArray->SetPixels( pixelBuffer1 );
	
	// environment room mask channel texture: mask, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 255, 0, 0, 0 );
	pEnvRoomMaskArray = new deoglArrayTexture( renderThread );
	pEnvRoomMaskArray->SetSize( 1, 1, 1 );
	pEnvRoomMaskArray->SetMapingFormat( 4, false, false );
	pEnvRoomMaskArray->SetPixels( pixelBuffer1 );
	
	// non-pbr metalness: metalness, n/a, n/a, n/a
	pixelBuffer1->SetToIntColor( 0, 0, 0, 0 );
	pNonPbrMetalnessArray = new deoglArrayTexture( renderThread );
	pNonPbrMetalnessArray->SetSize( 1, 1, 1 );
	pNonPbrMetalnessArray->SetMapingFormat( 4, false, false );
	pNonPbrMetalnessArray->SetPixels( pixelBuffer1 );
}

void deoglRTDefaultTextures::pCreateWeightsTexture( deoglRenderThread &renderThread ){
	pWeights = new deoglTexture( renderThread );
	pWeights->SetSize( 128, 128 );
	pWeights->SetMapingFormat( 4, true, false );
	pWeights->CreateTexture();
}

void deoglRTDefaultTextures::pCreateTextureMaskOpaque( deoglRenderThread &renderThread ){
	const deoglPixelBuffer::Ref pixelBuffer( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfByte1, 1, 1, 1 ) ) );
	
	( pixelBuffer->GetPointerByte1() )->r = 255;
	
	pMaskOpaque = new deoglTexture( renderThread );
	pMaskOpaque->SetSize( 1, 1 );
	pMaskOpaque->SetMapingFormat( 1, false, false );
	pMaskOpaque->CreateTexture();
	pMaskOpaque->SetPixels( pixelBuffer );
}

__attribute__((no_sanitize("signed-integer-overflow", "shift")))
static void deoglRTDefaultTextures_CreateNoiseData( deoglPixelBuffer &pixelBuffer, int size ){
	// the noise code is moved into an own function so it can be qualified with a no_sanitize.
	// the problem is that this code uses integer overflow to create random noise data. the gcc
	// sanitizer does not like this overflow behavior hence it is move to silence the warning
	deoglPixelBuffer::sByte4 *destInt32 = pixelBuffer.GetPointerByte4();
	const int seed = 17342954;
	int p, x, y;
	
	for( p=0, y=0; y<size; y++ ){
		for( x=0; x<size; x++, p++ ){
			int n = x + y * 57 + seed;
			n = ( n << 13 ) ^ n;
			const double rot = ( ( double )( ( n * ( n * n * 15731 + 789221 )
				+ 1376312589 ) & 0x7fffffff ) / 1073741824.0 );
			
			destInt32[ p ].r = ( unsigned char )( sin( rot * PI ) * 127.5 + 127.5 );
			destInt32[ p ].g = ( unsigned char )( cos( rot * PI ) * 127.5 + 127.5 );
			destInt32[ p ].b = 0;
			destInt32[ p ].a = 0;
		}
	}
}

void deoglRTDefaultTextures::pCreateTextureNoise2D( deoglRenderThread &renderThread ){
	const int size = 32;
	const deoglPixelBuffer::Ref pixelBuffer( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfByte4, size, size, 1 ) ) );
	deoglRTDefaultTextures_CreateNoiseData( pixelBuffer, size );
	
	pNoise2D = new deoglTexture( renderThread );
	pNoise2D->SetSize( size, size );
	pNoise2D->SetMapingFormat( 4, false, false );
	pNoise2D->CreateTexture();
	pNoise2D->SetPixels( pixelBuffer );
	
	/*
	int u, x, seedu = 17342954;
	int v, y, seedv = 28017626;
	int p, size = 32;
	deoglPixelBuffer pixelBuffer( deoglPixelBuffer::epfByte4, size, size );
	
	deoglPixelBuffer::sByte4 *destInt32 = pixelBuffer.GetPointerInt32();
	for( p=0, y=0; y<size; y++ ){
		for( x=0; x<size; x++, p++ ){
			u = x + y * 57 + seedu;
			u = ( u << 13 ) ^ u;
			destInt32[ p ].r = ( u * ( u * u * 15731 + 789221 ) + 1376312589 ) >> 24;
			
			v = x + y * 57 + seedv;
			v = ( v << 13 ) ^ v;
			destInt32[ p ].g = ( v * ( v * v * 15731 + 789221 ) + 1376312589 ) >> 24;
			
			destInt32[ p ].b = 0;
			destInt32[ p ].a = 0;
		}
	}
	
	pNoise2D = new deoglTexture( this );
	if( ! pNoise2D ) DETHROW( deeOutOfMemory );
	
	pNoise2D->SetMipMapped( false );
	pNoise2D->SetCompressed( false );
	pNoise2D->SetMapingFormat( 8, 4, false );
	pNoise2D->SetSize( size, size );
	pNoise2D->CreateTexture();
	pNoise2D->SetPixels( pixelBuffer );
	*/
}

void deoglRTDefaultTextures::pCreateShadowTextures( deoglRenderThread &renderThread ){
	// shadow map
	const deoglPixelBuffer::Ref pixelBuffer1( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfDepth, 1, 1, 1 ) ) );
	pixelBuffer1->SetToFloatColor( 1.0f, 1.0f, 1.0f, 1.0f );
	pShadowMap = new deoglTexture( renderThread );
	pShadowMap->SetSize( 1, 1 );
	pShadowMap->SetDepthFormat( false, false );
	pShadowMap->SetPixels( pixelBuffer1 );
	
	pixelBuffer1->SetToFloatColor( 0.0f, 0.0f, 0.0f, 1.0f );
	pShadowMapInverseDepth = new deoglTexture( renderThread );
	pShadowMapInverseDepth->SetSize( 1, 1 );
	pShadowMapInverseDepth->SetDepthFormat( false, true );
	pShadowMapInverseDepth->SetPixels( pixelBuffer1 );
	
	const deoglPixelBuffer::Ref pixelBuffer1b( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfByte3, 1, 1, 1 ) ) );
	pixelBuffer1b->SetToIntColor( 0, 0, 0, 255 );
	pShadowMapColor = new deoglTexture( renderThread );
	pShadowMapColor->SetSize( 1, 1 );
	pShadowMapColor->SetMapingFormat( 3, false, false );
	pShadowMapColor->SetPixels( pixelBuffer1b );
	
	// shadow cube
	const deoglPixelBuffer::Ref pixelBuffer2( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfDepth, 1, 1, 6 ) ) );
	pixelBuffer2->SetToFloatColor( 1.0f, 1.0f, 1.0f, 1.0f );
	pShadowCube = new deoglCubeMap( renderThread );
	pShadowCube->SetSize( 1 );
	pShadowCube->SetDepthFormat( false );
	pShadowCube->SetPixels( pixelBuffer2 );
	
	// shadow cube inverse depth
	pixelBuffer2->SetToFloatColor( 0.0f, 0.0f, 0.0f, 0.0f );
	pShadowCubeInverseDepth = new deoglCubeMap( renderThread );
	pShadowCubeInverseDepth->SetSize( 1 );
	pShadowCubeInverseDepth->SetDepthFormat( true );
	pShadowCubeInverseDepth->SetPixels( pixelBuffer2 );
	
	const deoglPixelBuffer::Ref pixelBuffer2b( deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer( deoglPixelBuffer::epfByte3, 1, 1, 6 ) ) );
	pixelBuffer2b->SetToIntColor( 0, 0, 0, 255 );
	pShadowCubeColor = new deoglCubeMap( renderThread );
	pShadowCubeColor->SetSize( 1 );
	pShadowCubeColor->SetMapingFormat( 3, false, false );
	pShadowCubeColor->SetPixels( pixelBuffer2b );
}
