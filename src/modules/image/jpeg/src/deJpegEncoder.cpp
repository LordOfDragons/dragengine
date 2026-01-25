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

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileWriter.h>

// include last to avoid conflicts with INT32 typedef and windows
#include "deJpegEncoder.h"



// Class deJpegEncoder
////////////////////////

// Constructor, destructor
////////////////////////////

deJpegEncoder::deJpegEncoder(deJpegModule *module){
	DEASSERT_NOTNULL(module)
	
	pModule = module;
	
	memset(&pErrorMgr, '\0', sizeof(pErrorMgr));
	memset(&pDestMgr, '\0', sizeof(pDestMgr));
	memset(&pEncode, '\0', sizeof(pEncode));
	pEncode.client_data = this;
	pEncode.err = &pErrorMgr;
}

deJpegEncoder::~deJpegEncoder() = default;


// Data Buffer
////////////////

void deJpegEncoder::InitWrite(decBaseFileWriter *writer){
	if(!writer){
		DETHROW(deeInvalidParam);
	}
	
	pWriter = writer;
	
	if(pDataBuffer.IsEmpty()){
		pDataBuffer.AddRange(1024, {});
	}
	
	pEncode.dest = &pDestMgr;
}

void deJpegEncoder::ResetBuffer(){
	pDestMgr.next_output_byte = pDataBuffer.GetArrayPointer();
	pDestMgr.free_in_buffer = 1024;
}

void deJpegEncoder::WriteEntireBuffer(){
	pWriter->Write(pDataBuffer.GetArrayPointer(), 1024);
	ResetBuffer();
}

void deJpegEncoder::WriteRemaining(){
	const int bytes = 1024 - (int)pDestMgr.free_in_buffer;
	
	if(bytes > 0){
		pWriter->Write(pDataBuffer.GetArrayPointer(), bytes);
	}
	
	pDestMgr.next_output_byte = nullptr;
	pDestMgr.free_in_buffer = 0;
}

void deJpegEncoder::CloseReader(){
	pWriter = nullptr;
}
