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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglLightShaderConfig.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>



// Class deoglLightShaderConfig
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightShaderConfig::deoglLightShaderConfig(){
	Reset();
}

deoglLightShaderConfig::deoglLightShaderConfig( const deoglLightShaderConfig &config ){
	Reset();
	*this = config;
}

deoglLightShaderConfig::~deoglLightShaderConfig(){
}



// Management
///////////////

void deoglLightShaderConfig::Reset(){
	pLightMode = elmSpot;
	pShadowTapMode = estmSingle;
	pShadowMappingAlgorithm1 = esma2D;
	pShadowMappingAlgorithm2 = esma2D;
	pMaterialNormalModeDec = emnmFloat;
	pMaterialNormalModeEnc = emnmFloat;
	pParticleMode = epmParticle;
	
	pDecodeInDepth = false;
	pDecodeInShadow = false;
	pEncodeOutDepth = false;
	pHWDepthCompare = true;
	pShadowMatrix2EqualsMatrix1 = true;
	pAmbientLighting = false;
	pShadowInverseDepth = false;
	pFullScreenQuad = false;
	pSubSurface = false;
	pLuminanceOnly = false;
	pGIRay = false;
	pGSRenderStereo = false;
	pVSRenderStereo = false;
	
	pTextureNoise = false;
	pTextureColor = false;
	pTextureColorOmniCube = false;
	pTextureColorOmniEquirect = false;
	pTextureShadow1Solid = false;
	pTextureShadow1Transparent = false;
	pTextureShadow2Solid = false;
	pTextureShadow2Transparent = false;
	pTextureShadow1Ambient = false;
	pTextureShadow2Ambient = false;
	
	UpdateKey();
}



void deoglLightShaderConfig::SetLightMode( eLightModes lightMode ){
	pLightMode = lightMode;
}

void deoglLightShaderConfig::SetShadowTapMode( eShadowTapModes shadowTapMode ){
	pShadowTapMode = shadowTapMode;
}

void deoglLightShaderConfig::SetShadowMappingAlgorithm1( eShadowMappingAlgorithms shadowMappingAlgorithm ){
	pShadowMappingAlgorithm1 = shadowMappingAlgorithm;
}

void deoglLightShaderConfig::SetShadowMappingAlgorithm2( eShadowMappingAlgorithms shadowMappingAlgorithm ){
	pShadowMappingAlgorithm2 = shadowMappingAlgorithm;
}

void deoglLightShaderConfig::SetMaterialNormalModeDec( eMaterialNormalModes materialNormalMode ){
	pMaterialNormalModeDec = materialNormalMode;
}

void deoglLightShaderConfig::SetMaterialNormalModeEnc( eMaterialNormalModes materialNormalMode ){
	pMaterialNormalModeEnc = materialNormalMode;
}

void deoglLightShaderConfig::SetParticleMode( eParticleModes mode ){
	pParticleMode = mode;
}



void deoglLightShaderConfig::SetDecodeInDepth( bool decodeInDepth ){
	pDecodeInDepth = decodeInDepth;
}

void deoglLightShaderConfig::SetDecodeInShadow( bool decodeInShadow ){
	pDecodeInShadow = decodeInShadow;
}

void deoglLightShaderConfig::SetEncodeOutDepth( bool encodeOutDepth ){
	pEncodeOutDepth = encodeOutDepth;
}

void deoglLightShaderConfig::SetHWDepthCompare( bool hwDepthCompare ){
	pHWDepthCompare = hwDepthCompare;
}

void deoglLightShaderConfig::SetShadowMatrix2EqualsMatrix1( bool shadowMatrix2EqualsMatrix1 ){
	pShadowMatrix2EqualsMatrix1 = shadowMatrix2EqualsMatrix1;
}

void deoglLightShaderConfig::SetAmbientLighting( bool ambientLighting ){
	pAmbientLighting = ambientLighting;
}

void deoglLightShaderConfig::SetShadowInverseDepth( bool shadowInverseDepth ){
	pShadowInverseDepth = shadowInverseDepth;
}

void deoglLightShaderConfig::SetFullScreenQuad( bool fullScreenQuad ){
	pFullScreenQuad = fullScreenQuad;
}

void deoglLightShaderConfig::SetSubSurface( bool subSurface ){
	pSubSurface = subSurface;
}

void deoglLightShaderConfig::SetLuminanceOnly( bool luminanceOnly ){
	pLuminanceOnly = luminanceOnly;
}

void deoglLightShaderConfig::SetGIRay( bool giRay ){
	pGIRay = giRay;
}

void deoglLightShaderConfig::SetGSRenderStereo( bool gsRenderStereo ){
	pGSRenderStereo = gsRenderStereo;
}

void deoglLightShaderConfig::SetVSRenderStereo( bool vsRenderStereo ){
	pVSRenderStereo = vsRenderStereo;
}



void deoglLightShaderConfig::SetTextureNoise( bool isUsed ){
	pTextureNoise = isUsed;
}

void deoglLightShaderConfig::SetTextureColor( bool useTexture ){
	pTextureColor = useTexture;
}

