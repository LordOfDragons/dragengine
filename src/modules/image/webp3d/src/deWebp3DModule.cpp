/* 
 * Drag[en]gine WebP-3D Image Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
