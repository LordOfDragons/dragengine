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

#include "deoglSkinChannel.h"
#include "deoglSCBuildConstructed.h"
#include "deoglSCConstructedDefinition.h"
#include "deoglSCConstructedDynamic.h"
#include "../deoglRSkin.h"
#include "../deoglSkinTexture.h"
#include "../deoglSkinPropertyMap.h"
#include "../deoglSkinConstructedProperty.h"
#include "../combinedTexture/deoglCombinedTexture.h"
#include "../visitor/deoglVSDetermineChannelFormat.h"
#include "../../deGraphicOpenGl.h"
#include "../../capabilities/deoglCapsTextureFormat.h"
#include "../../configuration/deoglConfiguration.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../../texture/deoglImage.h"
#include "../../texture/arraytexture/deoglArrayTexture.h"
#include "../../texture/cubemap/deoglCubeMap.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../video/deoglVideo.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/video/deVideo.h>



// Class deoglSkinChannel
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinChannel::deoglSkinChannel(deoglRenderThread &renderThread, eChannelTypes type) :
pRenderThread(renderThread),
pType(type),

pSize(1, 1, 1),
pComponentCount(0),
pFloatFormat(false),
pCompressed(true),

pFactorU(1.0f),
pFactorV(1.0f),
pTexture(NULL),
pCubeMap(NULL),
pArrayTexture(NULL),
pCombinedTexture(NULL),
pTextureType(ett2d),

pIsCached(false),
pCanBeCached(false),
pCacheVerify(NULL),
pCacheConstrDefSource1(NULL),
pCacheConstrDefSource2(NULL),
pCacheConstrVerifySource1(NULL),
pCacheConstrVerifySource2(NULL),

pDelayedCombineImage1(NULL),
pDelayedCombineImage2(NULL),

pUniform(true),
pDynamic(false),

pImage(nullptr),

pVideo(NULL),
pVideoPlayer(-1),
pSharedVideoPlayer(true),
pRenderable(-1),
pDynamicConstructed(-1),

pSolidityFilterPriority(0.5f){
}

deoglSkinChannel::~deoglSkinChannel(){
	if(pCombinedTexture){
		pCombinedTexture->RemoveUsage();
	}
	
	if(pArrayTexture){
		delete pArrayTexture;
	}
	if(pCubeMap){
		delete pCubeMap;
	}
	if(pTexture){
		delete pTexture;
	}
	
	
}



// Management
///////////////

void deoglSkinChannel::FinalizeAsyncResLoading(){
	// called synchronously on main thread
	
	pHoldImage = pImage; // claim reference in main thread
	
	if(pDelayedCombineImage1){
		pCombinedImage1 = pDelayedCombineImage1->GetRImage();
		pDelayedCombineImage1 = nullptr;
	}
	
	if(pDelayedCombineImage2){
		pCombinedImage2 = pDelayedCombineImage2->GetRImage();
		pDelayedCombineImage2 = nullptr;
	}
}



void deoglSkinChannel::SetSize(const decPoint3 &size){
	if(!(size > decPoint3())){
		DETHROW(deeInvalidParam);
	}
	
	pSize = size;
}

void deoglSkinChannel::SetComponentCount(int componentCount){
	if(componentCount < 1 || componentCount > 4){
		DETHROW(deeInvalidParam);
	}
	
	pComponentCount = componentCount;
}

void deoglSkinChannel::SetFactorU(float factor){
	pFactorU = factor;
}

void deoglSkinChannel::SetFactorV(float factor){
	pFactorV = factor;
}

void deoglSkinChannel::SetCombinedTexture(deoglCombinedTexture *combinedTexture){
	if(combinedTexture != pCombinedTexture){
		if(pCombinedTexture){
			pCombinedTexture->RemoveUsage();
		}
		pCombinedTexture = combinedTexture;
	}
}



void deoglSkinChannel::SetPixelBufferMipMap(deoglPixelBufferMipMap *pbmipmap){
	pPixelBufferMipMap = pbmipmap;
}

void deoglSkinChannel::SetIsCached(bool isCached){
	pIsCached = isCached;
}

void deoglSkinChannel::SetCanBeCached(bool canBeCached){
	pCanBeCached = canBeCached;
}

void deoglSkinChannel::SetCacheID(const char *cacheID){
	pCacheID = cacheID;
}



void deoglSkinChannel::SetUniformColor(const decColor& color){
	pUniformColor = color;
}

bool deoglSkinChannel::IsComponentStatic(int component) const{
	if(component < 0 || component > 3){
		DETHROW(deeInvalidParam);
	}
	return pUniformColorMask[component];
}

bool deoglSkinChannel::HasStaticComponent() const{
	int i;
	for(i=0; i<pComponentCount; i++){
		if(pUniformColorMask[i]){
			return true;
		}
	}
	return false;
}

bool deoglSkinChannel::AllComponentsStatic() const{
	int i;
	for(i=0; i<pComponentCount; i++){
		if(!pUniformColorMask[i]){
			return false;
		}
	}
	return true;
}

bool deoglSkinChannel::AllComponentsNotStatic() const{
	return !HasStaticComponent();
}

void deoglSkinChannel::SetUniform(bool uniform){
	pUniform = uniform;
}

void deoglSkinChannel::SetDynamic(bool dynamic){
	pDynamic = dynamic;
}

void deoglSkinChannel::SetVideo(deVideo *video){
	pVideo = video;
}

void deoglSkinChannel::SetVideoPlayer(int index){
	pVideoPlayer = index;
}

void deoglSkinChannel::SetSharedVideoPlayer(bool shared){
	pSharedVideoPlayer = shared;
}

void deoglSkinChannel::SetRenderable(int index){
	pRenderable = index;
}

void deoglSkinChannel::SetDynamicConstructed(int index){
	pDynamicConstructed = index;
}



void deoglSkinChannel::PrepareChannel(deoglRSkin &skin, deoglSkinTexture &texture,
const deSkinTexture &engTexture, const deoglVSDetermineChannelFormat &channelFormat){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects
	
	if(channelFormat.GetSharedImage()){
		// if shared image exists store it and claim reference during main thread time.
		// this is done always no matter if the image is build or just used
		pImage = channelFormat.GetSharedImage();
	}
	
	pInitUniformColor();
	
	pTextureType = channelFormat.GetTextureType();
	bool hasSkinUse = false;
	if(pImage){
		switch(pTextureType){
		case ett2d:
			hasSkinUse = pImage->GetSkinUseTexture();
			break;
			
		case ettCube:
			hasSkinUse = pImage->GetSkinUseCubeMap();
			break;
			
		case ettArray:
			hasSkinUse = pImage->GetSkinUseArrayTexture();
			break;
		}
	}
	
	if(!hasSkinUse){
		pCreatePixelBufferMipMap(texture, channelFormat);
		if(pPixelBufferMipMap){
			// the actual creation of the texture will be done during the delayed creation of the skin
			pCreateTextureObject(texture);
			pCanBeCached = true;
		}
	}
	
	const int propertyCount = engTexture.GetPropertyCount();
	int i;
	for(i=0; i<propertyCount; i++){
		pPrepareProperty(skin, texture, *engTexture.GetPropertyAt(i));
	}
	
	pBuildCacheID();
	pBuildCacheVerify();
}

void deoglSkinChannel::BuildChannel(const deSkinTexture &engTexture){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects
	
	if(!pPixelBufferMipMap){
		return; // texture is completly uniform
	}
	
	const int propertyCount = engTexture.GetPropertyCount();
	int i;
	for(i=0; i<propertyCount; i++){
		pBuildProperty(*engTexture.GetPropertyAt(i));
	}
	
	//printf( "type=%i mask=%i,%i,%i,%i color=%g,%g,%g,%g\n", pType, pUniformColorMask[0],
	//	pUniformColorMask[1], pUniformColorMask[2], pUniformColorMask[3], pUniformColor.r,
	//	pUniformColor.g, pUniformColor.b, pUniformColor.a );
	
	pFillWithUniformColor();
}

void deoglSkinChannel::ClearCacheData(){
		pCacheVerify = NULL;
	}
	
		pCacheConstrDefSource1 = NULL;
	}
		pCacheConstrDefSource2 = NULL;
	}
		pCacheConstrVerifySource1 = NULL;
	}
		pCacheConstrVerifySource2 = NULL;
	}
	
	pCacheIDSource1.Empty();
	pCacheIDSource2.Empty();
}

