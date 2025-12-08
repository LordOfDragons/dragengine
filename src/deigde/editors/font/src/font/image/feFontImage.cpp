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

#include "feFontImage.h"
#include "../feFont.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Font Editor"



// Class feFontImage
//////////////////////

// Constructor, destructor
////////////////////////////

feFontImage::feFontImage(deEngine *engine){
	if(!engine) DETHROW(deeInvalidParam);
	
	pEngine = engine;
	pEngImage = NULL;
	pParentFont = NULL;
	
	pWidth = 256;
	pHeight = 256;
	pFilename = "image.png";
	
	pChanged = false;
	pSaved = false;
	
	try{
		pEngImage.TakeOver(pEngine->GetImageManager()->CreateImage(pWidth, pHeight, 1, 4, 8);
		
		sRGBA8 * const pixels = pEngImage->GetDataRGBA8();
		const int pixelCount = pWidth * pHeight;
		int i;
		for(i=0; i<pixelCount; i++){
			pixels[i].red = 0;
			pixels[i].green = 0;
			pixels[i].blue = 0;
			pixels[i].alpha = 255;
		}
		
		pEngImage->NotifyImageDataChanged();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}


feFontImage::~feFontImage(){
	pCleanUp();
}



// Management
///////////////

void feFontImage::SetParentFont(feFont* font){
	pParentFont = font;
}



void feFontImage::SetSize(int width, int height){
	if(width < 1 || height < 1) DETHROW(deeInvalidParam);
	
	deImage *engImage = NULL;
	
	pWidth = width;
	pHeight = height;
	
	try{
		engImage = pEngine->GetImageManager()->CreateImage(pWidth, pHeight, 1, 4, 8);
		
		
		pEngImage = engImage;
		engImage->RetainImageData();
		
	}catch(const deException &){
		if(engImage) engImage->FreeReference();
		throw;
	}
	
	SetChanged(true);
}

void feFontImage::SetFilename(const char* filename, bool load){
	if(!filename) DETHROW(deeInvalidParam);
	
	if(!pFilename.Equals(filename)){
		pFilename = filename;
		
		if(load){
			LoadImage();
			
		}else{
			SetSaved(true);
			SetChanged(true);
		}
	}
}

void feFontImage::SetChanged(bool changed){
	pChanged = changed;
	NotifyImageChanged();
}

void feFontImage::SetSaved(bool saved){
	pSaved = saved;
}

void feFontImage::LoadImage(){
		pEngImage = NULL;
	}
	
	if(!pFilename.IsEmpty()){
		const char *basePath = "/";
		
		if(pParentFont){
			basePath = pParentFont->GetDirectoryPath();
		}
		
		if(pParentFont){
			pParentFont->GetLogger()->LogInfoFormat(LOGSOURCE, "FontImage.LoadImage: basePath='%s' path='%s'", basePath, pFilename.GetString());
		}
		
		try{
			pEngImage = pEngine->GetImageManager()->LoadImage(pFilename.GetString(), basePath);
			pEngImage->RetainImageData();
			
		}catch(const deException &e){
			if(pParentFont){
				pParentFont->GetLogger()->LogException(LOGSOURCE, e);
			}
		}
	}
	
	if(pEngImage){
		pWidth = pEngImage->GetWidth();
		pHeight = pEngImage->GetHeight();
		
		SetSaved(true);
		SetChanged(false);
		
	}else{
		SetSaved(false);
		SetChanged(true);
	}
}



void feFontImage::NotifyImageChanged(){
	if(pParentFont){
		pParentFont->SetChanged(true);
		pParentFont->NotifyImageChanged(this);
	}
}



// Private Functions
//////////////////////

void feFontImage::pCleanUp(){
}
