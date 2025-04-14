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
#include "dePngModule.h"
#include "dePngImageInfos.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/systems/deModuleSystem.h>


#ifndef WITH_INTERNAL_MODULE
#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *PNGCreateModule( deLoadableModule *loadableModule );
#ifdef __cplusplus
}
#endif
#endif

// Entry Point
////////////////

deBaseModule *PNGCreateModule( deLoadableModule *loadableModule ){
	try{
		return new dePngModule(*loadableModule);
		
	}catch(const deException &){
		return nullptr;
	}
}



// PNG callbacks
//////////////////

static void depngRead( png_structp readStruct, png_bytep data, png_size_t length ){
	decBaseFileReader *reader = ( decBaseFileReader* )png_get_io_ptr( readStruct );
	
	reader->Read( data, ( int )length );
}

static void depngWrite( png_structp writeStruct, png_bytep data, png_size_t length ){
	decBaseFileWriter *writer = ( decBaseFileWriter* )png_get_io_ptr( writeStruct );
	
	writer->Write( data, ( int )length );
}

static void depngWriteStatus( png_structp writeStruct, png_uint_32 row, int pass ){
}

static void depngFlush( png_structp writeStruct ){
}

static void depngError( png_structp errorStruct, png_const_charp message ){
	const dePngImageInfo::sFeedback &feedback = *( ( dePngImageInfo::sFeedback* )png_get_error_ptr( errorStruct ) );
	
	feedback.module->LogErrorFormat( "Error %s: %s", feedback.filename.GetString(), message );
	
	DETHROW( deeInvalidAction );
}

static void depngWarning( png_structp errorStruct, png_const_charp message ){
	const dePngImageInfo::sFeedback &feedback = *( ( dePngImageInfo::sFeedback* )png_get_error_ptr( errorStruct ) );
	
	feedback.module->LogWarnFormat( "Warning %s: %s", feedback.filename.GetString(), message );
}



// Class dePngModule
//////////////////////

// Constructor, destructor
////////////////////////////

dePngModule::dePngModule( deLoadableModule &loadableModule ) :
deBaseImageModule( loadableModule ){
}

dePngModule::~dePngModule(){
}



// Loading, Saving
////////////////////

deBaseImageInfo *dePngModule::InitLoadImage( decBaseFileReader &file ){
	dePngImageInfo *infos = NULL;
	bool troubles = false;
	int colorType;
	
	try{
		// create infos object
		infos = new dePngImageInfo( file.GetFilename() );
		if( ! infos ) DETHROW( deeOutOfMemory );
		
		infos->feedback.module = this;
		infos->feedback.filename = file.GetFilename();
		
		// create structures
		infos->readStruct = png_create_read_struct_2( PNG_LIBPNG_VER_STRING, ( png_voidp )&infos->feedback,
			( png_error_ptr )depngError, ( png_error_ptr )depngWarning, NULL, NULL, NULL );
		if( ! infos->readStruct ) DETHROW( deeOutOfMemory );
		
		infos->infoStruct = png_create_info_struct( infos->readStruct );
		if( ! infos->infoStruct ) DETHROW( deeOutOfMemory );
		
		// change callbacks
		png_set_read_fn( infos->readStruct, ( png_voidp )&file, ( png_rw_ptr )depngRead );
		
		// read infos
		png_read_info( infos->readStruct, infos->infoStruct );
		infos->width = ( int )png_get_image_width( infos->readStruct, infos->infoStruct );
		infos->height = ( int )png_get_image_height( infos->readStruct, infos->infoStruct );
		infos->bitCount = ( int )png_get_bit_depth( infos->readStruct, infos->infoStruct );
		colorType = ( int )png_get_color_type( infos->readStruct, infos->infoStruct );
		
		// determine what format we need
		switch( colorType ){
		case PNG_COLOR_TYPE_GRAY:
			infos->componentCount = 1;
			break;
			
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			png_set_gray_to_rgb( infos->readStruct );
			infos->componentCount = 4;
			break;
			
		case PNG_COLOR_TYPE_PALETTE:
			infos->componentCount = 3;
			break;
			
		case PNG_COLOR_TYPE_RGB:
			infos->componentCount = 3;
			break;
			
		case PNG_COLOR_TYPE_RGB_ALPHA:
			infos->componentCount = 4;
			break;
			
		default:
			troubles = true;
		}
		
		if( troubles ){
			delete infos;
			return NULL;
		}
		
		// NOTE libpng has separate function calls all linked to png_set_expand but using
		//      them is discuraged since it is not future proof. thus png_set_expand is
		//      used for all expanding cases letting libpng do the right thing
		
		// if there is a palette expand it into real colors
		if( colorType == PNG_COLOR_TYPE_PALETTE ){
			png_set_expand( infos->readStruct );
		}
		
		// bit count below 8 have to be expanded up to 8
		if( infos->bitCount < 8 ){
			png_set_expand( infos->readStruct );
			infos->bitCount = 8;
		}
		
		// expand transparency values
		if( png_get_valid( infos->readStruct, infos->infoStruct, PNG_INFO_tRNS ) ){
			png_set_expand( infos->readStruct );
			
			switch( colorType ){
			case PNG_COLOR_TYPE_PALETTE:
			case PNG_COLOR_TYPE_RGB:
				infos->componentCount = 4;
				break;
				
			case PNG_COLOR_TYPE_GRAY:
				png_set_gray_to_rgb( infos->readStruct );
				infos->componentCount = 4;
				break;
				
			default:
				break;
			}
		}
		
		// 16 bit is stored in network byte order but we need it the other way round
		if( infos->bitCount == 16 ){
			png_set_swap( infos->readStruct );
		}
		
		// and now the big update... we are ready to go
		png_read_update_info( infos->readStruct, infos->infoStruct );
		
	}catch( const deException & ){
		if( infos ) delete infos;
		throw;
	}
	
	return infos;
}

