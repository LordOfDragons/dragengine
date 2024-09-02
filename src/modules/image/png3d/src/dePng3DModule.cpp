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

#include "png.h"
#include "dePng3DModule.h"
#include "dePng3DImageInfos.h"
#include "dePng3DTarball.h"

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
MOD_ENTRY_POINT_ATTR deBaseModule *PNG3DCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry Point
////////////////

deBaseModule *PNG3DCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new dePng3DModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class dePng3DModule
////////////////////////

// Constructor, destructor
////////////////////////////

dePng3DModule::dePng3DModule( deLoadableModule &loadableModule ) :
deBaseImageModule( loadableModule ){
}

dePng3DModule::~dePng3DModule(){
}



// Loading, Saving
////////////////////

deBaseImageInfo *dePng3DModule::InitLoadImage( decBaseFileReader &file ){
	dePng3DImageInfo *infos = NULL;
	dePng3DTarball tarball( this );
	
	try{
		infos = new dePng3DImageInfo( file.GetFilename() );
		if( ! infos ) DETHROW( deeOutOfMemory );
		
		tarball.Get3DImageInfos( *infos, file );
		
	}catch( const deException & ){
		if( infos ) delete infos;
		throw;
	}
	
	return infos;
}

void dePng3DModule::LoadImage( decBaseFileReader &file, deImage &image, deBaseImageInfo &infos ){
	dePng3DTarball tarball( this );
	
	file.SetPosition( 0 );
	
	tarball.Load3DImage( ( dePng3DImageInfo& )infos, file, image );
}

void dePng3DModule::SaveImage( decBaseFileWriter &file, const deImage &image ){
	dePng3DTarball tarball( this );
	
	tarball.Save3DImage( file, image );
}
