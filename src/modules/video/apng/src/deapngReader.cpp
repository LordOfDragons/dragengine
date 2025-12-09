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

#include "deapngReader.h"
#include "deVideoApng.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/resources/video/deVideo.h>



// PNG callbacks
//////////////////

static void deapngRead(png_structp readStruct, png_bytep data, png_size_t length){
	const deapngReader &reader = *((deapngReader*)png_get_io_ptr(readStruct));
	reader.GetReader()->Read(data, (int)length);
}

static void deapngError(png_structp errorStruct, png_const_charp message){
	const deapngReader &reader = *((deapngReader*)png_get_error_ptr(errorStruct));
	reader.GetModule().LogErrorFormat("Error %s: %s", reader.GetReader()->GetFilename(), message);
	DETHROW(deeInvalidAction);
}

static void deapngWarning(png_structp errorStruct, png_const_charp message){
	const deapngReader &reader = *((deapngReader*)png_get_error_ptr(errorStruct));
	reader.GetModule().LogWarnFormat("Warning %s: %s", reader.GetReader()->GetFilename(), message);
}



// Class deapngReader
///////////////////////

// Constructor, destructor
////////////////////////////

deapngReader::deapngReader(deVideoApng &module, decBaseFileReader *reader) :
pModule(module),
pReadStruct(nullptr),
pInfoStruct(nullptr),
pWidth(0),
pHeight(0),
pComponentCount(0),
pFrameCount(0),
pFrameRate(0.0f),
pFirstFrame(0),
pRowLength(0),
pImageSize(0),
pCurFrame(0),
pLastFrameX(0),
pLastFrameY(0),
pLastFrameWidth(0),
pLastFrameHeight(0),
pLastFrameDop(PNG_DISPOSE_OP_NONE),
pAccumData(nullptr),
pAccumRows(nullptr),
pFrameData(nullptr),
pFrameRows(nullptr),
pLastFrameData(nullptr),
pLastFrameRows(nullptr),
pErrorState(false)
{
	if(!reader){
		DETHROW(deeInvalidParam);
	}
	
	try{
		pReader = reader;
		// create structs
		pReadStruct = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, (png_voidp)this,
			(png_error_ptr)deapngError, (png_error_ptr)deapngWarning, NULL, NULL, NULL);
		if(!pReadStruct){
			DETHROW(deeOutOfMemory);
		}
		
		pInfoStruct = png_create_info_struct(pReadStruct);
		if(!pInfoStruct){
			DETHROW(deeOutOfMemory);
		}
		
		// set callbacks
		png_set_read_fn(pReadStruct, (png_voidp)this, (png_rw_ptr)deapngRead);
		
		// read header
		pReadHeader();
		
		// create accum and frame data and rows table
		int i;
		
		pAccumData = new png_byte[pImageSize];
		pAccumRows = new png_bytep[pHeight];
		for(i=0; i<pHeight; i++){
			pAccumRows[i] = pAccumData + pRowLength * i;
		}
		
		pFrameData = new png_byte[pImageSize];
		pFrameRows = new png_bytep[pHeight];
		for(i=0; i<pHeight; i++){
			pFrameRows[i] = pFrameData + pRowLength * i;
		}
		
		pLastFrameData = new png_byte[pImageSize];
		pLastFrameRows = new png_bytep[pHeight];
		for(i=0; i<pHeight; i++){
			pLastFrameRows[i] = pLastFrameData + pRowLength * i;
		}
		
		pLastFrameWidth = pWidth;
		pLastFrameHeight = pHeight;
		
		// clear to transparent black
		memset(pAccumData, '\0', pImageSize);
		
	}catch(const deException &e){
		pModule.LogErrorFormat("Failed to create reader for file %s", reader->GetFilename());
		pModule.LogException(e);
		pCleanUp();
		throw;
	}
}

deapngReader::~deapngReader(){
	pCleanUp();
}



// Management
///////////////

void deapngReader::Rewind(){
	if(pErrorState){
		return;
	}
	
	if(pReadStruct){
		png_destroy_read_struct(&pReadStruct, &pInfoStruct, NULL);
		pReadStruct = NULL;
		pInfoStruct = NULL;
		
		pReadStruct = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, (png_voidp)this,
			(png_error_ptr)deapngError, (png_error_ptr)deapngWarning, NULL, NULL, NULL);
		if(!pReadStruct){
			DETHROW(deeOutOfMemory);
		}
		
		pInfoStruct = png_create_info_struct(pReadStruct);
		if(!pInfoStruct){
			DETHROW(deeOutOfMemory);
		}
		
		png_set_read_fn(pReadStruct, (png_voidp)this, (png_rw_ptr)deapngRead);
	}
	
	pReader->SetPosition(0);
	pReadHeader();
	pCurFrame = 0;
	pLastFrameX = 0;
	pLastFrameY = 0;
	pLastFrameWidth = pWidth;
	pLastFrameHeight = pHeight;
	pLastFrameDop = PNG_DISPOSE_OP_NONE;
	
	// clear to transparent black
	memset(pAccumData, '\0', pImageSize);
}

