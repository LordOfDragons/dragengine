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

#include "deoglRSkin.h"
#include "deoglSkinCalculatedProperty.h"
#include "deoglSkinConstructedProperty.h"
#include "deoglSkinTexture.h"
#include "deoglSkinPropertyMap.h"
#include "deoglSkinRenderable.h"
#include "shader/deoglSkinShaderManager.h"
#include "state/deoglSkinState.h"
#include "state/deoglSkinStateRenderable.h"
#include "visitor/deoglVSDetermineChannelFormat.h"
#include "../deoglCaches.h"
#include "../deGraphicOpenGl.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../configuration/deoglConfiguration.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../rendering/deoglRenderReflection.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../rendering/task/shared/deoglRenderTaskSharedPool.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/deoglShaderParameterBlock.h"
#include "../shaders/paramblock/shared/deoglSharedSPBList.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElementMapBuffer.h"
#include "../skin/channel/deoglSCConstructedDynamic.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/compression/deoglTextureCompression.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglImage.h"
#include "../texture/deoglRImage.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/pixelbuffer/deoglPixelBufferMipMap.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/threading/deMutexGuard.h>



// Definitions
////////////////

#define ENABLE_CACHE_LOGGING		false

#define CACHE_FILE_VERSION			1



// #ifdef OS_ANDROID
// #undef ENABLE_CACHE_LOGGING
// #define ENABLE_CACHE_LOGGING true
// #endif



// Class deoglSkinTexture
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinTexture::deoglSkinTexture(deoglRenderThread &renderThread, deoglRSkin &skin, const deSkinTexture &texture) :
pRenderThread(renderThread),
pRTSIndex(-1),
pSkin(skin),
pName(texture.GetName()),
pPipelines(*this)
{
	// NOTE this is called during asynchronous resource loading. careful accessing other objects
	
	int i;
	
	pHasSolidity = false;
	pHasZeroSolidity = false;
	pHasTransparency = false;
	pHasEmissivity = false;
	
	pSolid = true;
	pHasHoles = false;
	pQuickTransp = false;
	pDynamicChannels = false;
	pCalculatedProperties = false;
	pConstructedProperties = false;
	pBoneProperties = false;
	pRenderableChannels = false;
	pRenderableMaterialProperties = false;
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		pChannels[i] = NULL;
	}
	
	pAbsorption.Set(0.0f, 0.0f, 0.0f);
	pAbsorptionHalfIntensityDistance = 1.25f;
	pAbsorptionRange = 0.0f;
	pAmbientOcclusion = 1.0f;
	pAmbientOcclusionSolidityMultiplier = 1.0f;
	pColor.Set(0.0f, 0.0f, 0.0f);
	pColorGamma = 2.2f;
	pColorTint.Set(1.0f, 1.0f, 1.0f);
	pColorTintMask = 1.0f;
	pColorSolidityMultiplier = 1.0f;
	pColorOmnidirCube.Set(0.0f, 0.0f, 0.0f);
	pColorOmnidirEquirect.Set(0.0f, 0.0f, 0.0f);
	pEmissivityTint.Set(1.0f, 1.0f, 1.0f);
	pEmissivityIntensity = 0.0f;
	pEmissivity.SetZero();
	pEmissivityCameraAdapted = false;
	pEmissivityTint.Set(1.0f, 1.0f, 1.0f);
	pEnvironmentRoomEmissivityTint.Set(1.0f, 1.0f, 1.0f);
	pEnvironmentRoomEmissivityIntensity = 0.0f;
	pEnvironmentRoomTint.Set(1.0f, 1.0f, 1.0f);
	pEnvironmentRoomSize.Set(1.0f, 1.0f);
	pEnvironmentRoomOffset.Set(0.0f, 0.0f, 0.0f);
	pHeightScale = 0.1f;
	pHeightOffset = 0.0f;
	pNormal.Set(0.5f, 0.5f, 1.0f);
	pNormalStrength = 1.0f;
	pNormalSolidityMultiplier = 1.0f;
	pReflectivityMultiplier = 1.0f;
	pReflectivity.Set(0.0f, 0.0f, 0.0f);
	pReflectivitySolidityMultiplier = 1.0f;
	pRefractDistortStrength = 0.0f;
	pRoughness = 1.0f;
	pRoughnessGamma = 1.0f;
	pRoughnessRemapLower = 0.0f;
	pRoughnessRemapUpper = 1.0f;
	pRoughnessSolidityMultiplier = 1.0f;
	pThickness = 0.0f;
	pTransparency = 1.0f;
	pTransparencyMultiplier = 1.0f;
	pSolidity = 1.0f;
	pSolidityMultiplier = 1.0f;
	pSolidityMasked = false;
	pSolidityFilterPriority = 0.5f;
	pSolidityInvert = false;
	
	pReflected = true;
	pShadeless = false;
	pShadowNone = false;
	pShadowImportance = 10;
	pHintNoCompression = false;
	pHintLightBlocker = false;
	pTexCoordClamp = false;
	pTexCoordOffset.Set(0.0f, 0.0f);
	pTexCoordScale.Set(1.0f, 1.0f);
	pTexCoordRotate = 0.0f;
	pRenderTaskFilters = 0;
	
	pVariationU = false;
	pVariationV = false;
	
	pParticleSheetCount = 1;
	
	pOutlineColor.Set(0.0f, 0.0f, 0.0f);
	pOutlineColorTint.Set(1.0f, 1.0f, 1.0f);
	pOutlineThickness = 0.0f;
	pOutlineThicknessScreen = false;
	pOutlineSolidity = 1.0f;
	pOutlineEmissivity.Set(0.0f, 0.0f, 0.0f);
	pOutlineEmissivityTint.Set(1.0f, 1.0f, 1.0f);
	pOutlineEmissivityIntensity = 0.0f;
	pHasOutline = false;
	pIsOutlineSolid = true;
	pIsOutlineEmissive = false;
	
	pRimEmissivity.Set(0.0f, 0.0f, 0.0f);
	pRimEmissivityTint.Set(1.0f, 1.0f, 1.0f);
	pRimEmissivityIntensity = 0.0f;
	pRimAngle = 0.0f;
	pRimExponent = 1.0f;
	
	pNonPbrAlbedo.Set(0.0f, 0.0f, 0.0f);
	pNonPbrMetalness = 0.0f;
	
	pSkinClipPlane = 0.0f;
	pSkinClipPlaneBorder = 0.0f;
	
	pXRay = false;
	
	pMirror = false;
	pRendered = false;
	pReflects = false;
	
	try{
		pPrepareChannels(skin, texture);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
	
	// store texture features required to do educated choices later on
	if(pHasTransparency){
		pSolid = false;
		pHasHoles = false;
		
	}else if(pHasSolidity){
		if(pHasZeroSolidity){
			// special case. the entire material is invisible. in general this would mean
			// the material has not to be rendered at all but if emissivity is present
			// a faster rendering can be done for this material only adding light to the
			// scene. this is a special case not yet looked at so consider it masked solid
			pSolid = false;
			pHasHoles = false;
			
		}else if(pSolidityMasked){
			if(pHasEmissivity){
				// if we have emissivity we have a problem. non-solid fragmenents will be
				// discarded. if these have now emissivity larger than 0 the emissivity
				// would not be rendered. we need to make this texture non-solid
				pSolid = false;
				pHasHoles = false;
				
			}else{
				pSolid = true;
				pHasHoles = true;
			}
			
		}else{
			pSolid = false;
			pHasHoles = false;
		}
	}
	
	pUpdateRenderTaskFilters();
}

deoglSkinTexture::~deoglSkinTexture(){
	pCleanUp();
}



// Management
///////////////

void deoglSkinTexture::AssignRTSIndex(){
	if(pRTSIndex != -1){
		return;
	}
	
	pRTSIndex = pRenderThread.GetRenderTaskSharedPool().AssignSkinTexture(this);
	pRenderThread.GetShader().InvalidateSSBOSkinTextures();
}

void deoglSkinTexture::BuildChannels(deoglRSkin &skin, const deSkinTexture &texture){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects
	#ifdef DO_PERFORMANCE_TIMING
	decTimer timer;
	#endif
	
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	int i;
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		if(!pChannels[i]){
			continue;
		}
		if(pChannels[i]->GetIsCached()){
			continue; // loaded from cache
		}
		
		if(pChannels[i]->GetImage()){
			bool hasUseSkin = false;
			switch(pChannels[i]->GetTextureType()){
			case deoglSkinChannel::ett2d:
				hasUseSkin = pChannels[i]->GetImage()->GetSkinUseTexture();
				break;
				
			case deoglSkinChannel::ettCube:
				hasUseSkin = pChannels[i]->GetImage()->GetSkinUseCubeMap();
				break;
				
			case deoglSkinChannel::ettArray:
				hasUseSkin = pChannels[i]->GetImage()->GetSkinUseArrayTexture();
				break;
			}
			
			if(hasUseSkin){
				// already build by somebody else.
				// 
				// building channels runs potentially parallel to the render thread running delayed
				// operations to init skins. during this time skin-use parameter can be changed to true.
				// it is thus possible skin-use has been false while the skin-texture has been
				// constructed but not while BuildChannels() is run. this results in the pixel buffer
				// to be present but not initialized. then pWriteCached() would write an
				// non-initialized pixel buffer which breaks future loading. to be sure nothing goes
				// wrong the cache-id has to be cleared and the pixel buffer dropped. then
				// pWriteCached() will not pick it up and delayed operations will not try to init the
				// skin using non-initialized data
				pChannels[i]->SetCacheID("");
				pChannels[i]->SetCanBeCached(false);
				pChannels[i]->SetPixelBufferMipMap(NULL);
				continue;
			}
		}
		
		if(enableCacheLogging){
			pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Not cached, building"
				" (skin='%s' texture='%s' channel='%s' id='%s')",
				skin.GetFilename().GetString(), pName.GetString(),
				deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
				pChannels[i]->GetCacheID().GetString());
		}
		
		pChannels[i]->BuildChannel(texture);
		#ifdef DO_PERFORMANCE_TIMING
		pRenderThread.GetOgl().LogInfoFormat("Skin(%s) Texture(%s) Channel(%s): Build %dms",
			skin.GetFilename().GetString(), pName.GetString(),
			deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
			(int)(timer.GetElapsedTime() * 1e3f));
		#endif
	}
	
	pCreateMipMaps();
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat("Skin(%s) Texture(%s): MipMaps %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		(int)(timer.GetElapsedTime() * 1e3f));
	#endif
	
	pCompressTextures(skin, texture);
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat("Skin(%s) Texture(%s): Compress %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		(int)(timer.GetElapsedTime() * 1e3f));
	#endif
	
	// write channels to cache suitable for caching
	pWriteCached(skin);
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat("Skin(%s) Texture(%s): SaveCache %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		(int)(timer.GetElapsedTime() * 1e3f));
	#endif
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		if(pChannels[i]){
			pChannels[i]->ClearCacheData();
		}
	}
}

void deoglSkinTexture::FinalizeAsyncResLoading(){
	int i;
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		if(pChannels[i]){
			pChannels[i]->FinalizeAsyncResLoading();
		}
	}
}



