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

#include "deoglSkinShaderConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkinShaderConfig
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinShaderConfig::deoglSkinShaderConfig(){
	Reset();
}

deoglSkinShaderConfig::~deoglSkinShaderConfig(){
}



// Management
///////////////

void deoglSkinShaderConfig::Reset(){
	pShaderMode = esmGeometry;
	pGeometryMode = egmComponent;
	pDepthMode = edmProjection;
	pMaterialNormalModeDec = emnmIntBasic;
	pMaterialNormalModeEnc = emnmFloat;
	pParticleMode = epmParticle;
	pTessellationMode = etmNone;
	pDepthTestMode = edtmNone;
	
	pDecodeInDepth = false;
	pEncodeOutDepth = false;
	pInverseDepth = false;
	pMaskedSolidity = false;
	pClipPlane = false;
	pNoZClip = false;
	pOutputConstant = false;
	pOutputColor = false;
	pAmbientLightProbe = false;
	pBillboard = false;
	pSkinReflections = false;
	pFadeOutRange = false;
	pVariations = false;
	pUseNormalRoughnessCorrection = true;
	pGSRenderCube = false;
	pGSRenderCascaded = false;
	pGSRenderStereo = false;
	pVSRenderStereo = false;
	pSharedSPB = false;
	pOutline = false;
	pOutlineThicknessScreen = false;
	pLuminanceOnly = false;
	pGIMaterial = false;
	pXRay = false;
	
	pDynamicColorTint = false;
	pDynamicColorGamma = false;
	pDynamicColorSolidityMultiplier = false;
	pDynamicAmbientOcclusionSolidityMultiplier = false;
	pDynamicTransparencyMultiplier = false;
	pDynamicSolidityMultiplier = false;
	pDynamicHeightRemap = false;
	pDynamicNormalStrength = false;
	pDynamicNormalSolidityMultiplier = false;
	pDynamicRoughnessRemap = false;
	pDynamicRoughnessGamma = false;
	pDynamicRoughnessSolidityMultiplier = false;
	pDynamicReflectivitySolidityMultiplier = false;
	pDynamicReflectivityMultiplier = false;
	pDynamicRefractionDistortStrength = false;
	pDynamicEmissivityTint = false;
	pDynamicEmissivityIntensity = false;
	pDynamicEnvRoomSize = false;
	pDynamicEnvRoomOffset = false;
	pDynamicEnvRoomEmissivityTint = false;
	pDynamicEnvRoomEmissivityIntensity = false;
	pDynamicThickness = false;
	pDynamicAbsorption = false;
	pDynamicVariation = false;
	pDynamicOutlineColor = false;
	pDynamicOutlineColorTint = false;
	pDynamicOutlineThickness = false;
	pDynamicOutlineSolidity = false;
	pDynamicOutlineEmissivity = false;
	pDynamicOutlineEmissivityTint = false;
	pDynamicRimEmissivityTint = false;
	pDynamicRimEmissivityIntensity = false;
	pDynamicRimAngle = false;
	pDynamicRimExponent = false;
	
	pTextureColor = false;
	pTextureColorTintMask = false;
	pTextureTransparency = false;
	pTextureSolidity = false;
	pTextureNormal = false;
	pTextureHeight = false;
	pTextureReflectivity = false;
	pTextureRoughness = false;
	pTextureEmissivity = false;
	pTextureAO = false;
	pTextureEnvMap = false;
	pTextureEnvMapEqui = false;
	pTextureAbsorption = false;
	pTextureRenderColor = false;
	pTextureRefractionDistort = false;
	pTextureEnvRoom = false;
	pTextureEnvRoomMask = false;
	pTextureEnvRoomEmissivity = false;
	pTextureRimEmissivity = false;
	pTextureNonPbrAlbedo = false;
	pTextureNonPbrMetalness = false;
}



void deoglSkinShaderConfig::SetShaderMode( eShaderModes mode ){
	pShaderMode = mode;
}

void deoglSkinShaderConfig::SetGeometryMode( eGeometryModes mode ){
	pGeometryMode = mode;
}

void deoglSkinShaderConfig::SetDepthMode( eDepthModes mode ){
	pDepthMode = mode;
}

void deoglSkinShaderConfig::SetMaterialNormalModeDec( eMaterialNormalModes mode ){
	pMaterialNormalModeDec = mode;
}

void deoglSkinShaderConfig::SetMaterialNormalModeEnc( eMaterialNormalModes mode ){
	pMaterialNormalModeEnc = mode;
}

void deoglSkinShaderConfig::SetParticleMode( eParticleModes mode ){
	pParticleMode = mode;
}

void deoglSkinShaderConfig::SetTessellationMode( eTessallationModes mode ){
	pTessellationMode = mode;
}

void deoglSkinShaderConfig::SetDepthTestMode( eDepthTestModes mode ){
	pDepthTestMode = mode;
}