void deapngReader::SeekFrame(int frame){
	if(pErrorState){
		return;
	}
	
	// APNG does not support seeking. if the frame is located after the current frame skip
	// ahead to the frame. if frame is before the current frame we have to rewind the file
	// reader, parse the PNG header again and skip to the frame
	if(frame < pCurFrame){
		Rewind();
	}
	
	while(pCurFrame < frame){
		ReadImage();
	}
}

void deapngReader::ReadImage(){
	if(pErrorState){
		return;
	}
	
	// read next image. in case of an error stop reading and enter error state instead of
	// throwing an exception.
	try{
		pReadImage();
		
	}catch(const deException &){
		pEnterErrorState();
	}
}

void deapngReader::CopyAccumImage(void *buffer, int size) const{
	if(!buffer){
		DETHROW(deeInvalidParam);
	}
	if(size != pImageSize){
		DETHROW(deeInvalidParam);
	}
	
	memcpy(buffer, pAccumData, pImageSize);
}



// Private Functions
//////////////////////

void deapngReader::pCleanUp(){
	if(pLastFrameRows){
		delete [] pLastFrameRows;
	}
	if(pLastFrameData){
		delete [] pLastFrameData;
	}
	if(pFrameRows){
		delete [] pFrameRows;
	}
	if(pFrameData){
		delete [] pFrameData;
	}
	if(pAccumRows){
		delete [] pAccumRows;
	}
	if(pAccumData){
		delete [] pAccumData;
	}
	if(pReadStruct){
		png_destroy_read_struct(&pReadStruct, &pInfoStruct, NULL);
	}
}

void deapngReader::pReadHeader(){
	png_read_info(pReadStruct, pInfoStruct);
	pWidth = (int)png_get_image_width(pReadStruct, pInfoStruct);
	pHeight = (int)png_get_image_height(pReadStruct, pInfoStruct);
	const int bitCount = (int)png_get_bit_depth(pReadStruct, pInfoStruct);
	const int colorType = (int)png_get_color_type(pReadStruct, pInfoStruct);
	
	// determine what format we need
	switch(colorType){
	case PNG_COLOR_TYPE_GRAY:
	case PNG_COLOR_TYPE_PALETTE:
		pComponentCount = 3;
		png_set_gray_to_rgb(pReadStruct);
		break;
		
	case PNG_COLOR_TYPE_RGB:
		pComponentCount = 3;
		break;
		
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		pComponentCount = 4;
		png_set_gray_to_rgb(pReadStruct);
		break;
		
	case PNG_COLOR_TYPE_RGB_ALPHA:
		pComponentCount = 4;
		break;
		
	default:
		DETHROW_INFO(deeInvalidFileFormat, pReader->GetFilename());
	}
	
	// NOTE libpng has separate function calls all linked to png_set_expand but using
	//      them is discuraged since it is not future proof. thus png_set_expand is
	//      used for all expanding cases letting libpng do the right thing
	
	// if there is a palette expand it into real colors
	if(colorType == PNG_COLOR_TYPE_PALETTE){
		png_set_expand(pReadStruct);
	}
	
	// bit count below 8 have to be expanded up to 8
	if(bitCount < 8){
		png_set_expand(pReadStruct);
	}
	
	// expand transparency values
	if(png_get_valid(pReadStruct, pInfoStruct, PNG_INFO_tRNS)){
		png_set_expand(pReadStruct);
		
		switch(colorType){
		case PNG_COLOR_TYPE_PALETTE:
		case PNG_COLOR_TYPE_RGB:
			pComponentCount = 4;
			break;
			
		case PNG_COLOR_TYPE_GRAY:
			pComponentCount = 4;
			png_set_gray_to_rgb(pReadStruct);
			break;
			
		default:
			break;
		}
	}
	
	// 16 bit is stored in network byte order but we need it the other way round.
	// but 16 bit can not be handled right now so reduce it to 8 bit
	if(bitCount == 16){
		png_set_swap(pReadStruct);
		png_set_strip_16(pReadStruct);
	}
	
	// and now the big update... we are ready to go
	png_read_update_info(pReadStruct, pInfoStruct);
	
	pRowLength = pWidth * pComponentCount;
	pImageSize = pRowLength * pHeight;
	
	// check for animated png
	if(!png_get_valid(pReadStruct, pInfoStruct, PNG_INFO_acTL)){
		DETHROW_INFO(deeInvalidFileFormat, pReader->GetFilename());
	}
	
	if(png_get_first_frame_is_hidden(pReadStruct, pInfoStruct) != 0){
		pFirstFrame = 1;
	}
	pFrameCount = png_get_num_frames(pReadStruct, pInfoStruct);
	// number of times to play the animation: png_get_num_plays( pReadStruct, pInfoStruct );
	
	png_read_frame_head(pReadStruct, pInfoStruct);
	const int delayNum = png_get_next_frame_delay_num(pReadStruct, pInfoStruct);
	const int delayDenom = png_get_next_frame_delay_den(pReadStruct, pInfoStruct);
	pFrameRate = (float)delayDenom / (float)delayNum;
}

