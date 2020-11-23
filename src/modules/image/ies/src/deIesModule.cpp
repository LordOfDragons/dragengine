/* 
 * Drag[en]gine IES Photometric Image Module
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

#include "deIesModule.h"
#include "deIesImageInfo.h"
#include "deIesFormatLm63d1986.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/image/deImage.h>


#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *IESCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


deBaseModule *IESCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deIesModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
}



// Class deIesModule
//////////////////////

// Constructor, destructor
////////////////////////////

deIesModule::deIesModule( deLoadableModule &loadableModule ) :
deBaseImageModule( loadableModule ){
}

deIesModule::~deIesModule(){
}


// Loading and saving
///////////////////////

deBaseImageInfo *deIesModule::InitLoadImage( decBaseFileReader &file ){
	deIesImageInfo *info = NULL;
	
	try{
		info = new deIesFormatLm63d1986;
		
		if( ! info->LoadHeader( file ) ){
			delete info;
			return NULL;
		}
		
	}catch( const deException & ){
		if( info ){
			delete info;
		}
		throw;
	}
	
	return info;
}

void deIesModule::LoadImage( decBaseFileReader&, deImage &image, deBaseImageInfo &info ){
// 	( ( deIesImageInfo& )info ).LoadFile( ( float* )image.GetDataGrayscale32() );
	( ( deIesImageInfo& )info ).LoadFile( ( unsigned short* )image.GetDataGrayscale16() );
}

void deIesModule::SaveImage( decBaseFileWriter&, const deImage & ){
	// TODO not supported so far
	DETHROW_INFO( deeInvalidAction, "not supported" );
}
