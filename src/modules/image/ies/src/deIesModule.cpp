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