void deapngReader::pReadImage(){
	if(pLastFrameDop == PNG_DISPOSE_OP_PREVIOUS){
		png_uint_32 y;
		for(y=0; y<pLastFrameHeight; y++){
			const int offsetY = pHeight - 1 - (pLastFrameY + y);
			const int offsetX = pComponentCount * pLastFrameX;
			memcpy(pAccumRows[offsetY] + offsetX, pLastFrameRows[offsetY] + offsetX,
				pComponentCount * pLastFrameWidth);
		}
		
	}else if(pLastFrameDop == PNG_DISPOSE_OP_BACKGROUND){
		// works for both RGB and RGBA
		png_uint_32 y;
		for(y=0; y<pLastFrameHeight; y++){
			memset(pAccumRows[pHeight - 1 - (pLastFrameY + y)] + pComponentCount * pLastFrameX,
				'\0', pComponentCount * pLastFrameWidth);
		}
	}
	
	// get frame information
	png_uint_32 x0 = 0;
	png_uint_32 y0 = 0;
	png_uint_32 w0 = pWidth;
	png_uint_32 h0 = pHeight;
	unsigned short delay_num = 1;
	unsigned short delay_den = 10;
	unsigned char dop = PNG_DISPOSE_OP_NONE;
	unsigned char bop = PNG_BLEND_OP_SOURCE;
	png_get_next_frame_fcTL(pReadStruct, pInfoStruct, &w0, &h0, &x0, &y0, &delay_num, &delay_den, &dop, &bop);
	
	if(pCurFrame == pFirstFrame){
		bop = PNG_BLEND_OP_SOURCE;
		
		if(dop == PNG_DISPOSE_OP_PREVIOUS){
			dop = PNG_DISPOSE_OP_BACKGROUND;
		}
	}
	
	if(dop == PNG_DISPOSE_OP_PREVIOUS){
		png_uint_32 y;
		for(y=0; y<h0; y++){
			const int offsetY = pHeight - 1 - (y0 + y);
			const int offsetX = pComponentCount * x0;
			memcpy(pLastFrameRows[offsetY] + offsetX, pAccumRows[offsetY] + offsetX,
				pComponentCount * w0);
		}
	}
	
	pLastFrameX = x0;
	pLastFrameY = y0;
	pLastFrameWidth = w0;
	pLastFrameHeight = h0;
	pLastFrameDop = dop;
	
	// printf( "ReadImage frame=%i dop=%i bop=%i (%dx%d)(%dx%d)\n", pCurFrame, dop, bop, x0, y0, w0, h0 );
	// read image into frame rows so we can process it
	png_read_image(pReadStruct, pFrameRows);
	
	// combine the frame rows with the accum rows depending on dispose mode
	if(bop == PNG_BLEND_OP_SOURCE || pComponentCount < 4){
		png_uint_32 y;
		for(y=0; y<h0; y++){
			memcpy(pAccumRows[pHeight - 1 - (y0 + y)] + pComponentCount * x0,
				pFrameRows[y], pComponentCount * w0);
		}
		
	}else{ // bop == PNG_BLEND_OP_OVER, only works on RGBA images
		unsigned int x, y;
		
		for(y=0; y<h0; y++){
			unsigned char *sp = pFrameRows[y];
			unsigned char *dp = pAccumRows[pHeight - 1 - (y0 + y)] + x0 * 4;
			
			for(x=0; x<w0; x++, sp+=4, dp+=4){
				if(sp[3] == 255){
					memcpy(dp, sp, 4);
					
				}else if(sp[3] != 0){
					if(dp[3] != 0){
						const int u = sp[3] * 255;
						const int v = (255 - sp[3]) * dp[3];
						const int al = u + v;
						dp[0] = (sp[0] * u + dp[0] * v) / al;
						dp[1] = (sp[1] * u + dp[1] * v) / al;
						dp[2] = (sp[2] * u + dp[2] * v) / al;
						dp[3] = al / 255;
						
					}else{
						memcpy(dp, sp, 4);
					}
				}
			}  
		}
	}
	
	// advance to the next frame if not reaching the end
	if(pCurFrame < pFrameCount - 1){
		png_read_frame_head(pReadStruct, pInfoStruct); // read next header
	}
	pCurFrame++;
}

void deapngReader::pEnterErrorState(){
	// for the time being error state image is black image. could be done better by using
	// for example a red X on black background or anything like that
	int y;
	
	pErrorState = true;
	
	if(pComponentCount == 4){
		int x;
		
		for(y=0; y<pHeight; y++){
			uint32_t * const rowAccum = (uint32_t*)pAccumRows[y];
			uint32_t * const rowFrame = (uint32_t*)pFrameRows[y];
			
			for(x=0; x<pWidth; x++){
				rowAccum[x] = 0xff000000;
				rowFrame[x] = 0xff000000;
			}
		}
		
	}else{
		for(y=0; y<pHeight; y++){
			memset(pAccumRows[y], '\0', pRowLength);
			memset(pFrameRows[y], '\0', pRowLength);
		}
	}
}
