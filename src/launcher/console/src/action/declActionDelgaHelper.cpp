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

#include "declActionDelgaHelper.h"
#include "../declLauncher.h"
#include "../config/declConfiguration.h"

#include <delauncher/engine/delEngine.h>
#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameManager.h>
#include <delauncher/game/patch/delPatch.h>
#include <delauncher/game/patch/delPatchManager.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/logger/deLogger.h>



// Definitions
////////////////

#define LOGSOURCE "Launcher"



// Class declActionDelgaHelper
////////////////////////////////

// Constructor, destructor
////////////////////////////

declActionDelgaHelper::declActionDelgaHelper(declLauncher &launcher, const decString &filename) :
pLauncher(launcher),
pFilename(filename){
}

declActionDelgaHelper::~declActionDelgaHelper(){
}



// Management
///////////////

void declActionDelgaHelper::Load(){
	Unload();
	
	const delEngineInstance::Ref instance(pLauncher.GetEngineInstanceFactory().
		CreateEngineInstance(pLauncher, pLauncher.GetEngine().GetLogFile()));
	instance->StartEngine();
	instance->LoadModules();
	
	pLauncher.GetEngine().PutEngineIntoVFS(instance);
	pLauncher.GetGameManager().CreateDefaultProfile();
	
	pLauncher.GetGameManager().LoadGameFromDisk(instance, pFilename, pGames);
	pLauncher.GetPatchManager().LoadPatchFromDisk(instance, pFilename, pPatches);
}

void declActionDelgaHelper::Unload(){
	pGames.RemoveAll();
	pPatches.RemoveAll();
}

bool declActionDelgaHelper::HasContent() const{
	return pGames.GetCount() > 0 || pPatches.GetCount() > 0;
}

void declActionDelgaHelper::Install(){
	const deVFSDiskDirectory::Ref container(deVFSDiskDirectory::Ref::New(
		decPath::CreatePathNative(pLauncher.GetPathGames())));
	
	const decDiskFileReader::Ref reader(decDiskFileReader::Ref::New(pFilename));
	
	decPath target(decPath::CreatePathUnix("/"));
	target.AddComponent(decPath::CreatePathNative(pFilename).GetLastComponent());
	const decBaseFileWriter::Ref writer(container->OpenFileForWriting(target));
	
	printf("Installing");
	
	const int totalSize = reader->GetLength();
	const double percentageFactor = 100.0 / (double)totalSize;
	char * const buffer = new char[8192];
	int progressPercentage = 0;
	int bytesCopied = 0;
	
	try{
		while(bytesCopied < totalSize){
			const int percentage = (int)(percentageFactor * bytesCopied);
			if(percentage != progressPercentage){
				while(percentage > progressPercentage){
					progressPercentage++;
					printf(".");
					if(progressPercentage % 10 == 0){
						printf("%d%%", progressPercentage);
					}
				}
				fflush(stdout);
			}
			
			const int copyBytesCount = decMath::min(8192, totalSize - bytesCopied);
			reader->Read(buffer, copyBytesCount);
			writer->Write(buffer, copyBytesCount);
			
			bytesCopied += copyBytesCount;
		}
		
	}catch(const deException &e){
		delete [] buffer;
		pLauncher.GetLogger()->LogException(LOGSOURCE, e);
		try{
			if(container->ExistsFile(target)){
				container->DeleteFile(target);
			}
		}catch(const deException &e2){
			e2.PrintError();
		}
		throw;
	}
	
	printf("100%%\nFinshed successfully\n");
}

void declActionDelgaHelper::Uninstall(){
	printf("Uninstalling...\n");
	
	const deVFSDiskDirectory::Ref container(deVFSDiskDirectory::Ref::New(
		decPath::CreatePathUnix("/"), decPath::CreatePathNative(pLauncher.GetPathGames())));
	
	decPath target(decPath::CreatePathUnix("/"));
	target.AddComponent(decPath::CreatePathNative(pFilename).GetLastComponent());
	container->DeleteFile(target);
	
	printf("Finshed successfully\n");
}
