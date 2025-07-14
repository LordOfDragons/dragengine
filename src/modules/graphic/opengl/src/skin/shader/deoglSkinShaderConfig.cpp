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

#include "deoglSkinShaderConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkinShaderConfig
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinShaderConfig::deoglSkinShaderConfig(){
	Reset();
}

deoglSkinShaderConfig::deoglSkinShaderConfig( const deoglSkinShaderConfig &copy ){
	Reset();
	*this = copy;
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
	pSkinClipPlane = false;
	
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
	pDynamicEnvRoomTint = false;
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
	pDynamicSkinClipPlane = false;
	pDynamicSkinClipPlaneBorder = false;
	
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
	
	UpdateKey();
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

void deoglSkinShaderConfig::SetSkinClipPlane( bool skinClipPlane ){
	pSkinClipPlane = skinClipPlane;
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

void deoglSkinShaderConfig::SetDynamicEnvRoomTint(bool dynamic){
	pDynamicEnvRoomTint = dynamic;
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

void deoglSkinShaderConfig::SetDynamicSkinClipPlane( bool dynamic ){
	pDynamicSkinClipPlane = dynamic;
}

void deoglSkinShaderConfig::SetDynamicSkinClipPlaneBorder( bool dynamic ){
	pDynamicSkinClipPlaneBorder = dynamic;
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



void deoglSkinShaderConfig::UpdateKey(){
	pKey1 = ( uint32_t )0;
	pKey1 |= ( ( uint32_t )pShaderMode ) << 0; // 3 values, 2 bits
	pKey1 |= ( ( uint32_t )pGeometryMode ) << 2; // 6 values, 3 bits
	pKey1 |= ( ( uint32_t )pDepthMode ) << 5; // 3 values, 2 bits
	pKey1 |= ( ( uint32_t )pMaterialNormalModeDec ) << 7; // 3 values, 2 bits
	pKey1 |= ( ( uint32_t )pMaterialNormalModeEnc ) << 9; // 3 values, 2 bits
	pKey1 |= ( ( uint32_t )pParticleMode ) << 11; // 3 values, 2 bits
	pKey1 |= ( ( uint32_t )pTessellationMode ) << 13; // 2 values, 1 bits
	pKey1 |= ( ( uint32_t )pDepthTestMode ) << 14; // 3 values, 2 bits
	
	pKey2 = ( uint32_t )0;
	if( pDecodeInDepth ) pKey2 |= ( uint32_t )1 << 0;
	if( pInverseDepth ) pKey2 |= ( uint32_t )1 << 1;
	if( pEncodeOutDepth ) pKey2 |= ( uint32_t )1 << 2;
	if( pMaskedSolidity ) pKey2 |= ( uint32_t )1 << 3;
	if( pClipPlane ) pKey2 |= ( uint32_t )1 << 4;
	if( pOutputConstant ) pKey2 |= ( uint32_t )1 << 5;
	if( pOutputColor ) pKey2 |= ( uint32_t )1 << 6;
	if( pAmbientLightProbe ) pKey2 |= ( uint32_t )1 << 7;
	if( pBillboard ) pKey2 |= ( uint32_t )1 << 8;
	if( pSkinReflections ) pKey2 |= ( uint32_t )1 << 9;
	if( pFadeOutRange ) pKey2 |= ( uint32_t )1 << 10;
	if( pVariations ) pKey2 |= ( uint32_t )1 << 11;
	if( pUseNormalRoughnessCorrection ) pKey2 |= ( uint32_t )1 << 12;
	if( pGSRenderCube ) pKey2 |= ( uint32_t )1 << 13;
	if( pGSRenderCascaded ) pKey2 |= ( uint32_t )1 << 14;
	if( pVSRenderStereo ) pKey2 |= ( uint32_t )1 << 15;
	if( pGSRenderStereo ) pKey2 |= ( uint32_t )1 << 16;
	if( pSharedSPB ) pKey2 |= ( uint32_t )1 << 17;
	if( pOutline ) pKey2 |= ( uint32_t )1 << 18;
	if( pOutlineThicknessScreen ) pKey2 |= ( uint32_t )1 << 19;
	if( pLuminanceOnly ) pKey2 |= ( uint32_t )1 << 20;
	if( pGIMaterial ) pKey2 |= ( uint32_t )1 << 21;
	if( pSkinClipPlane ) pKey2 |= ( uint32_t )1 << 22;
	
	pKey3 = ( uint64_t )0;
	if( pDynamicColorTint ) pKey3 |= ( uint64_t )1 << 0;
	if( pDynamicColorGamma ) pKey3 |= ( uint64_t )1 << 1;
	if( pDynamicColorSolidityMultiplier ) pKey3 |= ( uint64_t )1 << 2;
	if( pDynamicAmbientOcclusionSolidityMultiplier ) pKey3 |= ( uint64_t )1 << 3;
	if( pDynamicTransparencyMultiplier ) pKey3 |= ( uint64_t )1 << 4;
	if( pDynamicSolidityMultiplier ) pKey3 |= ( uint64_t )1 << 5;
	if( pDynamicHeightRemap ) pKey3 |= ( uint64_t )1 << 6;
	if( pDynamicNormalStrength ) pKey3 |= ( uint64_t )1 << 7;
	if( pDynamicNormalSolidityMultiplier ) pKey3 |= ( uint64_t )1 << 8;
	if( pDynamicRoughnessRemap ) pKey3 |= ( uint64_t )1 << 9;
	if( pDynamicRoughnessGamma ) pKey3 |= ( uint64_t )1 << 10;
	if( pDynamicRoughnessSolidityMultiplier ) pKey3 |= ( uint64_t )1 << 11;
	if( pDynamicReflectivitySolidityMultiplier ) pKey3 |= ( uint64_t )1 << 12;
	if( pDynamicReflectivityMultiplier ) pKey3 |= ( uint64_t )1 << 13;
	if( pDynamicRefractionDistortStrength ) pKey3 |= ( uint64_t )1 << 14;
	if( pDynamicEmissivityTint ) pKey3 |= ( uint64_t )1 << 15;
	if( pDynamicEmissivityIntensity ) pKey3 |= ( uint64_t )1 << 16;
	if( pDynamicEnvRoomTint ) pKey3 |= (uint64_t)1 << 17;
	if( pDynamicEnvRoomSize ) pKey3 |= ( uint64_t )1 << 18;
	if( pDynamicEnvRoomOffset ) pKey3 |= ( uint64_t )1 << 19;
	if( pDynamicEnvRoomEmissivityTint ) pKey3 |= ( uint64_t )1 << 20;
	if( pDynamicEnvRoomEmissivityIntensity ) pKey3 |= ( uint64_t )1 << 21;
	if( pDynamicThickness ) pKey3 |= ( uint64_t )1 << 22;
	if( pDynamicAbsorption ) pKey3 |= ( uint64_t )1 << 23;
	if( pDynamicVariation ) pKey3 |= ( uint64_t )1 << 24;
	if( pDynamicOutlineColor ) pKey3 |= ( uint64_t )1 << 25;
	if( pDynamicOutlineColorTint ) pKey3 |= ( uint64_t )1 << 26;
	if( pDynamicOutlineThickness ) pKey3 |= ( uint64_t )1 << 27;
	if( pDynamicOutlineSolidity ) pKey3 |= ( uint64_t )1 << 28;
	if( pDynamicOutlineEmissivity ) pKey3 |= ( uint64_t )1 << 29;
	if( pDynamicOutlineEmissivityTint ) pKey3 |= ( uint64_t )1 << 30;
	if( pDynamicRimEmissivityTint ) pKey3 |= ( uint64_t )1 << 31;
	if( pDynamicRimEmissivityIntensity ) pKey3 |= ( uint64_t )1 << 32;
	if( pDynamicRimAngle ) pKey3 |= ( uint64_t )1 << 33;
	if( pDynamicRimExponent ) pKey3 |= ( uint64_t )1 << 34;
	if( pDynamicSkinClipPlane ) pKey3 |= ( uint64_t )1 << 35;
	if( pDynamicSkinClipPlaneBorder ) pKey3 |= ( uint64_t )1 << 36;
	
	pKey4 = ( uint32_t )0;
	if( pTextureColor ) pKey4 |= ( uint32_t )1 << 0;
	if( pTextureColorTintMask ) pKey4 |= ( uint32_t )1 << 1;
	if( pTextureTransparency ) pKey4 |= ( uint32_t )1 << 2;
	if( pTextureSolidity ) pKey4 |= ( uint32_t )1 << 3;
	if( pTextureNormal ) pKey4 |= ( uint32_t )1 << 4;
	if( pTextureHeight ) pKey4 |= ( uint32_t )1 << 5;
	if( pTextureReflectivity ) pKey4 |= ( uint32_t )1 << 6;
	if( pTextureRoughness ) pKey4 |= ( uint32_t )1 << 7;
	if( pTextureEmissivity ) pKey4 |= ( uint32_t )1 << 8;
	if( pTextureAO ) pKey4 |= ( uint32_t )1 << 9;
	if( pTextureEnvMap ) pKey4 |= ( uint32_t )1 << 10;
	if( pTextureEnvMapEqui ) pKey4 |= ( uint32_t )1 << 11;
	if( pTextureAbsorption ) pKey4 |= ( uint32_t )1 << 12;
	if( pTextureRenderColor ) pKey4 |= ( uint32_t )1 << 13;
	if( pTextureRefractionDistort ) pKey4 |= ( uint32_t )1 << 14;
	if( pTextureEnvRoom ) pKey4 |= ( uint32_t )1 << 15;
	if( pTextureEnvRoomMask ) pKey4 |= ( uint32_t )1 << 16;
	if( pTextureEnvRoomEmissivity ) pKey4 |= ( uint32_t )1 << 17;
	if( pTextureRimEmissivity ) pKey4 |= ( uint32_t )1 << 18;
	if( pTextureNonPbrAlbedo ) pKey4 |= ( uint32_t )1 << 19;
	if( pTextureNonPbrMetalness ) pKey4 |= ( uint32_t )1 << 20;
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
	if( pSkinClipPlane ){
		string.Append( " skinClpPl" );
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
	if(pDynamicEnvRoomTint){
		string.Append(" dynEnvRoomTint");
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
	if( pDynamicSkinClipPlane ){
		string.Append( " dynSkClpPl" );
	}
	if( pDynamicSkinClipPlaneBorder ){
		string.Append( " dynSkClpPlBrd" );
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
	pSkinClipPlane = config.pSkinClipPlane;
	
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
	pDynamicEnvRoomTint = config.pDynamicEnvRoomTint;
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
	pDynamicSkinClipPlane = config.pDynamicSkinClipPlane;
	pDynamicSkinClipPlaneBorder = config.pDynamicSkinClipPlaneBorder;
	
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
	
	pKey1 = config.pKey1;
	pKey2 = config.pKey2;
	pKey3 = config.pKey3;
	pKey4 = config.pKey4;
	
	return *this;
}

bool deoglSkinShaderConfig::operator==( const deoglSkinShaderConfig &config ) const{
	return pKey1 == config.pKey1
		&& pKey2 == config.pKey2
		&& pKey3 == config.pKey3
		&& pKey4 == config.pKey4;
}