void dePngModule::LoadImage( decBaseFileReader &file, deImage &image, deBaseImageInfo &infos ){
	dePngImageInfo &pngInfos = ( dePngImageInfo& )infos;
	int componentCount = pngInfos.componentCount;
	char *imageData = ( char* )image.GetData();
	int bitCount = pngInfos.bitCount;
	int height = pngInfos.height;
	int width = pngInfos.width;
	int r, rowLength;
	png_bytep *rows = NULL;
	
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
		// build rows array
		rows = new png_bytep[ height ];
		if( ! rows ) DETHROW( deeOutOfMemory );
		
		for( r=0; r<height; r++ ){
			rows[ r ] = ( png_bytep )( imageData + rowLength * r );
		}
		
		// read the image
		png_read_image( pngInfos.readStruct, rows );
		
		// finish file
		png_read_end( pngInfos.readStruct, NULL );
		
		// free rows array
		delete [] rows;
		
	}catch( const deException & ){
		if( rows ) delete [] rows;
		throw;
	}
}

void dePngModule::SaveImage( decBaseFileWriter &file, const deImage &image ){
	png_structp writeStruct = NULL;
	png_infop infoStruct = NULL;
	char *imageData = ( char* )image.GetData();
	int componentCount = image.GetComponentCount();
	int bitCount = image.GetBitCount();
	dePngImageInfo::sFeedback feedback;
	int height = image.GetHeight();
	int width = image.GetWidth();
	int r, rowLength;
	png_bytep *rows = NULL;
	int pngBitCount;
	int pngColorType;
	
	feedback.module = this;
	feedback.filename = file.GetFilename();
	
	// determine the length of a row in bytes
	if( bitCount == 8 ){
		rowLength = sizeof( unsigned char ) * componentCount * width;
		
	}else if( bitCount == 16 ){
		rowLength = sizeof( unsigned short ) * componentCount * width;
		
	}else{
		rowLength = sizeof( float ) * componentCount * width;
	}
	
	// determine the bit count and color type to save
	if( bitCount == 8 ){
		pngBitCount = 8;
		
	}else{
		pngBitCount = 16;
	}
	
	if( componentCount == 1 ){
		pngColorType = PNG_COLOR_TYPE_GRAY;
		
	}else if( componentCount == 2 ){
		pngColorType = PNG_COLOR_TYPE_RGB;
		// not supported. needs re-parking data into RGB to feed it to PNG. problem is
		// that 2-component images have to be red-green not grayscale-alpha which is the
		// only format PNG supports. the final solution needs to write them as red-green-0.
		DETHROW( deeInvalidParam );
		
	}else if( componentCount == 3 ){
		pngColorType = PNG_COLOR_TYPE_RGB;
		
	}else if( componentCount == 4 ){
		pngColorType = PNG_COLOR_TYPE_RGB_ALPHA;
		
	}else{
		DETHROW( deeInvalidParam );
	}
	
	// write the png file
	try{
		// create structures
		writeStruct = png_create_write_struct_2( PNG_LIBPNG_VER_STRING, ( png_voidp )&feedback,
			( png_error_ptr )depngError, ( png_error_ptr )depngWarning, NULL, NULL, NULL );
		if( ! writeStruct ) DETHROW( deeOutOfMemory );
		
		infoStruct = png_create_info_struct( writeStruct );
		if( ! infoStruct ) DETHROW( deeOutOfMemory );
		
		// change callbacks
		png_set_write_fn( writeStruct, ( png_voidp )&file, ( png_rw_ptr )depngWrite, ( png_flush_ptr )depngFlush );
		png_set_write_status_fn ( writeStruct, ( png_write_status_ptr )depngWriteStatus );
		
		// filters ( options, TODO )
		/* turn on or off filtering, and/or choose specific filters. You can use either a single
		PNG_FILTER_VALUE_NAME or the logical OR of one or more PNG_FILTER_NAME masks. */
		/* png_set_filter( writeStruct, 0,
			PNG_FILTER_NONE  | PNG_FILTER_VALUE_NONE |
			PNG_FILTER_SUB   | PNG_FILTER_VALUE_SUB  |
			PNG_FILTER_UP    | PNG_FILTER_VALUE_UP   |
			PNG_FILTER_AVE   | PNG_FILTER_VALUE_AVE  |
			PNG_FILTER_PAETH | PNG_FILTER_VALUE_PAETH|
			PNG_ALL_FILTERS);*/
		
		// compression options ( TODO )
		png_set_compression_level( writeStruct, 6 );
		
		/* set other zlib parameters */
		/*png_set_compression_mem_level(png_ptr, 8);
		png_set_compression_strategy(png_ptr,
			Z_DEFAULT_STRATEGY);
		png_set_compression_window_bits(png_ptr, 15);
		png_set_compression_method(png_ptr, 8);
		png_set_compression_buffer_size(png_ptr, 8192)*/
		
		// fill in the infos
		png_set_IHDR( writeStruct, infoStruct, width, height, pngBitCount, pngColorType,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT );
		
		//png_set_tIME( writeStruct, infoStruct, PNG_VALID_tIME );
		
		// build rows array
		rows = new png_bytep[ height ];
		if( ! rows ) DETHROW( deeOutOfMemory );
		
		for( r=0; r<height; r++ ){
			rows[ r ] = ( png_bytep )( imageData + rowLength * r );
		}
		
		// make image data available to the library
		png_set_rows( writeStruct, infoStruct, rows );
		
		// write the image to the file
		/*
		PNG_TRANSFORM_IDENTITY      No transformation
		PNG_TRANSFORM_PACKING       Pack 1, 2 and 4-bit samples
		PNG_TRANSFORM_PACKSWAP      Change order of packed pixels to LSB first
		PNG_TRANSFORM_INVERT_MONO   Invert monochrome images
		PNG_TRANSFORM_SHIFT         Normalize pixels to the sBIT depth
		PNG_TRANSFORM_BGR           Flip RGB to BGR, RGBA to BGRA
		PNG_TRANSFORM_SWAP_ALPHA    Flip RGBA to ARGB or GA to AG
		PNG_TRANSFORM_INVERT_ALPHA  Change alpha from opacity to transparency
		PNG_TRANSFORM_SWAP_ENDIAN   Byte-swap 16-bit samples
		PNG_TRANSFORM_STRIP_FILLER  Strip out filler bytes.
		*/
		png_write_png( writeStruct, infoStruct, PNG_TRANSFORM_SWAP_ENDIAN, NULL );
		
		// free rows array
		delete [] rows;
		rows = NULL;
		
		// clean up
		png_destroy_write_struct( &writeStruct, &infoStruct );
		
	}catch( const deException & ){
		if( writeStruct || infoStruct ) png_destroy_write_struct( &writeStruct, &infoStruct );
		throw;
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class dePngModuleInternal : public deInternalModule{
public:
	dePngModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("PNG");
		SetDescription("Handles images saved in the PNG format (lossless compression).");
		SetAuthor("Plüss Roland (roland@rptd.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtImage);
		SetDirectoryName("png");
		GetPatternList().Add(".png");
		SetDefaultExtension(".png");
		SetNoCompress(true);
		SetPriority(1);
	}
	
	void CreateModule() override{
		SetModule(PNGCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *depngRegisterInternalModule(deModuleSystem *system){
	return new dePngModuleInternal(system);
}
#endif
