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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxIcon.h"
#include "../../../resources/igdeIcon.h"
#include "../../../../engine/igdeEngineController.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class igdeNativeFoxIcon
////////////////////////////

// Constructor, destructor
////////////////////////////

void *igdeNativeFoxIcon::CreateNativeIcon(deImage &image){
	if(image.GetBitCount() != 8){
		DETHROW(deeInvalidParam);
	}
	
	const int hheight = image.GetHeight();
	const int wwidth = image.GetWidth();
	FXColor *nativeImageData = NULL;
	FXIcon *nativeIcon = NULL;
	
	try{
		// we have to create the pixel buffer the image is going to take ownership of.
		// we have to use fox allocation routines for this to work correctly
		if(! FXCALLOC(&nativeImageData, FXColor, wwidth * hheight)){
			DETHROW(deeOutOfMemory);
		}
		
		// copy pixel data from image to the allocated fox pixel buffer
		CopyPixelData(image, nativeImageData);
		
		// create icon. using pixelData=NULL the constructor creates the pixel data.
		// see the table in the comment below for the possible combinations of
		// parameters and the resulting behavior.
		// 
		// using IMAGE_SHMI and IMAGE_SHMP is only recommended for large images.
		// this call produces mostly small images so no need for this
		nativeIcon = new FXIcon(FXApp::instance(), nativeImageData,
			FXRGB(255, 255, 0), IMAGE_OWNED | IMAGE_KEEP, wwidth, hheight);
		
		// up to this point only the pixel data exists. calling create() creates the server
		// side data actually used for rendering later on. because IMAGE_KEEP is not used
		// in the constructor the pixel data is discarded after the server side resources
		// have been created. because IMAGE_OWNED is not used in the constructor the pixel
		// data is deleted.
		// 
		// the diffrent parameter used in the constructor affect the behavior like this:
		// | pixelBuffer | IMAGE_OWNED | IMAGE_KEEP | constructor |  after create   | destructor |
		// | ----------- | ----------- | ---------- | ----------- | --------------- | ---------- |
		// |     NULL    |     false   |    false   |   create    | delete set null |   nothing  |
		// |     NULL    |     true    |    false   |   create    | delete set null |   nothing  |
		// |     NULL    |     false   |    true    |   create    |     nothing     |   delete   |
		// |     NULL    |     true    |    true    |   create    |     nothing     |   delete   |
		// |   provided  |     false   |    false   |   nothing   |     set null    |   nothing  |
		// |   provided  |     true    |    false   |   nothing   | delete set null |   nothing  |
		// |   provided  |     false   |    true    |   nothing   |     nothing     |   nothing  |
		// |   provided  |     true    |    true    |   nothing   |     nothing     |   delete   |
		// 
		// we use pixelBuffer=NULL, IMAGE_OWNED=true and IMAGE_KEEP=false. this way the pixel
		// buffer is created by the icon resource and deleted after creating the server side
		// resources.
		// 
		// if we plan to modify the image we need to use pixelBuffer=NULL, IMAGE_OWNED=true
		// and IMAGE_KEEP=false. then we can call render again after changing the content
		nativeIcon->create();
		
	}catch(const deException &){
		if(nativeIcon){
			delete nativeIcon;
		}
		if(nativeImageData){
			FXFREE(&nativeImageData);
		}
		throw;
	}
	
	return nativeIcon;
}