void deoglSkinTexture::SetHasSolidity(bool hasSolidity){
	pHasSolidity = hasSolidity;
}

void deoglSkinTexture::SetHasZeroSolidity(bool hasZeroSolidity){
	pHasZeroSolidity = hasZeroSolidity;
}

void deoglSkinTexture::SetHasTransparency(bool hasTransparency){
	pHasTransparency = hasTransparency;
}

void deoglSkinTexture::SetHasEmissivity(bool hasEmissivity){
	pHasEmissivity = hasEmissivity;
}



void deoglSkinTexture::SetSolidityMasked(bool solidityMasked){
	pSolidityMasked = solidityMasked;
}

void deoglSkinTexture::SetSolidityInvert(bool solidityInvert){
	pSolidityInvert = solidityInvert;
}

void deoglSkinTexture::SetSolidityFilterPriority(float solidityFilterPriority){
	pSolidityFilterPriority = decMath::clamp(solidityFilterPriority, 0.0f, 1.0f);
}

void deoglSkinTexture::SetSolid(bool solid){
	pSolid = solid;
}

void deoglSkinTexture::SetHasHoles(bool hasHoles){
	pHasHoles = hasHoles;
}

void deoglSkinTexture::SetQuickTransparency(bool quickTransp){
	pQuickTransp = quickTransp;
}

void deoglSkinTexture::SetDynamicChannels(bool dynamicChannels){
	pDynamicChannels = dynamicChannels;
}

void deoglSkinTexture::SetCalculatedProperties(bool calculatedProperties){
	pCalculatedProperties = calculatedProperties;
}

void deoglSkinTexture::SetConstructedProperties(bool constructedProperties){
	pConstructedProperties = constructedProperties;
}

void deoglSkinTexture::SetBoneProperties(bool boneProperties){
	pBoneProperties = boneProperties;
}

void deoglSkinTexture::SetRenderableChannels(bool renderableChannels){
	pRenderableChannels = renderableChannels;
}

void deoglSkinTexture::SetRenderableMaterialProperties(bool renderableMaterialProperties){
	pRenderableMaterialProperties = renderableMaterialProperties;
}



deoglSkinChannel *deoglSkinTexture::GetChannelAt(deoglSkinChannel::eChannelTypes type) const{
	return pChannels[type];
}

bool deoglSkinTexture::IsChannelEnabled(deoglSkinChannel::eChannelTypes type) const{
	return pChannels[type] != NULL;
}

void deoglSkinTexture::DropAllCaches(){
	deoglCaches &caches = pRenderThread.GetOgl().GetCaches();
	deCacheHelper &cacheTextures = caches.GetSkinTextures();
	int i;
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		if(!pChannels[i]){
			continue;
		}
		if(pChannels[i]->GetCacheID().IsEmpty()){
			continue;
		}
		
		const deMutexGuard guard(caches.GetMutex());
		cacheTextures.Delete(pChannels[i]->GetCacheID());
		pChannels[i]->SetIsCached(false);
	}
}

void deoglSkinTexture::PrepareParamBlock(){
	if(pSharedSPBElement){
		return;
	}
	
	if(pRenderThread.GetChoices().GetSharedSPBUseSSBO()){
		pSharedSPBElement = pRenderThread.GetBufferObject()
			.GetSharedSPBList(deoglRTBufferObject::esspblSkinTextureSSBO).AddElement();
		
	}else{
		pSharedSPBElement = pRenderThread.GetBufferObject()
			.GetSharedSPBList(deoglRTBufferObject::esspblSkinTextureUBO).AddElement();
	}
	
	pUpdateParamBlock(deoglSharedSPBElementMapBuffer(*pSharedSPBElement), pSharedSPBElement->GetIndex());
}



// Material Properties
////////////////////////

void deoglSkinTexture::SetColor(const decColor &color){
	pColor = color;
}

void deoglSkinTexture::SetColorGamma(float gamma){
	pColorGamma = gamma;
}

void deoglSkinTexture::SetColorTint(const decColor &tint){
	pColorTint = tint;
}

void deoglSkinTexture::SetColorTintMask(float mask){
	pColorTintMask = decMath::clamp(mask, 0.0f, 1.0f);
}