void deoglSkinChannel::UseSharedImage(){
	// called in render thread. deleting opengl objects here is fine but using
	// deoglDelayedOperations results in dead-locking
	DEASSERT_NOTNULL(pImage)
	
	if(pTexture){
		if(pImage->GetSkinUseTexture()){
			delete pTexture;
			
		}else{
			pImage->SetTexture(pTexture); // guaranteed to not use deoglDelayedOperations
		}
		
		pTexture = nullptr;
		
	}else if(pCubeMap){
		if(pImage->GetSkinUseCubeMap()){
			delete pCubeMap;
			
		}else{
			pImage->SetCubeMap(pCubeMap); // guaranteed to not use deoglDelayedOperations
		}
		
		pCubeMap = nullptr;
		
	}else if(pArrayTexture){
		if(pImage->GetSkinUseArrayTexture()){
			delete pArrayTexture;
			
		}else{
			pImage->SetArrayTexture(pArrayTexture); // guaranteed to not use deoglDelayedOperations
		}
		
		pArrayTexture = nullptr;
	}
}



#include <dragengine/common/utils/decTimer.h>
void deoglSkinChannel::GenerateConeMap(){
	decTimer timer;
	
	// see http://http.developer.nvidia.com/GPUGems3/gpugems3_ch18.html
	const deoglPixelBuffer &pixelBufferBase = pPixelBufferMipMap->GetPixelBuffer(0);
	deoglPixelBuffer::sByte2 * const data = pixelBufferBase.GetPointerByte2();
	const int height = pixelBufferBase.GetHeight();
	const int width = pixelBufferBase.GetWidth();
	const int depth = pixelBufferBase.GetDepth();
	const float factorX = 1.0f / (float)width;
	const float factorY = 1.0f / (float)height;
	const float factorHeight = 1.0f / 255.0f;
	decVector destination;
	decVector direction;
	decVector source;
	decVector test;
	float coneRadius;
	float bestConeRadius;
	int x, y, z, x2, y2, i, x3, y3;
	const int stepCount = 16; //128;
	const float stepFactor = 1.0f / (float)stepCount;
	float dx, dy;
	
	source.z = 1.0; // top
	for(z=0; z<depth; z++){
		deoglPixelBuffer::sByte2 * const layer = data + width * height * z;
		
		for(y=0; y<height; y++){
			deoglPixelBuffer::sByte2 * const line = layer + width * y;
			source.y = factorY * (float)y;
			
			printf("line %i:%i\n", z, y);
			for(x=0; x<width; x++){
				const float sh = factorHeight * (float)line[x].r;
				source.x = factorX * (float)x;
				bestConeRadius = 1.0f;
				
				for(y2=0; y2<height; y2++){
					const deoglPixelBuffer::sByte2 * const line2 = layer + width * y2;
					destination.y = factorY * (float)y2;
					
					for(x2=0; x2<width; x2++){
						destination.z = factorHeight * (float)line2[x2].r;
						if(destination.z == source.z){
							continue; // avoid div-by zero and anyways if dest is on the same height as source it's no solution
						}
						
						destination.x = factorX * (float)x2;
						
						direction = destination - source;
						// scale direction so it runs from destination to bottom and split it into the appropriate step size
						direction *= stepFactor * destination.z / -direction.z;
						
						test = destination; // so destination is not modified
						x3 = x2;
						y3 = y2;
						for(i=0; i<stepCount; i++){
							test += direction;
							
							x3 = (int)(test.x * width);
							if(x3 < 0){
								x3 = 0;
							}else if(x3 >= width){
								x3 = width - 1;
							}
							
							y3 = (int)(test.y * height);
							if(y3 < 0){
								y3 = 0;
							}else if(y3 >= height){
								y3 = height - 1;
							}
							
							if(factorHeight * (float)layer[width * y3 + x3].r <= test.z){
								break;
							}
						}
						
						if(test.z > sh){ // destination has to be above source
							dx = factorX * (float)(x3 - x);
							//dx = test.x - source.x;
							dy = factorY * (float)(y3 - y);
							//dy = test.y - source.y;
							coneRadius = sqrtf(dx * dx + dy * dy) / (test.z - sh);
							if(coneRadius < bestConeRadius){
								bestConeRadius = coneRadius;
							}
						}
					}
					
					if(x2 < width){
						break;
					}
				}
				
				line[x].g = (GLubyte)(255.0f * bestConeRadius);
			}
		}
	}
	
	printf("GenerateConeMap in %fs\n", timer.GetElapsedTime());
}



const char *deoglSkinChannel::ChannelNameFor(eChannelTypes type){
	switch(type){
	case ectColor:
		return "Color";
		
	case ectColorTintMask:
		return "Color tint mask";
		
	case ectTransparency:
		return "Transparency";
		
	case ectColorOmnidirCube:
		return "Color omni-dir cubemap";
		
	case ectColorOmnidirEquirect:
		return "Color omni-dir equirect";
		
	case ectSolidity:
		return "Solidity";
		
	case ectNormal:
		return "Normal";
		
	case ectHeight:
		return "Height";
		
	case ectEmissivity:
		return "Emissivity";
		
	case ectRefractDistort:
		return "Refract distort";
		
	case ectAO:
		return "Ambient occlusion";
		
	case ectReflectivity:
		return "Reflectivity";
		
	case ectRoughness:
		return "Roughness";
		
	case ectEnvironmentMap:
		return "Environment map";
		
	case ectEnvironmentRoom:
		return "Environment room";
		
	case ectEnvironmentRoomMask:
		return "Environment room mask";
		
	case ectEnvironmentRoomEmissivity:
		return "Environment room emissivity";
		
	case ectAbsorption:
		return "Absorption";
		
	case ectRimEmissivity:
		return "RimEmissivity";
		
	case ectNonPbrAlbedo:
		return "NonPbrAlbedo";
		
	case ectNonPbrMetalness:
		return "NonPbrMetalness";
	}
	
	return "?";
}

bool deoglSkinChannel::ChannelTypeMatchingPropertyType(
deoglSkinPropertyMap::ePropertyTypes propertyType,
deoglSkinChannel::eChannelTypes& channelType){
	switch(propertyType){
	case deoglSkinPropertyMap::eptColor:
		channelType = ectColor;
		return true;
		
	case deoglSkinPropertyMap::eptTransparency:
		channelType = ectTransparency;
		return true;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
		channelType = ectColorOmnidirCube;
		return true;
		
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
		channelType = ectColorOmnidirEquirect;
		return true;
		
	case deoglSkinPropertyMap::eptColorTintMask:
		channelType = ectColorTintMask;
		return true;
		
	case deoglSkinPropertyMap::eptSolidity:
	case deoglSkinPropertyMap::eptSolidityFilterPriority:
		channelType = ectSolidity;
		return true;
		
	case deoglSkinPropertyMap::eptNormal:
		channelType = ectNormal;
		return true;
		
	case deoglSkinPropertyMap::eptHeight:
		channelType = ectHeight;
		return true;
		
	case deoglSkinPropertyMap::eptEmissivity:
		channelType = ectEmissivity;
		return true;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		channelType = ectRefractDistort;
		return true;
		
	case deoglSkinPropertyMap::eptReflectivity:
		channelType = ectReflectivity;
		return true;
		
	case deoglSkinPropertyMap::eptRoughness:
		channelType = ectRoughness;
		return true;
		
	case deoglSkinPropertyMap::eptAmbientOcclusion:
		channelType = ectAO;
		return true;
		
	case deoglSkinPropertyMap::eptEnvironmentMap:
		channelType = ectEnvironmentMap;
		return true;
		
	case deoglSkinPropertyMap::eptEnvironmentRoom:
		channelType = ectEnvironmentRoom;
		return true;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
		channelType = ectEnvironmentRoomMask;
		return true;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		channelType = ectEnvironmentRoomEmissivity;
		return true;
		
	case deoglSkinPropertyMap::eptAbsorption:
		channelType = ectAbsorption;
		return true;
		
	case deoglSkinPropertyMap::eptRimEmissivity:
		channelType = ectRimEmissivity;
		return true;
		
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
		channelType = ectNonPbrAlbedo;
		return true;
		
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		channelType = ectNonPbrMetalness;
		return true;
		
	default:
		return false;
	}
}



// Private Functions
//////////////////////

