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

#include "deoglRSky.h"
#include "deoglRSkyLayer.h"
#include "deoglRSkyControllerTarget.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkin.h"
#include "../skin/deoglSkinTexture.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/sky/deSkyLayerBody.h>



// Class deoglRSkyLayer
////////////////////////

// Constructor, destructor
////////////////////////////

deoglRSkyLayer::deoglRSkyLayer(const deSkyLayer &layer) :
pLayerType(eltUnknown),

pBodies(NULL),
pBodyCount(0),

pOffset(layer.GetOffset()),
pOrientation(layer.GetOrientation()),

pColor(layer.GetColor()),
pIntensity(layer.GetIntensity()),
pTransparency(layer.GetTransparency()),

pLightOrientation(layer.GetLightOrientation()),
pLightColor(layer.GetLightColor()),
pLightIntensity(layer.GetLightIntensity()),
pAmbientIntensity(layer.GetAmbientIntensity()),

pMulBySkyLight(layer.GetMuliplyBySkyLight()),
pMulBySkyColor(layer.GetMuliplyBySkyColor())
{
	int i;
	for(i=0; i<6; i++){
		pTextures[i] = -1;
	}
	
	for(i=deSkyLayer::etOffsetX; i<=deSkyLayer::etAmbientIntensity; i++){
		pTargets[i] = NULL;
	}
	
	if(layer.GetSkin()){
		pSkin = ((deoglSkin*)layer.GetSkin()->GetPeerGraphic())->GetRSkin();
	}
	
	pUpdateSkins();
	pInitBodies(layer);
	
	for(i=deSkyLayer::etOffsetX; i<=deSkyLayer::etAmbientIntensity; i++){
		const deSkyControllerTarget &target = layer.GetTarget((deSkyLayer::eTargets)i);
		const int linkCount = target.GetLinkCount();
		if(linkCount > 0){
			pTargets[i] = new deoglRSkyControllerTarget(target);
		}
	}
}

deoglRSkyLayer::~deoglRSkyLayer(){
	int i;
	if(pBodies){
		delete [] pBodies;
	}
	
	for(i=deSkyLayer::etOffsetX; i<=deSkyLayer::etAmbientIntensity; i++){
		if(pTargets[i]){
			delete pTargets[i];
		}
	}
}



// Private Functions
//////////////////////

void deoglRSkyLayer::pUpdateSkins(){
	pLayerType = eltUnknown;
	
	int i;
	for(i=0; i<6; i++){
		pTextures[i] = -1;
	}
	
	if(!pSkin){
		return;
	}
	
	const int textureCount = pSkin->GetTextureCount();
	
	for(i=0; i<textureCount; i++){
		const char * const textureName = pSkin->GetTextureAt(i).GetName();
		
		if(strcmp(textureName, "sky.box.left") == 0){
			if(pLayerType == eltUnknown || pLayerType == eltSkyBox){
				pLayerType = eltSkyBox;
				pTextures[eiBoxLeft] = i;
			}
			
		}else if(strcmp(textureName, "sky.box.right") == 0){
			if(pLayerType == eltUnknown || pLayerType == eltSkyBox){
				pLayerType = eltSkyBox;
				pTextures[eiBoxRight] = i;
			}
			
		}else if(strcmp(textureName, "sky.box.top") == 0){
			if(pLayerType == eltUnknown || pLayerType == eltSkyBox){
				pLayerType = eltSkyBox;
				pTextures[eiBoxTop] = i;
			}
			
		}else if(strcmp(textureName, "sky.box.bottom") == 0){
			if(pLayerType == eltUnknown || pLayerType == eltSkyBox){
				pLayerType = eltSkyBox;
				pTextures[eiBoxBottom] = i;
			}
			
		}else if(strcmp(textureName, "sky.box.front") == 0){
			if(pLayerType == eltUnknown || pLayerType == eltSkyBox){
				pLayerType = eltSkyBox;
				pTextures[eiBoxFront] = i;
			}
			
		}else if(strcmp(textureName, "sky.box.back") == 0){
			if(pLayerType == eltUnknown || pLayerType == eltSkyBox){
				pLayerType = eltSkyBox;
				pTextures[eiBoxBack] = i;
			}
			
		}else if(strcmp(textureName, "sky.sphere") == 0){
			if(pLayerType == eltUnknown || pLayerType == eltSkySphere){
				pLayerType = eltSkySphere;
				pTextures[eiSphere] = i;
			}
		}
	}
}



// Private Functions
//////////////////////

void deoglRSkyLayer::pInitBodies(const deSkyLayer &layer){
	const int bodyCount = layer.GetBodyCount();
	if(bodyCount == 0){
		return;
	}
	
	pBodies = new sBody[bodyCount];
	
	for(pBodyCount=0; pBodyCount<bodyCount; pBodyCount++){
		const deSkyLayerBody &engBody = layer.GetBodyAt(pBodyCount);
		sBody &body = pBodies[pBodyCount];
		
		body.color = engBody.GetColor();
		body.size = engBody.GetSize();
		body.orientation = engBody.GetOrientation();
		
		body.skin = nullptr;
		if(engBody.GetSkin()){
			body.skin = ((deoglSkin*)engBody.GetSkin()->GetPeerGraphic())->GetRSkin();
		}
		
		float scaleU = 1.0f;
		float scaleV = 1.0f;
		
		if(body.skin && body.skin->GetTextureCount() > 0){
			const deoglSkinChannel * const oglChannel =
				body.skin->GetTextureAt(0).GetChannelAt(deoglSkinChannel::ectColor);
			
			if(oglChannel){
				scaleU = oglChannel->GetFactorU();
				scaleV = oglChannel->GetFactorV();
			}
		}
		
		body.texCoords[0].Set(0.0f, 0.0f);
		body.texCoords[1].Set(0.0f, scaleV);
		body.texCoords[2].Set(scaleU, scaleV);
		body.texCoords[3].Set(scaleU, 0.0f);
		
		const decMatrix matrix(decMatrix::CreateFromQuaternion(engBody.GetOrientation()));
		const decVector2 &size = engBody.GetSize();
		
		body.vertex[0] = matrix.Transform(-size.x, size.y, 1.0f);
		body.vertex[1] = matrix.Transform(-size.x, -size.y, 1.0f);
		body.vertex[2] = matrix.Transform(size.x, -size.y, 1.0f);
		body.vertex[3] = matrix.Transform(size.x, size.y, 1.0f);
	}
}