void *igdeNativeFoxIcon::CreateNativeIconPNG(decBaseFileReader &reader){
	FXPNGIcon *nativeIcon = NULL;
	char *imageData = NULL;
	FXMemoryStream stream;
	int imageSize = 0;
	
	try{
		imageSize = reader.GetLength();
		if(imageSize == 0){
			DETHROW_INFO(deeInvalidFileFormat, reader.GetFilename());
		}
		
		imageData = new char[imageSize];
		reader.Read(imageData, imageSize);
		
		nativeIcon = new FXPNGIcon(FXApp::instance(), 0, FXRGB(192,192,192), IMAGE_KEEP | IMAGE_SHMI | IMAGE_SHMP);
		#ifdef FOX_OLD_MEMORY_STREAM
		if(! stream.open(FX::FXStreamLoad, (FXuval)imageSize, (FXuchar*)imageData)){
		#else
		if(! stream.open(FX::FXStreamLoad, (FXuchar*)imageData, (FXuval)imageSize, false)){
		#endif
			DETHROW(deeInvalidParam);
		}
		if(! nativeIcon->loadPixels(stream)){
			DETHROW_INFO(deeInvalidFileFormat, reader.GetFilename());
		}
		
		delete [] imageData;
		imageData = NULL;
		
		nativeIcon->create();
		
	}catch(const deException &){
		if(nativeIcon){
			delete nativeIcon;
		}
		if(imageData){
			delete [] imageData;
		}
		throw;
	}
	
	return nativeIcon;
}

void *igdeNativeFoxIcon::DuplicateNativeIcon(void *native){
	const FXIcon * const sourceIcon = (FXIcon*)native;
	const int hheight = sourceIcon->getHeight();
	const int wwidth = sourceIcon->getWidth();
	const int pixelCount = wwidth * hheight;
	
	FXColor *imageData = NULL;
	if(! FXCALLOC(&imageData, FXColor, pixelCount)){
		DETHROW(deeOutOfMemory);
	}
	memcpy(imageData, sourceIcon->getData(), sizeof(FXColor) * pixelCount);
	
	return new FXIcon(FXApp::instance(), imageData, 0, IMAGE_OWNED | IMAGE_KEEP, wwidth, hheight);
}

void igdeNativeFoxIcon::DestroyNativeIcon(void *native){
	delete (FXIcon*)native;
}



// Management
///////////////

decPoint igdeNativeFoxIcon::GetSize(void *native){
	const FXIcon &iicon = *((FXIcon*)native);
	return decPoint(iicon.getWidth(), iicon.getHeight());
}

void igdeNativeFoxIcon::Scale(const decPoint &size, void *native){
	FXIcon &iicon = *((FXIcon*)native);
	const decPoint iconSize(iicon.getWidth(), iicon.getHeight());
	if(! (iconSize == size)){
		iicon.scale(size.x, size.y, 1);
	}
}

void igdeNativeFoxIcon::UpdatePixels(void *native, deImage &image){
	FXIcon &iicon = *((FXIcon*)native);
	const decPoint iconSize(iicon.getWidth(), iicon.getHeight());
	
	if(image.GetWidth() != iconSize.x || image.GetHeight() != iconSize.y || image.GetBitCount() != 8){
		DETHROW(deeInvalidParam);
	}
	
	CopyPixelData(image, iicon.getData()); // getData() is valid because of IMAGE_KEEP
	
	iicon.render();
}



void igdeNativeFoxIcon::CopyPixelData(deImage &image, FXColor *foxData){
	const int hheight = image.GetHeight();
	const int wwidth = image.GetWidth();
	const int pixelCount = wwidth * hheight;
	int i;
	
	if(image.GetComponentCount() == 1){
		image.RetainImageData();
		const sGrayscale8 * const dataIn = image.GetDataGrayscale8();
		for(i=0; i<pixelCount; i++){
			foxData[i] = FXRGBA(dataIn[i].value, dataIn[i].value, dataIn[i].value, 255);
		}
		image.ReleaseImageData();
		
	}else if(image.GetComponentCount() == 3){
		image.RetainImageData();
		const sRGB8 * const dataIn = image.GetDataRGB8();
		for(i=0; i<pixelCount; i++){
			foxData[i] = FXRGBA(dataIn[i].red, dataIn[i].green, dataIn[i].blue, 255);
		}
		image.ReleaseImageData();
		
	}else if(image.GetComponentCount() == 4){
		image.RetainImageData();
		const sRGBA8 * const dataIn = image.GetDataRGBA8();
		for(i=0; i<pixelCount; i++){
			foxData[i] = FXRGBA(dataIn[i].red, dataIn[i].green, dataIn[i].blue, dataIn[i].alpha);
		}
		image.ReleaseImageData();
		
	}else{
		DETHROW(deeInvalidParam);
	}
}

#endif
