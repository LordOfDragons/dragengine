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

#include <dragengine/dragengine_configuration.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef OS_W32
	#include <dragengine/app/deOSWindows.h>
#else
	#include <sys/time.h>
#endif

#include "dePng3DTarball.h"
#include "dePng3DImageInfos.h"
#include "dePng3DModule.h"

#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>



// Structures
///////////////

struct sTarballHeader{/* byte offset */
	char name[100];			/*   0 */
	char mode[8];			/* 100 */
	char uid[8];			/* 108 */
	char gid[8];			/* 116 */
	char size[12];			/* 124 */
	char mtime[12];			/* 136 */
	char chksum[8];			/* 148 */
	char typeflag;			/* 156 */
	char linkname[100];		/* 157 */
	char magic[6];			/* 257 */
	char version[2];		/* 263 */
	char uname[32];			/* 265 */
	char gname[32];			/* 297 */
	char devmajor[8];		/* 329 */
	char devminor[8];		/* 337 */
	char prefix[155];		/* 345 */
	char padding[12];		/* 500 */
};							/* 512 */

#define REGTYPE				'0'		/* regular file */
#define AREGTYPE			'\0'	/* regular file */

struct sFeedback{
	dePng3DModule *module;
	const char *filename;
};



// PNG Callbacks
//////////////////

static void depng3dRead(png_structp readStruct, png_bytep data, png_size_t length){
	decBaseFileReader &reader = *((decBaseFileReader*)png_get_io_ptr(readStruct));
	
	reader.Read(data, (int)length);
}

static void depng3dWrite(png_structp writeStruct, png_bytep data, png_size_t length){
	decBaseFileWriter &writer = *((decBaseFileWriter*)png_get_io_ptr(writeStruct));
	
	writer.Write(data, (int)length);
}

static void depng3dWriteStatus(png_structp writeStruct, png_uint_32 row, int pass){
}

static void depng3dFlush(png_structp writeStruct){
}

static void depng3dError(png_structp errorStruct, png_const_charp message){
	const sFeedback &feedback = *((sFeedback*)png_get_error_ptr(errorStruct));
	
	feedback.module->LogErrorFormat("Error %s: %s", feedback.filename, message);
	
	DETHROW(deeInvalidAction);
}

static void depng3dWarning(png_structp errorStruct, png_const_charp message){
	const sFeedback &feedback = *((sFeedback*)png_get_error_ptr(errorStruct));
	
	feedback.module->LogWarnFormat("Warning %s: %s", feedback.filename, message);
}



// Class dePng3DTarball
/////////////////////////////

// Constructor, destructor
////////////////////////////

dePng3DTarball::dePng3DTarball(dePng3DModule *module){
	pModule = module;
}

dePng3DTarball::~dePng3DTarball(){
}



// Management
///////////////