void deoglSkinTexture::SetColorSolidityMultiplier(float multiplier){
	pColorSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetColorOmnidirCube(const decColor &color){
	pColorOmnidirCube = color;
}

void deoglSkinTexture::SetColorOmnidirEquirect(const decColor &color){
	pColorOmnidirEquirect = color;
}

void deoglSkinTexture::SetAmbientOcclusion(float ao){
	pAmbientOcclusion = ao;
}

void deoglSkinTexture::SetAmbientOcclusionSolidityMultiplier(float multiplier){
	pAmbientOcclusionSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetTransparency(float transparency){
	pTransparency = transparency;
}

void deoglSkinTexture::SetTransparencyMultiplier(float multiplier){
	pTransparencyMultiplier = multiplier;
}

void deoglSkinTexture::SetSolidity(float solidity){
	pSolidity = solidity;
}

void deoglSkinTexture::SetSolidityMultiplier(float multiplier){
	pSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetHeightScale(float scale){
	pHeightScale = scale;
}

void deoglSkinTexture::SetHeightOffset(float offset){
	pHeightOffset = offset;
}

void deoglSkinTexture::SetNormal(const decColor &normal){
	pNormal = normal;
}

void deoglSkinTexture::SetNormalStrength(float strength){
	pNormalStrength = strength;
}

void deoglSkinTexture::SetNormalSolidityMultiplier(float multiplier){
	pNormalSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetRoughness(float roughness){
	pRoughness = roughness;
}

void deoglSkinTexture::SetRoughnessRemapLower(float value){
	pRoughnessRemapLower = value;
}

void deoglSkinTexture::SetRoughnessRemapUpper(float value){
	pRoughnessRemapUpper = value;
}

void deoglSkinTexture::SetRoughnessGamma(float gamma){
	pRoughnessGamma = gamma;
}

void deoglSkinTexture::SetRoughnessSolidityMultiplier(float multiplier){
	pRoughnessSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetRefractDistortStrength(float strength){
	pRefractDistortStrength = strength;
}

void deoglSkinTexture::SetReflectivity(const decColor &reflectivity){
	pReflectivity = reflectivity;
}

void deoglSkinTexture::SetReflectivityMultiplier(float multiplier){
	pReflectivityMultiplier = multiplier;
}

void deoglSkinTexture::SetReflectivitySolidityMultiplier(float multiplier){
	pReflectivitySolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetReflected(bool reflected){
	pReflected = reflected;
}

void deoglSkinTexture::SetEnvironmentRoomTint(const decColor &tint){
	pEnvironmentRoomTint = tint;
}

void deoglSkinTexture::SetEnvironmentRoomSize(const decVector2 &size){
	pEnvironmentRoomSize = size;
}

void deoglSkinTexture::SetEnvironmentRoomOffset(const decVector &offset){
	pEnvironmentRoomOffset = offset;
}

void deoglSkinTexture::SetEnvironmentRoomEmissivityTint(const decColor &tint){
	pEnvironmentRoomEmissivityTint = tint;
}

void deoglSkinTexture::SetEnvironmentRoomEmissivityIntensity(float intensity){
	pEnvironmentRoomEmissivityIntensity = intensity;
}

void deoglSkinTexture::SetEmissivity(const decColor &emissivity){
	pEmissivity = emissivity;
}

void deoglSkinTexture::SetEmissivityTint(const decColor &tint){
	pEmissivityTint = tint;
}

void deoglSkinTexture::SetEmissivityIntensity(float intensity){
	pEmissivityIntensity = intensity;
}

void deoglSkinTexture::SetEmissivityCameraAdapted(bool cameraAdapted){
	pEmissivityCameraAdapted = cameraAdapted;
}

void deoglSkinTexture::SetThickness(float thickness){
	pThickness = thickness;
}

void deoglSkinTexture::SetAbsorption(const decColor &absorption){
	pAbsorption = absorption;
}

void deoglSkinTexture::SetAbsorptionRange(float range){
	pAbsorptionRange = range;
}

void deoglSkinTexture::SetAbsorptionHalfIntensityDistance(float distance){
	pAbsorptionHalfIntensityDistance = distance;
}

void deoglSkinTexture::SetRimEmissivity(const decColor &emissivity){
	pRimEmissivity = emissivity;
}

void deoglSkinTexture::SetRimEmissivityTint(const decColor &tint){
	pRimEmissivityTint = tint;
}

void deoglSkinTexture::SetRimEmissivityIntensity(float intensity){
	pRimEmissivityIntensity = decMath::max(intensity, 0.0f);
}

void deoglSkinTexture::SetRimAngle(float angle){
	pRimAngle = decMath::clamp(angle, 0.0f, 1.0f);
}

void deoglSkinTexture::SetRimExponent(float exponent){
	pRimExponent = decMath::max(exponent, 0.0f);
}

void deoglSkinTexture::SetSkinClipPlane(float clipPlane){
	pSkinClipPlane = decMath::clamp(clipPlane, 0.0f, 1.0f);
}

void deoglSkinTexture::SetSkinClipPlaneBorder(float border){
	pSkinClipPlaneBorder = border;
}



void deoglSkinTexture::SetShadeless(bool shadeless){
	pShadeless = shadeless;
}

void deoglSkinTexture::SetShadowNone(bool shadowNone){
	pShadowNone = shadowNone;
}

void deoglSkinTexture::SetShadowImportance(int shadowImportance){
	if(shadowImportance < 0){
		pShadowImportance = 0;
		
	}else if(shadowImportance > 10){
		pShadowImportance = 10;
		
	}else{
		pShadowImportance = (char)shadowImportance;
	}
}

void deoglSkinTexture::SetHintNoCompression(bool hintNoCompression){
	pHintNoCompression = hintNoCompression;
}

void deoglSkinTexture::SetHintLightBlocker(bool hintLightBlocker){
	pHintLightBlocker = hintLightBlocker;
}



void deoglSkinTexture::SetTexCoordClamp(bool clamp){
	pTexCoordClamp = clamp;
}

void deoglSkinTexture::SetTexCoordOffset(const decVector2 &offset){
	pTexCoordOffset = offset;
}

void deoglSkinTexture::SetTexCoordScale(const decVector2 &scale){
	pTexCoordScale = scale;
}

void deoglSkinTexture::SetTexCoordRotate(float rotate){
	pTexCoordRotate = rotate;
}

void deoglSkinTexture::SetOmniDirRotate(const decVector &rotate){
	pOmniDirRotate = rotate;
}

void deoglSkinTexture::SetOmniDirRotateSpot(const decVector &rotate){
	pOmniDirRotateSpot = rotate;
}



void deoglSkinTexture::SetVariationU(bool enable){
	pVariationU = enable;
}

void deoglSkinTexture::SetVariationV(bool enable){
	pVariationV = enable;
}

void deoglSkinTexture::SetParticleSheetCount(int count){
	if(count < 1){
		DETHROW(deeInvalidParam);
	}
	pParticleSheetCount = count;
}



void deoglSkinTexture::SetOutlineColor(const decColor &color){
	pOutlineColor = color;
}

void deoglSkinTexture::SetOutlineColorTint(const decColor &color){
	pOutlineColorTint = color;
}

void deoglSkinTexture::SetOutlineSolidity(float solidity){
	pOutlineSolidity = decMath::clamp(solidity, 0.0f, 1.0f);
}

void deoglSkinTexture::SetOutlineThickness(float thickness){
	pOutlineThickness = decMath::max(thickness, 0.0f);
}

void deoglSkinTexture::SetOutlineThicknessScreen(bool enable){
	pOutlineThicknessScreen = enable;
}

void deoglSkinTexture::SetOutlineEmissivity(const decColor &emissivity){
	pOutlineEmissivity = emissivity;
}

void deoglSkinTexture::SetOutlineEmissivityTint(const decColor &emissivity){
	pOutlineEmissivityTint = emissivity;
}

void deoglSkinTexture::SetOutlineEmissivityIntensity(float intensity){
	pOutlineEmissivityIntensity = decMath::max(intensity, 0.0f);
}

void deoglSkinTexture::SetNonPbrAlbedo(const decColor &albedo){
	pNonPbrAlbedo = albedo;
}

void deoglSkinTexture::SetNonPbrMetalness(float metalness){
	pNonPbrMetalness = metalness;
}

void deoglSkinTexture::SetXRay(bool xray){
	pXRay = xray;
}



void deoglSkinTexture::SetMirror(bool mirror){
	pMirror = mirror;
}

void deoglSkinTexture::SetRendered(bool rendered){
	pRendered = rendered;
}

void deoglSkinTexture::SetReflects(bool reflects){
	pReflects = reflects;
}



deoglSkinTextureProperty &deoglSkinTexture::GetMaterialPropertyAt(int property){
	if(property < 0 || property >= EMP_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	return pMaterialProperties[property];
}

const deoglSkinTextureProperty &deoglSkinTexture::GetMaterialPropertyAt(int property) const{
	if(property < 0 || property >= EMP_COUNT){
		DETHROW(deeInvalidParam);
	}
	
	return pMaterialProperties[property];
}



// Private Functions
//////////////////////

void deoglSkinTexture::pCleanUp(){
	if(pRTSIndex != -1){
		pRenderThread.GetRenderTaskSharedPool().ReturnSkinTexture(pRTSIndex);
		pRTSIndex = -1;
		pRenderThread.GetShader().InvalidateSSBOSkinTextures();
	}
	int i;
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		if(pChannels[i]){
			delete pChannels[i];
			pChannels[i] = NULL;
		}
	}
}



#ifdef OS_ANDROID
	#define DO_PERFORMANCE_TIMING
#endif

void deoglSkinTexture::pPrepareChannels(deoglRSkin &skin, const deSkinTexture &texture){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects
	#ifdef DO_PERFORMANCE_TIMING
	decTimer timer;
	#endif
	
	const int propertyCount = texture.GetPropertyCount();
	int i;
	for(i=0; i<propertyCount; i++){
		pProcessProperty(skin, *texture.GetPropertyAt(i));
	}
	
	// determine if the skin uses renderables at all
	pRenderableMaterialProperties = false;
	
	for(i=0; i<EMP_COUNT; i++){
		if(pMaterialProperties[i].GetRenderable() != -1){
			pRenderableMaterialProperties = true;
			break;
		}
	}
	
	// determine if the texture is rendered
	pRendered = pMirror;
	
	// prepare channels required for cache loading
	deoglVSDetermineChannelFormat determineFormat(pRenderThread, skin, *this, texture);
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		determineFormat.ProcessChannel((deoglSkinChannel::eChannelTypes)i);
		if(!determineFormat.GetIsDefined()){
			continue;
		}
		
		/*
		if(determineFormat.GetSharedImage() && determineFormat.GetSharedImage()->GetSkinUse()){
			pRenderThread.GetOgl().LogInfoFormat("Skin(%s) Texture(%s) Channel(%s): Using existing image %s",
				skin.GetFilename().GetString(), pName.GetString(),
				deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
				determineFormat.GetSharedImage()->GetFilename().GetString());
		}
		*/
		
		pChannels[i] = new deoglSkinChannel(pRenderThread, (deoglSkinChannel::eChannelTypes)i);
		pChannels[i]->PrepareChannel(skin, *this, texture, determineFormat);
		
		if(i == deoglSkinChannel::ectReflectivity){
			pReflects = true;
		}
	}
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat("Skin(%s) Texture(%s): Prepare %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		(int)(timer.GetElapsedTime() * 1e3f));
	#endif
	
	// load caches
	pLoadCached(skin);
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat("Skin(%s) Texture(%s): LoadCache %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		(int)(timer.GetElapsedTime() * 1e3f));
	#endif
	
	// for the rest BuildChannel() has to be called in time
}

void deoglSkinTexture::pLoadCached(deoglRSkin &skin){
	// try to load caches using the calculated cache ids
	deoglCaches &caches = pRenderThread.GetOgl().GetCaches();
	deCacheHelper &cacheTextures = caches.GetSkinTextures();
	char *verifyData = NULL;
	int i;
	
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		if(!pChannels[i]){
			continue;
		}
		if(pChannels[i]->GetCacheID().IsEmpty()){
			continue;
		}
		
		try{
			decBaseFileReader::Ref reader;
			{
			const deMutexGuard guard(caches.GetMutex());
			reader.TakeOver(cacheTextures.Read(pChannels[i]->GetCacheID()));
			}
			if(!reader){
				if(enableCacheLogging){
					pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Cache file absent"
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
						pChannels[i]->GetCacheID().GetString());
				}
				continue;
			}
			
			// check cache version in case we upgraded
			if(reader->ReadByte() != CACHE_FILE_VERSION){
				// cache file outdated
				reader = nullptr;
				{
				const deMutexGuard guard(caches.GetMutex());
				cacheTextures.Delete(pChannels[i]->GetCacheID());
				}
				
				if(enableCacheLogging){
					pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Old version cache file"
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
						pChannels[i]->GetCacheID().GetString());
				}
				
				continue;
			}
			
			// check source verification data to reject the cached file if the source data changed.
			const decMemoryFile * const expectedVerify = pChannels[i]->GetCacheVerify();
			const int verifyLen = reader->ReadInt();
			bool verified = false;
			
			if(expectedVerify && verifyLen == expectedVerify->GetLength()){
				if(verifyLen == 0){
					verified = true;
					
				}else{
					verifyData = new char[verifyLen];
					reader->Read(verifyData, verifyLen);
					
					verified = memcmp(verifyData, expectedVerify->GetPointer(), verifyLen) == 0;
					
					delete [] verifyData;
					verifyData = nullptr;
				}
			}
			
			if(!verified){
				// cache file verify size mismatch or sanity check failed
				reader = nullptr;
				{
				const deMutexGuard guard(caches.GetMutex());
				cacheTextures.Delete(pChannels[i]->GetCacheID());
				}
				
				if(enableCacheLogging){
					pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Verify data mismatch"
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
						pChannels[i]->GetCacheID().GetString());
				}
				
				continue;
			}
			
			// read pixel buffer
			const int pixBufCount = (int)reader->ReadByte();
			const int width = (int)reader->ReadShort();
			const int height = (int)reader->ReadShort();
			const int depth = (int)reader->ReadShort();
			const deoglPixelBuffer::ePixelFormats pbformat =
				(deoglPixelBuffer::ePixelFormats)reader->ReadByte();
			int j;
			
			if(pChannels[i]->GetSize() != decPoint3(width, height, depth)){
				// cache file image size mismatch. this can happen for example in editors where the variation
				// texture property changed. in this case array textures of depth 1 can be cached due to
				// variation being disabled while the next time variation is enabled and the full depth is
				// used. also if the user changes module parameters it might be possible the textures sizes
				// are reduzed resulting in cache files not be valid anymore.
				reader = nullptr;
				{
				const deMutexGuard guard(caches.GetMutex());
				cacheTextures.Delete(pChannels[i]->GetCacheID());
				}
				
				if(enableCacheLogging){
					pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Cached image size mismatch."
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
						pChannels[i]->GetCacheID().GetString());
				}
				
				continue;
			}
			
			const int maxMipMapLevel = decMath::max(pixBufCount - 1, 0);
			
			int expectedMaxMipMaxLevel = maxMipMapLevel;
			/*
			if(i == deoglSkinChannel::ectSolidity){
				expectedMaxMipMaxLevel = decMath::clamp(
					(int)(floorf(log2f((float)((height > width) ? height : width)))) - 3, 0, 100);
			}
			*/
			
			if(maxMipMapLevel < expectedMaxMipMaxLevel){
				reader = nullptr;
				{
				const deMutexGuard guard(caches.GetMutex());
				cacheTextures.Delete(pChannels[i]->GetCacheID());
				}
				
				if(enableCacheLogging){
					pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Cached Mip-Map Count lower."
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
						pChannels[i]->GetCacheID().GetString());
				}
				
				continue;
			}
			
			const deoglPixelBufferMipMap::Ref pixelBufferMipMap(deoglPixelBufferMipMap::Ref::NewWith(pbformat, width, height, depth, maxMipMapLevel));
			
			for(j=0; j<pixBufCount; j++){
				deoglPixelBuffer &pixelBuffer = pixelBufferMipMap->GetPixelBuffer(j);
				reader->Read(pixelBuffer.GetPointer(), pixelBuffer.GetImageSize());
			}
			
			if(maxMipMapLevel > expectedMaxMipMaxLevel){
				pixelBufferMipMap->ReducePixelBufferCount(maxMipMapLevel - expectedMaxMipMaxLevel);
			}
			
			pChannels[i]->SetPixelBufferMipMap(pixelBufferMipMap);
			
			// done
			if(enableCacheLogging){
				pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Loaded from cache"
					" (skin='%s' texture='%s' channel='%s' id='%s')",
					skin.GetFilename().GetString(), pName.GetString(),
					deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
					pChannels[i]->GetCacheID().GetString());
			}
			
			pChannels[i]->SetIsCached(true);
			
		}catch(const deException &){
			if(verifyData){
				delete [] verifyData;
				verifyData = nullptr;
			}
			
			{
			const deMutexGuard guard(caches.GetMutex());
			cacheTextures.Delete(pChannels[i]->GetCacheID());
			}
			
			if(enableCacheLogging){
				pRenderThread.GetOgl().LogInfoFormat(
					"Skin Cache: Cache file damaged, forcing rebuild"
					" (skin='%s' texture='%s' channel='%s' id='%s')",
					skin.GetFilename().GetString(), pName.GetString(),
					deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
					pChannels[i]->GetCacheID().GetString());
			}
		}
	}
}

