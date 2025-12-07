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

#include "deoglRTDefaultTextures.h"
#include "deoglRenderThread.h"
#include "deoglRTLogger.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTDefaultTextures
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTDefaultTextures::deoglRTDefaultTextures(deoglRenderThread &renderThread) :
pColor(NULL),
pTransparency(NULL),
pNormal(NULL),
pHeight(NULL),
pReflectivity(NULL),
pRoughness(NULL),
pEmissivity(NULL),
pAO(NULL),
pSolidity(NULL),
pEnvRoomMask(NULL),
pNonPbrMetalness(nullptr),
pShadowMap(NULL),
pShadowMapInverseDepth(NULL),
pShadowMapColor(NULL),
pShadowCube(NULL),
pShadowCubeInverseDepth(nullptr),
pShadowCubeColor(NULL),

pWeights(NULL),
pMaskOpaque(NULL),
pNoise2D(NULL),
pEnvMap(NULL),

pColorArray(NULL),
pTransparencyArray(NULL),
pNormalArray(NULL),
pHeightArray(NULL),
pReflectivityArray(NULL),
pRoughnessArray(NULL),
pEmissivityArray(NULL),
pAOArray(NULL),
pSolidityArray(NULL),
pEnvRoomMaskArray(NULL),
pNonPbrMetalnessArray(nullptr)
{
	try{
		pCreateDefaultTextures(renderThread);
		pCreateDefaultTexturesArray(renderThread);
		pCreateTextureMaskOpaque(renderThread);
		pCreateTextureNoise2D(renderThread);
		//pCreateWeightsTexture( renderThread );
		pCreateShadowTextures(renderThread);
		
	}catch(const deException &){
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
	if(pNonPbrMetalnessArray){
		delete pNonPbrMetalnessArray;
	}
	if(pEnvRoomMaskArray){
		delete pEnvRoomMaskArray;
	}
	if(pSolidityArray){
		delete pSolidityArray;
	}
	if(pAOArray){
		delete pAOArray;
	}
	if(pEmissivityArray){
		delete pEmissivityArray;
	}
	if(pRoughnessArray){
		delete pRoughnessArray;
	}
	if(pReflectivityArray){
		delete pReflectivityArray;
	}
	if(pHeightArray){
		delete pHeightArray;
	}
	if(pNormalArray){
		delete pNormalArray;
	}
	if(pTransparencyArray){
		delete pTransparencyArray;
	}
	if(pColorArray){
		delete pColorArray;
	}
	
	if(pEnvMap){
		delete pEnvMap;
	}
	if(pNoise2D){
		delete pNoise2D;
	}
	if(pMaskOpaque){
		delete pMaskOpaque;
	}
	if(pWeights){
		delete pWeights;
	}
	
	if(pShadowCubeColor){
		delete pShadowCubeColor;
	}
	if(pShadowCube){
		delete pShadowCube;
	}
	if(pShadowCubeInverseDepth){
		delete pShadowCubeInverseDepth;
	}
	if(pShadowMapColor){
		delete pShadowMapColor;
	}
	if(pShadowMap){
		delete pShadowMap;
	}
	if(pShadowMapInverseDepth){
		delete pShadowMapInverseDepth;
	}
	if(pNonPbrMetalness){
		delete pNonPbrMetalness;
	}
	if(pEnvRoomMask){
		delete pEnvRoomMask;
	}
	if(pSolidity){
		delete pSolidity;
	}
	if(pAO){
		delete pAO;
	}
	if(pEmissivity){
		delete pEmissivity;
	}
	if(pRoughness){
		delete pRoughness;
	}
	if(pReflectivity){
		delete pReflectivity;
	}
	if(pHeight){
		delete pHeight;
	}
	if(pNormal){
		delete pNormal;
	}
	if(pTransparency){
		delete pTransparency;
	}
	if(pColor){
		delete pColor;
	}
}

void deoglRTDefaultTextures::pCreateDefaultTextures(deoglRenderThread &renderThread){
	// diffuse channel texture: color.rgb, alpha
	const deoglPixelBuffer::Ref pbByte4(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfByte4, 1, 1, 1)));
	pbByte4->SetToIntColor(0, 0, 0, 255);
	pColor = new deoglTexture(renderThread);
	pColor->SetSize(1, 1);
	pColor->SetMapingFormat(4, false, false);
	pColor->SetPixels(pbByte4);
	
	// transparency channel texture: alpha, n/a, n/a, n/a
	pbByte4->SetToIntColor(255, 0, 0, 0);
	pTransparency = new deoglTexture(renderThread);
	pTransparency->SetSize(1, 1);
	pTransparency->SetMapingFormat(4, false, false);
	pTransparency->SetPixels(pbByte4);
	
	// normal channel texture: normal.rgb, normalDeviation
	pbByte4->SetToIntColor(128, 128, 255, 0);
	pNormal = new deoglTexture(renderThread);
	pNormal->SetSize(1, 1);
	pNormal->SetMapingFormat(4, false, false);
	pNormal->SetPixels(pbByte4);
	
	// height channel texture: height, cone, n/a, n/a
	const deoglPixelBuffer::Ref pbByte2(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfByte2, 1, 1, 1)));
	pbByte2->SetToIntColor(255, 255, 0, 0);
	pHeight = new deoglTexture(renderThread);
	pHeight->SetSize(1, 1);
	pHeight->SetMapingFormat(2, false, false);
	pHeight->SetPixels(pbByte2);
	
	// reflectivity channel texture: reflectivity.rgb
	pbByte4->SetToIntColor(0, 0, 0, 0);
	pReflectivity = new deoglTexture(renderThread);
	pReflectivity->SetSize(1, 1);
	pReflectivity->SetMapingFormat(4, false, false);
	pReflectivity->SetPixels(pbByte4);
	
	// roughness channel texture: roughness
	pbByte4->SetToIntColor(255, 0, 0, 0);
	pRoughness = new deoglTexture(renderThread);
	pRoughness->SetSize(1, 1);
	pRoughness->SetMapingFormat(4, false, false);
	pRoughness->SetPixels(pbByte4);
	
	// emissivity channel texture: emissivity.rgb, n/a
	pbByte4->SetToIntColor(0, 0, 0, 0);
	pEmissivity = new deoglTexture(renderThread);
	pEmissivity->SetSize(1, 1);
	pEmissivity->SetMapingFormat(4, false, false);
	pEmissivity->SetPixels(pbByte4);
	
	// ambient occlusion channel texture: ambinetOcclusion, n/a, n/a, n/a
	pbByte4->SetToIntColor(255, 255, 255, 255);
	pAO = new deoglTexture(renderThread);
	pAO->SetSize(1, 1);
	pAO->SetMapingFormat(4, false, false);
	pAO->SetPixels(pbByte4);
	
	// solidity channel texture: solidity, n/a, n/a, n/a
	pbByte4->SetToIntColor(255, 255, 255, 255);
	pSolidity = new deoglTexture(renderThread);
	pSolidity->SetSize(1, 1);
	pSolidity->SetMapingFormat(4, false, false);
	pSolidity->SetPixels(pbByte4);
	
	// environment room mask channel texture: mask, n/a, n/a, n/a
	pbByte4->SetToIntColor(255, 0, 0, 0);
	pEnvRoomMask = new deoglTexture(renderThread);
	pEnvRoomMask->SetSize(1, 1);
	pEnvRoomMask->SetMapingFormat(4, false, false);
	pEnvRoomMask->SetPixels(pbByte4);
	
	// environment map: 50% gray for all pixels
	const deoglPixelBuffer::Ref pbFloat3(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfFloat3, 1, 1, 6)));
	pbFloat3->SetToFloatColor(0.5f, 0.5f, 0.5f, 1.0f);
	pEnvMap = new deoglCubeMap(renderThread);
	pEnvMap->SetSize(1);
	pEnvMap->SetMapingFormat(3, true, false);
	pEnvMap->SetPixels(pbFloat3);
	
	// non-pbr metalness: metalness, n/a, n/a, n/a
	pbByte4->SetToIntColor(0, 0, 0, 0);
	pNonPbrMetalness = new deoglTexture(renderThread);
	pNonPbrMetalness->SetSize(1, 1);
	pNonPbrMetalness->SetMapingFormat(4, false, false);
	pNonPbrMetalness->SetPixels(pbByte4);
}

