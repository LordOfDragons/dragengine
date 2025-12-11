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
#include <sys/stat.h>

#include "decDiskFileReader.h"
#include "../exceptions.h"
#include "../../dragengine_configuration.h"

#ifdef OS_W32
#include "../../app/deOSWindows.h"
#endif



// Class decDiskFileReader
///////////////////////////

// Constructor, Destructor
////////////////////////////

decDiskFileReader::decDiskFileReader(const char *filename){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	pFile = NULL;
	
	
	
	try{
		pFilename = filename;
		
#ifdef OS_W32
		wchar_t widePath[MAX_PATH];
		deOSWindows::Utf8ToWide(filename, widePath, MAX_PATH);
		
		WIN32_FILE_ATTRIBUTE_DATA fa;
		if(!GetFileAttributesExW(widePath, GetFileExInfoStandard, &fa)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		
		#ifdef OS_W32_VS
		pFile = _wfsopen(widePath, L"rb", _SH_DENYNO);
		if(!pFile){
			errno_t result = 0;
			_get_errno(&result);
			DETHROW_INFO(deeFileNotFound, pFormatError(result));
		}
		#else
		pFile = _wfopen(widePath, L"rb");
		if(!pFile){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		#endif
		
		pLength = (int)(((uint64_t)fa.nFileSizeHigh << 32) + (uint64_t)fa.nFileSizeLow);
		
		SYSTEMTIME stime;
		if(!FileTimeToSystemTime(&fa.ftLastWriteTime, &stime)){
			DETHROW(deeInvalidParam);
		}
		
		decDateTime modTime;
		modTime.SetYear(stime.wYear);
		modTime.SetMonth(stime.wMonth - 1);
		modTime.SetDay(stime.wDay - 1);
		modTime.SetHour(stime.wHour);
		modTime.SetMinute(stime.wMinute);
		modTime.SetSecond(stime.wSecond);
		
		pModificationTime = modTime.ToSystemTime();
		
#else
		// Android silently kills an application if a file does not exist.
		// for this reason the file is first stat-ed before opened. if the
		// file does not exist this prevents a strange crash. for all other
		// platforms this order of action is fine too
		struct stat st;
		
		if(stat(filename, &st)){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		
		pFile = fopen(filename, "rb");
		if(!pFile){
			DETHROW_INFO(deeFileNotFound, filename);
		}
		
		pLength = (int)st.st_size;
		pModificationTime = (TIME_SYSTEM)st.st_mtime;
#endif
		
	}catch(const deException &){
		if(pFile){
			fclose(pFile);
		}
		throw;
	}
}

decDiskFileReader::~decDiskFileReader(){
	if(pFile){
		fclose(pFile);
	}
}



// Management
///////////////

const char *decDiskFileReader::GetFilename(){
	return pFilename;
}

int decDiskFileReader::GetLength(){
	return pLength;
}

TIME_SYSTEM decDiskFileReader::GetModificationTime(){
	return pModificationTime;
}



// Seeking
////////////

int decDiskFileReader::GetPosition(){
	return (int)ftell(pFile);
}

void decDiskFileReader::SetPosition(int position){
	if(fseek(pFile, position, SEEK_SET)){
		DETHROW_INFO(deeReadFile, pFilename);
	}
}

void decDiskFileReader::MovePosition(int offset){
	if(fseek(pFile, offset, SEEK_CUR)){
		DETHROW_INFO(deeReadFile, pFilename);
	}
}

void decDiskFileReader::SetPositionEnd(int position){
	if(fseek(pFile, position, SEEK_END)){
		DETHROW_INFO(deeReadFile, pFilename);
	}
}



// Reading
////////////

void decDiskFileReader::Read(void *buffer, int size){
	const int readBytes = (int)fread(buffer, 1, size, pFile);
	
	if(readBytes == size){
		return;
	}
	
	const bool endOfFile = (feof(pFile) != 0);
	clearerr(pFile);  // required to support growing files
	
	if(!endOfFile){
		DETHROW_INFO(deeReadFile, pFilename);
	}
}

decBaseFileReader::Ref decDiskFileReader::Duplicate(){
	const decDiskFileReader::Ref reader(decDiskFileReader::Ref::NewWith(pFilename));
	if(fseek(reader->pFile, ftell(pFile), SEEK_SET)){
		DETHROW_INFO(deeReadFile, pFilename);
	}
	return reader;
}



// Private Functions
//////////////////////

#ifdef OS_W32_VS
decString decDiskFileReader::pFormatError(errno_t error) const{
	char buffer[100];
	strerror_s(buffer, sizeof(buffer), error);

	decString message;
	message.Format("%s: %s (%d)", pFilename, buffer, error);
	return message;
}
#endif