void deoglSkinTexture::pCreateMipMaps(){
	int i;
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		if(!pChannels[i]){
			continue;
		}
		if(pChannels[i]->GetIsCached()){
			continue;
		}
		
		deoglPixelBufferMipMap * const pbMipMap = pChannels[i]->GetPixelBufferMipMap();
		if(!pbMipMap){
			continue;
		}
		
		switch((deoglSkinChannel::eChannelTypes)i){
		case deoglSkinChannel::ectColor:
		case deoglSkinChannel::ectTransparency:
		case deoglSkinChannel::ectColorOmnidirCube:
		case deoglSkinChannel::ectColorOmnidirEquirect:
		case deoglSkinChannel::ectColorTintMask:
		case deoglSkinChannel::ectHeight:
		case deoglSkinChannel::ectEmissivity:
		case deoglSkinChannel::ectRefractDistort:
		case deoglSkinChannel::ectReflectivity:
		case deoglSkinChannel::ectRoughness:
		case deoglSkinChannel::ectEnvironmentMap:
		case deoglSkinChannel::ectEnvironmentRoom:
		case deoglSkinChannel::ectEnvironmentRoomMask:
		case deoglSkinChannel::ectEnvironmentRoomEmissivity:
		case deoglSkinChannel::ectAbsorption:
		case deoglSkinChannel::ectRimEmissivity:
		case deoglSkinChannel::ectNonPbrAlbedo:
		case deoglSkinChannel::ectNonPbrMetalness:
			pbMipMap->CreateMipMaps();
			break;
			
		case deoglSkinChannel::ectSolidity:
			// solidity is a tricky problem, especially with masked solidity. shaders use 35%
			// solidity as threshold to discard fragments. by using box filtering 0 values are
			// pulled towards 1 values and quickly end up above the 35% threshold. especially
			// with solidity textures placing holes near borders (for example to create blinds)
			// this causes the holes to vanish and can break shadow casting and GI handling
			// which rely on high LOD levels. there are now three possible solutions:
			// 
			// 1) use maximum filter. this favors solid geometry over holes.
			// 2) use minimum filter. this favors holes over solid geometry.
			// 3) use textureQueryLod in shader to figure out mip-map level used and pull
			//    the threshold from 35% closer to for example 95%.
			// 
			// the third solution has the problem of the function existing only since opengl 4.0
			// and what threshold to pull towards and in what speed.
			// 
			// the other two solutions are more practical and work also for opengl 3.3 GPUs.
			// the main problem here is what to favor. in general filling up holes over distance
			// is more visible since color texture is shown inside holes where it is not supposed
			// to show. this produces visible artifacts and requires carefully made textures
			// to hide these problems. by favoring holes geometry vanishes quicker which is
			// also noticable but does not add artifacts. favoring holes also helps with solidity
			// textures using holes at the border (like blinders). also vanishing solidity
			// geometry quicker improves performance. for vegetation it is a problem since
			// leaves vanish quicker.
			// 
			// one solution could be to limit the maximum mip-map level while creating mip-maps.
			// this avoids the extreme situations and should allow both types of geometry to
			// work better.
			// 
			// in tests both favor holes and favor geometry did not look that good.
			// so right now regular mip-mapping is used without limiting the mip-map level
			#if 0
			pbMipMap->ReducePixelBufferCount(3); // minimum 8x8
			if(pbMipMap->GetPixelBufferCount() == 0){
				pChannels[i]->SetPixelBufferMipMap(NULL);
				break;
			}
			#endif
			if(pSolidityFilterPriority < 0.35f){
				pbMipMap->CreateMipMapsMin();
				
			}else if(pSolidityFilterPriority > 0.65f){
				pbMipMap->CreateMipMapsMax();
				
			}else{
				pbMipMap->CreateMipMaps();
			}
			break;
			
		case deoglSkinChannel::ectNormal:
			pbMipMap->CreateNormalMipMaps();
			break;
			
		case deoglSkinChannel::ectAO:
			pbMipMap->CreateMipMapsMax();
			break;
			
		default:
			break;
		}
	}
}

