/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deAvif3DModule.h"
#include "deAvif3DImageInfo.h"
#include "deAvif3DTarball.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/resources/image/deImage.h>



#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *AVIF3DCreateModule(deLoadableModule *loadableModule);
#ifdef  __cplusplus
}
#endif
#endif


// Entry Point
////////////////

deBaseModule *AVIF3DCreateModule(deLoadableModule *loadableModule){
	try{
		return new deAvif3DModule(*loadableModule);
		
	}catch(const deException &){
		return nullptr;
	}
}



// Class deAvif3DModule
/////////////////////////

// Constructor, destructor
////////////////////////////

deAvif3DModule::deAvif3DModule(deLoadableModule &loadableModule) :
deBaseImageModule(loadableModule){
}

deAvif3DModule::~deAvif3DModule(){
}



// Loading, Saving
////////////////////

deBaseImageInfo *deAvif3DModule::InitLoadImage(decBaseFileReader &file){
	deAvif3DImageInfo *info = nullptr;
	
	try{
		info = new deAvif3DImageInfo(file.GetFilename());
		deAvif3DTarball(*this).Get3DImageInfos(*info, file);
		
	}catch(const deException &){
		if(info){
			delete info;
		}
		throw;
	}
	
	return info;
}

void deAvif3DModule::LoadImage(decBaseFileReader &file, deImage &image, deBaseImageInfo &infos){
	file.SetPosition(0);
	deAvif3DTarball(*this).Load3DImage((deAvif3DImageInfo&)infos, file, image);
}

void deAvif3DModule::SaveImage(decBaseFileWriter &file, const deImage &image){
	deAvif3DTarball(*this).Save3DImage(file, image);
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

class deAvif3DModuleInternal : public deInternalModule{
public:
	using Ref = deTObjectReference<deAvif3DModuleInternal>;
	
	deAvif3DModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("AVIF-3D");
		SetDescription("Handles images saved in the AVIF-3D format (lossless and lossy compression). \
AVIF-3D files are actually a tarball with AVIF images one for each z coordinate in the 3D-image. \
Files inside the tarball are named zX.avif where X is the z coordinate without leading 0s.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtImage);
		SetDirectoryName("avif3d");
		GetPatternList().Add(".avif3d");
		SetDefaultExtension(".avif3d");
		SetNoCompress(true);
		SetPriority(1);
		SetDefaultLoggingName();
	}
	
	void CreateModule() override{
		SetModule(AVIF3DCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deTObjectReference<deInternalModule> deAvif3DRegisterInternalModule(deModuleSystem *system){
	return deAvif3DModuleInternal::Ref::New(system);
}
#endif