void deoglRTDefaultTextures::pCreateDefaultTexturesArray(deoglRenderThread &renderThread){
	// diffuse channel texture: color.rgb, alpha
	const deoglPixelBuffer::Ref pbByte4(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfByte4, 1, 1, 1)));
	pbByte4->SetToIntColor(0, 0, 0, 255);
	pColorArray = new deoglArrayTexture(renderThread);
	pColorArray->SetSize(1, 1, 1);
	pColorArray->SetMapingFormat(4, false, false);
	pColorArray->SetPixels(pbByte4);
	
	// transparency channel texture: alpha, n/a, n/a, n/a
	pbByte4->SetToIntColor(255, 0, 0, 0);
	pTransparencyArray = new deoglArrayTexture(renderThread);
	pTransparencyArray->SetSize(1, 1, 1);
	pTransparencyArray->SetMapingFormat(4, false, false);
	pTransparencyArray->SetPixels(pbByte4);
	
	// normal channel texture: normal.rgb, normalDeviation
	pbByte4->SetToIntColor(128, 128, 255, 0);
	pNormalArray = new deoglArrayTexture(renderThread);
	pNormalArray->SetSize(1, 1, 1);
	pNormalArray->SetMapingFormat(4, false, false);
	pNormalArray->SetPixels(pbByte4);
	
	// height channel texture: height, cone, n/a, n/a
	const deoglPixelBuffer::Ref pbByte2(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfByte2, 1, 1, 1)));
	pbByte2->SetToIntColor(255, 255, 0, 0);
	pHeightArray = new deoglArrayTexture(renderThread);
	pHeightArray->SetSize(1, 1, 1);
	pHeightArray->SetMapingFormat(2, false, false);
	pHeightArray->SetPixels(pbByte2);
	
	// reflectivity channel texture: roughness
	pbByte4->SetToIntColor(0, 0, 0, 0);
	pReflectivityArray = new deoglArrayTexture(renderThread);
	pReflectivityArray->SetSize(1, 1, 1);
	pReflectivityArray->SetMapingFormat(4, false, false);
	pReflectivityArray->SetPixels(pbByte4);
	
	// roughness channel texture: roughness
	pbByte4->SetToIntColor(255, 0, 0, 0);
	pRoughnessArray = new deoglArrayTexture(renderThread);
	pRoughnessArray->SetSize(1, 1, 1);
	pRoughnessArray->SetMapingFormat(4, false, false);
	pRoughnessArray->SetPixels(pbByte4);
	
	// emissivity channel texture: emissivity.rgb, n/a
	pbByte4->SetToIntColor(0, 0, 0, 0);
	pEmissivityArray = new deoglArrayTexture(renderThread);
	pEmissivityArray->SetSize(1, 1, 1);
	pEmissivityArray->SetMapingFormat(4, false, false);
	pEmissivityArray->SetPixels(pbByte4);
	
	// ambient occlusion channel texture: ambinetOcclusion, n/a, n/a, n/a
	pbByte4->SetToIntColor(255, 255, 255, 255);
	pAOArray = new deoglArrayTexture(renderThread);
	pAOArray->SetSize(1, 1, 1);
	pAOArray->SetMapingFormat(4, false, false);
	pAOArray->SetPixels(pbByte4);
	
	// solidity channel texture: solidity, n/a, n/a, n/a
	pbByte4->SetToIntColor(255, 255, 255, 255);
	pSolidityArray = new deoglArrayTexture(renderThread);
	pSolidityArray->SetSize(1, 1, 1);
	pSolidityArray->SetMapingFormat(4, false, false);
	pSolidityArray->SetPixels(pbByte4);
	
	// environment room mask channel texture: mask, n/a, n/a, n/a
	pbByte4->SetToIntColor(255, 0, 0, 0);
	pEnvRoomMaskArray = new deoglArrayTexture(renderThread);
	pEnvRoomMaskArray->SetSize(1, 1, 1);
	pEnvRoomMaskArray->SetMapingFormat(4, false, false);
	pEnvRoomMaskArray->SetPixels(pbByte4);
	
	// non-pbr metalness: metalness, n/a, n/a, n/a
	pbByte4->SetToIntColor(0, 0, 0, 0);
	pNonPbrMetalnessArray = new deoglArrayTexture(renderThread);
	pNonPbrMetalnessArray->SetSize(1, 1, 1);
	pNonPbrMetalnessArray->SetMapingFormat(4, false, false);
	pNonPbrMetalnessArray->SetPixels(pbByte4);
}