void deoglSkinTexture::pCompressTextures(deoglRSkin &skin, const deSkinTexture &texture){
	deoglTextureCompression textureCompression;
	int i;
	
	textureCompression.SetFastCompression(true);
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		if(!pChannels[i]){
			continue;
		}
		if(pChannels[i]->GetIsCached()){
			continue;
		}
		
		deoglPixelBufferMipMap * const pbMipMapSource = pChannels[i]->GetPixelBufferMipMap();
		if(!pbMipMapSource){
			continue;
		}
		
		// determine texture format depending on the texture type
		const deoglCapsTextureFormat *texformat;
		
		if(pChannels[i]->GetTexture()){
			texformat = pChannels[i]->GetTexture()->GetFormat();
			
		}else if(pChannels[i]->GetCubeMap()){
			texformat = pChannels[i]->GetCubeMap()->GetFormat();
			
		}else if(pChannels[i]->GetArrayTexture()){
			texformat = pChannels[i]->GetArrayTexture()->GetFormat();
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
		// if the texture format is not compressed continue
		if(!texformat->GetIsCompressed()){
			continue;
		}
		
		// determine the compression technique
		const GLint glformat = texformat->GetFormat();
		bool canCompress = false;
		deoglPixelBuffer::ePixelFormats pbformat;
		
		if(glformat == GL_COMPRESSED_RGB_S3TC_DXT1){
			pbformat = deoglPixelBuffer::epfDXT1;
			canCompress = true;
			
		}else if(glformat == GL_COMPRESSED_RGBA_S3TC_DXT1){
			//pbformat = deoglPixelBuffer::epfDXT1;
			
		}else if(glformat == GL_COMPRESSED_RGBA_S3TC_DXT3){
			pbformat = deoglPixelBuffer::epfDXT3;
			canCompress = true;
			
		}else if(glformat == GL_COMPRESSED_RGBA_S3TC_DXT5){
			//pbformat = deoglPixelBuffer::epfDXT5;
		}
		
		if(!canCompress){
			// we can not compress it. in theory the driver would compress for us. unfortunately
			// various drivers exhibit crashes and even GPU lock-ups due to faulty compression
			// code included. for this reason only formats are allowed which we know how to
			// compress. should we end up here something went wrong.
			pRenderThread.GetLogger().LogErrorFormat("CompressTextures of not supported format:"
				" skin='%s' texture='%s' channel=%i pbformat=%x", skin.GetFilename().GetString(),
				texture.GetName().GetString(), i, pChannels[i]->GetTexture()->GetFormat()->GetFormat());
			
			DETHROW(deeInvalidParam);
		}
		
		const deoglPixelBuffer &basePixelBuffer = pbMipMapSource->GetPixelBuffer(0);
		const deoglPixelBufferMipMap::Ref pbMipMapCompressed(deoglPixelBufferMipMap::Ref::NewWith(pbformat,
				basePixelBuffer.GetWidth(), basePixelBuffer.GetHeight(),
				basePixelBuffer.GetDepth(), pbMipMapSource->GetPixelBufferCount() - 1));
		
		textureCompression.SetDecompressedDataMipMap(pbMipMapSource);
		textureCompression.SetCompressedDataMipMap(pbMipMapCompressed);
		//decTimer timer;
		textureCompression.CompressMipMap();
		/*float elapsed = timer.GetElapsedTime();
		pRenderThread.GetOgl().LogInfoFormat("CompressTextures: skin='%s' texture='%s' channel=%i"
			" pbformat=%x width=%i height=%i mipmaps=%i elapsed=%iys",
			skin.GetSkin()->GetFilename(), texture.GetName(), i,
			compressedPixelBufferMipMap->GetPixelBuffer(0)->GetFormat(),
			basePixelBuffer.GetWidth(), basePixelBuffer.GetHeight(),
			compressedPixelBufferMipMap->GetPixelBufferCount(),
			(int)(elapsed * 1e6f));*/
		
		pChannels[i]->SetPixelBufferMipMap(pbMipMapCompressed);
	}
}

void deoglSkinTexture::pWriteCached(deoglRSkin &skin){
	deoglCaches &caches = pRenderThread.GetOgl().GetCaches();
	deCacheHelper &cacheTextures = caches.GetSkinTextures();
	int i;
	
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	
	for(i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++){
		try{
			if(!pChannels[i]){
				continue;
			}
			if(pChannels[i]->GetIsCached()){
				continue; // has been loaded from cache
			}
			if(pChannels[i]->GetCacheID().IsEmpty()){
				continue; // do not cache channel
			}
			
			if(pChannels[i]->GetImage()){
				bool hasSkinUse = false;
				switch(pChannels[i]->GetTextureType()){
				case deoglSkinChannel::ett2d:
					hasSkinUse = pChannels[i]->GetImage()->GetSkinUseTexture();
					break;
					
				case deoglSkinChannel::ettCube:
					hasSkinUse = pChannels[i]->GetImage()->GetSkinUseCubeMap();
					break;
					
				case deoglSkinChannel::ettArray:
					hasSkinUse = pChannels[i]->GetImage()->GetSkinUseArrayTexture();
					break;
					
				}
				
				if(hasSkinUse){
					// already build by somebody else.
					// 
					// this is an optimization. building channels runs potentially parallel to the
					// render thread running delayed operations to init skins. during this time
					// skin-use parameter can be changed to true. it is possible skin-use has not been
					// set during the first check in BuildChannels(). this forces the channels to be
					// build. if in the mean time the skin-use has been set to true it would be a
					// waste of time to create and upload textures during delayed operation time if
					// we know the textures are going to be dropped right away because somebody else
					// created them already. dropping the cache id and pixel buffer prevents any
					// further processing of this channel
					pChannels[i]->SetCacheID("");
					pChannels[i]->SetCanBeCached(false);
					pChannels[i]->SetPixelBufferMipMap(nullptr);
					continue;
				}
			}
			
			decBaseFileWriter::Ref writer;
			{
			const deMutexGuard guard(caches.GetMutex());
			writer.TakeOver(cacheTextures.Write(pChannels[i]->GetCacheID()));
			}
			
			// write cache version
			writer->WriteByte(CACHE_FILE_VERSION);
			
			// write source verification to reject the cached file if the source data changed
			const decMemoryFile * const verifyData = pChannels[i]->GetCacheVerify();
			if(!verifyData){
				DETHROW(deeInvalidParam);
			}
			
			writer->WriteInt(verifyData->GetLength());
			writer->Write(verifyData->GetPointer(), verifyData->GetLength());
			
			// write pixel buffer
			const deoglPixelBufferMipMap * const pixelBufferMipMap =
				pChannels[i]->GetPixelBufferMipMap();
			
			if(pixelBufferMipMap){
				const int pixBufCount = pixelBufferMipMap->GetPixelBufferCount();
				int j;
				
				const deoglPixelBuffer &pixelBufferBase = pixelBufferMipMap->GetPixelBuffer(0);
				writer->WriteByte((unsigned char)pixBufCount);
				writer->WriteShort((short)pixelBufferBase.GetWidth());
				writer->WriteShort((short)pixelBufferBase.GetHeight());
				writer->WriteShort((short)pixelBufferBase.GetDepth());
				writer->WriteByte((unsigned char)pixelBufferBase.GetFormat());
				
				for(j=0; j<pixBufCount; j++){
					const deoglPixelBuffer &pixelBuffer = pixelBufferMipMap->GetPixelBuffer(j);
					writer->Write(pixelBuffer.GetPointer(), pixelBuffer.GetImageSize());
				}
				
			}else{
				// this should not happen
				writer->WriteByte(0);
				
				//if(enableCacheLogging){
					pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Pixel buffer missing"
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
						pChannels[i]->GetCacheID().GetString());
				//}
			}
			
			// done
			if(enableCacheLogging){
				pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Written to cache"
					" (skin='%s' texture='%s' channel='%s' id='%s')",
					skin.GetFilename().GetString(), pName.GetString(),
					deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
					pChannels[i]->GetCacheID().GetString());
			}
			
		}catch(const deException &){
			{
			const deMutexGuard guard(caches.GetMutex());
			cacheTextures.Delete(pChannels[i]->GetCacheID());
			}
			
			if(enableCacheLogging){
				pRenderThread.GetOgl().LogInfoFormat("Skin Cache: Written cache file failed"
					" (skin='%s' texture='%s' channel='%s' id='%s')",
					skin.GetFilename().GetString(), pName.GetString(),
					deoglSkinChannel::ChannelNameFor((deoglSkinChannel::eChannelTypes)i),
					pChannels[i]->GetCacheID().GetString());
			}
		}
	}
	
	//cacheTextures.DebugPrint(*pRenderThread.GetOgl().GetGameEngine()->GetLogger(), "OpenGL");
}