void deoglSkinChannel::pInitUniformColor(){
	pUniformColorMask[0] = true;
	pUniformColorMask[1] = true;
	pUniformColorMask[2] = true;
	pUniformColorMask[3] = true;
	
	switch(pType){
	case ectColor:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		break;
		
	case ectColorTintMask:
		pUniformColor.Set(1.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectTransparency:
		pUniformColor.Set(1.0f, 1.0f, 1.0f, 1.0f);
		break;
		
	case ectColorOmnidirCube:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		break;
		
	case ectColorOmnidirEquirect:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		break;
		
	case ectSolidity:
		pUniformColor.Set(1.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectNormal:
		pUniformColor.Set(0.5f, 0.5f, 1.0f, 0.0f);
		break;
		
	case ectHeight:
		pUniformColor.Set(1.0f, 1.0f, 0.0f, 0.0f);
		break;
		
	case ectEmissivity:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectRefractDistort:
		pUniformColor.Set(0.5f, 0.5f, 0.0f, 0.0f);
		break;
		
	case ectAO:
		pUniformColor.Set(1.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectReflectivity:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectRoughness:
		pUniformColor.Set(1.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectEnvironmentMap:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		break;
		
	case ectEnvironmentRoom:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		break;
		
	case ectEnvironmentRoomMask:
		pUniformColor.Set(1.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectEnvironmentRoomEmissivity:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 1.0f);
		break;
		
	case ectAbsorption:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectRimEmissivity:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectNonPbrAlbedo:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	case ectNonPbrMetalness:
		pUniformColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
		break;
		
	default:
		DETHROW(deeInvalidParam);
	}
}

bool deoglSkinChannel::pIsPropertyValidForType(deoglSkinPropertyMap::ePropertyTypes propertyType) const{
	eChannelTypes channelType;
	
	if(ChannelTypeMatchingPropertyType(propertyType, channelType)){
		return pType == channelType;
		
	}else{
		return false;
	}
}



void deoglSkinChannel::pCreatePixelBufferMipMap(deoglSkinTexture &texture,
const deoglVSDetermineChannelFormat &channelFormat){
	const decPoint3 &pixelBufferSize = channelFormat.GetRequiredSize();
	deoglPixelBuffer::ePixelFormats pixelBufferFormat = deoglPixelBuffer::epfByte3;
	bool mipMapped = false;
	
	SetPixelBufferMipMap(nullptr);
	
	pSize = channelFormat.GetRequiredSize();
	pComponentCount = channelFormat.GetRequiredComponentCount();
	pFloatFormat = channelFormat.GetRequiresFloat();
	pCompressed = true;
	pFactorU = (float)pSize.x / (float)pixelBufferSize.x;
	pFactorV = (float)pSize.y / (float)pixelBufferSize.y;
	pUniform = channelFormat.GetIsUniform();
	pDynamic = channelFormat.GetIsDynamic();
	
	// determine if mip mapped and compressed has to be used
	if(pSize.x < 2 && pSize.y < 2){
		// otherwise some drivers run into problems like messing up the texture while trying to
		// create mip maps for a texture which has only one mip map level
		mipMapped = false;
		pCompressed = false;
		
	}else{
		mipMapped = channelFormat.GetAllowMipMap();
		
		if(pType == ectNormal){
			pCompressed = true;
			
		}else if(texture.GetHintNoCompression()){
			pCompressed = false;
			
		}else{
			pCompressed = true;
		}
	}
	
	if(!pRenderThread.GetConfiguration().GetUseTextureCompression()){
		pCompressed = false;
	}
	
#ifdef OS_ANDROID
	// creating mip-maps is horribly slow under android. furthermore cache space on
	// disk is limited so keeping cache files small helps. another solution could
	// be improving mip map creation speed but it is questionable if this can be done
	
	// oh man... if disabled faster but the noise... U_U
	//mipMapped = false;
#endif
	
	// determine the pixel buffer format to use
	if(pComponentCount == 1){
		if(pFloatFormat){
			pixelBufferFormat = deoglPixelBuffer::epfFloat1;
			
		}else{
			pixelBufferFormat = deoglPixelBuffer::epfByte1;
		}
		
	}else if(pComponentCount == 2){
		if(pFloatFormat){
			pixelBufferFormat = deoglPixelBuffer::epfFloat2;
			
		}else{
			pixelBufferFormat = deoglPixelBuffer::epfByte2;
		}
		
	}else if(pComponentCount == 3){
		if(pFloatFormat){
			pixelBufferFormat = deoglPixelBuffer::epfFloat3;
			
		}else{
			pixelBufferFormat = deoglPixelBuffer::epfByte3;
		}
		
	}else{ // componentCount == 4
		if(pFloatFormat){
			pixelBufferFormat = deoglPixelBuffer::epfFloat4;
			
		}else{
			pixelBufferFormat = deoglPixelBuffer::epfByte4;
		}
	}
	
	// create pixel buffer if required and fill it
	//if( ! pUniform ){
		if(mipMapped){
			pPixelBufferMipMap.TakeOver(new deoglPixelBufferMipMap(pixelBufferFormat,
				pixelBufferSize.x, pixelBufferSize.y, pixelBufferSize.z, 100));
			
		}else{
			pPixelBufferMipMap.TakeOver(new deoglPixelBufferMipMap(pixelBufferFormat,
				pixelBufferSize.x, pixelBufferSize.y, pixelBufferSize.z, 0));
		}
	//}
}

void deoglSkinChannel::pCreateTextureObject(const deoglSkinTexture &texture){
	const bool mipMapped = pPixelBufferMipMap->GetPixelBufferCount() > 1;
	
	switch(pType){
	case ectColorOmnidirCube:
	case ectEnvironmentMap:
	case ectEnvironmentRoom:
	case ectEnvironmentRoomEmissivity:
		pCubeMap = new deoglCubeMap(pRenderThread);
		pCubeMap->SetMipMapped(mipMapped);
		pCubeMap->SetSize(pSize.x);
		pCubeMap->SetMapingFormat(pComponentCount, pFloatFormat, pCompressed);
		break;
		
	default:
		if(texture.GetVariationU() || texture.GetVariationV()){
			pArrayTexture = new deoglArrayTexture(pRenderThread);
			pArrayTexture->SetMipMapped(mipMapped);
			pArrayTexture->SetSize(pSize.x, pSize.y, pSize.z);
			pArrayTexture->SetMapingFormat(pComponentCount, pFloatFormat, pCompressed);
			
		}else{
			pTexture = new deoglTexture(pRenderThread);
			pTexture->SetMipMapped(mipMapped);
			pTexture->SetSize(pSize.x, pSize.y);
			pTexture->SetMapingFormat(pComponentCount, pFloatFormat, pCompressed);
		}
	}
}

void deoglSkinChannel::pPrepareProperty(deoglRSkin &skin, deoglSkinTexture &texture,
deSkinProperty &property){
	pCacheID.Empty();
	
	const deoglSkinPropertyMap::ePropertyTypes propertyType =
		deoglSkinPropertyMap::GetTypeFor(property.GetType());
	
	if(!pIsPropertyValidForType(propertyType)){
		return;
	}
	
	deSkinPropertyVisitorIdentify identify;
	property.Visit(identify);
	
	if(identify.IsImage()){
		pPreparePropertyImage(propertyType, texture, identify.CastToImage());
		
	}else if(identify.IsColor()){
		pPreparePropertyColor(propertyType, texture, identify.CastToColor());
		
	}else if(identify.IsValue()){
		pPreparePropertyValue(propertyType, texture, identify.CastToValue());
		
	}else if(identify.IsVideo()){
		pPreparePropertyVideo(propertyType, skin, texture, identify.CastToVideo());
		
	}else if(identify.IsConstructed()){
		pPreparePropertyConstructed(propertyType, skin, texture, identify.CastToConstructed());
		
	}else if(identify.IsMapped()){
		pPreparePropertyMapped(propertyType, skin, texture, identify.CastToMapped());
	}
	
	if(!property.GetRenderable().IsEmpty()){
		pRenderable = skin.AddRenderable(property.GetRenderable());
		texture.SetRenderableChannels(true);
	}
}

void deoglSkinChannel::pPreparePropertyValue(deoglSkinPropertyMap::ePropertyTypes propertyType,
deoglSkinTexture &texture, const deSkinPropertyValue &property){
	const float value = property.GetValue();
	
	switch(propertyType){
	case deoglSkinPropertyMap::eptColor:
	case deoglSkinPropertyMap::eptColorOmnidir:
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptAbsorption:
		pUniformColor.r = value;
		pUniformColor.g = value;
		pUniformColor.b = value;
		break;
		
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
	case deoglSkinPropertyMap::eptRimEmissivity:
		pUniformColor.r = value;
		pUniformColor.g = value;
		pUniformColor.b = value;
		texture.SetHasEmissivity(value > 0.001f);
		break;
		
	case deoglSkinPropertyMap::eptHeight:
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
		pUniformColor.r = value;
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
		//pUniformColor.a = value;
		pUniformColor.r = value;
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
		//pUniformColor.a = value; // if combined with color
		if(value < 0.999f){
			pUniformColor.r = value;
			texture.SetHasTransparency(true);
		}
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		if(value < 0.999f){
			pUniformColor.r = value;
			
			texture.SetHasSolidity(true);
			
			const float realValue = texture.GetSolidityInvert() ? 1.0f - value : value;
			
			if(texture.GetSolidityMasked()){
				texture.SetHasZeroSolidity(realValue < 0.5f);
				
			}else{
				texture.SetHasZeroSolidity(realValue < 0.001f);
			}
		}
		break;
		
	case deoglSkinPropertyMap::eptSolidityFilterPriority:
		pSolidityFilterPriority = decMath::clamp(value, 0.0f, 1.0f);
		break;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		pUniformColor.r = value;
		pUniformColor.g = value;
		break;
		
	default:
		break;
	}
}

void deoglSkinChannel::pPreparePropertyColor(deoglSkinPropertyMap::ePropertyTypes propertyType,
deoglSkinTexture &texture, const deSkinPropertyColor &property){
	const decColor &color = property.GetColor();
	
	switch(propertyType){
	case deoglSkinPropertyMap::eptHeight:
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptRoughness:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		pUniformColor.r = color.r;
		break;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		pUniformColor.r = color.r;
		pUniformColor.g = color.g;
		break;
		
	case deoglSkinPropertyMap::eptColor:
	case deoglSkinPropertyMap::eptColorOmnidir:
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptAbsorption:
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
		pUniformColor.r = color.r;
		pUniformColor.g = color.g;
		pUniformColor.b = color.b;
		break;
		
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
	case deoglSkinPropertyMap::eptRimEmissivity:
		pUniformColor.r = color.r;
		pUniformColor.g = color.g;
		pUniformColor.b = color.b;
		texture.SetHasEmissivity(color.r > 0.001f || color.g > 0.001f || color.b > 0.001f);
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
		//pUniformColor.a = color.r; // if combined with color
		if(color.r < 0.999f){
			pUniformColor.r = color.r;
			texture.SetHasTransparency(true);
		}
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		if(color.r < 0.999f){
			pUniformColor.r = color.r;
			
			texture.SetHasSolidity(true);
			
			const float realValue = texture.GetSolidityInvert() ? 1.0f - color.r : color.r;
			
			if(texture.GetSolidityMasked()){
				texture.SetHasZeroSolidity(realValue < 0.5f);
				
			}else{
				texture.SetHasZeroSolidity(realValue < 0.001f);
			}
		}
		break;
		
	default:
		break;
	}
}

void deoglSkinChannel::pPreparePropertyImage(deoglSkinPropertyMap::ePropertyTypes propertyType,
deoglSkinTexture &texture, const deSkinPropertyImage &property){
	const deImage * const image = property.GetImage();
	if(!image){
		return;
	}
	
	switch(propertyType){
	case deoglSkinPropertyMap::eptColor:
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptTransparency:
	case deoglSkinPropertyMap::eptSolidity:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptHeight:
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptRefractionDistort:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptRoughness:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
	case deoglSkinPropertyMap::eptAbsorption:
	case deoglSkinPropertyMap::eptRimEmissivity:
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		if(image->GetComponentCount() == 1){
			pClearUniformMasks(0, 100, 100, 100);
			pClearUniformMasks(1, 100, 100, 100);
			pClearUniformMasks(2, 100, 100, 100);
			
		}else{
			pClearUniformMasks(0, 1, 2, 3);
		}
		pDelayedCombineImage1 = (deoglImage*)image->GetPeerGraphic();
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		if(image->GetDepth() == 6){
			if(image->GetComponentCount() == 1){
				pClearUniformMasks(0, 100, 100, 100);
				pClearUniformMasks(1, 100, 100, 100);
				pClearUniformMasks(2, 100, 100, 100);
				
			}else{
				pClearUniformMasks(0, 1, 2, 3);
			}
			pDelayedCombineImage1 = (deoglImage*)image->GetPeerGraphic();
		}
		break;
		
	default:
		break;
	}
	
	switch(propertyType){
	case deoglSkinPropertyMap::eptTransparency:
		texture.SetHasTransparency(true);
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		texture.SetHasSolidity(true);
		texture.SetHasZeroSolidity(false);
		break;
		
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptRimEmissivity:
		texture.SetHasEmissivity(true);
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		if(image->GetDepth() == 6){
			texture.SetHasEmissivity(true);
		}
		break;
		
	default:
		break;
	}
}

void deoglSkinChannel::pPreparePropertyVideo(deoglSkinPropertyMap::ePropertyTypes propertyType,
deoglRSkin &skin, deoglSkinTexture &texture, const deSkinPropertyVideo &property){
	pVideo = property.GetVideo();
	if(!pVideo){
		return;
	}
	
	pVideoPlayer = skin.AddVideoPlayer();
	pSharedVideoPlayer = property.GetSharedTime();
	
	// caching not possible if videos
	pCanBeCached = false;
	
	// if the video is going to be cached set the channel dynamic. this is required
	// since with cached videos the texture used changes with each frame in contrary
	// to not cached videos where all frames are uploaded into the same texture
	deoglVideo &oglVideo = *((deoglVideo*)pVideo->GetPeerGraphic());
	if(oglVideo.CacheFrames()){
		texture.SetDynamicChannels(true);
	}
	
	switch(propertyType){
	case deoglSkinPropertyMap::eptTransparency:
		texture.SetHasTransparency(true);
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		texture.SetHasSolidity(true);
		texture.SetHasZeroSolidity(false);
		break;
		
	default:
		break;
	}
}

void deoglSkinChannel::pPreparePropertyMapped(deoglSkinPropertyMap::ePropertyTypes propertyType,
deoglRSkin &skin, deoglSkinTexture &texture, const deSkinPropertyMapped &property){
	texture.SetCalculatedProperties(true);
	pCanBeCached = false;
	
	switch(propertyType){
	case deoglSkinPropertyMap::eptTransparency:
		texture.SetHasTransparency(true);
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		texture.SetHasSolidity(true);
		texture.SetHasZeroSolidity(false);
		break;
		
	default:
		break;
	}
}

void deoglSkinChannel::pPreparePropertyConstructed(deoglSkinPropertyMap::ePropertyTypes propertyType,
deoglRSkin &skin, deoglSkinTexture &texture, const deSkinPropertyConstructed &property){
	int targetRed = 100;
	int targetGreen = 100;
	int targetBlue = 100;
	int targetAlpha = 100;
	bool second = false;
	
	switch(propertyType){
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptHeight:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		targetRed = 0;
		break;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		targetRed = 0;
		targetGreen = 1;
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptAbsorption:
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
		targetRed = 0;
		targetGreen = 1;
		targetBlue = 2;
		break;
		
	case deoglSkinPropertyMap::eptColor:
		targetRed = 0;
		targetGreen = 1;
		targetBlue = 2;
		targetAlpha = 3;
		break;
		
	case deoglSkinPropertyMap::eptSolidity:
		targetRed = 0;
		texture.SetHasSolidity(true);
		texture.SetHasZeroSolidity(false);
		break;
		
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
	case deoglSkinPropertyMap::eptRimEmissivity:
		targetRed = 0;
		targetGreen = 1;
		targetBlue = 2;
		texture.SetHasEmissivity(true);
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
		//targetRed = 3; // if combined with color
		targetRed = 0;
		texture.SetHasTransparency(true);
		second = true;
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
		//targetRed = 3;
		targetRed = 0;
		second = true;
		break;
		
	default:
		break;
	}
	
	if(targetRed == 100 && targetGreen == 100 && targetBlue == 100 && targetAlpha == 100){
		return;
	}
	
	pClearUniformMasks(targetRed, targetGreen, targetBlue, targetAlpha);
	
	// calculate constructed definition. this is used for the cache id and cache file header
	decMemoryFile *memoryFileDef;
	decMemoryFile *memoryFileVerify;
	
	if(second){
		if(pCacheConstrDefSource2 || pCacheConstrVerifySource2){
			pCanBeCached = false;
			return;
		}
		
		pCacheConstrDefSource2 = new decMemoryFile("");
		memoryFileDef = pCacheConstrDefSource2;
		
		pCacheConstrVerifySource2 = new decMemoryFile("");
		memoryFileVerify = pCacheConstrVerifySource2;
		
	}else{
		if(pCacheConstrDefSource1 || pCacheConstrVerifySource1){
			pCanBeCached = false;
			return;
		}
		
		pCacheConstrDefSource1 = new decMemoryFile("");
		memoryFileDef = pCacheConstrDefSource1;
		
		pCacheConstrVerifySource1 = new decMemoryFile("");
		memoryFileVerify = pCacheConstrVerifySource1;
	}
	
	// dynamic constructed
	if(deoglSCConstructedDynamic::IsDynamic(property.GetContent())){
		pDynamicConstructed = skin.AddConstructedProperty(deoglSkinConstructedProperty::Ref::New(
			new deoglSkinConstructedProperty(property)));
		texture.SetConstructedProperties(true);
		pCanBeCached = false;
		return;
	}
	
	// caching
	deoglSCConstructedDefinition visitor(*pRenderThread.GetOgl().GetGameEngine(),
		memoryFileDef, memoryFileVerify, property);
	property.GetContent().Visit(visitor);
	if(!visitor.GetCacheValid()){
		pCanBeCached = false;
		return;
	}
}

#ifdef __GNUG__
	// fix for gcc bug with -Werror=array-bounds
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Warray-bounds"
#endif

void deoglSkinChannel::pClearUniformMasks(int targetRed, int targetGreen,
int targetBlue, int targetAlpha){
	if(targetRed < pComponentCount){
		pUniformColorMask[targetRed] = false;
	}
	if(targetGreen < pComponentCount){
		pUniformColorMask[targetGreen] = false;
	}
	if(targetBlue < pComponentCount){
		pUniformColorMask[targetBlue] = false;
	}
	if(targetAlpha < pComponentCount){
		pUniformColorMask[targetAlpha] = false;
	}
}

#ifdef __GNUG__
	#pragma GCC diagnostic pop
#endif

static uint32_t vCRC32Table[] = {/* CRC polynomial 0xedb88320 */
	0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
	0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
	0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
	0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
	0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
	0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
	0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
	0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
	0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
	0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
	0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
	0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
	0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
	0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
	0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
	0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
	0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
	0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
	0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
	0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
	0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
	0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
	0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
	0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
	0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
	0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
	0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
	0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
	0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
	0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
	0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
	0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
	0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
	0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
	0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
	0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
	0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
	0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
	0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
	0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
	0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
	0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
	0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

decString deoglSkinChannel::pCRC32String(const decString &string) const{
	// calculate crc
	const int len = string.GetLength();
	uint32_t crc32 = 0xffffffff;
	int i;
	
	for(i=0; i<len; i++){
		crc32 = vCRC32Table[(crc32 ^ (uint8_t)string[i]) & 0xff] ^ (crc32 >> 8);
	}
	crc32 = ~crc32;
	
	// convert to hex
	decString result;
	int shift = 28;
	
	result.Set('0', 8);
	
	for(i=0; i<8; i++, shift-=4){
		const int nibble = (crc32 >> shift) & 0xf;
		
		if(nibble < 10){
			result[i] = (char)('0' + nibble);
			
		}else{
			result[i] = (char)('a' + (nibble - 10));
		}
	}
	
	return result;
}

decString deoglSkinChannel::pCRC32Data(const decMemoryFile &data) const{
	// calculate crc
	const char * const ptrdata = data.GetPointer();
	const int len = data.GetLength();
	uint32_t crc32 = 0xffffffff;
	int i;
	
	for(i=0; i<len; i++){
		crc32 = vCRC32Table[(crc32 ^ (uint8_t)ptrdata[i]) & 0xff] ^ (crc32 >> 8);
	}
	crc32 = ~crc32;
	
	// convert to hex
	decString result;
	int shift = 28;
	
	result.Set('0', 8);
	
	for(i=0; i<8; i++, shift-=4){
		const int nibble = (crc32 >> shift) & 0xf;
		
		if(nibble < 10){
			result[i] = (char)('0' + nibble);
			
		}else{
			result[i] = (char)('a' + (nibble - 10));
		}
	}
	
	return result;
}

decString deoglSkinChannel::pColor2CacheBinary(const decColor &color) const{
	const uint64_t bincolor = 
		(((uint64_t)(uint16_t)(int16_t)(color.r * 255.0f)) << 48)
		+ (((uint64_t)(uint16_t)(int16_t)(color.g * 255.0f)) << 32)
		+ (((uint64_t)(uint16_t)(int16_t)(color.b * 255.0f)) << 16)
		+ (uint64_t)(uint16_t)(int16_t)(color.a * 255.0f);
	
	decString result;
	int shift = 60;
	int i;
	
	result.Set('0', 16);
	
	for(i=0; i<16; i++){
		const int nibble = (bincolor >> shift) & 0xf;
		
		if(nibble < 10){
			result[i] = (char)('0' + nibble);
			
		}else{
			result[i] = (char)('a' + (nibble - 10));
		}
	}
	
	return result;
}

void deoglSkinChannel::pBuildCacheID(){
	// structure of cache id:
	//   <compressed> <mipmap-filter> <uniform-color> ';' <source1> { ';' <source2> }
	// 
	// <compressed> defines if the channel texture is stored compressed:
	//   'c' => channel data is compressed
	//   '-' => channel data is not compressed
	// 
	// <mipmap-filter> defines the filtering used for mip map data if present:
	//   'b' => mip maps are present and filtered using box-filtering
	//   'n' => mip maps are present and filtered using 3d-normal respecting filter
	//   'm' => mip maps are present and filtered uisng maximum filter
	//   'M' => mip maps are present and filtered uisng minimum filter
	//   '-' => mip maps are not present
	// 
	// <uniform-colors> defines the uniform color as 64-bit hex value in the form RGBA.
	// 
	// <source1> and <source2> define the data source. the first source is mandatory whereas
	// the second source is optional and specified only if the channel is a combined channel.
	// the source definition has the following formats depending on the type:
	// 
	// static color value from either a value or color type texture property.
	//   <empty-string>
	// 
	// static image data from an image type texture property. the path is deImage::GetFilename()
	// without virtual file system. this might result in a problem later on but the virtual file
	// system can not be stored since it has no identifier of any kind (at least not yet).
	// to avoid problems caching is disabled if the virtual file system used to load the image
	// is not the same as the one used by the game engine. this solution is not optimal but safe.
	//   'I' <path-image-file>
	// 
	// data from a constructed type property. the construction definition is hashed into a 32-bit
	// hash value using CRC32 stored as hex value. with CRC32 there exists the possibility for two
	// construction definitions to map to the same hash value but the chance is mathematically 0.
	// even if this case would ever happen the result is only discarding the cache file since the
	// precise configuration data stored in the cache file header would not match. this causes
	// only a slower loading time but no breaking of any kind.
	//   'C' <construction-definition-hash>
	// 
	// the construction definition is complex binary string and organized as tree. at the root
	// is the node group used in the texture property. each node is stored the following way:
	//   <node-type> <node-parameters>
	// 
	// the node type is the following:
	//   0 => end of list marker
	//   1 => group node
	//   2 => image node
	//   3 => shape node
	//   4 => text node
	// 
	// <node-parameters> contains all node parameters stored in binary form. for groups the child
	// nodes are stored one after the other. the end of the list is marker by a end of list marker
	// 0-byte. no length is stored since the construction is not supposed to be parsed from the
	// string. as an added protection the node definition binary string is also stored in the
	// cache file header to ensure no crc collision happened.
	// 
	// for all path used (images, fonts and alike) the same rule applies as for the image texture
	// property. if the resource does not belong to the virtual file system used by the game
	// engine the cache id is not used to avoid problems
	
	if(!pCanBeCached){
		return;
	}
	if(pComponentCount < 1){
		return;
	}
	if(AllComponentsStatic() /*HasStaticComponent()*/){
		return;
	}
	if(pPixelBufferMipMap->GetPixelBufferCount() == 0){
		return;
	}
	
	// for images update the cache id sources if present
	if(pDelayedCombineImage1){
		const decString &filename = pDelayedCombineImage1->GetImage().GetFilename();
		if(filename.IsEmpty()){
			pCanBeCached = false; // this one is problematic. dont do it
			return;
		}
		
		if(pDelayedCombineImage1->GetImage().GetVirtualFileSystem() !=
		pRenderThread.GetOgl().GetGameEngine()->GetVirtualFileSystem()){
			pCanBeCached = false;
			return;
		}
		
		if(!pDelayedCombineImage1->GetImage().GetVirtualFileSystem()->CanReadFile(
		decPath::CreatePathUnix(filename))){
			pCanBeCached = false;
			return;
		}
		
		pCacheIDSource1 = "I";
		pCacheIDSource1.Append(filename);
	}
	
	if(pDelayedCombineImage2){
		const decString &filename = pDelayedCombineImage2->GetImage().GetFilename();
		if(filename.IsEmpty()){
			pCanBeCached = false; // this one is problematic. dont do it
			return;
		}
		
		if(pDelayedCombineImage2->GetImage().GetVirtualFileSystem() !=
		pRenderThread.GetOgl().GetGameEngine()->GetVirtualFileSystem()){
			pCanBeCached = false;
			return;
		}
		
		if(!pDelayedCombineImage2->GetImage().GetVirtualFileSystem()->CanReadFile(
		decPath::CreatePathUnix(filename))){
			pCanBeCached = false;
			return;
		}
		
		pCacheIDSource2 = "I";
		pCacheIDSource2.Append(filename);
	}
	
	// for constructed update the cache id sources if present
	if(pCacheConstrDefSource1){
		pCacheIDSource1 = "C";
		pCacheIDSource1.Append(pCRC32Data(*pCacheConstrDefSource1));
		
		/*
		pRenderThread.GetOgl().LogInfoFormat("CACHE ID: %s", pCacheIDSource1.GetString());
		decString hex;
		const char * const data = pCacheConstrDefSource1->GetPointer();
		const int size = pCacheConstrDefSource1->GetLength();
		for(int i=0; i<size; i++){
			hex.AppendFormat("%02x ", data[i]);
		}
		pRenderThread.GetOgl().LogInfoFormat("DEF STR1: %s", hex.GetString());
		*/
	}
	
	if(pCacheConstrDefSource2){
		pCacheIDSource2 = "C";
		pCacheIDSource2.Append(pCRC32Data(*pCacheConstrDefSource2));
	}
	
	// add compression and mip map filtering parameter
	bool compressed = false;
	
	if(pTexture){
		compressed = pTexture->GetFormat()->GetIsCompressed();
		
	}else if(pCubeMap){
		compressed = pCubeMap->GetFormat()->GetIsCompressed();
		
	}else if(pArrayTexture){
		compressed = pArrayTexture->GetFormat()->GetIsCompressed();
		
	}else{
		// should not happen
		pCacheID.Empty();
		return;
	}
	
	pCacheID.AppendCharacter(compressed ? 'c' : '-');
	
	if(pPixelBufferMipMap->GetPixelBufferCount() == 1){
		pCacheID.AppendCharacter('-');
		
	}else{
		switch(pType){
		case ectAbsorption:
		case ectColor:
		case ectColorTintMask:
		case ectTransparency:
		case ectColorOmnidirCube:
		case ectColorOmnidirEquirect:
		case ectEmissivity:
		case ectEnvironmentMap:
		case ectEnvironmentRoom:
		case ectEnvironmentRoomEmissivity:
		case ectEnvironmentRoomMask:
		case ectHeight:
		case ectReflectivity:
		case ectRoughness:
		case ectRefractDistort:
		case ectRimEmissivity:
		case ectNonPbrAlbedo:
		case ectNonPbrMetalness:
			pCacheID.AppendCharacter('b');
			break;
			
		case ectSolidity:
			if(pSolidityFilterPriority < 0.35f){
				pCacheID.AppendCharacter('M'); // minimum filter
				
			}else if(pSolidityFilterPriority > 0.65f){
				pCacheID.AppendCharacter('m'); // maximum filter
				
			}else{
				pCacheID.AppendCharacter('b'); // box filter (averaging)
			}
			break;
			
		case ectNormal:
			pCacheID.AppendCharacter('n');
			break;
			
		case ectAO:
			pCacheID.AppendCharacter('m');
			break;
		};
	}
	
	// assemble the cache id
	pCacheID.Append(pColor2CacheBinary(pUniformColor));
	
	pCacheID.AppendCharacter(';');
	pCacheID.Append(pCacheIDSource1);
	
	if(!pCacheIDSource2.IsEmpty()){
		pCacheID.AppendCharacter(';');
		pCacheID.Append(pCacheIDSource2);
	}
	
	/*
	printf("CacheID '%s' (%p %p)(%p %p) \n", pCacheID.GetString(), pDelayedCombineImage1,
		pDelayedCombineImage2, pCacheConstrDefSource1, pCacheConstrDefSource2);
	*/
}

void deoglSkinChannel::pBuildCacheVerify(){
	// structure of cache verify:
	//   <source1> { <source2> }
	// 
	// <source1> and <source2> define the verify source. the first source is mandatory whereas
	// the second source is optional and specified only if the channel is a combined channel.
	// the source verify has the following formats depending on the type:
	// 
	// static color value from either a value or color type texture property.
	//   <empty-data>
	// 
	// static image data from an image type texture property. the path is deImage::GetFilename()
	// without virtual file system. this might result in a problem later on but the virtual file
	// system can not be stored since it has no identifier of any kind (at least not yet).
	// to avoid problems caching is disabled if the virtual file system used to load the image
	// is not the same as the one used by the game engine. this solution is not optimal but safe.
	//   <image-file access-time as UInt32>
	// 
	// data from a constructed type property. same as for cache id but in binary form without
	// crc32 applied.
	//   <construction-definition UInt8[]>
	
	if(pCacheID.IsEmpty() || !pCanBeCached){
		return;
	}
	
	if(!pCacheVerify){
		pCacheVerify = new decMemoryFile("");
	}
	
	decMemoryFileWriter *writer = NULL;
	
	try{
		writer = new decMemoryFileWriter(pCacheVerify, false);
		
		// source 1 verify
		if(pDelayedCombineImage1){
			const decString &filename = pDelayedCombineImage1->GetImage().GetFilename();
			if(filename.IsEmpty()){
				DETHROW(deeInvalidAction); // this one is problematic. dont do it
			}
			
			const decPath path(decPath::CreatePathUnix(filename));
			writer->WriteUInt((uint32_t)pDelayedCombineImage1->GetImage().
				GetVirtualFileSystem()->GetFileModificationTime(path));
			
		}else if(pCacheConstrVerifySource1){
			if(pCacheConstrDefSource1->GetLength() > 0){
				writer->Write(pCacheConstrDefSource1->GetPointer(), pCacheConstrDefSource1->GetLength());
			}
			if(pCacheConstrVerifySource1->GetLength() > 0){
				writer->Write(pCacheConstrVerifySource1->GetPointer(), pCacheConstrVerifySource1->GetLength());
			}
		}
		
		// source 2 verify
		if(pDelayedCombineImage2){
			const decString &filename = pDelayedCombineImage2->GetImage().GetFilename();
			if(filename.IsEmpty()){
				DETHROW(deeInvalidParam); // this one is problematic. dont do it
			}
			
			const decPath path(decPath::CreatePathUnix(filename));
			writer->WriteUInt((uint32_t)pDelayedCombineImage2->GetImage().
				GetVirtualFileSystem()->GetFileModificationTime(path));
			
		}else if(pCacheConstrVerifySource2){
			if(pCacheConstrDefSource2->GetLength() > 0){
				writer->Write(pCacheConstrDefSource2->GetPointer(), pCacheConstrDefSource2->GetLength());
			}
			if(pCacheConstrVerifySource2->GetLength() > 0){
				writer->Write(pCacheConstrVerifySource2->GetPointer(), pCacheConstrVerifySource2->GetLength());
			}
		}
		
		writer->FreeReference();
		
	}catch(const deException &){
		if(writer){
			writer->FreeReference();
		}
		throw;
	}
	
	/*
	printf("Verify %d '%s'\n", pCacheVerify->GetLength(), pCacheID.GetString());
	if(pCacheVerify->GetLength() > 8){
		printf("  ");
		for(int i=0; i<pCacheVerify->GetLength(); i++){
			printf("%02x ", (uint8_t)pCacheVerify->GetPointer()[i]);
		}
		printf("\n");
	}
	*/
}



void deoglSkinChannel::pBuildProperty(deSkinProperty &property){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects.
	//      in particular calling AddReference on render objects can lead to ugly bugs
	
	const deoglSkinPropertyMap::ePropertyTypes propertyType =
		deoglSkinPropertyMap::GetTypeFor(property.GetType());
	
	if(!pIsPropertyValidForType(propertyType)){
		return;
	}
	
	deSkinPropertyVisitorIdentify identify;
	property.Visit(identify);
	
	if(identify.IsImage()){
		pBuildPropertyImage(propertyType, identify.CastToImage());
		
	}else if(identify.IsConstructed()){
		pBuildPropertyConstructed(propertyType, identify.CastToConstructed());
	}
}

void deoglSkinChannel::pBuildPropertyImage(deoglSkinPropertyMap::ePropertyTypes propertyType,
const deSkinPropertyImage &property){
	const deImage * const image = property.GetImage();
	if(!image){
		return;
	}
	
	int i;
	
	switch(propertyType){
	case deoglSkinPropertyMap::eptColor:
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
	case deoglSkinPropertyMap::eptTransparency:
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptSolidity:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptHeight:
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptRefractionDistort:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptRoughness:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
	case deoglSkinPropertyMap::eptAbsorption:
	case deoglSkinPropertyMap::eptRimEmissivity:
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		if(image->GetComponentCount() == 1){
			pWriteImageToPixelBuffer(*image, 0, 100, 100, 100);
			pWriteImageToPixelBuffer(*image, 1, 100, 100, 100);
			pWriteImageToPixelBuffer(*image, 2, 100, 100, 100);
			
		}else{
			pWriteImageToPixelBuffer(*image, 0, 1, 2, 3);
		}
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
		if(image->GetDepth() == 6){
			if(image->GetComponentCount() == 1){
				for(i=0; i<6; i++){
					pWriteImageToPixelBuffer(*image, i, i, 0, 100, 100, 100);
					pWriteImageToPixelBuffer(*image, i, i, 1, 100, 100, 100);
					pWriteImageToPixelBuffer(*image, i, i, 2, 100, 100, 100);
				}
				
			}else{
				for(i=0; i<6; i++){
					pWriteImageToPixelBuffer(*image, i, i, 0, 1, 2, 3);
				}
			}
		}
		break;
		
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
		if(image->GetDepth() == 6){
			pWriteImageToPixelBuffer(*image, 0, 0, 0, 1, 2, 3); // positive x
			pWriteImageToPixelBuffer(*image, 1, 1, 0, 1, 2, 3); // negative x
			pWriteImageToPixelBuffer(*image, 2, 2, 0, 1, 2, 3); // positive y
			pWriteImageToPixelBuffer(*image, 3, 3, 0, 1, 2, 3); // negative y
			pWriteImageToPixelBuffer(*image, 4, 4, 0, 1, 2, 3); // positive z
			pWriteImageToPixelBuffer(*image, 5, 5, 0, 1, 2, 3); // negative z
		}
		break;
		
	default:
		break;
	}
}

void deoglSkinChannel::pBuildPropertyConstructed(deoglSkinPropertyMap::ePropertyTypes propertyType,
const deSkinPropertyConstructed &property){
	int targetRed = 100;
	int targetGreen = 100;
	int targetBlue = 100;
	int targetAlpha = 100;
	
	switch(propertyType){
	case deoglSkinPropertyMap::eptColorTintMask:
	case deoglSkinPropertyMap::eptSolidity:
	case deoglSkinPropertyMap::eptHeight:
	case deoglSkinPropertyMap::eptAmbientOcclusion:
	case deoglSkinPropertyMap::eptEnvironmentRoomMask:
	case deoglSkinPropertyMap::eptNonPbrMetalness:
		targetRed = 0;
		break;
		
	case deoglSkinPropertyMap::eptRefractionDistort:
		targetRed = 0;
		targetGreen = 1;
		break;
		
	case deoglSkinPropertyMap::eptColorOmnidir:
	case deoglSkinPropertyMap::eptColorOmnidirEquirect:
	case deoglSkinPropertyMap::eptNormal:
	case deoglSkinPropertyMap::eptEmissivity:
	case deoglSkinPropertyMap::eptReflectivity:
	case deoglSkinPropertyMap::eptEnvironmentMap:
	case deoglSkinPropertyMap::eptEnvironmentRoom:
	case deoglSkinPropertyMap::eptEnvironmentRoomEmissivity:
	case deoglSkinPropertyMap::eptAbsorption:
	case deoglSkinPropertyMap::eptRimEmissivity:
	case deoglSkinPropertyMap::eptNonPbrAlbedo:
		targetRed = 0;
		targetGreen = 1;
		targetBlue = 2;
		break;
		
	case deoglSkinPropertyMap::eptColor:
		targetRed = 0;
		targetGreen = 1;
		targetBlue = 2;
		targetAlpha = 3;
		break;
		
	case deoglSkinPropertyMap::eptTransparency:
		//targetRed = 3; // if combined with color
		targetRed = 0;
		break;
		
	case deoglSkinPropertyMap::eptRoughness:
		//targetRed = 3;
		targetRed = 0;
		break;
		
	default:
		break;
	}
	
	if(targetRed == 100 && targetGreen == 100 && targetBlue == 100 && targetAlpha == 100){
		return;
	}
	
	deoglSCBuildConstructed visitor(*this);
	if(!visitor.BuildFromProperty(property, targetRed, targetGreen, targetBlue, targetAlpha)){
		return;
	}
}

void deoglSkinChannel::pWriteImageToPixelBuffer(const deImage &image,
int targetRed, int targetGreen, int targetBlue, int targetAlpha){
	pWriteImageToPixelBuffer(image, 0, 0, targetRed, targetGreen, targetBlue, targetAlpha);
}

void deoglSkinChannel::pWriteImageToPixelBuffer(const deImage &image,
int srcLayer, int destLayer, int targetRed, int targetGreen, int targetBlue, int targetAlpha){
	const int targets[4] = {targetRed, targetGreen, targetBlue, targetAlpha};
	const float factor2 = 1.0f / 65535.0f;
	const float factor1 = 1.0f / 255.0f;
	int i, x, y;
	
	// checks
	const int componentCount = image.GetComponentCount();
	const int bitCount = image.GetBitCount();
	
	if(!pPixelBufferMipMap){
		return;
	}
	
	if(image.GetWidth() > pSize.x){
		return;
	}
	if(image.GetHeight() > pSize.y){
		return;
	}
	
	if(srcLayer < 0 || srcLayer >= image.GetDepth()){
		DETHROW(deeInvalidParam);
	}
	if(destLayer < 0 || destLayer >= pSize.z){
		DETHROW(deeInvalidParam);
	}
	
	// get the right pointer for the image data
	const int srcLayerSize = image.GetWidth() * image.GetHeight() * componentCount;
	const unsigned char *srcData8 = NULL;
	const unsigned short *srcData16 = NULL;
	const float *srcData32 = NULL;
	const GLubyte *srcDataPb8 = NULL;
	const GLfloat *srcDataPb32 = NULL;
	
	deoglPixelBuffer * const pixelBuffer = ((deoglImage*)image.GetPeerGraphic())->GetPixelBuffer();
	
	if(pixelBuffer){
		// image skin pixel buffer is used for memory optimization
		
		switch(pixelBuffer->GetFormat()){
		case deoglPixelBuffer::epfByte1:
		case deoglPixelBuffer::epfByte2:
		case deoglPixelBuffer::epfByte3:
		case deoglPixelBuffer::epfByte4:
			srcDataPb8 = (GLubyte*)pixelBuffer->GetPointer() + (srcLayerSize * srcLayer);
			break;
			
		case deoglPixelBuffer::epfFloat1:
		case deoglPixelBuffer::epfFloat2:
		case deoglPixelBuffer::epfFloat3:
		case deoglPixelBuffer::epfFloat4:
			srcDataPb32 = (GLfloat*)pixelBuffer->GetPointer() + (srcLayerSize * srcLayer);
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		
	}else if(image.GetData()){
		// direct memory access is used for regular mode and as fallback
		
		if(bitCount == 8){
			srcData8 = (unsigned char *)image.GetData() + (srcLayerSize * srcLayer);
			
		}else if(bitCount == 16){
			srcData16 = (unsigned short *)image.GetData() + (srcLayerSize * srcLayer);
			
		}else{
			srcData32 = (float*)image.GetData() + (srcLayerSize * srcLayer);
		}
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	// copy the pixels to the right place
	deoglPixelBuffer &pixbuf = pPixelBufferMipMap->GetPixelBuffer(0);
	
	if(pFloatFormat){
		// TODO if srcDataPb32 is not NULL, component count matches and targets[] in range
		//      are (0,1,2,3) a direct memory copy can be used. after the combined textures
		//      are removed this should always be the case
		
		GLfloat *destData = (GLfloat*)pixbuf.GetPointer() + (pSize.x * pSize.y * pComponentCount * destLayer);
		GLfloat pixels[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // silence -Werror=maybe-uninitialized
		GLfloat *destRow;
		
		for(y=0; y<pSize.y; y++){
			destRow = destData + pComponentCount * pSize.x * y;
			
			for(x=0; x<pSize.x; x++){
				if(srcDataPb8){
					for(i=0; i<componentCount; i++){
						pixels[i] = (GLfloat)(srcDataPb8[i] * factor1);
					}
					srcDataPb8 += componentCount;
					
				}else if(srcDataPb32) {
					for(i=0; i<componentCount; i++){
						pixels[i] = srcDataPb32[i];
					}
					srcDataPb32 += componentCount;
					
				}else if(srcData8){
					for(i=0; i<componentCount; i++){
						pixels[i] = (GLfloat)(srcData8[i] * factor1);
					}
					srcData8 += componentCount;
					
				}else if(srcData16){
					for(i=0; i<componentCount; i++){
						pixels[i] = (GLfloat)(srcData16[i] * factor2);
					}
					srcData16 += componentCount;
					
				}else{
					for(i=0; i<componentCount; i++){
						pixels[i] = (GLfloat)srcData32[i];
					}
					srcData32 += componentCount;
				}
				
				for(i=0; i<pComponentCount; i++){
					if(targets[i] < pComponentCount){
						destRow[targets[i]] = pixels[i];
					}
				}
				destRow += pComponentCount;
			}
		}
		
	}else{
		// TODO if srcDataPb8 is not NULL, component count matches and targets[] in range
		//      are (0,1,2,3) a direct memory copy can be used. after the combined textures
		//      are removed this should always be the case
		
		GLubyte *destData = (GLubyte*)pixbuf.GetPointer() + (pSize.x * pSize.y * pComponentCount * destLayer);
		GLubyte pixels[4] = {0, 0, 0, 0}; // silence -Werror=maybe-uninitialized
		GLubyte *destRow;
		
		for(y=0; y<pSize.y; y++){
			destRow = destData + pComponentCount * pSize.x * y;
			
			for(x=0; x<pSize.x; x++){
				if(srcDataPb8){
					for(i=0; i<componentCount; i++){
						pixels[i] = srcDataPb8[i];
					}
					srcDataPb8 += componentCount;
					
				}else if(srcDataPb32){
					for(i=0; i<componentCount; i++){
						pixels[i] = (GLubyte)(srcDataPb32[i] * 255.0f);
					}
					srcDataPb32 += componentCount;
					
				}else if(srcData8){
					for(i=0; i<componentCount; i++){
						pixels[i] = (GLubyte)srcData8[i];
					}
					srcData8 += componentCount;
					
				}else if(srcData16){
					for(i=0; i<componentCount; i++){
						pixels[i] = (GLubyte)(srcData16[i] >> 8);
					}
					srcData16 += componentCount;
					
				}else{
					for(i=0; i<componentCount; i++){
						pixels[i] = (GLubyte)(srcData32[i] * 255.0f);
					}
					srcData32 += componentCount;
				}
				
				for(i=0; i<pComponentCount; i++){
					if(targets[i] < pComponentCount){
						destRow[targets[i]] = pixels[i];
					}
				}
				destRow += pComponentCount;
			}
		}
	}
}

void deoglSkinChannel::pFillWithUniformColor(){
	deoglPixelBuffer &pixbuf = pPixelBufferMipMap->GetPixelBuffer(0);
	const int pixelCount = pSize.x * pSize.y * pSize.z;
	int i;
	
	if(pFloatFormat){
		const GLfloat oglR = (GLfloat)pUniformColor.r;
		const GLfloat oglG = (GLfloat)pUniformColor.g;
		const GLfloat oglB = (GLfloat)pUniformColor.b;
		const GLfloat oglA = (GLfloat)pUniformColor.a;
		
		if(pComponentCount == 1){
			deoglPixelBuffer::sFloat1 * const dest = pixbuf.GetPointerFloat1();
			for(i=0; i<pixelCount; i++){
				if(pUniformColorMask[0]){
					dest[i].r = oglR;
				}
			}
			
		}else if(pComponentCount == 2){
			deoglPixelBuffer::sFloat2 * const dest = pixbuf.GetPointerFloat2();
			for(i=0; i<pixelCount; i++){
				if(pUniformColorMask[0]){
					dest[i].r = oglR;
				}
				if(pUniformColorMask[1]){
					dest[i].g = oglG;
				}
			}
			
		}else if(pComponentCount == 3){
			deoglPixelBuffer::sFloat3 * const dest = pixbuf.GetPointerFloat3();
			for(i=0; i<pixelCount; i++){
				if(pUniformColorMask[0]){
					dest[i].r = oglR;
				}
				if(pUniformColorMask[1]){
					dest[i].g = oglG;
				}
				if(pUniformColorMask[2]){
					dest[i].b = oglB;
				}
			}
			
		}else{
			deoglPixelBuffer::sFloat4 * const dest = pixbuf.GetPointerFloat4();
			for(i=0; i<pixelCount; i++){
				if(pUniformColorMask[0]){
					dest[i].r = oglR;
				}
				if(pUniformColorMask[1]){
					dest[i].g = oglG;
				}
				if(pUniformColorMask[2]){
					dest[i].b = oglB;
				}
				if(pUniformColorMask[3]){
					dest[i].a = oglA;
				}
			}
		}
		
	}else{
		const GLubyte oglR = (GLubyte)(pUniformColor.r * 255.0f);
		const GLubyte oglG = (GLubyte)(pUniformColor.g * 255.0f);
		const GLubyte oglB = (GLubyte)(pUniformColor.b * 255.0f);
		const GLubyte oglA = (GLubyte)(pUniformColor.a * 255.0f);
		
		if(pComponentCount == 1){
			deoglPixelBuffer::sByte1 * const dest = pixbuf.GetPointerByte1();
			for(i=0; i<pixelCount; i++){
				if(pUniformColorMask[0]){
					dest[i].r = oglR;
				}
			}
			
		}else if(pComponentCount == 2){
			deoglPixelBuffer::sByte2 * const dest = pixbuf.GetPointerByte2();
			for(i=0; i<pixelCount; i++){
				if(pUniformColorMask[0]){
					dest[i].r = oglR;
				}
				if(pUniformColorMask[1]){
					dest[i].g = oglG;
				}
			}
			
		}else if(pComponentCount == 3){
			deoglPixelBuffer::sByte3 * const dest = pixbuf.GetPointerByte3();
			for(i=0; i<pixelCount; i++){
				if(pUniformColorMask[0]){
					dest[i].r = oglR;
				}
				if(pUniformColorMask[1]){
					dest[i].g = oglG;
				}
				if(pUniformColorMask[2]){
					dest[i].b = oglB;
				}
			}
			
		}else{
			deoglPixelBuffer::sByte4 * const dest = pixbuf.GetPointerByte4();
			for(i=0; i<pixelCount; i++){
				if(pUniformColorMask[0]){
					dest[i].r = oglR;
				}
				if(pUniformColorMask[1]){
					dest[i].g = oglG;
				}
				if(pUniformColorMask[2]){
					dest[i].b = oglB;
				}
				if(pUniformColorMask[3]){
					dest[i].a = oglA;
				}
			}
		}
	}
}