void deoglSkinShaderConfig::SetDecodeInDepth( bool decodeInDepth ){
	pDecodeInDepth = decodeInDepth;
}

void deoglSkinShaderConfig::SetEncodeOutDepth( bool encodeOutDepth ){
	pEncodeOutDepth = encodeOutDepth;
}

void deoglSkinShaderConfig::SetInverseDepth( bool inverseDepth ){
	pInverseDepth = inverseDepth;
}

void deoglSkinShaderConfig::SetMaskedSolidity( bool maskedSolidity ){
	pMaskedSolidity = maskedSolidity;
}

void deoglSkinShaderConfig::SetClipPlane( bool clipPlane ){
	pClipPlane = clipPlane;
}

void deoglSkinShaderConfig::SetNoZClip( bool noZClip ){
	pNoZClip = noZClip;
}

void deoglSkinShaderConfig::SetOutputConstant( bool outputConstant ){
	pOutputConstant = outputConstant;
}

void deoglSkinShaderConfig::SetOutputColor( bool outputColor ){
	pOutputColor = outputColor;
}

void deoglSkinShaderConfig::SetAmbientLightProbe( bool ambientLightProbe ){
	pAmbientLightProbe = ambientLightProbe;
}

void deoglSkinShaderConfig::SetBillboard( bool billboard ){
	pBillboard = billboard;
}

void deoglSkinShaderConfig::SetSkinReflections( bool skinReflections ){
	pSkinReflections = skinReflections;
}

void deoglSkinShaderConfig::SetFadeOutRange( bool fadeOutRange ){
	pFadeOutRange = fadeOutRange;
}

void deoglSkinShaderConfig::SetVariations( bool variations ){
	pVariations = variations;
}

void deoglSkinShaderConfig::SetUseNormalRoughnessCorrection( bool useNormalRoughnessCorrection ){
	pUseNormalRoughnessCorrection = useNormalRoughnessCorrection;
}

void deoglSkinShaderConfig::SetGSRenderCube( bool gsRenderCube ){
	pGSRenderCube = gsRenderCube;
}

void deoglSkinShaderConfig::SetGSRenderCascaded( bool gsRenderCascaded ){
	pGSRenderCascaded = gsRenderCascaded;
}

void deoglSkinShaderConfig::SetGSRenderStereo( bool gsRenderStereo ){
	pGSRenderStereo = gsRenderStereo;
}

void deoglSkinShaderConfig::SetVSRenderStereo( bool vsRenderStereo ){
	pVSRenderStereo = vsRenderStereo;
}

void deoglSkinShaderConfig::SetSharedSPB( bool sharedSPB ){
	pSharedSPB = sharedSPB;
}

void deoglSkinShaderConfig::SetOutline( bool outline ){
	pOutline = outline;
}

void deoglSkinShaderConfig::SetOutlineThicknessScreen( bool enable ){
	pOutlineThicknessScreen = enable;
}

void deoglSkinShaderConfig::SetLuminanceOnly( bool luminanceOnly ){
	pLuminanceOnly = luminanceOnly;
}

void deoglSkinShaderConfig::SetGIMaterial( bool gimaterial ){
	pGIMaterial = gimaterial;
}

void deoglSkinShaderConfig::SetXRay( bool xray ){
	pXRay = xray;
}



void deoglSkinShaderConfig::SetDynamicColorTint( bool dynamic ){
	pDynamicColorTint = dynamic;
}

void deoglSkinShaderConfig::SetDynamicColorGamma( bool dynamic ){
	pDynamicColorGamma = dynamic;
}

void deoglSkinShaderConfig::SetDynamicColorSolidityMultiplier( bool dynamic ){
	pDynamicColorSolidityMultiplier = dynamic;
}

void deoglSkinShaderConfig::SetDynamicAmbientOcclusionSolidityMultiplier( bool dynamic ){
	pDynamicAmbientOcclusionSolidityMultiplier = dynamic;
}

void deoglSkinShaderConfig::SetDynamicTransparencyMultiplier( bool dynamic ){
	pDynamicTransparencyMultiplier = dynamic;
}

void deoglSkinShaderConfig::SetDynamicSolidityMultiplier( bool dynamic ){
	pDynamicSolidityMultiplier = dynamic;
}

void deoglSkinShaderConfig::SetDynamicHeightRemap( bool dynamic ){
	pDynamicHeightRemap = dynamic;
}

void deoglSkinShaderConfig::SetDynamicNormalStrength( bool dynamic ){
	pDynamicNormalStrength = dynamic;
}

void deoglSkinShaderConfig::SetDynamicNormalSolidityMultiplier( bool dynamic ){
	pDynamicNormalSolidityMultiplier = dynamic;
}

void deoglSkinShaderConfig::SetDynamicRoughnessRemap( bool dynamic ){
	pDynamicRoughnessRemap = dynamic;
}

