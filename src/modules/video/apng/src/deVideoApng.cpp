/* 
 * Drag[en]gine Animated PNG Video Module
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

#include "deVideoApng.h"
#include "deapngDecoder.h"
#include "deapngReader.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/systems/modules/video/deBaseVideoInfo.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *APNGCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *APNGCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deVideoApng( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Class deVideoApng
//////////////////////

// Constructor, destructor
////////////////////////////

deVideoApng::deVideoApng( deLoadableModule &loadableModule ) :
deBaseVideoModule( loadableModule ){
}

deVideoApng::~deVideoApng(){
}



// Management
///////////////

void deVideoApng::InitLoadVideo( decBaseFileReader &reader, deBaseVideoInfo &info ){
	deapngReader apngReader( *this, &reader );
	
	info.SetWidth( apngReader.GetWidth() );
	info.SetHeight( apngReader.GetHeight() );
	info.SetPixelFormat( apngReader.GetPixelFormat() );
	info.SetFrameCount( apngReader.GetFrameCount() );
	info.SetFrameRate( apngReader.GetFrameRate() );
}

void deVideoApng::SaveVideo( decBaseFileWriter &reader, const deVideo &video ){
	// not supported yet
}

deBaseVideoDecoder *deVideoApng::CreateDecoder( decBaseFileReader *reader ){
	if( ! reader ){
		DETHROW( deeInvalidParam );
	}
	return new deapngDecoder( *this, reader );
}

deBaseVideoAudioDecoder *deVideoApng::CreateAudioDecoder( decBaseFileReader* ){
	return NULL;
}