void deoglSkinTexture::pProcessProperty(deoglRSkin &skin, deSkinProperty &property){
	const deoglSkinPropertyMap::ePropertyTypes propertyType =
		deoglSkinPropertyMap::GetTypeFor(property.GetType().GetString());
	deSkinPropertyVisitorIdentify identify;
	float value;
	
	property.Visit(identify);
	
	if(identify.IsValue()){
		value = identify.CastToValue().GetValue();
		
		switch(propertyType){
		case deoglSkinPropertyMap::eptColor:
			pColor.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptColorGamma:
			pColorGamma = value;
			break;
			
		case deoglSkinPropertyMap::eptColorTint:
			pColorTint.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptColorTintMask:
			pColorTintMask = value;
			break;
			
		case deoglSkinPropertyMap::eptColorSolidityMultiplier:
			pColorSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidir:
			pColorOmnidirCube.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidirEquirect:
			pColorOmnidirEquirect.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusion:
			pAmbientOcclusion = value;
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusionSolidityMultiplier:
			pAmbientOcclusionSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptHeightScale:
			pHeightScale = value;
			break;
			
		case deoglSkinPropertyMap::eptHeightOffset:
			pHeightOffset = value;
			break;
			
		case deoglSkinPropertyMap::eptNormal:
			pNormal.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptNormalStrength:
			pNormalStrength = value;
			break;
			
		case deoglSkinPropertyMap::eptNormalSolidityMultiplier:
			pNormalSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughness:
			pRoughness = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapLower:
			pRoughnessRemapLower = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapUpper:
			pRoughnessRemapUpper = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessGamma:
			pRoughnessGamma = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessSolidityMultiplier:
			pRoughnessSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptTransparency:
			pTransparency = value;
			break;
			
		case deoglSkinPropertyMap::eptTransparencyMultiplier:
			pTransparencyMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptSolidity:
			pSolidity = value;
			pHasSolidity = true;
			break;
			
		case deoglSkinPropertyMap::eptSolidityMultiplier:
			pSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptSolidityMasked:
			pSolidityMasked = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptSolidityInvert:
			pSolidityInvert = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptSolidityFilterPriority:
			pSolidityFilterPriority = decMath::clamp(value, 0.0f, 1.0f);
			break;
			
		case deoglSkinPropertyMap::eptRefractionDistortStrength:
			pRefractDistortStrength = value;
			break;
			
		case deoglSkinPropertyMap::eptReflectivity:
			pReflectivity.Set(value, value, value);
			pReflects = (value > FLOAT_SAFE_EPSILON);
			break;
			
		case deoglSkinPropertyMap::eptReflectivityMultiplier:
			pReflectivityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptReflectivitySolidityMultiplier:
			pReflectivitySolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptReflected:
			pReflected = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptMirror:
			pMirror = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptEmissivity:
			pEmissivity.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptEmissivityTint:
			pEmissivityTint.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptEmissivityIntensity:
			pEmissivityIntensity = value;
			break;
			
		case deoglSkinPropertyMap::eptEmissivityCameraAdapted:
			pEmissivityCameraAdapted = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomTint:
			pEnvironmentRoomTint.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomSize:
			pEnvironmentRoomSize.Set(value, value);
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomOffset:
			pEnvironmentRoomOffset.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityIntensity:
			pEnvironmentRoomEmissivityIntensity = value;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityTint:
			pEnvironmentRoomEmissivityTint.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptThickness:
			pThickness = value;
			break;
			
		case deoglSkinPropertyMap::eptAbsorption:
			pAbsorption.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionRange:
			pAbsorptionRange = value;
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionHalfIntensityDistance:
			pAbsorptionHalfIntensityDistance = value;
			break;
			
		case deoglSkinPropertyMap::eptShadeless:
			pShadeless = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptHintNoCompression:
			pHintNoCompression = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptHintLightBlocker:
			pHintLightBlocker = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordClamp:
			pTexCoordClamp = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordOffset:
			pTexCoordOffset.Set(value, value);
			break;
			
		case deoglSkinPropertyMap::eptTexCoordScale:
			pTexCoordScale.Set(value, value);
			break;
			
		case deoglSkinPropertyMap::eptTexCoordRotate:
			pTexCoordRotate = value;
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotate:
			pOmniDirRotate.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotateSpot:
			pOmniDirRotateSpot.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptShadowNone:
			pShadowNone = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptShadowImportance:
			pShadowImportance = (int)(value * 10.0f + 0.01f);
			break;
			
		case deoglSkinPropertyMap::eptVariationU:
			pVariationU = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptVariationV:
			pVariationV = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptParticleSheets:
			pParticleSheetCount = decMath::max((int)(value + 0.5f), 1);
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivity:
			pEmissivity.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityTint:
			pRimEmissivityTint.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityIntensity:
			pRimEmissivityIntensity = value;
			break;
			
		case deoglSkinPropertyMap::eptRimAngle:
			pRimAngle = decMath::clamp(value, 0.0f, 1.0f);
			break;
			
		case deoglSkinPropertyMap::eptRimExponent:
			pRimExponent = decMath::max(value, 0.0f);
			break;
			
		case deoglSkinPropertyMap::eptOutlineColor:
			pOutlineColor.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptOutlineColorTint:
			pOutlineColorTint.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptOutlineThickness:
			pOutlineThickness = decMath::max(value, 0.0f);
			pHasOutline = pOutlineThickness != 0.0f || !property.GetRenderable().IsEmpty();
			break;
			
		case deoglSkinPropertyMap::eptOutlineThicknessScreen:
			pOutlineThicknessScreen = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptOutlineSolidity:
			pOutlineSolidity = decMath::clamp(value, 0.0f, 1.0f);
			pIsOutlineSolid = pOutlineSolidity == 1.0f && property.GetRenderable().IsEmpty();
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivity:
			pOutlineEmissivity.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityTint:
			pOutlineEmissivityTint.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityIntensity:
			pOutlineEmissivityIntensity = decMath::max(value, 0.0f);
			pIsOutlineEmissive = pOutlineEmissivityIntensity != 0.0f || !property.GetRenderable().IsEmpty();
			break;
			
		case deoglSkinPropertyMap::eptNonPbrAlbedo:
			pNonPbrAlbedo.Set(value, value, value);
			break;
			
		case deoglSkinPropertyMap::eptNonPbrMetalness:
			pNonPbrMetalness = value;
			break;
			
		case deoglSkinPropertyMap::eptXRay:
			pXRay = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptClipPlane:
			pSkinClipPlane = decMath::clamp(value, 0.0f, 1.0f);
			pHasSolidity = true;
			pBoneProperties = true;
			break;
			
		case deoglSkinPropertyMap::eptClipPlaneBorder:
			pSkinClipPlaneBorder = value;
			break;
			
		default:
			break;
		}
		
	}else if(identify.IsColor()){
		const decColor &color = identify.CastToColor().GetColor();
		
		switch(propertyType){
		case deoglSkinPropertyMap::eptAbsorption:
			pAbsorption = color;
			break;
			
		case deoglSkinPropertyMap::eptColor:
			pColor = color;
			break;
			
		case deoglSkinPropertyMap::eptColorTint:
			pColorTint = color;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidir:
			pColorOmnidirCube = color;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidirEquirect:
			pColorOmnidirEquirect = color;
			break;
			
		case deoglSkinPropertyMap::eptNormal:
			pNormal = color;
			break;
			
		case deoglSkinPropertyMap::eptReflectivity:
			pReflectivity = color;
			pReflects = (color.r > FLOAT_SAFE_EPSILON || color.g > FLOAT_SAFE_EPSILON || color.b > FLOAT_SAFE_EPSILON);
			break;
			
		case deoglSkinPropertyMap::eptEmissivity:
			pEmissivity = color;
			break;
			
		case deoglSkinPropertyMap::eptEmissivityTint:
			pEmissivityTint = color;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomTint:
			pEnvironmentRoomTint = color;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomSize:
			pEnvironmentRoomSize.Set(color.r, color.g);
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomOffset:
			pEnvironmentRoomOffset.Set(color.r, color.g, color.b);
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityTint:
			pEnvironmentRoomEmissivityTint = color;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordOffset:
			pTexCoordOffset.Set(color.r, color.g);
			break;
			
		case deoglSkinPropertyMap::eptTexCoordScale:
			pTexCoordScale.Set(color.r, color.g);
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotate:
			pOmniDirRotate.Set(color.r, color.g, color.b);
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotateSpot:
			pOmniDirRotateSpot.Set(color.r, color.g, color.b);
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivity:
			pRimEmissivity = color;
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityTint:
			pRimEmissivityTint = color;
			break;
			
		case deoglSkinPropertyMap::eptOutlineColor:
			pOutlineColor = color;
			break;
			
		case deoglSkinPropertyMap::eptOutlineColorTint:
			pOutlineColorTint = color;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivity:
			pOutlineEmissivity = color;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityTint:
			pOutlineEmissivityTint = color;
			break;
			
		case deoglSkinPropertyMap::eptNonPbrAlbedo:
			pNonPbrAlbedo = color;
			break;
			
		default:
			break;
		}
		
	}else if(identify.IsMapped()){
		deoglSkinTextureProperty *materialProperty = nullptr;
		bool requiresTexture = false;
		
		switch(propertyType){
		case deoglSkinPropertyMap::eptColor:
			materialProperty = pMaterialProperties + empColor;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidir:
			materialProperty = pMaterialProperties + empColorOmnidirCube;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidirEquirect:
			materialProperty = pMaterialProperties + empColorOmnidirEquirect;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptColorTint:
			materialProperty = pMaterialProperties + empColorTint;
			break;
			
		case deoglSkinPropertyMap::eptColorTintMask:
			materialProperty = pMaterialProperties + empColorTintMask;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptColorGamma:
			materialProperty = pMaterialProperties + empColorGamma;
			break;
			
		case deoglSkinPropertyMap::eptColorSolidityMultiplier:
			materialProperty = pMaterialProperties + empColorSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusion:
			materialProperty = pMaterialProperties + empAmbientOcclusion;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusionSolidityMultiplier:
			materialProperty = pMaterialProperties + empAmbientOcclusionSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptTransparency:
			materialProperty = pMaterialProperties + empTransparency;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptTransparencyMultiplier:
			materialProperty = pMaterialProperties + empTransparencyMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptSolidity:
			materialProperty = pMaterialProperties + empSolidity;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptSolidityMultiplier:
			materialProperty = pMaterialProperties + empSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptHeightScale:
			materialProperty = pMaterialProperties + empHeightScale;
			break;
			
		case deoglSkinPropertyMap::eptHeightOffset:
			materialProperty = pMaterialProperties + empHeightOffset;
			break;
			
		case deoglSkinPropertyMap::eptNormal:
			materialProperty = pMaterialProperties + empNormal;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptNormalStrength:
			materialProperty = pMaterialProperties + empNormalStrength;
			break;
			
		case deoglSkinPropertyMap::eptNormalSolidityMultiplier:
			materialProperty = pMaterialProperties + empNormalSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptRoughness:
			materialProperty = pMaterialProperties + empRoughness;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapLower:
			materialProperty = pMaterialProperties + empRoughnessRemapLower;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapUpper:
			materialProperty = pMaterialProperties + empRoughnessRemapUpper;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessGamma:
			materialProperty = pMaterialProperties + empRoughnessGamma;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessSolidityMultiplier:
			materialProperty = pMaterialProperties + empRoughnessSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptRefractionDistortStrength:
			materialProperty = pMaterialProperties + empRefractDistortStrength;
			break;
			
		case deoglSkinPropertyMap::eptReflectivity:
			materialProperty = pMaterialProperties + empReflectivity;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptReflectivityMultiplier:
			materialProperty = pMaterialProperties + empReflectivityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptReflectivitySolidityMultiplier:
			materialProperty = pMaterialProperties + empReflectivitySolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptEmissivity:
			materialProperty = pMaterialProperties + empEmissivity;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptEmissivityTint:
			materialProperty = pMaterialProperties + empEmissivityTint;
			break;
			
		case deoglSkinPropertyMap::eptEmissivityIntensity:
			materialProperty = pMaterialProperties + empEmissivityIntensity;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomTint:
			materialProperty = pMaterialProperties + empEnvironmentRoomTint;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomSize:
			materialProperty = pMaterialProperties + empEnvironmentRoomSize;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomOffset:
			materialProperty = pMaterialProperties + empEnvironmentRoomOffset;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityTint:
			materialProperty = pMaterialProperties + empEnvironmentRoomEmissivityTint;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityIntensity:
			materialProperty = pMaterialProperties + empEnvironmentRoomEmissivityIntensity;
			break;
			
		case deoglSkinPropertyMap::eptThickness:
			materialProperty = pMaterialProperties + empThickness;
			break;
			
		case deoglSkinPropertyMap::eptAbsorption:
			materialProperty = pMaterialProperties + empAbsorption;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionRange:
			materialProperty = pMaterialProperties + empAbsorptionRange;
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionHalfIntensityDistance:
			materialProperty = pMaterialProperties + empAbsorptionHalfIntensityDistance;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordClamp:
			materialProperty = pMaterialProperties + empTexCoordClamp;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordOffset:
			materialProperty = pMaterialProperties + empTexCoordOffset;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordScale:
			materialProperty = pMaterialProperties + empTexCoordScale;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordRotate:
			materialProperty = pMaterialProperties + empTexCoordRotate;
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotate:
			materialProperty = pMaterialProperties + empOmniDirRotate;
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotateSpot:
			materialProperty = pMaterialProperties + empOmniDirRotateSpot;
			break;
			
		case deoglSkinPropertyMap::eptVariationU:
			materialProperty = pMaterialProperties + empVariationU;
			break;
			
		case deoglSkinPropertyMap::eptVariationV:
			materialProperty = pMaterialProperties + empVariationV;
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivity:
			materialProperty = pMaterialProperties + empRimEmissivity;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityTint:
			materialProperty = pMaterialProperties + empRimEmissivityTint;
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityIntensity:
			materialProperty = pMaterialProperties + empRimEmissivityIntensity;
			break;
			
		case deoglSkinPropertyMap::eptRimAngle:
			materialProperty = pMaterialProperties + empRimAngle;
			break;
			
		case deoglSkinPropertyMap::eptRimExponent:
			materialProperty = pMaterialProperties + empRimExponent;
			break;
			
		case deoglSkinPropertyMap::eptOutlineColor:
			materialProperty = pMaterialProperties + empOutlineColor;
			break;
			
		case deoglSkinPropertyMap::eptOutlineColorTint:
			materialProperty = pMaterialProperties + empOutlineColorTint;
			break;
			
		case deoglSkinPropertyMap::eptOutlineThickness:
			materialProperty = pMaterialProperties + empOutlineThickness;
			pHasOutline = true;
			break;
			
		case deoglSkinPropertyMap::eptOutlineSolidity:
			materialProperty = pMaterialProperties + empOutlineSolidity;
			pIsOutlineSolid = false;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivity:
			materialProperty = pMaterialProperties + empOutlineEmissivity;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityTint:
			materialProperty = pMaterialProperties + empOutlineEmissivityTint;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityIntensity:
			materialProperty = pMaterialProperties + empOutlineEmissivityIntensity;
			pIsOutlineEmissive = true;
			break;
			
		case deoglSkinPropertyMap::eptNonPbrAlbedo:
			materialProperty = pMaterialProperties + empNonPbrAlbedo;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptNonPbrMetalness:
			materialProperty = pMaterialProperties + empNonPbrMetalness;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptClipPlane:
			materialProperty = pMaterialProperties + empSkinClipPlane;
			break;
			
		case deoglSkinPropertyMap::eptClipPlaneBorder:
			materialProperty = pMaterialProperties + empSkinClipPlaneBorder;
			break;
			
		default:
			break;
		}
		
		if(materialProperty){
			const deSkinPropertyMapped &mapped = identify.CastToMapped();
			
			const deoglSkinCalculatedProperty::Ref calculated(
				deoglSkinCalculatedProperty::Ref::NewWith());
			
			calculated->SetMappedComponent(0, mapped.GetRed());
			calculated->SetMappedComponent(1, mapped.GetGreen());
			calculated->SetMappedComponent(2, mapped.GetBlue());
			calculated->SetMappedComponent(3, mapped.GetAlpha());
			calculated->SetRequiresTexture(requiresTexture);
			
			materialProperty->SetCalculatedProperty(skin.AddCalculatedProperty(calculated));
			pCalculatedProperties = true;
		}
	}
	
	// check if the property has a renderable
	if(!property.GetRenderable().IsEmpty()){
		const decString &renderable = property.GetRenderable();
		
		switch(propertyType){
		case deoglSkinPropertyMap::eptColor:
			pMaterialProperties[empColor].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empColor].GetRenderable()).SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidir:
			pMaterialProperties[empColorOmnidirCube].SetRenderable(
				skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empColorOmnidirCube].GetRenderable()).
				SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidirEquirect:
			pMaterialProperties[empColorOmnidirEquirect].SetRenderable(
				skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empColorOmnidirEquirect].GetRenderable()).
				SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptColorTint:
			pMaterialProperties[empColorTint].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptColorTintMask:
			pMaterialProperties[empColorTintMask].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empColorTintMask].GetRenderable()).
				SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptColorGamma:
			pMaterialProperties[empColorGamma].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptColorSolidityMultiplier:
			pMaterialProperties[empColorSolidityMultiplier].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusion:
			pMaterialProperties[empAmbientOcclusion].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empAmbientOcclusion].GetRenderable())
				.SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusionSolidityMultiplier:
			pMaterialProperties[empAmbientOcclusionSolidityMultiplier]
				.SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptTransparency:
			pMaterialProperties[empTransparency].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empTransparency].GetRenderable()).SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptTransparencyMultiplier:
			pMaterialProperties[empTransparencyMultiplier].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptSolidity:
			pMaterialProperties[empSolidity].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empSolidity].GetRenderable()).SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptSolidityMultiplier:
			pMaterialProperties[empSolidityMultiplier].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptHeightScale:
			pMaterialProperties[empHeightScale].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptHeightOffset:
			pMaterialProperties[empHeightOffset].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptNormal:
			pMaterialProperties[empNormal].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empNormal].GetRenderable()).SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptNormalStrength:
			pMaterialProperties[empNormalStrength].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptNormalSolidityMultiplier:
			pMaterialProperties[empNormalSolidityMultiplier].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptRoughness:
			pMaterialProperties[empRoughness].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empRoughness].GetRenderable()).SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapLower:
			pMaterialProperties[empRoughnessRemapLower].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapUpper:
			pMaterialProperties[empRoughnessRemapUpper].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptRoughnessGamma:
			pMaterialProperties[empRoughnessGamma].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptRoughnessSolidityMultiplier:
			pMaterialProperties[empRoughnessSolidityMultiplier].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptRefractionDistortStrength:
			pMaterialProperties[empRefractDistortStrength].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptReflectivity:
			pMaterialProperties[empReflectivity].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empReflectivity].GetRenderable())
				.SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptReflectivityMultiplier:
			pMaterialProperties[empReflectivityMultiplier].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptReflectivitySolidityMultiplier:
			pMaterialProperties[empReflectivitySolidityMultiplier]
				.SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptEmissivity:
			pMaterialProperties[empEmissivity].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empEmissivity].GetRenderable())
				.SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptEmissivityTint:
			pMaterialProperties[empEmissivityTint].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptEmissivityIntensity:
			pMaterialProperties[empEmissivityIntensity].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomTint:
			pMaterialProperties[empEnvironmentRoomTint].SetRenderable(
				skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomSize:
			pMaterialProperties[empEnvironmentRoomSize].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomOffset:
			pMaterialProperties[empEnvironmentRoomOffset].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityTint:
			pMaterialProperties[empEnvironmentRoomEmissivityTint]
				.SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityIntensity:
			pMaterialProperties[empEnvironmentRoomEmissivityIntensity]
				.SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptThickness:
			pMaterialProperties[empThickness].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptAbsorption:
			pMaterialProperties[empAbsorption].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empAbsorption]
				.GetRenderable()).SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionRange:
			pMaterialProperties[empAbsorptionRange].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionHalfIntensityDistance:
			pMaterialProperties[empAbsorptionHalfIntensityDistance]
				.SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptTexCoordClamp:
			pMaterialProperties[empTexCoordClamp].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptTexCoordOffset:
			pMaterialProperties[empTexCoordOffset].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptTexCoordScale:
			pMaterialProperties[empTexCoordScale].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptTexCoordRotate:
			pMaterialProperties[empTexCoordRotate].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotate:
			pMaterialProperties[empOmniDirRotate].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotateSpot:
			pMaterialProperties[empOmniDirRotateSpot].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptVariationU:
			pMaterialProperties[empVariationU].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptVariationV:
			pMaterialProperties[empVariationV].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivity:
			pMaterialProperties[empRimEmissivity].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empRimEmissivity].GetRenderable())
				.SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityTint:
			pMaterialProperties[empRimEmissivityTint].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityIntensity:
			pMaterialProperties[empRimEmissivityIntensity].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptRimAngle:
			pMaterialProperties[empRimAngle].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptRimExponent:
			pMaterialProperties[empRimExponent].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptOutlineColor:
			pMaterialProperties[empOutlineColor].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptOutlineColorTint:
			pMaterialProperties[empOutlineColorTint].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptOutlineThickness:
			pMaterialProperties[empOutlineThickness].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptOutlineSolidity:
			pMaterialProperties[empOutlineSolidity].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivity:
			pMaterialProperties[empOutlineEmissivity].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityTint:
			pMaterialProperties[empOutlineEmissivityTint].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityIntensity:
			pMaterialProperties[empOutlineEmissivityIntensity].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptNonPbrAlbedo:
			pMaterialProperties[empNonPbrAlbedo].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empNonPbrAlbedo].GetRenderable()).SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptNonPbrMetalness:
			pMaterialProperties[empNonPbrMetalness].SetRenderable(skin.AddRenderable(renderable));
			skin.GetRenderableAt(pMaterialProperties[empNonPbrMetalness].GetRenderable()).SetRequiresTexture(true);
			break;
			
		case deoglSkinPropertyMap::eptClipPlane:
			pMaterialProperties[empSkinClipPlane].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		case deoglSkinPropertyMap::eptClipPlaneBorder:
			pMaterialProperties[empSkinClipPlaneBorder].SetRenderable(skin.AddRenderable(renderable));
			break;
			
		default:
			break;
		}
	}
	
	// check if the property has a boner
	if(!property.GetBone().IsEmpty()){
		switch(propertyType){
		case deoglSkinPropertyMap::eptClipPlane:
			pMaterialProperties[empSkinClipPlane].SetBone(skin.AddBone(property.GetBone()));
			break;
			
		default:
			break;
		}
	}
}