void deoglSkinShaderConfig::SetDynamicRoughnessGamma( bool dynamic ){
	pDynamicRoughnessGamma = dynamic;
}

void deoglSkinShaderConfig::SetDynamicRoughnessSolidityMultiplier( bool dynamic ){
	pDynamicRoughnessSolidityMultiplier = dynamic;
}

void deoglSkinShaderConfig::SetDynamicReflectivitySolidityMultiplier( bool dynamic ){
	pDynamicReflectivitySolidityMultiplier = dynamic;
}

void deoglSkinShaderConfig::SetDynamicReflectivityMultiplier( bool dynamic ){
	pDynamicReflectivityMultiplier = dynamic;
}

void deoglSkinShaderConfig::SetDynamicRefractionDistortStrength( bool dynamic ){
	pDynamicRefractionDistortStrength = dynamic;
}

void deoglSkinShaderConfig::SetDynamicEmissivityTint( bool dynamic ){
	pDynamicEmissivityTint = dynamic;
}

void deoglSkinShaderConfig::SetDynamicEmissivityIntensity( bool dynamic ){
	pDynamicEmissivityIntensity = dynamic;
}

void deoglSkinShaderConfig::SetDynamicEnvRoomSize( bool dynamic ){
	pDynamicEnvRoomSize = dynamic;
}

void deoglSkinShaderConfig::SetDynamicEnvRoomOffset( bool dynamic ){
	pDynamicEnvRoomOffset = dynamic;
}

void deoglSkinShaderConfig::SetDynamicEnvRoomEmissivityTint( bool dynamic ){
	pDynamicEnvRoomEmissivityTint = dynamic;
}

void deoglSkinShaderConfig::SetDynamicEnvRoomEmissivityIntensity( bool dynamic ){
	pDynamicEnvRoomEmissivityIntensity = dynamic;
}

void deoglSkinShaderConfig::SetDynamicThickness( bool dynamic ){
	pDynamicThickness = dynamic;
}

void deoglSkinShaderConfig::SetDynamicAbsorption( bool dynamic ){
	pDynamicAbsorption = dynamic;
}

void deoglSkinShaderConfig::SetDynamicVariation( bool dynamic ){
	pDynamicVariation = dynamic;
}

void deoglSkinShaderConfig::SetDynamicOutlineColor( bool dynamic ){
	pDynamicOutlineColor = dynamic;
}

void deoglSkinShaderConfig::SetDynamicOutlineColorTint( bool dynamic ){
	pDynamicOutlineColorTint = dynamic;
}

void deoglSkinShaderConfig::SetDynamicOutlineThickness( bool dynamic ){
	pDynamicOutlineThickness = dynamic;
}

void deoglSkinShaderConfig::SetDynamicOutlineSolidity( bool dynamic ){
	pDynamicOutlineSolidity = dynamic;
}

void deoglSkinShaderConfig::SetDynamicOutlineEmissivity( bool dynamic ){
	pDynamicOutlineEmissivity = dynamic;
}

void deoglSkinShaderConfig::SetDynamicOutlineEmissivityTint( bool dynamic ){
	pDynamicOutlineEmissivityTint = dynamic;
}

void deoglSkinShaderConfig::SetDynamicRimEmissivityTint( bool dynamic ){
	pDynamicRimEmissivityTint = dynamic;
}

void deoglSkinShaderConfig::SetDynamicRimEmissivityIntensity( bool dynamic ){
	pDynamicRimEmissivityIntensity = dynamic;
}

void deoglSkinShaderConfig::SetDynamicRimAngle( bool dynamic ){
	pDynamicRimAngle = dynamic;
}

void deoglSkinShaderConfig::SetDynamicRimExponent( bool dynamic ){
	pDynamicRimExponent = dynamic;
}



void deoglSkinShaderConfig::SetTextureColor( bool hasTexture ){
	pTextureColor = hasTexture;
}

void deoglSkinShaderConfig::SetTextureColorTintMask( bool hasTexture ){
	pTextureColorTintMask = hasTexture;
}

void deoglSkinShaderConfig::SetTextureTransparency( bool textureTransparency ){
	pTextureTransparency = textureTransparency;
}

void deoglSkinShaderConfig::SetTextureSolidity( bool textureSolidity ){
	pTextureSolidity = textureSolidity;
}

void deoglSkinShaderConfig::SetTextureNormal( bool textureNormal ){
	pTextureNormal = textureNormal;
}

void deoglSkinShaderConfig::SetTextureHeight( bool textureHeightCone ){
	pTextureHeight = textureHeightCone;
}

void deoglSkinShaderConfig::SetTextureReflectivity( bool textureReflectivity ){
	pTextureReflectivity = textureReflectivity;
}

