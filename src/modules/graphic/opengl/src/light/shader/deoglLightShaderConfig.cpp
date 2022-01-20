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
	
	return *this;
}

bool deoglLightShaderConfig::operator==( const deoglLightShaderConfig &config ) const{
	return pLightMode == config.pLightMode
		&& pShadowTapMode == config.pShadowTapMode
		&& pShadowMappingAlgorithm1 == config.pShadowMappingAlgorithm1
		&& pShadowMappingAlgorithm2 == config.pShadowMappingAlgorithm2
		&& pMaterialNormalModeDec == config.pMaterialNormalModeDec
		&& pMaterialNormalModeEnc == config.pMaterialNormalModeEnc
		&& pParticleMode == config.pParticleMode
		
		&& pDecodeInDepth == config.pDecodeInDepth
		&& pDecodeInShadow == config.pDecodeInShadow
		&& pEncodeOutDepth == config.pEncodeOutDepth
		&& pHWDepthCompare == config.pHWDepthCompare
		&& pShadowMatrix2EqualsMatrix1 == config.pShadowMatrix2EqualsMatrix1
		&& pAmbientLighting == config.pAmbientLighting
		&& pShadowInverseDepth == config.pShadowInverseDepth
		&& pFullScreenQuad == config.pFullScreenQuad
		&& pSubSurface == config.pSubSurface
		&& pLuminanceOnly == config.pLuminanceOnly
		&& pGIRay == config.pGIRay
		
		&& pTextureNoise == config.pTextureNoise
		&& pTextureColor == config.pTextureColor
		&& pTextureColorOmniCube == config.pTextureColorOmniCube
		&& pTextureColorOmniEquirect == config.pTextureColorOmniEquirect
		&& pTextureShadow1Solid == config.pTextureShadow1Solid
		&& pTextureShadow1Transparent == config.pTextureShadow1Transparent
		&& pTextureShadow2Solid == config.pTextureShadow2Solid
		&& pTextureShadow2Transparent == config.pTextureShadow2Transparent
		&& pTextureShadow1Ambient == config.pTextureShadow1Ambient
		&& pTextureShadow2Ambient == config.pTextureShadow2Ambient;
}