void deoglRTDefaultTextures::pCreateWeightsTexture(deoglRenderThread &renderThread){
	pWeights = new deoglTexture(renderThread);
	pWeights->SetSize(128, 128);
	pWeights->SetMapingFormat(4, true, false);
	pWeights->CreateTexture();
}

void deoglRTDefaultTextures::pCreateTextureMaskOpaque(deoglRenderThread &renderThread){
	const deoglPixelBuffer::Ref pbByte1(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfByte1, 1, 1, 1)));
	
	pbByte1->SetToIntColor(255, 255, 255, 255);
	pMaskOpaque = new deoglTexture(renderThread);
	pMaskOpaque->SetSize(1, 1);
	pMaskOpaque->SetMapingFormat(1, false, false);
	pMaskOpaque->CreateTexture();
	pMaskOpaque->SetPixels(pbByte1);
}

#ifdef OS_UNIX
__attribute__((no_sanitize("signed-integer-overflow", "shift")))
#endif
static void deoglRTDefaultTextures_CreateNoiseData(deoglPixelBuffer &pixelBuffer, int size){
	// the noise code is moved into an own function so it can be qualified with a no_sanitize.
	// the problem is that this code uses integer overflow to create random noise data. the gcc
	// sanitizer does not like this overflow behavior hence it is move to silence the warning
	deoglPixelBuffer::sByte4 *destInt32 = pixelBuffer.GetPointerByte4();
	const int seed = 17342954;
	int p, x, y;
	
	for(p=0, y=0; y<size; y++){
		for(x=0; x<size; x++, p++){
			int n = x + y * 57 + seed;
			n = (n << 13) ^ n;
			const double rot = ((double)((n * (n * n * 15731 + 789221)
				+ 1376312589) & 0x7fffffff) / 1073741824.0);
			
			destInt32[p].r = (unsigned char)(sin(rot * PI) * 127.5 + 127.5);
			destInt32[p].g = (unsigned char)(cos(rot * PI) * 127.5 + 127.5);
			destInt32[p].b = 0;
			destInt32[p].a = 0;
		}
	}
}