void deoglSkinShaderConfig::SetTextureRoughness( bool textureRoughness ){
	pTextureRoughness = textureRoughness;
}

void deoglSkinShaderConfig::SetTextureEmissivity( bool textureEmissivity ){
	pTextureEmissivity = textureEmissivity;
}

void deoglSkinShaderConfig::SetTextureAO( bool textureAO ){
	pTextureAO = textureAO;
}

void deoglSkinShaderConfig::SetTextureEnvMap( bool textureEnvMap ){
	pTextureEnvMap = textureEnvMap;
}

void deoglSkinShaderConfig::SetTextureEnvMapEqui( bool textureEnvMapEqui ){
	pTextureEnvMapEqui = textureEnvMapEqui;
}

void deoglSkinShaderConfig::SetTextureAbsorption( bool textureAbsorption ){
	pTextureAbsorption = textureAbsorption;
}

void deoglSkinShaderConfig::SetTextureRenderColor( bool textureRenderColor ){
	pTextureRenderColor = textureRenderColor;
}

void deoglSkinShaderConfig::SetTextureRefractionDistort( bool textureRefractionDistort ){
	pTextureRefractionDistort = textureRefractionDistort;
}

void deoglSkinShaderConfig::SetTextureEnvRoom( bool textureEnvRoom ){
	pTextureEnvRoom = textureEnvRoom;
}

void deoglSkinShaderConfig::SetTextureEnvRoomMask( bool textureEnvRoomMask ){
	pTextureEnvRoomMask = textureEnvRoomMask;
}

void deoglSkinShaderConfig::SetTextureEnvRoomEmissivity( bool textureEnvRoomEmissivity ){
	pTextureEnvRoomEmissivity = textureEnvRoomEmissivity;
}

void deoglSkinShaderConfig::SetTextureRimEmissivity( bool useTexture ){
	pTextureRimEmissivity = useTexture;
}

void deoglSkinShaderConfig::SetTextureNonPbrAlbedo( bool texture ){
	pTextureNonPbrAlbedo = texture;
}

void deoglSkinShaderConfig::SetTextureNonPbrMetalness( bool texture ){
	pTextureNonPbrMetalness = texture;
}



// Debug
//////////

