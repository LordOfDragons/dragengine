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

#include "decDiskFileWriter.h"
#include "../exceptions.h"
#include "../../dragengine_configuration.h"

#ifdef OS_W32
#include "../../app/deOSWindows.h"
#endif



// Class decDiskFileWriter
///////////////////////////

// Constructor, destructor
////////////////////////////

decDiskFileWriter::decDiskFileWriter(const char *filename, bool append) :
pFilename(filename),
pFile(NULL)
{
#ifdef OS_W32
	wchar_t widePath[MAX_PATH];
	deOSWindows::Utf8ToWide(filename, widePath, MAX_PATH);
	
#ifdef OS_W32_VS
	pFile = _wfsopen(widePath, append ? L"ab" : L"wb", _SH_DENYNO);
	if(!pFile){
		errno_t result = 0;
		_get_errno(&result);
		DETHROW_INFO(deeFileNotFound, pFormatError(result));
	}

#else
	pFile = _wfopen(widePath, append ? L"ab" : L"wb");
#endif
	
#else
	pFile = fopen(filename, append ? "ab" : "wb");
#endif
	if(!pFile){
		DETHROW_INFO(deeFileNotFound, filename);
	}
}

decDiskFileWriter::~decDiskFileWriter(){
	if(pFile){
		fclose(pFile);
	}
}



// Management
///////////////

const char *decDiskFileWriter::GetFilename(){
	return pFilename.GetString();
}



// Seeking
////////////

int decDiskFileWriter::GetPosition(){
	return (int)ftell(pFile);
}

void decDiskFileWriter::SetPosition(int position){
	if(fseek(pFile, position, SEEK_SET)){
		DETHROW_INFO(deeReadFile, pFilename);
	}
}

void decDiskFileWriter::MovePosition(int offset){
	if(fseek(pFile, offset, SEEK_CUR)){
		DETHROW(deeReadFile);
	}
}

void decDiskFileWriter::SetPositionEnd(int position){
	if(fseek(pFile, position, SEEK_END)){
		DETHROW(deeReadFile);
	}
}



// Reading
////////////

void decDiskFileWriter::Write(const void *buffer, int size){
	if(size == 0){
		return;
	}
	
	if(fwrite(buffer, size, 1, pFile) != 1){
		DETHROW_INFO(deeWriteFile, pFilename);
	}
}

decBaseFileWriter::Ref decDiskFileWriter::Duplicate(){
	const decBaseFileWriter::Ref writer(decBaseFileWriter::Ref::New(
		new decDiskFileWriter(pFilename, true)));
	if(fseek(((decDiskFileWriter&)(decBaseFileWriter&)writer).pFile, ftell(pFile), SEEK_SET)){
		DETHROW_INFO(deeReadFile, pFilename);
	}
	return writer;
}



// Private Functions
//////////////////////

#ifdef OS_W32_VS
decString decDiskFileWriter::pFormatError(errno_t error) const{
	char buffer[100];
	strerror_s(buffer, sizeof(buffer), error);

	decString message;
	message.Format("%s: %s (%d)", pFilename, buffer, error);
	return message;
}
#endif
