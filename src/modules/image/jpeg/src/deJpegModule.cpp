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

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/image/deImage.h>

#include "deJpegModule.h"
#include "deJpegImageInfos.h"
#include "deJpegEncoder.h"

// include last to avoid conflicts with INT32 typedef and windows
#ifdef OS_W32
	// with libjpeg 2.x this problem is gone
	/*
	#ifndef HAVE_BOOLEAN
		#define HAVE_BOOLEAN 1
	#endif
	*/
#endif
#include <jerror.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *JPEGCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif



// Entry Point
////////////////

deBaseModule *JPEGCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deJpegModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// Callbacks
//////////////

void dejpegErrorExit( j_common_ptr cinfo ){
	DETHROW( deeInvalidParam );
}

void dejpegEmitMessage( j_common_ptr cinfo, int msg_level ){
}

void dejpegResetErrorMgr( j_common_ptr cinfo ){
}



void dejpegInitSource( j_decompress_ptr cinfo ){
	( ( deJpegImageInfo* )cinfo->client_data )->GetSourceMgr().bytes_in_buffer = 0;
}

boolean dejpegFillInputBuffer( j_decompress_ptr cinfo ){
	( ( deJpegImageInfo* )cinfo->client_data )->ReadNext();
	return TRUE;
}

void dejpegSkipInputData( j_decompress_ptr cinfo, long num_bytes ){
	( ( deJpegImageInfo* )cinfo->client_data )->SkipNext( ( int )num_bytes );
}

/*
boolean dejpegResyncToRestart( j_decompress_ptr cinfo, int desired ){
	printf( "dejpegResyncToRestart desired=%i\n", desired );
	return jpeg_resync_to_restart( cinfo, desired );
}
*/

void dejpegTermSource( j_decompress_ptr cinfo ){
}



void dejpegInitDestination( j_compress_ptr cinfo ){
	( ( deJpegEncoder* )cinfo->client_data )->ResetBuffer();
}

boolean dejpegEmptyOutputBuffer( j_compress_ptr cinfo ){
	( ( deJpegEncoder* )cinfo->client_data )->WriteEntireBuffer();
	return TRUE;
}

void dejpegTermDestination( j_compress_ptr cinfo ){
	( ( deJpegEncoder* )cinfo->client_data )->WriteRemaining();
}



// Class deJpegModule
///////////////////////

// Constructor, destructor
////////////////////////////

deJpegModule::deJpegModule( deLoadableModule &loadableModule ) :
deBaseImageModule( loadableModule ){
}

deJpegModule::~deJpegModule(){
}



// Loading, Saving
////////////////////

deBaseImageInfo *deJpegModule::InitLoadImage( decBaseFileReader &file ){
	deJpegImageInfo *info = NULL;
	
	try{
		info = new deJpegImageInfo( this, file.GetFilename() );
		
		info->GetErrorMgr().error_exit = dejpegErrorExit;
		info->GetErrorMgr().emit_message = dejpegEmitMessage;
		info->GetErrorMgr().reset_error_mgr = dejpegResetErrorMgr;
		
		jpeg_create_decompress( info->GetDecompressPtr() );
		
		info->InitRead( &file );
		
		info->GetSourceMgr().init_source = dejpegInitSource;
		info->GetSourceMgr().fill_input_buffer = dejpegFillInputBuffer;
		info->GetSourceMgr().skip_input_data = dejpegSkipInputData;
// 		info->GetSourceMgr().resync_to_restart = dejpegResyncToRestart;
		info->GetSourceMgr().term_source = dejpegTermSource;
		
		jpeg_read_header( info->GetDecompressPtr(), TRUE );
		
	}catch( const deException & ){
		if( info ){
			delete info;
		}
		throw;
	}
	
	return info;
}

void deJpegModule::LoadImage( decBaseFileReader &file, deImage &image, deBaseImageInfo &infos ){
	deJpegImageInfo &info = ( deJpegImageInfo& )infos;
	int componentCount = info.GetComponentCount();
	char *imageData = ( char* )image.GetData();
	int bitCount = info.GetBitCount();
	int height = info.GetHeight();
	int width = info.GetWidth();
	int rowLength;
	JSAMPROW row;
	
	// determine the length of a row in bytes
	if( bitCount == 8 ){
		rowLength = sizeof( unsigned char ) * componentCount * width;
		
	}else if( bitCount == 16 ){
		rowLength = sizeof( unsigned short ) * componentCount * width;
		
	}else{
		rowLength = sizeof( float ) * componentCount * width;
	}
	
	// read the image
	try{
		jpeg_start_decompress( info.GetDecompressPtr() );
		
		// verify everything is as we expect it to be
		if( ( int )info.GetDecompress().output_width != width ){
			DETHROW( deeInvalidParam );
		}
		if( ( int )info.GetDecompress().output_height != height ){
			DETHROW( deeInvalidParam );
		}
		if( ( int )info.GetDecompress().output_components != componentCount ){
			DETHROW( deeInvalidParam );
		}
		
		// read the image
		while( ( int )info.GetDecompress().output_scanline < height ){
			row = ( JSAMPROW )( imageData + rowLength * ( int )info.GetDecompress().output_scanline );
			jpeg_read_scanlines( info.GetDecompressPtr(), &row, 1 );
		}
		
		// clean up
		jpeg_finish_decompress( info.GetDecompressPtr() );
		jpeg_destroy_decompress( info.GetDecompressPtr() );
		
		info.CloseReader();
		
	}catch( const deException & ){
		jpeg_finish_decompress( info.GetDecompressPtr() );
		jpeg_destroy_decompress( info.GetDecompressPtr() );
		
		info.CloseReader();
		
		throw;
	}
}