void dePng3DTarball::Get3DImageInfos(dePng3DImageInfo &infos, decBaseFileReader &file){
	bool firstImage = true;
	sTarballHeader header;
	sImageInfos infos2D;
	unsigned short z;
	int filesize;
	int position;
	int padding;
	bool skip;
	int i;
	
	infos.width = 0;
	infos.height = 0;
	infos.depth = 0;
	infos.bitCount = 0;
	
	while(true){
		file.Read(&header, 512);
		
		// we detect the 0-blocks just by checking the file names are 0 length.
		// since 0 length file names do not exist this should be accurate enough
		if(!header.name[0] && !header.linkname[0]) break;
		
		// save the position so we can quickly skip to the end of the file
		position = file.GetPosition();
		
		// determine the file size. spaces are skipped in front of the number.
		// after that the number is read as octal value. in the end would be
		// another space but we know this so we can skip it.
		skip = true;
		filesize = 0;
		
		for(i=0; i<11; i++){
			if(skip){
				if(header.size[i] != ' '){
					skip = false;
					filesize = (int)(header.size[i] - '0');
				}
				
			}else{
				if(header.size[i] == ' '){
					break;
				}
				
				filesize = filesize * 8 + (int)(header.size[i] - '0');
			}
		}
		
		// tar requires files to be aligned in chunks so determine the amount
		// of padding 0 bytes required
		padding = 512 - (filesize % 512);
		if(padding == 512){
			padding = 0;
		}
		
// 		pModule->LogInfoFormat( "%s: header name='%s' filesize=%i padding=%i",
// 			infos.filename.GetString(), header.name, filesize, padding );
		
		// only files are accepted
		if(header.typeflag == REGTYPE || header.typeflag == AREGTYPE){
			// retrieve the z-coordinate
			z = decString(header.name).GetMiddle(1, -4).ToInt();

			// if the z coordinate is larger than the depth of the image found so far bump it up
			if((int)z >= infos.depth){
				infos.depth = (int)z + 1;
			}
				
			// retrieve the image properties
			Get2DImageInfos(infos, infos2D, file);
				
			// if this is the first image store them away
			if(firstImage){
				infos.width = infos2D.width;
				infos.height = infos2D.height;
				infos.bitCount = infos2D.bitCount;
				infos.componentCount = infos2D.componentCount;
					
				firstImage = false;
					
			// otherwise check if the properties match
			}else{
				if(infos2D.width != infos.width || infos2D.height != infos.height
				|| infos2D.bitCount != infos.bitCount || infos2D.componentCount != infos.componentCount){
					pModule->LogErrorFormat("the files in the archive '%s' do not match in size or format.", infos.filename.GetString());
					DETHROW_INFO(deeInvalidFileFormat, infos.filename.GetString());
				}
			}
		}
		
		// skip to the end of the file
		file.SetPosition(position + filesize + padding);
	}
	
	// no images in the archive
	if(firstImage){
		pModule->LogErrorFormat("no images in the archive '%s'. at least one image is required.", infos.filename.GetString());
		DETHROW_INFO(deeInvalidFileFormat, infos.filename.GetString());
	}
// 	pModule->LogInfoFormat( "%s: width=%i height=%i depth=%i bitcount=%i componentcount=%i",
// 		infos.filename.GetString(), infos.width, infos.height, infos.depth, infos.bitCount,
// 		infos.componentCount );
}

void dePng3DTarball::Load3DImage(dePng3DImageInfo &infos, decBaseFileReader &file, deImage &image){
	char *imageData = (char*)image.GetData();
	png_bytep *rows = NULL;
	sTarballHeader header;
	char *sliceImageData;
	unsigned short z;
	int strideImage;
	int strideLine;
	int filesize;
	int position;
	int padding;
	bool skip;
	int i, r;
	
	strideLine = image.GetWidth() * image.GetComponentCount() * (image.GetBitCount() >> 3);
	strideImage = strideLine * image.GetHeight();
	
	try{
		// create the rows array
		rows = new png_bytep[image.GetHeight()];
		if(!rows) DETHROW(deeOutOfMemory);
		
		// read the archive with all files filling the images one by one
		while(true){
			file.Read(&header, 512);
			
			// we detect the 0-blocks just by checking the file names are 0 length.
			// since 0 length file names do not exist this should be accurate enough
			if(!header.name[0] && !header.linkname[0]) break;
			
			// save the position so we can quickly skip to the end of the file
			position = file.GetPosition();
			
			// determine the file size. spaces are skipped in front of the number.
			// after that the number is read as octal value. in the end would be
			// another space but we know this so we can skip it.
			skip = true;
			filesize = 0;
			
			for(i=0; i<11; i++){
				if(skip){
					if(header.size[i] != ' '){
						skip = false;
						filesize = (int)(header.size[i] - '0');
					}
					
				}else{
					if(header.size[i] == ' '){
						break;
					}
					
					filesize = filesize * 8 + (int)(header.size[i] - '0');
				}
			}
			
			// tar requires files to be aligned in chunks so determine the amount
			// of padding 0 bytes required
			padding = 512 - (filesize % 512);
			if(padding == 512){
				padding = 0;
			}
			
// 			pModule->LogInfoFormat( "%s: header name='%s' filesize=%i padding=%i",
// 				infos.filename.GetString(), header.name, filesize, padding );
			
			// only files are accepted
			if(header.typeflag == REGTYPE || header.typeflag == AREGTYPE){
				// retrieve the z-coordinate
				z = decString(header.name).GetMiddle(1, -4).ToInt();

				// populate the rows array
				sliceImageData = imageData + (strideImage * (int)z);
					
				for(r=0; r<image.GetHeight(); r++){
					rows[r] = (png_bytep)(sliceImageData + strideLine * r);
				}
					
				// now we can load the image using these information
				Load2DImage(infos, file, rows);
			}
			
			// skip to the end of the file
			file.SetPosition(position + filesize + padding);
		}
		
		// free rows array
		delete [] rows;
		
	}catch(const deException &){
		if(rows) delete [] rows;
		throw;
	}
}

