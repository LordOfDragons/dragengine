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

#include "deFileResourceManager.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../common/file/decBaseFileReader.h"
#include "../common/file/decBaseFileWriter.h"
#include "../common/file/decPath.h"
#include "../filesystem/deVirtualFileSystem.h"
#include "../systems/deInputSystem.h"
#include "../systems/modules/input/deBaseInputModule.h"



// Class deFileResourceManager
////////////////////////////////

// Constructor, destructor
////////////////////////////

deFileResourceManager::deFileResourceManager(deEngine *engine, eResourceType type) :
deResourceManager(engine, type){
}

deFileResourceManager::~deFileResourceManager(){
}



// Management
///////////////

bool deFileResourceManager::FindFileForReading(decPath &path, const deVirtualFileSystem &vfs,
const char *filename, const char *basePath) const{
	if(!decPath::IsUnixPathAbsolute(basePath)){
		DETHROW(deeInvalidParam);
	}
	
	if(decPath::IsUnixPathAbsolute(filename)){
		path.SetFromUnix(filename);
		if(vfs.ExistsFile(path)){
			return true;
		}
		
	}else{
		path.SetFromUnix(basePath);
		path.AddUnixPath(filename);
		if(vfs.ExistsFile(path)){
			return true;
		}
	}
	
	return false;
}

decBaseFileReader::Ref deFileResourceManager::OpenFileForReading(
const deVirtualFileSystem &vfs, const char *filename) const{
	return vfs.OpenFileForReading(decPath::CreatePathUnix(filename));
}

decBaseFileWriter::Ref deFileResourceManager::OpenFileForWriting(
const deVirtualFileSystem &vfs, const char *filename) const{
	return vfs.OpenFileForWriting(decPath::CreatePathUnix(filename));
}
