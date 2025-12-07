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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deTgaModule.h"
#include "deTgaImageInfos.h"
#include "dragengine/resources/image/deImage.h"
#include "dragengine/common/file/decBaseFileReader.h"
#include "dragengine/common/file/decBaseFileWriter.h"
#include <dragengine/common/exceptions.h>


// export definition
#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *TGACreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// entry function
// used to create a tga image module.
// has to be named CreateModule returning deBaseModule.
// returns NULL on error.
/////////////////////////////////////////////////////////
deBaseModule *TGACreateModule(deLoadableModule *loadableModule){
	deBaseModule *module = NULL;
	try{
		module = new deTgaModule(*loadableModule);
	}catch(const deException &){
		return NULL;
	}
	return module;
}



// class deTgaModule
//////////////////////

// constructor, destructor
deTgaModule::deTgaModule(deLoadableModule &loadableModule) :
deBaseImageModule(loadableModule){
}
deTgaModule::~deTgaModule(){
}

// images
deBaseImageInfo *deTgaModule::InitLoadImage(decBaseFileReader &file){
	deTgaImageInfo *infos = NULL;
	bool troubles = false;
	try{
		// create infos object
		infos = new deTgaImageInfo;
		if(!infos) DETHROW(deeOutOfMemory);
		// read tgaInfos.header
		file.Read(&infos->header, sizeof(tgaHdr));
		if(infos->header.imageType!=3 && infos->header.imageType!=2 &&
			infos->header.imageType!=1) troubles = true;
		if(infos->header.bpp!=8 && infos->header.bpp!=24 && infos->header.bpp!=32) troubles = true;
		if(infos->header.idLength > 0) file.MovePosition(infos->header.idLength);
		// finished
		if(troubles){
			delete infos;
			return NULL;
		}
	}catch(const deException &){
		if(infos) delete infos;
		throw;
	}
	return infos;
}
void deTgaModule::LoadImage(decBaseFileReader &file, deImage &image, deBaseImageInfo &infos){
	deTgaImageInfo &tgaInfos = (deTgaImageInfo&)infos;
	sRGBA8 *dataRGBA;
	sRGB8 *dataRGB;
	sGrayscale8 *dataAlpha;
	int width = tgaInfos.header.width;
	int height = tgaInfos.header.height;
	int y, x, fromY, stepY, fromX, stepX, ry, rx;
	// read pixel data
	if(tgaInfos.header.imgDesc & 0x20){ // top -> bottom
		fromY = 0;
		stepY = 1;
	}else{ // bottom -> top
		fromY = height - 1;
		stepY = -1;
	}
	if(tgaInfos.header.imgDesc & 0x10){ // right -> left
		fromX = width - 1;
		stepX = -1;
	}else{ // left -> right
		fromX = 0;
		stepX = 1;
	}
	for(ry=fromY,y=0; y<height; ry+=stepY,y++){
		if(tgaInfos.header.bpp == 8){
			dataAlpha = image.GetDataGrayscale8() + width*ry;
			for(rx=fromX,x=0; x<width; rx+=stepX,x++){
				dataAlpha[rx].value = file.ReadByte();
			}
		}else if(tgaInfos.header.bpp == 24){
			dataRGB = image.GetDataRGB8() + width*ry;
			for(rx=fromX,x=0; x<width; rx+=stepX,x++){
				dataRGB[rx].blue = file.ReadByte();
				dataRGB[rx].green = file.ReadByte();
				dataRGB[rx].red = file.ReadByte();
			}
		}else{
			dataRGBA = image.GetDataRGBA8() + width*ry;
			for(rx=fromX,x=0; x<width; rx+=stepX,x++){
				dataRGBA[rx].blue = file.ReadByte();
				dataRGBA[rx].green = file.ReadByte();
				dataRGBA[rx].red = file.ReadByte();
				dataRGBA[rx].alpha = file.ReadByte();
			}
		}
	}
}
void deTgaModule::SaveImage(decBaseFileWriter &file, const deImage &image){
	if(image.GetBitCount() != 8) DETHROW(deeInvalidParam);
	int componentCount = image.GetComponentCount();
	if(componentCount != 1 && componentCount != 3 && componentCount != 4) DETHROW(deeInvalidParam);
	
	sRGBA8 *dataRGBA;
	sRGB8 *dataRGB;
	sGrayscale8 *dataAlpha;
	tgaHdr header;
	int p, size;
	int width = image.GetWidth();
	int height = image.GetHeight();
	// write header
	header.idLength = 0;
	header.clrMapType = 0;
	if(componentCount == 4){
		header.imageType = 1;
	}else if(componentCount == 3){
		header.imageType = 2;
	}else{
		header.imageType = 3;
	}
	header.firstClrMapID = 0;
	header.countClrMapEntries = 0;
	header.entrySize = 0;
	header.posX = 0;
	header.posY = 0;
	header.width = width;
	header.height = height;
	header.bpp = componentCount * 8;
	header.imgDesc = 32;
	file.Write(&header, sizeof(header));
	// save pixel data
	size = width * height;
	if(componentCount == 4){
		dataRGBA = image.GetDataRGBA8();
		for(p=0; p<size; p++){
			file.WriteByte(dataRGBA[p].blue);
			file.WriteByte(dataRGBA[p].green);
			file.WriteByte(dataRGBA[p].red);
			file.WriteByte(dataRGBA[p].alpha);
		}
	}else if(componentCount == 3){
		dataRGB = image.GetDataRGB8();
		for(p=0; p<size; p++){
			file.WriteByte(dataRGB[p].blue);
			file.WriteByte(dataRGB[p].green);
			file.WriteByte(dataRGB[p].red);
		}
	}else{
		dataAlpha = image.GetDataGrayscale8();
		for(p=0; p<size; p++){
			file.WriteByte(dataAlpha[p].value);
		}
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deTgaModuleInternal : public deInternalModule{
public:
	deTgaModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("Targa");
		SetDescription("Handles images in uncompressed Targe format.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtImage);
		SetDirectoryName("tga");
		GetPatternList().Add(".tga");
		SetDefaultExtension(".tga");
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(TGACreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deTgaRegisterInternalModule(deModuleSystem *system){
	return new deTgaModuleInternal(system);
}
#endif