void dePng3DTarball::Save3DImage(decBaseFileWriter &file, const deImage &image){
	char *imageData = (char*)image.GetData();
	decMemoryFile::Ref memoryFile;
	unsigned char *headerBytes;
	char paddingBytes[512];
	struct timeval curtime;
	png_bytep *rows = NULL;
	sTarballHeader header;
	unsigned int checksum;
	char *sliceImageData;
	int pngColorType;
	int pngBitCount;
	int strideImage;
	int strideLine;
	int filesize;
	int padding;
	int z, r;
	
	// determine strides
	strideLine = image.GetWidth() * image.GetComponentCount() * (image.GetBitCount() >> 3);
	strideImage = strideLine * image.GetHeight();
	
	// determine the bit count and color type to save
	if(image.GetBitCount() == 8){
		pngBitCount = 8;
		
	}else{
		pngBitCount = 16;
	}
	
	if(image.GetComponentCount() == 1){
		pngColorType = PNG_COLOR_TYPE_GRAY;
		
	}else if(image.GetComponentCount() == 2){
		pngColorType = PNG_COLOR_TYPE_RGB;
		// not supported. needs re-parking data into RGB to feed it to PNG. problem is
		// that 2-component images have to be red-green not grayscale-alpha which is the
		// only format PNG supports. the final solution needs to write them as red-green-0.
		DETHROW(deeInvalidParam);
		
	}else if(image.GetComponentCount() == 3){
		pngColorType = PNG_COLOR_TYPE_RGB;
		
	}else if(image.GetComponentCount() == 4){
		pngColorType = PNG_COLOR_TYPE_RGB_ALPHA;
		
	}else{
		DETHROW(deeInvalidParam);
	}
	
	// build the header with the default parameters. we only have to replace
	// the name, file size and checksum later on and the rest stays the same
	memset(&header, '\0', 512);
	
	#ifdef OS_W32
		strcpy_s(&header.mode[0], sizeof(header.mode), "100600 ");
		strcpy_s(&header.uid[0], sizeof(header.uid),   "  1750 ");
		strcpy_s(&header.gid[0], sizeof(header.gid),   "   144 ");
	#else
		strcpy(&header.mode[0], "100600 ");
		strcpy(&header.uid[0],  "  1750 ");
		strcpy(&header.gid[0],  "   144 ");
	#endif

	#ifdef OS_W32
		{
		ULARGE_INTEGER x;
        ULONGLONG usec;
        static const ULONGLONG epoch_offset_us = 11644473600000000ULL;

		FILETIME filetime;
		GetSystemTimeAsFileTime(&filetime);

		x.LowPart = filetime.dwLowDateTime;
        x.HighPart = filetime.dwHighDateTime;
        usec = x.QuadPart / 10  -  epoch_offset_us;
        curtime.tv_sec  = (long)(usec / 1000000ULL);
        curtime.tv_usec = (long)(usec % 1000000ULL);
		}
	#else
		gettimeofday(&curtime, NULL);
	#endif

	header.mtime[11] = ' ';
	for(r=10; r>=0; r--){
		if(curtime.tv_sec){
			header.mtime[r] = (char)('0' + (char)(curtime.tv_sec & 0x7));
			
		}else{ // no leading 0s please
			header.mtime[r] = ' ';
		}
		curtime.tv_sec >>= 3;
	}
	
	header.typeflag = (char)REGTYPE;
	
	headerBytes = (unsigned char *)&header;
	
	// set the padding bytes to 0
	memset(&paddingBytes[0], '\0', 512);
	
	// save the file
	decMemoryFileWriter::Ref memoryFileWriter = NULL;
	
	try{
		memoryFile.TakeOver(new decMemoryFile(""));
		
		// create the rows array
		rows = new png_bytep[image.GetHeight()];
		if(!rows) DETHROW(deeOutOfMemory);
		
		// write file by creating an archive with an image for each z coordinate
		for(z=0; z<image.GetDepth(); z++){
// 			pModule->LogInfoFormat( "%s: writing %i", file.GetFilename(), z );
			// create memory file containing the file content
			memoryFileWriter.TakeOver(new decMemoryFileWriter(memoryFile, false));
			
			sliceImageData = imageData + (strideImage * z);
			
			for(r=0; r<image.GetHeight(); r++){
				rows[r] = (png_bytep)(sliceImageData + strideLine * r);
			}
			
			Save2DImage(image.GetWidth(), image.GetHeight(), *memoryFileWriter, rows, pngColorType, pngBitCount);
			memoryFileWriter = nullptr;
			
			// determine the file size and padding. tar requires files to be
			// aligned in chunks so determine the amount of padding 0 bytes
			// required to fill up to the next chunk boundary
			filesize = memoryFile->GetLength();
			padding = 512 - (filesize % 512);
			if(padding == 512){
				padding = 0;
			}
			
			// update header
			#ifdef OS_W32
				sprintf_s(&header.name[0], sizeof(header.name), "z%hu.png", (unsigned short)z);
			#else
				sprintf(&header.name[0], "z%hu.png", (unsigned short)z);
			#endif

			header.size[11] = ' ';
			for(r=10; r>=0; r--){
				if(filesize){
					header.size[r] = (char)('0' + (char)(filesize & 0x7));
					
				}else{ // no leading 0s please
					header.size[r] = ' ';
				}
				filesize >>= 3;
			}
			
			memset(&header.chksum[0], ' ', 8);
			checksum = 0;
			for(r=0; r<512; r++){
				checksum += headerBytes[r];
			}
			header.chksum[7] = '\0';
			header.chksum[6] = ' ';
			for(r=5; r>=0; r--){
				if(checksum){
					header.chksum[r] = (char)('0' + (char)(checksum & 0x7));
					
				}else{ // no leading 0s please
					header.chksum[r] = ' ';
				}
				checksum >>= 3;
			}
			
			// write the header, file data and padding 0 bytes
			file.Write(&header, 512);
			file.Write(memoryFile->GetPointer(), memoryFile->GetLength());
			if(padding > 0){
				file.Write(&paddingBytes, padding);
			}
		}
		
		// write two padding blocks at the end
		file.Write(&paddingBytes, 512);
		file.Write(&paddingBytes, 512);
		
		// free rows array
		delete [] rows;
	}catch(const deException &){
		if(rows) delete [] rows;
		throw;
	}
}