void deoglLightShaderConfig::SetTextureColorOmnidirCube( bool useTexture ){
	pTextureColorOmniCube = useTexture;
}

void deoglLightShaderConfig::SetTextureColorOmnidirEquirect( bool useTexture ){
	pTextureColorOmniEquirect = useTexture;
}

void deoglLightShaderConfig::SetTextureShadow1Solid( bool isUsed ){
	pTextureShadow1Solid = isUsed;
}

void deoglLightShaderConfig::SetTextureShadow1Transparent( bool isUsed ){
	pTextureShadow1Transparent = isUsed;
}

void deoglLightShaderConfig::SetTextureShadow2Solid( bool isUsed ){
	pTextureShadow2Solid = isUsed;
}

void deoglLightShaderConfig::SetTextureShadow2Transparent( bool isUsed ){
	pTextureShadow2Transparent = isUsed;
}

void deoglLightShaderConfig::SetTextureShadow1Ambient( bool isUsed ){
	pTextureShadow1Ambient = isUsed;
}

void deoglLightShaderConfig::SetTextureShadow2Ambient( bool isUsed ){
	pTextureShadow2Ambient = isUsed;
}



void deoglLightShaderConfig::UpdateKey(){
	pKey1 = ( uint32_t )0;
	pKey1 |= ( ( uint32_t )pLightMode ) << 0; // 5 values, 3 bits
	pKey1 |= ( ( uint32_t )pShadowTapMode ) << 3; // 4 values, 2 bits
	pKey1 |= ( ( uint32_t )pShadowMappingAlgorithm1 ) << 5; // 4 values, 2 bits
	pKey1 |= ( ( uint32_t )pShadowMappingAlgorithm2 ) << 7; // 4 values, 2 bits
	pKey1 |= ( ( uint32_t )pMaterialNormalModeDec ) << 9; // 4 values, 2 bits
	pKey1 |= ( ( uint32_t )pMaterialNormalModeEnc ) << 11; // 4 values, 2 bits
	pKey1 |= ( ( uint32_t )pParticleMode ) << 13; // 3 values, 2 bits
	
	pKey2 = ( uint32_t )0;
	if( pDecodeInDepth ) pKey2 |= ( uint32_t )1 << 0;
	if( pDecodeInShadow ) pKey2 |= ( uint32_t )1 << 1;
	if( pEncodeOutDepth ) pKey2 |= ( uint32_t )1 << 2;
	if( pHWDepthCompare ) pKey2 |= ( uint32_t )1 << 3;
	if( pShadowMatrix2EqualsMatrix1 ) pKey2 |= ( uint32_t )1 << 4;
	if( pShadowInverseDepth ) pKey2 |= ( uint32_t )1 << 5;
	if( pAmbientLighting ) pKey2 |= ( uint32_t )1 << 6;
	if( pFullScreenQuad ) pKey2 |= ( uint32_t )1 << 7;
	if( pSubSurface ) pKey2 |= ( uint32_t )1 << 8;
	if( pLuminanceOnly ) pKey2 |= ( uint32_t )1 << 9;
	if( pGIRay ) pKey2 |= ( uint32_t )1 << 10;
	if( pGSRenderStereo ) pKey2 |= ( uint32_t )1 << 11;
	if( pVSRenderStereo ) pKey2 |= ( uint32_t )1 << 12;
	
	pKey3 = ( uint32_t )0;
	if( pTextureNoise ) pKey3 |= ( uint32_t )1 << 0;
	if( pTextureColor ) pKey3 |= ( uint32_t )1 << 1;
	if( pTextureColorOmniCube ) pKey3 |= ( uint32_t )1 << 2;
	if( pTextureColorOmniEquirect ) pKey3 |= ( uint32_t )1 << 3;
	if( pTextureShadow1Solid ) pKey3 |= ( uint32_t )1 << 4;
	if( pTextureShadow1Transparent ) pKey3 |= ( uint32_t )1 << 5;
	if( pTextureShadow2Solid ) pKey3 |= ( uint32_t )1 << 6;
	if( pTextureShadow2Transparent ) pKey3 |= ( uint32_t )1 << 7;
	if( pTextureShadow1Ambient ) pKey3 |= ( uint32_t )1 << 8;
	if( pTextureShadow2Ambient ) pKey3 |= ( uint32_t )1 << 9;
}



// Debug
//////////

