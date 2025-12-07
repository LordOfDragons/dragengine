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

#include <stdlib.h>
#include <string.h>

#include "deovrTextureMap.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/skin/deSkinBuilder.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>



// Class deovrTextureMap
///////////////////////////

// Constructor, destructor
////////////////////////////

deovrTextureMap::deovrTextureMap(deVROpenVR &ovr, vr::TextureID_t id) :
pOvr(ovr),
pID(id)
{
	pLoadTextureMap();
}

deovrTextureMap::~deovrTextureMap(){
}



// Private Functions
//////////////////////

class deovrTextureMap_BuildSkin : public deSkinBuilder {
private:
	deImage *pImageColor;
	deImage *pImageSolidity;
	
public:
	deovrTextureMap_BuildSkin(deImage *imageColor, deImage *imageSolidity) :
	pImageColor(imageColor),
	pImageSolidity(imageSolidity){
	}
	
	virtual void BuildSkin(deSkin *skin){
		// add texture
		deSkinTexture * const texture = new deSkinTexture("material");
		skin->AddTexture(texture);
		
		// add "color" property
		if(pImageColor){
			deSkinPropertyImage * const property = new deSkinPropertyImage("color");
			texture->AddProperty(property);
			property->SetImage(pImageColor);
			property->SetPath(pImageColor->GetFilename());
			
		}else{
			deSkinPropertyColor * const property = new deSkinPropertyColor("color");
			texture->AddProperty(property);
			property->SetColor(decColor(0.5f, 0.5f, 0.5f));
		}
		
		// add "solidity" property
		if(pImageSolidity){
			deSkinPropertyImage * const property = new deSkinPropertyImage("solidity");
			texture->AddProperty(property);
			property->SetImage(pImageSolidity);
			property->SetPath(pImageSolidity->GetFilename());
			
		}else{
			deSkinPropertyValue * const property = new deSkinPropertyValue("solidity");
			texture->AddProperty(property);
			property->SetValue(1.0f);
		}
		
		// add "solidity.masked" property
		// NOTE if this is set to 0 the skin fails... why?
		{
		deSkinPropertyValue * const property = new deSkinPropertyValue("solidity.masked");
		texture->AddProperty(property);
		property->SetValue(1.0f);
		}
	}
};

void deovrTextureMap::pLoadTextureMap(){
	vr::IVRRenderModels &vrrm = pOvr.GetVRRenderModels();
	vr::RenderModel_TextureMap_t *textureMap = nullptr;
	vr::EVRRenderModelError error;
	
	pOvr.LogInfoFormat("Loading texture map %d", pID);
	decTimer timer;
	
	while(true){
		error = vrrm.LoadTexture_Async(pID, &textureMap);
		if(error == vr::VRRenderModelError_None){
			break;
		}
		if(error != vr::VRRenderModelError_Loading){
			pOvr.LogErrorFormat("Failed loading texture map %d: %d", pID, error);
			DETHROW_INFO(deeInvalidAction, "Failed loading texture map");
		}
		// sleep a bit... but not here. well cache this later ( /cache/global/textureMaps/map<id>.deskin )
	}
	
	if(textureMap->format == vr::VRRenderModelTextureFormat_RGBA8_SRGB){
		const sRGBA8 * const srcPixels = (const sRGBA8*)textureMap->rubTextureMapData;
		int x, y;
		
		{
		pImageColor.TakeOver(pOvr.GetGameEngine()->GetImageManager()->CreateImage(
			textureMap->unWidth, textureMap->unHeight, 1, 3, 8));
		
		sRGB8 * const destPixels = pImageColor->GetDataRGB8();
		for(y=0; y<textureMap->unHeight; y++){
			const sRGBA8 * const srcPixelLine = srcPixels + textureMap->unWidth * y;
			sRGB8 * const destPixelLine = destPixels + textureMap->unWidth * y;
			for(x=0; x<textureMap->unWidth; x++){
				const sRGBA8 &srcPixel = srcPixelLine[x];
				sRGB8 &destPixel = destPixelLine[x];
				
				destPixel.red = srcPixel.red;
				destPixel.green = srcPixel.green;
				destPixel.blue = srcPixel.blue;
			}
		}
		
		pImageColor->NotifyImageDataChanged();
		
// 		decString filename;
// 		filename.Format("/capture/color_%u.png", pID);
// 		pOvr.GetGameEngine()->GetImageManager()->SaveImage(&pOvr.GetVFS(), pImageColor, filename);
		}
		
		{
		pImageSolidity.TakeOver(pOvr.GetGameEngine()->GetImageManager()->CreateImage(
			textureMap->unWidth, textureMap->unHeight, 1, 1, 8));
		
		sGrayscale8 * const destPixels = pImageSolidity->GetDataGrayscale8();
		for(y=0; y<textureMap->unHeight; y++){
			const sRGBA8 * const srcPixelLine = srcPixels + textureMap->unWidth * y;
			sGrayscale8 * const destPixelLine = destPixels + textureMap->unWidth * y;
			for(x=0; x<textureMap->unWidth; x++){
				destPixelLine[x].value = srcPixelLine[x].alpha;
			}
		}
		
		pImageSolidity->NotifyImageDataChanged();
		
// 		decString filename;
// 		filename.Format("/capture/solidity_%u.png", pID);
// 		pOvr.GetGameEngine()->GetImageManager()->SaveImage(&pOvr.GetVFS(), pImageSolidity, filename);
		}
		
	}else{
		pOvr.LogErrorFormat("Unsupported texture format %d: %d", pID, textureMap->format);
	}
	
	decPath path(decPath::CreatePathUnix("/openvr/textureMaps"));
	decString filetitle;
	filetitle.Format("map%u.deskin", pID);
	path.AddComponent(filetitle);
	
	try{
		deovrTextureMap_BuildSkin builder(pImageColor, pImageSolidity);
		pSkin.TakeOver(pOvr.GetGameEngine()->GetSkinManager()->CreateSkin(path.GetPathUnix(), builder));
		
	}catch(const deException &){
		vrrm.FreeTexture(textureMap);
		throw;
	}
	
	vrrm.FreeTexture(textureMap);
	
	pOvr.LogInfoFormat("Loading texture map %d finished in %.1fms", pID, timer.GetElapsedTime() * 1e3f);
}