void dePng3DTarball::Get2DImageInfos(dePng3DImageInfo &infos3D, sImageInfos &infos2D, decBaseFileReader &file){
	png_structp readStruct = NULL;
	png_infop infoStruct = NULL;
	sFeedback feedback;
	int colorType;
	
	feedback.module = pModule;
	feedback.filename = infos3D.filename.GetString();
	
	try{
		// create structures
		readStruct = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, (png_voidp)&feedback,
			(png_error_ptr)depng3dError, (png_error_ptr)depng3dWarning, NULL, NULL, NULL);
		if(!readStruct) DETHROW(deeOutOfMemory);
		
		infoStruct = png_create_info_struct(readStruct);
		if(!infoStruct) DETHROW(deeOutOfMemory);
		
		// change callbacks
		png_set_read_fn(readStruct, (png_voidp)&file, (png_rw_ptr)depng3dRead);
		
		// read infos
		png_read_info(readStruct, infoStruct);
		infos2D.width = (int)png_get_image_width(readStruct, infoStruct);
		infos2D.height = (int)png_get_image_height(readStruct, infoStruct);
		infos2D.bitCount = (int)png_get_bit_depth(readStruct, infoStruct);
		colorType = (int)png_get_color_type(readStruct, infoStruct);
		
		// determine what format we need
		if(colorType == PNG_COLOR_TYPE_GRAY){
			infos2D.componentCount = 1;
			
		}else if(colorType == PNG_COLOR_TYPE_GRAY_ALPHA){
			infos2D.componentCount = 4;
			
		}else if(colorType == PNG_COLOR_TYPE_PALETTE){
			infos2D.componentCount = 3;
			
		}else if(colorType == PNG_COLOR_TYPE_RGB){
			infos2D.componentCount = 3;
			
		}else if(colorType == PNG_COLOR_TYPE_RGB_ALPHA){
			infos2D.componentCount = 4;
			
		}else{
			DETHROW_INFO(deeInvalidFileFormat, feedback.filename);
		}
		
		// bit count below 8 have to be expanded up to 8
		if(infos2D.bitCount < 8){
			infos2D.bitCount = 8;
		}
		
		// clean up
		png_destroy_read_struct(&readStruct, &infoStruct, NULL);
		
	}catch(const deException &){
		if(readStruct){
			png_destroy_read_struct(&readStruct, &infoStruct, NULL);
		}
		
		throw;
	}
}