void deoglSkinShaderConfig::DebugGetConfigString( decString &string ) const{
	string = "(";
	
	switch( pGeometryMode ){
		case egmComponent: string = "component"; break;
		case egmBillboard: string = "billboard"; break;
		case egmDecal: string = "decal"; break;
		case egmPropField: string = "propField"; break;
		case egmParticle: string = "particle"; break;
		case egmHeightMap: string = "heightMap"; break;
		default: string = "?";
	}
	
	switch( pShaderMode ){
		case esmGeometry: string.Append( " geometry" ); break;
		case esmDepth: string.Append( " depth" ); break;
		case esmEnvMap: string.Append( " envMap" ); break;
		default: string.Append( " ?" );
	}
	
	switch( pDepthMode ){
		case edmProjection: string.Append( " projection" ); break;
		case edmOrthogonal: string.Append( " orthogonal" ); break;
		case edmDistance: string.Append( " distance" ); break;
		default: string.Append( " ?" );
	}
	
	switch( pMaterialNormalModeDec ){
		case emnmFloat: string.Append( " matnorFloat" ); break;
		case emnmIntBasic: string.Append( " matnorIntBasic" ); break;
		case emnmSpheremap: string.Append( " matnorSpheremap" ); break;
		default: string.Append( " ?" );
	}
	
	switch( pMaterialNormalModeEnc ){
		case emnmFloat: string.Append( "/matnorFloat" ); break;
		case emnmIntBasic: string.Append( "/matnorIntBasic" ); break;
		case emnmSpheremap: string.Append( "/matnorSpheremap" ); break;
		default: string.Append( " ?" );
	}
	
	switch( pParticleMode ){
		case epmParticle: string.Append( " particle" ); break;
		case epmRibbon: string.Append( " ribbon" ); break;
		case epmBeam: string.Append( " beam" ); break;
		default: string.Append( " ?" );
	}
	
	switch( pTessellationMode ){
		case etmNone: string.Append( " tessNone" ); break;
		case etmLinear: string.Append( " tessLinear" ); break;
		default: string.Append( " ?" );
	}
	
	switch( pDepthTestMode ){
		case edtmNone: string.Append( " depthNone" ); break;
		case edtmSmaller: string.Append( " depthSmaller" ); break;
		case edtmLarger: string.Append( " depthLarger" ); break;
		default: string.Append( " ?" );
	}
	
	if( pDecodeInDepth ){
		string.Append( " decodeInDepth" );
	}
	if( pEncodeOutDepth ){
		string.Append( " encodeOutDepth" );
	}
	if( pInverseDepth ){
		string.Append( " inverseDepth" );
	}
	if( pMaskedSolidity ){
		string.Append( " maskSol" );
	}
	if( pClipPlane ){
		string.Append( " clipPlane" );
	}
	if( pNoZClip ){
		string.Append( " noZClip" );
	}
	if( pOutputConstant ){
		string.Append( " outputConstant" );
	}
	if( pOutputColor ){
		string.Append( " outputColor" );
	}
	if( pAmbientLightProbe ){
		string.Append( " ambientLightProbe" );
	}
	if( pBillboard ){
		string.Append( " billboard" );
	}
	if( pSkinReflections ){
		string.Append( " skinReflections" );
	}
	if( pFadeOutRange ){
		string.Append( " fadeOutRange" );
	}
	if( pVariations ){
		string.Append( " variations" );
	}
	if( pUseNormalRoughnessCorrection ){
		string.Append( " norRoughCorr" );
	}
	if( pGSRenderCube ){
		string.Append( " gsRenderCube" );
	}
	if( pGSRenderCascaded ){
		string.Append( " gsRenderCascaded" );
	}
	if( pGSRenderStereo ){
		string.Append( " gsRenderStereo" );
	}
	if( pVSRenderStereo ){
		string.Append( " vsRenderStereo" );
	}
	if( pSharedSPB ){
		string.Append( " sharedSPB" );
	}
	if( pOutline ){
		string.Append( " outline" );
	}
	if( pOutlineThicknessScreen ){
		string.Append( " outlineTS" );
	}
	if( pLuminanceOnly ){
		string.Append( " luminanceOnly" );
	}
	if( pGIMaterial ){
		string.Append( " giMaterial" );
	}
	if( pXRay ){
		string.Append( " xray" );
	}
	
	if( pDynamicColorTint ){
		string.Append( " dynClrTint" );
	}
	if( pDynamicColorGamma ){
		string.Append( " dynClrGamma" );
	}
	if( pDynamicColorSolidityMultiplier ){
		string.Append( " dynClrTraMul" );
	}
	if( pDynamicAmbientOcclusionSolidityMultiplier ){
		string.Append( " dynAOTraMul" );
	}
	if( pDynamicTransparencyMultiplier ){
		string.Append( " dynTraMul" );
	}
	if( pDynamicSolidityMultiplier ){
		string.Append( " dynSolMul" );
	}
	if( pDynamicHeightRemap ){
		string.Append( " dynHeiRemap" );
	}
	if( pDynamicNormalStrength ){
		string.Append( " dynNorStr" );
	}
	if( pDynamicNormalSolidityMultiplier ){
		string.Append( " dynNorTraMul" );
	}
	if( pDynamicRoughnessRemap ){
		string.Append( " dynRouRemap" );
	}
	if( pDynamicRoughnessGamma ){
		string.Append( " dynRouGamma" );
	}
	if( pDynamicRoughnessSolidityMultiplier ){
		string.Append( " dynRouTraMul" );
	}
	if( pDynamicReflectivitySolidityMultiplier ){
		string.Append( " dynReflSolMul" );
	}
	if( pDynamicReflectivityMultiplier ){
		string.Append( " dynReflMul" );
	}
	if( pDynamicRefractionDistortStrength ){
		string.Append( " dynRefrDisStr" );
	}
	if( pDynamicEmissivityTint ){
		string.Append( " dynEmiTint" );
	}
	if( pDynamicEmissivityIntensity ){
		string.Append( " dynEmiInt" );
	}
	if( pDynamicEnvRoomSize ){
		string.Append( " dynEnvRoomSize" );
	}
	if( pDynamicEnvRoomOffset ){
		string.Append( " dynEnvRoomOffset" );
	}
	if( pDynamicEnvRoomEmissivityTint ){
		string.Append( " dynEnvRoomEmiTint" );
	}
	if( pDynamicEnvRoomEmissivityIntensity ){
		string.Append( " dynEnvRoomEmiInt" );
	}
	if( pDynamicThickness ){
		string.Append( " dynThick" );
	}
	if( pDynamicAbsorption ){
		string.Append( " dynAbsorb" );
	}
	if( pDynamicVariation ){
		string.Append( " dynVar" );
	}
	if( pDynamicOutlineColor ){
		string.Append( " dynOutColor" );
	}
	if( pDynamicOutlineColorTint ){
		string.Append( " dynOutColorTint" );
	}
	if( pDynamicOutlineThickness ){
		string.Append( " dynOutThick" );
	}
	if( pDynamicOutlineSolidity ){
		string.Append( " dynOutSol" );
	}
	if( pDynamicOutlineEmissivity ){
		string.Append( " dynOutEmis" );
	}
	if( pDynamicOutlineEmissivityTint ){
		string.Append( " dynOutEmisTint" );
	}
	if( pDynamicRimEmissivityTint ){
		string.Append( " dynRimEmisTint" );
	}
	if( pDynamicRimEmissivityIntensity ){
		string.Append( " dynRimEmisInt" );
	}
	if( pDynamicRimAngle ){
		string.Append( " dynRimAng" );
	}
	if( pDynamicRimExponent ){
		string.Append( " dynRimExp" );
	}
	
	if( pTextureColor ){
		string.Append( " texCol" );
	}
	if( pTextureColorTintMask ){
		string.Append( " texColTintMask" );
	}
	if( pTextureTransparency ){
		string.Append( " texTran" );
	}
	if( pTextureSolidity ){
		string.Append( " texSol" );
	}
	if( pTextureNormal ){
		string.Append( " texNor" );
	}
	if( pTextureHeight ){
		string.Append( " texHei" );
	}
	if( pTextureReflectivity ){
		string.Append( " texRefl" );
	}
	if( pTextureRoughness ){
		string.Append( " texRoug" );
	}
	if( pTextureEmissivity ){
		string.Append( " texEmis" );
	}
	if( pTextureAO ){
		string.Append( " texAO" );
	}
	if( pTextureEnvMap ){
		string.Append( " texEnvMap" );
		if( pTextureEnvMapEqui ){
			string.Append( "Equi" );
		}
	}
	if( pTextureAbsorption ){
		string.Append( " texAbsorb" );
	}
	if( pTextureRenderColor ){
		string.Append( " texRenCol" );
	}
	if( pTextureRefractionDistort ){
		string.Append( " texRefrDist" );
	}
	if( pTextureEnvRoom ){
		string.Append( " texEnvRoom" );
	}
	if( pTextureEnvRoomMask ){
		string.Append( " texEnvRoomMask" );
	}
	if( pTextureEnvRoomEmissivity ){
		string.Append( " texEnvRoomEmi" );
	}
	if( pTextureRimEmissivity ){
		string.Append( " texRimEmis" );
	}
	
	if( pTextureNonPbrAlbedo ){
		string.Append( " texNPAlb" );
	}
	if( pTextureNonPbrMetalness ){
		string.Append( " texNPMet" );
	}
	
	string.Append( ")" );
}



