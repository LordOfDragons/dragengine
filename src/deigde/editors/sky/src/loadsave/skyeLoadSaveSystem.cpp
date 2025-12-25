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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "skyeLoadSaveSystem.h"
#include "skyeLoadSaveSky.h"
#include "../sky/skyeSky.h"
#include "../gui/skyeWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Definitions
////////////////

#define LOGSOURCE "Sky Editor"



// Class skyeLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

skyeLoadSaveSystem::skyeLoadSaveSystem(skyeWindowMain &windowMain) :
pWindowMain(windowMain),
pLSSky(nullptr)
{
	pLSSky = new skyeLoadSaveSky(*this, windowMain.GetEnvironment().GetLogger(), LOGSOURCE);
	pBuildFilePattern();
}

skyeLoadSaveSystem::~skyeLoadSaveSystem(){
	if(pLSSky){
		delete pLSSky;
	}
}



// Management
///////////////

skyeSky::Ref skyeLoadSaveSystem::LoadSky(const char *filename){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	const skyeSky::Ref sky(skyeSky::Ref::New(&pWindowMain.GetEnvironment()));
	sky->SetFilePath(filename);
	pLSSky->LoadSky(*this, *sky, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
	sky->SetChanged(false);
	sky->SetSaved(true);
	
	return sky;
}

void skyeLoadSaveSystem::SaveSky(skyeSky *sky, const char *filename){
	if(!sky || !filename){
		DETHROW(deeInvalidParam);
	}
	
	pLSSky->SaveSky(*this, *sky, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
}




// Private Functions
//////////////////////	

void skyeLoadSaveSystem::pBuildFilePattern(){
	decString pattern;
	pattern.Format("*%s", pLSSky->GetPattern().GetString());
	pFPSky.Add(igdeFilePattern::Ref::New(pLSSky->GetName(), pattern, pLSSky->GetPattern()));
}