void deoglRTDefaultTextures::pCreateTextureNoise2D(deoglRenderThread &renderThread){
	const int size = 32;
	const deoglPixelBuffer::Ref pbByte4(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfByte4, size, size, 1)));
	deoglRTDefaultTextures_CreateNoiseData(pbByte4, size);
	
	pNoise2D = new deoglTexture(renderThread);
	pNoise2D->SetSize(size, size);
	pNoise2D->SetMapingFormat(4, false, false);
	pNoise2D->CreateTexture();
	pNoise2D->SetPixels(pbByte4);
	
	/*
	int u, x, seedu = 17342954;
	int v, y, seedv = 28017626;
	int p, size = 32;
	deoglPixelBuffer pixelBuffer(deoglPixelBuffer::epfByte4, size, size);
	
	deoglPixelBuffer::sByte4 *destInt32 = pixelBuffer.GetPointerInt32();
	for(p=0, y=0; y<size; y++){
		for(x=0; x<size; x++, p++){
			u = x + y * 57 + seedu;
			u = (u << 13) ^ u;
			destInt32[p].r = (u * (u * u * 15731 + 789221) + 1376312589) >> 24;
			
			v = x + y * 57 + seedv;
			v = (v << 13) ^ v;
			destInt32[p].g = (v * (v * v * 15731 + 789221) + 1376312589) >> 24;
			
			destInt32[p].b = 0;
			destInt32[p].a = 0;
		}
	}
	
	pNoise2D = new deoglTexture(this);
	if(! pNoise2D) DETHROW(deeOutOfMemory);
	
	pNoise2D->SetMipMapped(false);
	pNoise2D->SetCompressed(false);
	pNoise2D->SetMapingFormat(8, 4, false);
	pNoise2D->SetSize(size, size);
	pNoise2D->CreateTexture();
	pNoise2D->SetPixels(pixelBuffer);
	*/
}

