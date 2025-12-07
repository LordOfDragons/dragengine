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



// Class decDiskFileReader
///////////////////////////

// Constructor, Destructor
////////////////////////////

decDiskFileReader::decDiskFileReader(const char *filename){
	if(! filename){
		DETHROW(deeInvalidParam);
	}
	
	struct stat st;
	
	pFile = NULL;
	
	try{
		pFilename = filename;
		
		// Android silently kills an application if a file does not exist.
		// for this reason the file is first stat-ed before opened. if the
		// file does not exist this prevents a strange crash. for all other
		// platforms this order of action is fine too
		if(stat(filename, &st)){
			DETHROW(deeFileNotFound);
		}
		
		pFile = fopen(filename, "rb");
		if(! pFile){
			DETHROW(deeFileNotFound);
		}
		
		pLength = (int)st.st_size;
		pModificationTime = (TIME_SYSTEM)st.st_mtime;
		
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
		DETHROW(deeReadFile);
	}
}

void decDiskFileReader::MovePosition(int offset){
	if(fseek(pFile, offset, SEEK_CUR)){
		DETHROW(deeReadFile);
	}
}

void decDiskFileReader::SetPositionEnd(int position){
	if(fseek(pFile, position, SEEK_END)){
		DETHROW(deeReadFile);
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
	
	if(! endOfFile){
		DETHROW(deeReadFile);
	}
}
