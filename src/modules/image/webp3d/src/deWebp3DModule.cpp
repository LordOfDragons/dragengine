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

#include "deWebp3DModule.h"
#include "deWebp3DImageInfo.h"
#include "deWebp3DTarball.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/resources/image/deImage.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *WEBP3DCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry Point
////////////////

deBaseModule *WEBP3DCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deWebp3DModule( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



// Class deWebp3DModule
/////////////////////////

// Constructor, destructor
////////////////////////////

deWebp3DModule::deWebp3DModule( deLoadableModule &loadableModule ) :
deBaseImageModule( loadableModule ){
}

deWebp3DModule::~deWebp3DModule(){
}



// Loading, Saving
////////////////////

deBaseImageInfo *deWebp3DModule::InitLoadImage( decBaseFileReader &file ){
	deWebp3DImageInfo *info = nullptr;
	
	try{
		info = new deWebp3DImageInfo( file.GetFilename() );
		deWebp3DTarball( *this ).Get3DImageInfos( *info, file );
		
	}catch( const deException & ){
		if( info ){
			delete info;
		}
		throw;
	}
	
	return info;
}

void deWebp3DModule::LoadImage( decBaseFileReader &file, deImage &image, deBaseImageInfo &infos ){
	file.SetPosition( 0 );
	deWebp3DTarball( *this ).Load3DImage( ( deWebp3DImageInfo& )infos, file, image );
}

void deWebp3DModule::SaveImage( decBaseFileWriter &file, const deImage &image ){
	deWebp3DTarball( *this ).Save3DImage( file, image );
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class deWebp3DModuleInternal : public deInternalModule{
public:
	deWebp3DModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("WebP-3D");
		SetDescription("Handles images saved in the WebP-3D format (lossless and lossy compression).\
WebP-3D files are actually a tarball with webp images one for each z coordinate in the 3D-image.\
Files inside the tarball are named zX.webp where X is the z coordinate without leading 0s.");
		SetAuthor("DragonDreams GmbH (info@dragondreams.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtImage);
		SetDirectoryName("webp3d");
		GetPatternList().Add(".webp3d");
		SetDefaultExtension(".webp3d");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(WEBP3DCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deWebp3DRegisterInternalModule(deModuleSystem *system){
	return new deWebp3DModuleInternal(system);
}
#endif