void deoglRTDefaultTextures::pCreateShadowTextures(deoglRenderThread &renderThread){
	// shadow map
	const deoglPixelBuffer::Ref bpDepth(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfDepth, 1, 1, 1)));
	bpDepth->SetToFloatColor(1.0f, 1.0f, 1.0f, 1.0f);
	pShadowMap = new deoglTexture(renderThread);
	pShadowMap->SetSize(1, 1);
	pShadowMap->SetDepthFormat(false, false);
	pShadowMap->SetPixels(bpDepth);
	
	bpDepth->SetToFloatColor(0.0f, 0.0f, 0.0f, 1.0f);
	pShadowMapInverseDepth = new deoglTexture(renderThread);
	pShadowMapInverseDepth->SetSize(1, 1);
	pShadowMapInverseDepth->SetDepthFormat(false, true);
	pShadowMapInverseDepth->SetPixels(bpDepth);
	
	const deoglPixelBuffer::Ref pbByte3(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfByte3, 1, 1, 1)));
	pbByte3->SetToIntColor(0, 0, 0, 255);
	pShadowMapColor = new deoglTexture(renderThread);
	pShadowMapColor->SetSize(1, 1);
	pShadowMapColor->SetMapingFormat(3, false, false);
	pShadowMapColor->SetPixels(pbByte3);
	
	// shadow cube
	pShadowCube = new deoglCubeMap(renderThread);
	pShadowCube->SetSize(1);
	pShadowCube->SetDepthFormat(false);
	if(pShadowCube->GetFormat()->GetIsStencil()){
		const deoglPixelBuffer::Ref pbDepthStencilCube(deoglPixelBuffer::Ref::New(
			new deoglPixelBuffer(deoglPixelBuffer::epfDepthStencil, 1, 1, 6)));
		pbDepthStencilCube->SetToDepthStencil(1.0f, 0);
		pShadowCube->SetPixels(pbDepthStencilCube);
		
	}else{
		const deoglPixelBuffer::Ref pbDepthCube(deoglPixelBuffer::Ref::New(
			new deoglPixelBuffer(deoglPixelBuffer::epfDepth, 1, 1, 6)));
		pbDepthCube->SetToFloatColor(1.0f, 1.0f, 1.0f, 1.0f);
		pShadowCube->SetPixels(pbDepthCube);
	}
	
	// shadow cube inverse depth
	const deoglPixelBuffer::Ref pbDepthCube(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfDepth, 1, 1, 6)));
	pbDepthCube->SetToFloatColor(0.0f, 0.0f, 0.0f, 0.0f);
	pShadowCubeInverseDepth = new deoglCubeMap(renderThread);
	pShadowCubeInverseDepth->SetSize(1);
	pShadowCubeInverseDepth->SetDepthFormat(true);
	pShadowCubeInverseDepth->SetPixels(pbDepthCube);
	
	const deoglPixelBuffer::Ref pbByte3Cube(deoglPixelBuffer::Ref::New(
		new deoglPixelBuffer(deoglPixelBuffer::epfByte3, 1, 1, 6)));
	pbByte3Cube->SetToIntColor(0, 0, 0, 255);
	pShadowCubeColor = new deoglCubeMap(renderThread);
	pShadowCubeColor->SetSize(1);
	pShadowCubeColor->SetMapingFormat(3, false, false);
	pShadowCubeColor->SetPixels(pbByte3Cube);
}
