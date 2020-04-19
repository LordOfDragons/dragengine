/* 
 * Drag[en]gine 3D PNG Image Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