// Operators
//////////////

deoglSkinShaderConfig &deoglSkinShaderConfig::operator=( const deoglSkinShaderConfig &config ){
	pShaderMode = config.pShaderMode;
	pGeometryMode = config.pGeometryMode;
	pDepthMode = config.pDepthMode;
	pMaterialNormalModeDec = config.pMaterialNormalModeDec;
	pMaterialNormalModeEnc = config.pMaterialNormalModeEnc;
	pParticleMode = config.pParticleMode;
	pTessellationMode = config.pTessellationMode;
	pDepthTestMode = config.pDepthTestMode;
	
	pDecodeInDepth = config.pDecodeInDepth;
	pEncodeOutDepth = config.pEncodeOutDepth;
	pInverseDepth = config.pInverseDepth;
	pMaskedSolidity = config.pMaskedSolidity;
	pClipPlane = config.pClipPlane;
	pNoZClip = config.pNoZClip;
	pOutputConstant = config.pOutputConstant;
	pOutputColor = config.pOutputColor;
	pAmbientLightProbe = config.pAmbientLightProbe;
	pBillboard = config.pBillboard;
	pSkinReflections = config.pSkinReflections;
	pFadeOutRange = config.pFadeOutRange;
	pVariations = config.pVariations;
	pUseNormalRoughnessCorrection = config.pUseNormalRoughnessCorrection;
	pGSRenderCube = config.pGSRenderCube;
	pGSRenderCascaded = config.pGSRenderCascaded;
	pGSRenderStereo = config.pGSRenderStereo;
	pVSRenderStereo = config.pVSRenderStereo;
	pSharedSPB = config.pSharedSPB;
	pOutline = config.pOutline;
	pOutlineThicknessScreen = config.pOutlineThicknessScreen;
	pLuminanceOnly = config.pLuminanceOnly;
	pGIMaterial = config.pGIMaterial;
	pXRay = config.pXRay;
	
	pDynamicColorTint = config.pDynamicColorTint;
	pDynamicColorGamma = config.pDynamicColorGamma;
	pDynamicColorSolidityMultiplier = config.pDynamicColorSolidityMultiplier;
	pDynamicAmbientOcclusionSolidityMultiplier = config.pDynamicAmbientOcclusionSolidityMultiplier;
	pDynamicTransparencyMultiplier = config.pDynamicTransparencyMultiplier;
	pDynamicSolidityMultiplier = config.pDynamicSolidityMultiplier;
	pDynamicHeightRemap = config.pDynamicHeightRemap;
	pDynamicNormalStrength = config.pDynamicNormalStrength;
	pDynamicNormalSolidityMultiplier = config.pDynamicNormalSolidityMultiplier;
	pDynamicRoughnessRemap = config.pDynamicRoughnessRemap;
	pDynamicRoughnessGamma = config.pDynamicRoughnessGamma;
	pDynamicRoughnessSolidityMultiplier = config.pDynamicRoughnessSolidityMultiplier;
	pDynamicReflectivitySolidityMultiplier = config.pDynamicReflectivitySolidityMultiplier;
	pDynamicReflectivityMultiplier = config.pDynamicReflectivityMultiplier;
	pDynamicRefractionDistortStrength = config.pDynamicRefractionDistortStrength;
	pDynamicEmissivityTint = config.pDynamicEmissivityTint;
	pDynamicEmissivityIntensity = config.pDynamicEmissivityIntensity;
	pDynamicEnvRoomSize = config.pDynamicEnvRoomSize;
	pDynamicEnvRoomOffset = config.pDynamicEnvRoomOffset;
	pDynamicEnvRoomEmissivityTint = config.pDynamicEnvRoomEmissivityTint;
	pDynamicEnvRoomEmissivityIntensity = config.pDynamicEnvRoomEmissivityIntensity;
	pDynamicThickness = config.pDynamicThickness;
	pDynamicAbsorption = config.pDynamicAbsorption;
	pDynamicVariation = config.pDynamicVariation;
	pDynamicOutlineColor = config.pDynamicOutlineColor;
	pDynamicOutlineColorTint = config.pDynamicOutlineColorTint;
	pDynamicOutlineThickness = config.pDynamicOutlineThickness;
	pDynamicOutlineSolidity = config.pDynamicOutlineSolidity;
	pDynamicOutlineEmissivity = config.pDynamicOutlineEmissivity;
	pDynamicOutlineEmissivityTint = config.pDynamicOutlineEmissivityTint;
	pDynamicRimEmissivityTint = config.pDynamicRimEmissivityTint;
	pDynamicRimEmissivityIntensity = config.pDynamicRimEmissivityIntensity;
	pDynamicRimAngle = config.pDynamicRimAngle;
	pDynamicRimExponent = config.pDynamicRimExponent;
	
	pTextureColor = config.pTextureColor;
	pTextureColorTintMask = config.pTextureColorTintMask;
	pTextureTransparency = config.pTextureTransparency;
	pTextureSolidity = config.pTextureSolidity;
	pTextureNormal = config.pTextureNormal;
	pTextureHeight = config.pTextureHeight;
	pTextureReflectivity = config.pTextureReflectivity;
	pTextureRoughness = config.pTextureRoughness;
	pTextureEmissivity = config.pTextureEmissivity;
	pTextureAO = config.pTextureAO;
	pTextureEnvMap = config.pTextureEnvMap;
	pTextureEnvMapEqui = config.pTextureEnvMapEqui;
	pTextureAbsorption = config.pTextureAbsorption;
	pTextureRenderColor = config.pTextureRenderColor;
	pTextureRefractionDistort = config.pTextureRefractionDistort;
	pTextureEnvRoom = config.pTextureEnvRoom;
	pTextureEnvRoomMask = config.pTextureEnvRoomMask;
	pTextureEnvRoomEmissivity = config.pTextureEnvRoomEmissivity;
	pTextureRimEmissivity = config.pTextureRimEmissivity;
	pTextureNonPbrAlbedo = config.pTextureNonPbrAlbedo;
	pTextureNonPbrMetalness = config.pTextureNonPbrMetalness;
	
	return *this;
}