void dePng3DTarball::Load2DImage(dePng3DImageInfo &infos3D, decBaseFileReader &file, png_bytep *rows){
	if(!rows) DETHROW(deeInvalidParam);
	
	png_structp readStruct = NULL;
	png_infop infoStruct = NULL;
	int componentCount;
	sFeedback feedback;
	int colorType;
	int bitCount;
	int height;
	int width;
	
	feedback.module = pModule;
	feedback.filename = infos3D.filename.GetString();
	
	// read image
	try{
		// create structures
		readStruct = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, (png_voidp)&feedback,
			(png_error_ptr)depng3dError, (png_error_ptr)depng3dWarning, NULL, NULL, NULL);
		if(!readStruct) DETHROW(deeOutOfMemory);
		
		infoStruct = png_create_info_struct(readStruct);
		if(!infoStruct) DETHROW(deeOutOfMemory);
		
		// change callbacks
		png_set_read_fn(readStruct, (png_voidp)&file, (png_rw_ptr)depng3dRead);
		
		// read infos
		png_read_info(readStruct, infoStruct);
		width = (int)png_get_image_width(readStruct, infoStruct);
		height = (int)png_get_image_height(readStruct, infoStruct);
		bitCount = (int)png_get_bit_depth(readStruct, infoStruct);
		colorType = (int)png_get_color_type(readStruct, infoStruct);
		
		// determine what format we need
		switch(colorType){
		case PNG_COLOR_TYPE_GRAY:
			componentCount = 1;
			break;
			
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			png_set_gray_to_rgb(readStruct);
			componentCount = 4;
			break;
			
		case PNG_COLOR_TYPE_PALETTE:
			componentCount = 3;
			break;
			
		case PNG_COLOR_TYPE_RGB:
			componentCount = 3;
			break;
			
		case PNG_COLOR_TYPE_RGB_ALPHA:
			componentCount = 4;
			break;
			
		default:
			DETHROW_INFO(deeInvalidFileFormat, feedback.filename);
		}
		
		// NOTE libpng has separate function calls all linked to png_set_expand but using
		//      them is discuraged since it is not future proof. thus png_set_expand is
		//      used for all expanding cases letting libpng do the right thing
		
		// if there is a palette expand it into real colors
		if(colorType == PNG_COLOR_TYPE_PALETTE){
			png_set_expand(readStruct);
		}
		
		// bit count below 8 have to be expanded up to 8
		if(bitCount < 8){
			png_set_expand(readStruct);
			bitCount = 8;
		}
		
		// expand transparency values
		if(png_get_valid(readStruct, infoStruct, PNG_INFO_tRNS)){
			png_set_expand(readStruct);
			
			switch(colorType){
			case PNG_COLOR_TYPE_PALETTE:
			case PNG_COLOR_TYPE_RGB:
				componentCount = 4;
				break;
				
			case PNG_COLOR_TYPE_GRAY:
				png_set_gray_to_rgb(readStruct);
				componentCount = 4;
				break;
				
			default:
				break;
			}
		}
		
		// 16 bit is stored in network byte order but we need it the other way round
		if(bitCount == 16){
			png_set_swap(readStruct);
		}
		
		// and now the big update... we are ready to go
		png_read_update_info(readStruct, infoStruct);
		
		// check if the parameters match the image. it should never be a mismatch
		// but you never know if somebody messed with the loading process
		if(width != infos3D.width || height != infos3D.height
		|| bitCount != infos3D.bitCount || componentCount != infos3D.componentCount){
			DETHROW(deeInvalidParam);
		}
		
		// read the image
		png_read_image(readStruct, rows);
		
		// finish file
		png_read_end(readStruct, NULL);
		
		// clean up
		png_destroy_read_struct(&readStruct, &infoStruct, NULL);
		
	}catch(const deException &){
		if(readStruct){
			png_destroy_read_struct(&readStruct, &infoStruct, NULL);
		}
		
		throw;
	}
}

