/* 
 * Drag[en]gine Webp Image Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <webp/decode.h>
#include <webp/encode.h>

#include "deWebpModule.h"
#include "deWebpImageInfo.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/image/deImage.h>


#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *WebpCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry Point
////////////////

deBaseModule *WebpCreateModule( deLoadableModule *loadableModule ){
	try{
		return new deWebpModule( *loadableModule );
		
	}catch( const deException & ){
		return nullptr;
	}
}



// Class deWebpModule
///////////////////////

// Constructor, destructor
////////////////////////////

deWebpModule::deWebpModule( deLoadableModule &loadableModule ) :
deBaseImageModule( loadableModule ){
}

deWebpModule::~deWebpModule(){
}



// Loading, Saving
////////////////////

deBaseImageInfo *deWebpModule::InitLoadImage( decBaseFileReader &file ){
	return new deWebpImageInfo( file );
}

void deWebpModule::LoadImage( decBaseFileReader&, deImage &image, deBaseImageInfo &infos ){
	deWebpImageInfo &webpInfo = ( deWebpImageInfo& )infos;
	uint8_t *result = nullptr;
	
	try{
		if( webpInfo.GetHasAlpha() ){
			result = WebPDecodeRGBAInto( ( const uint8_t* )webpInfo.GetData().GetPointer(),
				webpInfo.GetData().GetLength(), ( uint8_t* )image.GetDataRGBA8(),
				image.GetWidth() * image.GetHeight() * 4, image.GetWidth() * 4 );
			
		}else{
			result = WebPDecodeRGBInto( ( const uint8_t* )webpInfo.GetData().GetPointer(),
				webpInfo.GetData().GetLength(), ( uint8_t* )image.GetDataRGB8(),
				image.GetWidth() * image.GetHeight() * 3, image.GetWidth() * 3 );
		}
		
		if( ! result ){
			DETHROW( deeInvalidFileFormat );
		}
		
	}catch( const deException & ){
		throw;
	}
}

void deWebpModule::SaveImage( decBaseFileWriter &file, const deImage &image ){
	if( image.GetBitCount() != 8 ){
		DETHROW_INFO( deeInvalidParam, "Only 8-Bit supporeted" );
	}
	
	if( image.GetComponentCount() < 3 ){
		DETHROW_INFO( deeInvalidParam, "Only 3 or 4 components supported" );
	}
	
	size_t size = 0;
	uint8_t *output = nullptr;
	const float quality = 95.0f;
	
	try{
		if( image.GetComponentCount() == 4 ){
			size = WebPEncodeRGBA( ( const uint8_t* )image.GetDataRGBA8(), image.GetWidth(),
				image.GetHeight(), image.GetWidth() * 4, quality, &output );
			
		}else{
			size = WebPEncodeRGB( ( const uint8_t* )image.GetDataRGB8(), image.GetWidth(),
				image.GetHeight(), image.GetWidth() * 3, quality, &output );
		}
		
		if( size == 0 || ! output ){
			DETHROW( deeWriteFile );
		}
		
		file.Write( output, size );
		
		WebPFree( output );
		
	}catch( const deException & ){
		if( output ){
			WebPFree( output );
		}
		throw;
	}
}
