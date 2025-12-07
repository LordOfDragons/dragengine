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

#include "deJpegImageInfos.h"



// Class deJpegImageInfo
//////////////////////////

// Constructor, destructor
////////////////////////////

deJpegImageInfo::deJpegImageInfo(deJpegModule *module, const char *filename) :
pModule(module),

pFilename(filename),
pFileSize(0),
pFilePosition(0),

pDataBuffer(NULL),
pDataBufferPosition(0),
pReader(NULL)
{
	if(! module || ! filename){
		DETHROW(deeInvalidParam);
	}
	
	memset(&pErrorMgr, '\0', sizeof(pErrorMgr));
	memset(&pSourceMgr, '\0', sizeof(pSourceMgr));
	memset(&pDecompress, '\0', sizeof(pDecompress));
	pDecompress.client_data = this;
	pDecompress.err = &pErrorMgr;
}

deJpegImageInfo::~deJpegImageInfo(){
	if(pDataBuffer){
		delete [] pDataBuffer;
	}
}



// Management
///////////////



// Data Buffer
////////////////

void deJpegImageInfo::InitRead(decBaseFileReader *reader){
	if(! reader){
		DETHROW(deeInvalidParam);
	}
	
	pReader = reader;
	
	pFileSize = reader->GetLength();
	pFilePosition = 0;
	
	if(! pDataBuffer){
		pDataBuffer = new JOCTET[1024];
	}
	pDataBufferPosition = 0;
	
	pSourceMgr.bytes_in_buffer = 0;
	pDecompress.src = &pSourceMgr;
}

void deJpegImageInfo::ReadNext(){
	if(! pReader || ! pDataBuffer){
		DETHROW(deeInvalidParam);
	}
	
	const int bytes = decMath::min(pFileSize - pFilePosition, 1024);
	
	if(bytes > 0){
		pReader->Read(pDataBuffer, bytes);
		pFilePosition += bytes;
	}
	
	pDataBufferPosition = 0;
	
	pSourceMgr.next_input_byte = pDataBuffer;
	pSourceMgr.bytes_in_buffer = (size_t)bytes;
}

void deJpegImageInfo::SkipNext(int bytes){
	if(! pReader || ! pDataBuffer){
		DETHROW(deeInvalidParam);
	}
	
// 	printf( "Skip bytes=%i dbp=%i\n", bytes, pDataBufferPosition );
	
	/*
	pDataBufferPosition += bytes;
	
	if(pDataBufferPosition > 1024){
		pReader->MovePosition(pDataBufferPosition - 1024);
		pDataBufferPosition = 1024;
	}
	
	pSourceMgr.next_input_byte = pDataBuffer + pDataBufferPosition;
	pSourceMgr.bytes_in_buffer = (size_t)(1024 - pDataBufferPosition);
	*/
	
	if(bytes <= 0){
		return;
	}
	
	while(bytes > (int)pSourceMgr.bytes_in_buffer){
		bytes -= (int)pSourceMgr.bytes_in_buffer;
		ReadNext();
	}
	pSourceMgr.next_input_byte += bytes;
	pSourceMgr.bytes_in_buffer -= bytes;
}

void deJpegImageInfo::CloseReader(){
	if(pDataBuffer){
		delete [] pDataBuffer;
		pDataBuffer = NULL;
	}
	pReader = NULL;
}



// Notifications
//////////////////

int deJpegImageInfo::GetWidth(){
	return (int)pDecompress.image_width;
}

int deJpegImageInfo::GetHeight(){
	return (int)pDecompress.image_height;
}

int deJpegImageInfo::GetDepth(){
	return 1;
}

int deJpegImageInfo::GetComponentCount(){
	return (int)pDecompress.num_components;
}

int deJpegImageInfo::GetBitCount(){
	return 8;
}
