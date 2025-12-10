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

#include "delEngineModule.h"
#include "../delEngine.h"
#include "../../delLauncher.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/extern/sha1/sha1.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/modules/deInternalModule.h>



// Class delEngineModule
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

delEngineModule::delEngineModule() :
pType(deModuleSystem::emtUnknown),
pPriority(0),
pIsFallback(false),
pStatus(emsNotTested),
pErrorCode(0),

pLibFileSizeShould(0),
pLibFileSizeIs(0)
{
	pLibFileHashIs.Set('0', 20);
	pLibFileHashShould.Set('0', 20);
}

delEngineModule::delEngineModule(const deInternalModule &module) :
pType(module.GetType()),
pName(module.GetName()),
pDescription(decUnicodeString::NewFromUTF8(module.GetDescription())),
pAuthor(decUnicodeString::NewFromUTF8(module.GetAuthor())),
pVersion(module.GetVersion()),
pDirName(module.GetDirectoryName()),
pPattern(module.GetPatternList().Join(",")),
pPriority(module.GetPriority()),
pIsFallback(module.GetIsFallback()),
pStatus(emsNotTested),
pErrorCode(0){
}

delEngineModule::~delEngineModule(){
}



// Management
///////////////

void delEngineModule::SetType(deModuleSystem::eModuleTypes type){
	pType = type;
}

void delEngineModule::SetName(const char *name){
	pName = name;
}

void delEngineModule::SetDescription(const decUnicodeString &description){
	pDescription = description;
}

void delEngineModule::SetAuthor(const decUnicodeString &author){
	pAuthor = author;
}

void delEngineModule::SetVersion(const char *version){
	pVersion = version;
}

void delEngineModule::SetDirectoryName(const char *dirname){
	pDirName = dirname;
}

void delEngineModule::SetPattern(const char *pattern){
	pPattern = pattern;
}

void delEngineModule::SetPriority(int priority){
	pPriority = priority;
}

void delEngineModule::SetIsFallback(bool isFallback){
	pIsFallback = isFallback;
}

void delEngineModule::SetStatus(eModuleStatus status){
	pStatus = status;
}

void delEngineModule::SetErrorCode(int errorCode){
	pErrorCode = errorCode;
}



void delEngineModule::SetLibFileName(const char *name){
	pLibFileName = name;
}

void delEngineModule::SetLibFileSizeShould(int size){
	if(size < 0){
		DETHROW_INFO(deeInvalidParam, "size < 0");
	}
	pLibFileSizeShould = size;
}

void delEngineModule::SetLibFileSizeIs(int size){
	if(size < 0){
		DETHROW_INFO(deeInvalidParam, "size < 0");
	}
	pLibFileSizeIs = size;
}

void delEngineModule::SetLibFileHashShould(const char *hash){
	pLibFileHashShould = hash;
}

void delEngineModule::SetLibFileHashIs(const char *hash){
	pLibFileHashIs = hash;
}

void delEngineModule::SetLibFileEntryPoint(const char *name){
	pLibFileEntryPoint = name;
}

void delEngineModule::CalcSizeAndHashes(delLauncher &launcher){
	pLibFileSizeIs = 0;
	pLibFileHashIs.Set('0', 20);
	
	if(pLibFileName.IsEmpty()){
		return; // internal module
	}
	
	decBaseFileReader::Ref reader;
	unsigned char buffer[4096];
	unsigned int values[5];
	decPath path;
	SHA1 sha1;
	int i;
	
	try{
		if(decPath::IsNativePathAbsolute(pLibFileName)){
			reader.TakeOver(new decDiskFileReader(pLibFileName));
			
		}else{
			path.SetFromUnix("/engine/lib/modules");
			path.AddComponent(deModuleSystem::GetTypeDirectory(pType));
			path.AddUnixPath(pDirName);
			path.AddComponent(pVersion);
			path.AddUnixPath(pLibFileName);
			reader = launcher.GetVFS()->OpenFileForReading(path);
		}
		
		pLibFileSizeIs = reader->GetLength();
		
		sha1.Reset();
		for(i=0; i<pLibFileSizeIs; i+=4096){
			const int bytes = decMath::min(4096, pLibFileSizeIs - i);
			reader->Read(buffer, bytes);
			sha1.Input(buffer, bytes);
		}
		
		if(sha1.Result(values)){
			pLibFileHashIs.Format("%08x%08x%08x%08x%08x",
				values[0], values[1], values[2], values[3], values[4]);
		}
		
	}catch(const deException &e){
		launcher.GetLogger()->LogErrorFormat(launcher.GetLogSource(),
			"EngineModule.CalcSizeAndHashes failed with exception (module=%s)", pName.GetString());
		launcher.GetLogger()->LogException(launcher.GetLogSource(), e);
	}
}