bool deoglSkinShaderConfig::operator==( const deoglSkinShaderConfig &config ) const{
	return pShaderMode == config.pShaderMode
		&& pGeometryMode == config.pGeometryMode
		&& pDepthMode == config.pDepthMode
		&& pMaterialNormalModeDec == config.pMaterialNormalModeDec
		&& pMaterialNormalModeEnc == config.pMaterialNormalModeEnc
		&& pParticleMode == config.pParticleMode
		&& pTessellationMode == config.pTessellationMode
		&& pDepthTestMode == config.pDepthTestMode
		
		&& pDecodeInDepth == config.pDecodeInDepth
		&& pEncodeOutDepth == config.pEncodeOutDepth
		&& pInverseDepth == config.pInverseDepth
		&& pMaskedSolidity == config.pMaskedSolidity
		&& pClipPlane == config.pClipPlane
		&& pNoZClip == config.pNoZClip
		&& pOutputConstant == config.pOutputConstant
		&& pOutputColor == config.pOutputColor
		&& pAmbientLightProbe == config.pAmbientLightProbe
		&& pBillboard == config.pBillboard
		&& pSkinReflections == config.pSkinReflections
		&& pFadeOutRange == config.pFadeOutRange
		&& pVariations == config.pVariations
		&& pUseNormalRoughnessCorrection == config.pUseNormalRoughnessCorrection
		&& pGSRenderCube == config.pGSRenderCube
		&& pGSRenderCascaded == config.pGSRenderCascaded
		&& pGSRenderStereo == config.pGSRenderStereo
		&& pVSRenderStereo == config.pVSRenderStereo
		&& pSharedSPB == config.pSharedSPB
		&& pOutline == config.pOutline
		&& pOutlineThicknessScreen == config.pOutlineThicknessScreen
		&& pLuminanceOnly == config.pLuminanceOnly
		&& pGIMaterial == config.pGIMaterial
		&& pXRay == config.pXRay
		
		&& pDynamicColorTint == config.pDynamicColorTint
		&& pDynamicColorGamma == config.pDynamicColorGamma
		&& pDynamicColorSolidityMultiplier == config.pDynamicColorSolidityMultiplier
		&& pDynamicAmbientOcclusionSolidityMultiplier == config.pDynamicAmbientOcclusionSolidityMultiplier
		&& pDynamicTransparencyMultiplier == config.pDynamicTransparencyMultiplier
		&& pDynamicSolidityMultiplier == config.pDynamicSolidityMultiplier
		&& pDynamicHeightRemap == config.pDynamicHeightRemap
		&& pDynamicNormalStrength == config.pDynamicNormalStrength
		&& pDynamicNormalSolidityMultiplier == config.pDynamicNormalSolidityMultiplier
		&& pDynamicRoughnessRemap == config.pDynamicRoughnessRemap
		&& pDynamicRoughnessGamma == config.pDynamicRoughnessGamma
		&& pDynamicRoughnessSolidityMultiplier == config.pDynamicRoughnessSolidityMultiplier
		&& pDynamicReflectivitySolidityMultiplier == config.pDynamicReflectivitySolidityMultiplier
		&& pDynamicReflectivityMultiplier == config.pDynamicReflectivityMultiplier
		&& pDynamicRefractionDistortStrength == config.pDynamicRefractionDistortStrength
		&& pDynamicEmissivityTint == config.pDynamicEmissivityTint
		&& pDynamicEmissivityIntensity == config.pDynamicEmissivityIntensity
		&& pDynamicEnvRoomSize == config.pDynamicEnvRoomSize
		&& pDynamicEnvRoomOffset == config.pDynamicEnvRoomOffset
		&& pDynamicEnvRoomEmissivityTint == config.pDynamicEnvRoomEmissivityTint
		&& pDynamicEnvRoomEmissivityIntensity == config.pDynamicEnvRoomEmissivityIntensity
		&& pDynamicThickness == config.pDynamicThickness
		&& pDynamicAbsorption == config.pDynamicAbsorption
		&& pDynamicVariation == config.pDynamicVariation
		&& pDynamicOutlineColor == config.pDynamicOutlineColor
		&& pDynamicOutlineColorTint == config.pDynamicOutlineColorTint
		&& pDynamicOutlineThickness == config.pDynamicOutlineThickness
		&& pDynamicOutlineSolidity == config.pDynamicOutlineSolidity
		&& pDynamicOutlineEmissivity == config.pDynamicOutlineEmissivity
		&& pDynamicOutlineEmissivityTint == config.pDynamicOutlineEmissivityTint
		&& pDynamicRimEmissivityTint == config.pDynamicRimEmissivityTint
		&& pDynamicRimEmissivityIntensity == config.pDynamicRimEmissivityIntensity
		&& pDynamicRimAngle == config.pDynamicRimAngle
		&& pDynamicRimExponent == config.pDynamicRimExponent
		
		&& pTextureColor == config.pTextureColor
		&& pTextureColorTintMask == config.pTextureColorTintMask
		&& pTextureTransparency == config.pTextureTransparency
		&& pTextureSolidity == config.pTextureSolidity
		&& pTextureNormal == config.pTextureNormal
		&& pTextureHeight == config.pTextureHeight
		&& pTextureReflectivity == config.pTextureReflectivity
		&& pTextureRoughness == config.pTextureRoughness
		&& pTextureEmissivity == config.pTextureEmissivity
		&& pTextureAO == config.pTextureAO
		&& pTextureEnvMap == config.pTextureEnvMap
		&& pTextureEnvMapEqui == config.pTextureEnvMapEqui
		&& pTextureAbsorption == config.pTextureAbsorption
		&& pTextureRenderColor == config.pTextureRenderColor
		&& pTextureRefractionDistort == config.pTextureRefractionDistort
		&& pTextureEnvRoom == config.pTextureEnvRoom
		&& pTextureEnvRoomMask == config.pTextureEnvRoomMask
		&& pTextureEnvRoomEmissivity == config.pTextureEnvRoomEmissivity
		&& pTextureRimEmissivity == config.pTextureRimEmissivity
		&& pTextureNonPbrAlbedo == config.pTextureNonPbrAlbedo
		&& pTextureNonPbrMetalness == config.pTextureNonPbrMetalness;
}