void dePng3DTarball::Save2DImage(int width, int height, decBaseFileWriter &file, png_bytep *rows, int pngColorType, int pngBitCount){
	if(!rows) DETHROW(deeInvalidParam);
	
	png_structp writeStruct = NULL;
	png_infop infoStruct = NULL;
	sFeedback feedback;
	
	feedback.module = pModule;
	feedback.filename = file.GetFilename();
	
	try{
		// create structures
		writeStruct = png_create_write_struct_2(PNG_LIBPNG_VER_STRING, (png_voidp)&feedback,
			(png_error_ptr)depng3dError, (png_error_ptr)depng3dWarning, NULL, NULL, NULL);
		if(!writeStruct) DETHROW(deeOutOfMemory);
		
		infoStruct = png_create_info_struct(writeStruct);
		if(!infoStruct) DETHROW(deeOutOfMemory);
		
		// change callbacks
		png_set_write_fn(writeStruct, (png_voidp)&file, (png_rw_ptr)depng3dWrite, (png_flush_ptr)depng3dFlush);
		png_set_write_status_fn (writeStruct, (png_write_status_ptr)depng3dWriteStatus);
		
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
		png_set_compression_level(writeStruct, 6);
		
		/* set other zlib parameters */
		/*png_set_compression_mem_level(png_ptr, 8);
		png_set_compression_strategy(png_ptr,
			Z_DEFAULT_STRATEGY);
		png_set_compression_window_bits(png_ptr, 15);
		png_set_compression_method(png_ptr, 8);
		png_set_compression_buffer_size(png_ptr, 8192)*/
		
		// fill in the infos
		png_set_IHDR(writeStruct, infoStruct, width, height, pngBitCount, pngColorType,
			PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
		
		//png_set_tIME( writeStruct, infoStruct, PNG_VALID_tIME );
		
		// make image data available to the library
		png_set_rows(writeStruct, infoStruct, rows);
		
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
		png_write_png(writeStruct, infoStruct, PNG_TRANSFORM_SWAP_ENDIAN, NULL);
		
		// clean up
		png_destroy_write_struct(&writeStruct, &infoStruct);
		
	}catch(const deException &){
		if(writeStruct || infoStruct) png_destroy_write_struct(&writeStruct, &infoStruct);
		throw;
	}
}