void deoglSkinTexture::pUpdateParamBlock(deoglShaderParameterBlock &spb, int element){
	// etutValueColorTransparency,
	// etutValueNormalHeight,
	// etutValueReflectivityRoughness,
	// etutValueRefractionDistort,
	// etutValueAO,
	spb.SetParameterDataVec3(deoglSkinShader::etutTexColorTint, element,
		powf(pColorTint.r, 2.2f), powf(pColorTint.g, 2.2f), powf(pColorTint.b, 2.2f));
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexColorGamma, element, pColorGamma);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexColorSolidMultiplier, element,
		pColorSolidityMultiplier);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexAOSolidityMultiplier, element,
		pAmbientOcclusionSolidityMultiplier);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexTransparencyMultiplier, element,
		pTransparencyMultiplier);
	
	{
	float lower = 0.0f, upper = pSolidityMultiplier;
	if(pSolidityInvert){
		lower = 1.0f - lower;
		upper = 1.0f - upper;
	}
	spb.SetParameterDataVec2(deoglSkinShader::etutTexSolidityRemap, element,
		decMath::clamp(lower, 0.0f, 1.0f), decMath::clamp(upper, 0.0f, 1.0f));
	}
	
	// (texHeight - 0.5 + offset) * scale = texHeight*scale + (offset-0.5)*scale
	spb.SetParameterDataVec2(deoglSkinShader::etutTexHeightRemap, element,
		pHeightScale, (pHeightOffset - 0.5f) * pHeightScale);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexNormalStrength, element, pNormalStrength);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexNormalSolidityMultiplier, element,
		pNormalSolidityMultiplier);
	
	spb.SetParameterDataVec2(deoglSkinShader::etutTexRoughnessRemap, element,
		pRoughnessRemapUpper - pRoughnessRemapLower, pRoughnessRemapLower);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexRoughnessGamma, element, pRoughnessGamma);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexRoughnessSolidityMultiplier, element,
		pRoughnessSolidityMultiplier);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexReflectivitySolidityMultiplier, element,
		pReflectivitySolidityMultiplier);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexRefractionDistortStrength, element,
		pRefractDistortStrength);
	
	spb.SetParameterDataVec3(deoglSkinShader::etutTexEmissivityIntensity, element,
		powf(pEmissivityTint.r, 2.2f) * pEmissivityIntensity,
		powf(pEmissivityTint.g, 2.2f) * pEmissivityIntensity,
		powf(pEmissivityTint.b, 2.2f) * pEmissivityIntensity);
	
	spb.SetParameterDataVec3(deoglSkinShader::etutTexEnvRoomTint, element,
		powf(pEnvironmentRoomTint.r, 2.2f),
		powf(pEnvironmentRoomTint.g, 2.2f),
		powf(pEnvironmentRoomTint.b, 2.2f));
	
	spb.SetParameterDataVec2(deoglSkinShader::etutTexEnvRoomSize, element,
		1.0f / pEnvironmentRoomSize.x, -1.0f / pEnvironmentRoomSize.y);
	
	spb.SetParameterDataVec3(deoglSkinShader::etutTexEnvRoomOffset, element,
		pEnvironmentRoomOffset);
	
	spb.SetParameterDataVec3(deoglSkinShader::etutTexEnvRoomEmissivityIntensity, element,
		powf(pEnvironmentRoomEmissivityTint.r, 2.2f) * pEnvironmentRoomEmissivityIntensity,
		powf(pEnvironmentRoomEmissivityTint.g, 2.2f) * pEnvironmentRoomEmissivityIntensity,
		powf(pEnvironmentRoomEmissivityTint.b, 2.2f) * pEnvironmentRoomEmissivityIntensity);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexThickness, element, pThickness);
	spb.SetParameterDataFloat(deoglSkinShader::etutTexAbsorptionRange, element, pAbsorptionRange);
	
	spb.SetParameterDataVec2(deoglSkinShader::etutTexVariationEnableScale, element,
		pVariationU ? 1.0f : 0.0f, pVariationV ? 1.0f : 0.0f);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexReflectivityMultiplier, element,
		pReflectivityMultiplier);
	
	// see shader source geometry/defren/skin/particle_ribbon.glsl for the
	// calculation of the maximum number of supported sheets (4)
	spb.SetParameterDataInt(deoglSkinShader::etutTexParticleSheetCount, element,
		decMath::clamp(pParticleSheetCount, 1, 4));
	
	spb.SetParameterDataVec3(deoglSkinShader::etutTexRimEmissivityIntensity, element,
		powf(pRimEmissivityTint.r, 2.2f) * pRimEmissivityIntensity,
		powf(pRimEmissivityTint.g, 2.2f) * pRimEmissivityIntensity,
		powf(pRimEmissivityTint.b, 2.2f) * pRimEmissivityIntensity);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexRimAngle, element,
		pRimAngle > 0.001f ? 1.0f / (pRimAngle * HALF_PI) : 0.0f);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexRimExponent, element,
		decMath::max(pRimExponent, 0.001f));
	
	spb.SetParameterDataVec3(deoglSkinShader::etutTexOutlineColor, element,
		powf(pOutlineColor.r, 2.2f), powf(pOutlineColor.g, 2.2f),
		powf(pOutlineColor.b, 2.2f));
	
	spb.SetParameterDataVec3(deoglSkinShader::etutTexOutlineColorTint, element,
		powf(pOutlineColorTint.r, 2.2f), powf(pOutlineColorTint.g, 2.2f),
		powf(pOutlineColorTint.b, 2.2f));
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexOutlineThickness, element, pOutlineThickness);
	spb.SetParameterDataFloat(deoglSkinShader::etutTexOutlineSolidity, element, pOutlineSolidity);
	
	spb.SetParameterDataVec3(deoglSkinShader::etutTexOutlineEmissivity, element,
		powf(pOutlineEmissivity.r, 2.2f) * pOutlineEmissivityIntensity,
		powf(pOutlineEmissivity.g, 2.2f) * pOutlineEmissivityIntensity,
		powf(pOutlineEmissivity.b, 2.2f) * pOutlineEmissivityIntensity);
	
	spb.SetParameterDataVec3(deoglSkinShader::etutTexOutlineEmissivityTint, element,
		powf(pOutlineEmissivityTint.r, 2.2f), powf(pOutlineEmissivityTint.g, 2.2f),
		powf(pOutlineEmissivityTint.b, 2.2f));
	
	spb.SetParameterDataBool(deoglSkinShader::etutTexEmissivityCameraAdapted, element,
		pEmissivityCameraAdapted);
	
	spb.SetParameterDataFloat(deoglSkinShader::etutTexSkinClipPlane, element, pSkinClipPlane);
	spb.SetParameterDataFloat(deoglSkinShader::etutTexSkinClipPlaneBorder, element, pSkinClipPlaneBorder);
}

void deoglSkinTexture::pUpdateRenderTaskFilters(){
	pRenderTaskFilters = ertfRender;
	if(pXRay){
		pRenderTaskFilters |= ertfXRay;
	}
	if(pSolid){
		pRenderTaskFilters |= ertfSolid;
	}
	if(pHasOutline){
		pRenderTaskFilters |= ertfOutline;
	}
	if(pIsOutlineSolid){
		pRenderTaskFilters |= ertfOutlineSolid;
	}
	if(pHasHoles){
		pRenderTaskFilters |= ertfHoles;
	}
	if(pShadowNone){
		pRenderTaskFilters |= ertfShadowNone;
	}
	if(pReflected){
		pRenderTaskFilters |= ertfReflected;
	}
	if(pRendered){
		pRenderTaskFilters |= ertfRendered;
	}
}
