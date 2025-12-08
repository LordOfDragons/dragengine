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

#include "deoglDelayedFileWrite.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class deoglDelayedFileWrite
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDelayedFileWrite::deoglDelayedFileWrite(const decPath &path) :
pMemoryFile(NULL),
pPath(path){
	try{
		pMemoryFile.TakeOverWith(path.GetPathUnix());
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglDelayedFileWrite::~deoglDelayedFileWrite(){
	pCleanUp();
}



// Management
///////////////

decBaseFileWriter *deoglDelayedFileWrite::GetFileWriter() const{
	return new decMemoryFileWriter(pMemoryFile, false);
}

void deoglDelayedFileWrite::SaveFile(deVirtualFileSystem &vfs){
	decBaseFileWriter *fileWriter = NULL;
	decPath noConstPath(pPath);
	
	try{
		fileWriter = vfs.OpenFileForWriting(noConstPath);
		fileWriter->Write(pMemoryFile->GetPointer(), pMemoryFile->GetLength());
		fileWriter->FreeReference();
		
	}catch(const deException &){
		if(fileWriter){
			fileWriter->FreeReference();
		}
		throw;
	}
}



// Private Functions
//////////////////////

void deoglDelayedFileWrite::pCleanUp(){
}