void deJpegModule::SaveImage( decBaseFileWriter &file, const deImage &image ){
	char * const imageData = ( char* )image.GetData();
	const int componentCount = image.GetComponentCount();
	const int bitCount = image.GetBitCount();
	const int height = image.GetHeight();
	const int width = image.GetWidth();
	deJpegEncoder encoder( this );
	J_COLOR_SPACE jpegColorSpace;
	int jpegComponentCount;
	int rowLength;
	JSAMPROW row;
	
	// determine the length of a row in bytes
	if( bitCount == 8 ){
		rowLength = sizeof( unsigned char ) * componentCount * width;
		
	}else if( bitCount == 16 ){
		rowLength = sizeof( unsigned short ) * componentCount * width;
		
	}else{
		rowLength = sizeof( float ) * componentCount * width;
	}
	
	// only 8 bit count is supported by jpeg
	if( bitCount == 8 ){
		//jpegBitCount = 8;
		
	}else{
		DETHROW( deeInvalidParam );
	}
	
	if( componentCount == 1 ){
		jpegColorSpace = JCS_GRAYSCALE;
		jpegComponentCount = 1;
		
	}else if( componentCount == 2 ){
		DETHROW( deeInvalidParam ); // TODO fix this with a temporary array
		//jpegColorSpace = JCS_RGB;
		//jpegComponentCount = 3;
		
	}else if( componentCount == 3 ){
		jpegColorSpace = JCS_RGB;
		jpegComponentCount = 3;
		
	}else if( componentCount == 4 ){
		DETHROW( deeInvalidParam ); // TODO fix this with a temporary array
		//jpegColorSpace = JCS_RGB; // alpha can not be saved, silently ignore it
		//jpegComponentCount = 3;
		
	}else{
		DETHROW( deeInvalidParam );
	}
	
	// write the jpeg file
	try{
		encoder.GetErrorMgr().error_exit = dejpegErrorExit;
		encoder.GetErrorMgr().emit_message = dejpegEmitMessage;
		encoder.GetErrorMgr().reset_error_mgr = dejpegResetErrorMgr;
		
		jpeg_create_compress( encoder.GetEncodePtr() );
		
		encoder.InitWrite( &file );
		
		encoder.GetDestinationMgr().init_destination = dejpegInitDestination;
		encoder.GetDestinationMgr().empty_output_buffer = dejpegEmptyOutputBuffer;
		encoder.GetDestinationMgr().term_destination = dejpegTermDestination;
		
		encoder.GetEncode().image_width = ( JDIMENSION )width;
		encoder.GetEncode().image_height = ( JDIMENSION )height;
		encoder.GetEncode().input_components = jpegComponentCount;
		encoder.GetEncode().in_color_space = jpegColorSpace;
		jpeg_set_defaults( encoder.GetEncodePtr() );
		
		jpeg_set_quality( encoder.GetEncodePtr(), 85, TRUE );
		//jpeg_simple_progression( encoder.GetEncodePtr() );
		encoder.GetEncode().dct_method = JDCT_FLOAT;
		
		jpeg_start_compress( encoder.GetEncodePtr(), TRUE );
		
		while( ( int )encoder.GetEncode().next_scanline < height ){
			row = ( JSAMPROW )( imageData + rowLength * ( int )encoder.GetEncode().next_scanline );
			jpeg_write_scanlines( encoder.GetEncodePtr(), &row, 1 );
		}
		
		jpeg_finish_compress( encoder.GetEncodePtr() );
		jpeg_destroy_compress( encoder.GetEncodePtr() );
		encoder.CloseReader();
		
	}catch( const deException & ){
		jpeg_finish_compress( encoder.GetEncodePtr() );
		jpeg_destroy_compress( encoder.GetEncodePtr() );
		encoder.CloseReader();
		throw;
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class deJpegModuleInternal : public deInternalModule{
public:
	deJpegModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("JPEG");
		SetDescription("Handles images saved in the JPEG format (lossy compression).");
		SetAuthor("Plüss Roland (roland@rptd.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtImage);
		SetDirectoryName("jpeg");
		GetPatternList().Add(".jpg");
		SetDefaultExtension(".jpg");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(JPEGCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *deJpegRegisterInternalModule(deModuleSystem *system){
	return new deJpegModuleInternal(system);
}
#endif
