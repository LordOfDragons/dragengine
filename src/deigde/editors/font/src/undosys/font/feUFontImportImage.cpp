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

#include "feUFontImportImage.h"
#include "../../font/feFont.h"
#include "../../font/image/feFontImage.h"

#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>



// Class feUFontImportImage
/////////////////////////////

// Constructor, destructor
////////////////////////////

feUFontImportImage::feUFontImportImage(feFontImage *fontImage, deImage *newImage, const char *newPath){
	if(!newImage || !newPath) DETHROW(deeInvalidParam);
	
	if(newImage->GetComponentCount() != 4) DETHROW(deeInvalidParam);
	
	deImage &oldImage = *fontImage->GetEngineImage();
	
	pOldWidth = oldImage.GetWidth();
	pOldHeight = oldImage.GetHeight();
	
	pNewWidth = newImage->GetWidth();
	pNewHeight = newImage->GetHeight();
	
	pOldPath = fontImage->GetFilename();
	pNewPath = newPath;
	
	SetShortInfo("@Font.Undo.ImportImage");
	SetMemoryConsumption(sizeof(feUFontImportImage) + sizeof(decColor) * (pOldWidth * pOldHeight + pNewWidth * pNewHeight));
	
	newImage->RetainImageData();
	try{
		pOldColor.AddRange(pOldWidth * pOldHeight, {});
		pStoreColors(oldImage, pOldColor.GetArrayPointer());
		
		pNewColor.AddRange(pNewWidth * pNewHeight, {});
		pStoreColors(*newImage, pNewColor.GetArrayPointer());
		
		newImage->ReleaseImageData();
		
	}catch(const deException &){
		newImage->ReleaseImageData();
		throw;
	}
	
	pImage = fontImage;
}

feUFontImportImage::~feUFontImportImage() = default;



// Management
///////////////

void feUFontImportImage::Undo(){
	pImage->SetSize(pOldWidth, pOldHeight);
	pImage->SetFilename(pOldPath.GetString(), false);
	pRestoreColors(*pImage->GetEngineImage(), pOldColor.GetArrayPointer());
}

void feUFontImportImage::Redo(){
	pImage->SetSize(pNewWidth, pNewHeight);
	pImage->SetFilename(pNewPath.GetString(), false);
	pRestoreColors(*pImage->GetEngineImage(), pNewColor.GetArrayPointer());
}



// Private
////////////

void feUFontImportImage::pStoreColors(deImage &image, decColor *colors){
	int p, pixelCount = image.GetWidth() * image.GetHeight();
	int bitCount = image.GetBitCount();
	float factor;
	
	if(bitCount == 8){
		sRGBA8 *data = image.GetDataRGBA8();
		factor = 1.0f / 255.0f;
		
		for(p=0; p<pixelCount; p++){
			const sRGBA8 &pixel = data[p];
			
			colors[p].r = (float)pixel.red * factor;
			colors[p].g = (float)pixel.green * factor;
			colors[p].b = (float)pixel.blue * factor;
			colors[p].a = (float)pixel.alpha * factor;
		}
		
	}else if(bitCount == 16){
		sRGBA16 *data = image.GetDataRGBA16();
		factor = 1.0f / 65535.0f;
		
		for(p=0; p<pixelCount; p++){
			const sRGBA16 &pixel = data[p];
			
			colors[p].r = (float)pixel.red * factor;
			colors[p].g = (float)pixel.green * factor;
			colors[p].b = (float)pixel.blue * factor;
			colors[p].a = (float)pixel.alpha * factor;
		}
		
	}else{ // bitCount == 32
		memcpy(pNewColor.GetArrayPointer(), image.GetDataRGBA32(), sizeof(decColor) * pixelCount);
	}
}

void feUFontImportImage::pRestoreColors(deImage &image, decColor *colors){
	int p, pixelCount = image.GetWidth() * image.GetHeight();
	int bitCount = image.GetBitCount();
	float factor;
	
	if(bitCount == 8){
		sRGBA8 *data = image.GetDataRGBA8();
		factor = 255.0f;
		
		for(p=0; p<pixelCount; p++){
			sRGBA8 &pixel = data[p];
			
			pixel.red = (unsigned char)(colors[p].r * factor);
			pixel.green = (unsigned char)(colors[p].g * factor);
			pixel.blue = (unsigned char)(colors[p].b * factor);
			pixel.alpha = (unsigned char)(colors[p].a * factor);
		}
		
	}else if(bitCount == 16){
		sRGBA16 *data = image.GetDataRGBA16();
		factor = 65535.0f;
		
		for(p=0; p<pixelCount; p++){
			sRGBA16 &pixel = data[p];
			
			pixel.red = (unsigned short)(colors[p].r * factor);
			pixel.green = (unsigned short)(colors[p].g * factor);
			pixel.blue = (unsigned short)(colors[p].b * factor);
			pixel.alpha = (unsigned short)(colors[p].a * factor);
		}
		
	}else{ // bitCount == 32
		memcpy(image.GetDataRGBA32(), pNewColor.GetArrayPointer(), sizeof(decColor) * pixelCount);
	}
	
	image.NotifyImageDataChanged();
}