void deoglLightShaderConfig::DebugGetConfigString( decString &string ) const{
	const char * const lightModeStrings[] = { "point", "spot", "projector", "sky", "particle" };
	const char * const shadowTapModeStrings[] = { "single", "pcf4", "pcf9", "pcfvar" };
	const char * const shadowMappingAlgorithmStrings[] = { "2D", "cube", "dualpara", "pyramid" };
	const char * const materialNormalModeStrings[] = { "matnorFloat", "matnorIntBasic", "matnorSpheremap" };
	const char * const particleModeStrings[] = { "particle", "ribbon", "beam" };
	
	string.Format( "(%s %s %s/%s %s sma1=%s sma2=%s",
		lightModeStrings[ pLightMode ],
		shadowTapModeStrings[ pShadowTapMode ],
		materialNormalModeStrings[ pMaterialNormalModeDec ],
		materialNormalModeStrings[ pMaterialNormalModeEnc ],
		particleModeStrings[ pParticleMode ],
		shadowMappingAlgorithmStrings[ pShadowMappingAlgorithm1 ],
		shadowMappingAlgorithmStrings[ pShadowMappingAlgorithm2 ] );
	
	if( pTextureNoise ){
		string.Append( " noise" );
	}
	
	if( pTextureColor ){
		string.Append( " texColor" );
	}
	if( pTextureColorOmniCube ){
		string.Append( " texColorCube" );
	}
	if( pTextureColorOmniEquirect ){
		string.Append( " texColorEqui" );
	}
	
	if( pDecodeInDepth ){
		string.Append( " decodeInDepth" );
	}
	if( pDecodeInShadow ){
		string.Append( " decodeInShadow" );
	}
	if( pEncodeOutDepth ){
		string.Append( " encodeOutDepth" );
	}
	if( pEncodeOutDepth ){
		string.Append( " hwDepthCompare" );
	}
	if( pShadowMatrix2EqualsMatrix1 ){
		string.Append( " sm2eqm1" );
	}
	if( pAmbientLighting ){
		string.Append( " ambientLighting" );
	}
	if( pShadowInverseDepth ){
		string.Append( " shadowInverseDepth" );
	}
	if( pFullScreenQuad ){
		string.Append( " fullScreenQuad" );
	}
	if( pSubSurface ){
		string.Append( " subSurface" );
	}
	if( pLuminanceOnly ){
		string.Append( " luminanceOnly" );
	}
	if( pGIRay ){
		string.Append( " giRay" );
	}
	if( pGSRenderStereo ){
		string.Append( " gsRenderStereo" );
	}
	if( pVSRenderStereo ){
		string.Append( " vsRenderStereo" );
	}
	
	if( pTextureShadow1Solid ){
		string.Append( " shadow1Solid" );
	}
	if( pTextureShadow1Transparent ){
		string.Append( " shadow1Transp" );
	}
	if( pTextureShadow2Solid ){
		string.Append( " shadow2Solid" );
	}
	if( pTextureShadow2Transparent ){
		string.Append( " shadow2Transp" );
	}
	if( pTextureShadow1Ambient ){
		string.Append( " shadow1Ambient" );
	}
	if( pTextureShadow2Ambient ){
		string.Append( " shadow2Ambient" );
	}
	
	string.Append( ")" );
}



// Operators
//////////////

deoglLightShaderConfig &deoglLightShaderConfig::operator=( const deoglLightShaderConfig &config ){
	pLightMode = config.pLightMode;
	pShadowTapMode = config.pShadowTapMode;
	pShadowMappingAlgorithm1 = config.pShadowMappingAlgorithm1;
	pShadowMappingAlgorithm2 = config.pShadowMappingAlgorithm2;
	pMaterialNormalModeDec = config.pMaterialNormalModeDec;
	pMaterialNormalModeEnc = config.pMaterialNormalModeEnc;
	pParticleMode = config.pParticleMode;
	
	pDecodeInDepth = config.pDecodeInDepth;
	pDecodeInShadow = config.pDecodeInShadow;
	pEncodeOutDepth = config.pEncodeOutDepth;
	pHWDepthCompare = config.pHWDepthCompare;
	pShadowMatrix2EqualsMatrix1 = config.pShadowMatrix2EqualsMatrix1;
	pAmbientLighting = config.pAmbientLighting;
	pShadowInverseDepth = config.pShadowInverseDepth;
	pFullScreenQuad = config.pFullScreenQuad;
	pSubSurface = config.pSubSurface;
	pLuminanceOnly = config.pLuminanceOnly;
	pGIRay = config.pGIRay;
	pGSRenderStereo = config.pGSRenderStereo;
	pVSRenderStereo = config.pVSRenderStereo;
	
	pTextureNoise = config.pTextureNoise;
	pTextureColor = config.pTextureColor;
	pTextureColorOmniCube = config.pTextureColorOmniCube;
	pTextureColorOmniEquirect = config.pTextureColorOmniEquirect;
	pTextureShadow1Solid = config.pTextureShadow1Solid;
	pTextureShadow1Transparent = config.pTextureShadow1Transparent;
	pTextureShadow2Solid = config.pTextureShadow2Solid;
	pTextureShadow2Transparent = config.pTextureShadow2Transparent;
	pTextureShadow1Ambient = config.pTextureShadow1Ambient;
	pTextureShadow2Ambient = config.pTextureShadow2Ambient;
	
	pKey1 = config.pKey1;
	pKey2 = config.pKey2;
	pKey3 = config.pKey3;
	
	return *this;
}

bool deoglLightShaderConfig::operator==( const deoglLightShaderConfig &config ) const{
	return pKey1 == config.pKey1
		&& pKey2 == config.pKey2
		&& pKey3 == config.pKey3;
}
