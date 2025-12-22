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

#include "reLoadSaveSystem.h"
#include "reLSRig.h"
#include "../rig/reRig.h"
#include "../rig/constraint/reRigConstraint.h"
#include "../gui/reWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/rig/deBaseRigModule.h>
#include <dragengine/filesystem/dePatternList.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decDiskFileWriter.h>


// Class reLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

reLoadSaveSystem::reLoadSaveSystem(reWindowMain &windowMain) :
pWindowMain(windowMain){
}

reLoadSaveSystem::~reLoadSaveSystem(){
}


// Management
///////////////

void reLoadSaveSystem::UpdateLSRigs()
{
	deEngine *engine = pWindowMain.GetEngineController().GetEngine();
	deModuleSystem *modSys = engine->GetModuleSystem();
	int m, moduleCount = modSys->GetModuleCount();
	
	pLSRigs.RemoveAll();
	
	// add a new load save rig for each rig module found in the engine that is also
	// running and usable therefore
	for(m=0; m<moduleCount; m++){
		const deLoadableModule &loadableModule = *modSys->GetModuleAt(m);
		
		if(loadableModule.GetType() != deModuleSystem::emtRig){
			continue;
		}
		if(!loadableModule.IsLoaded()){
			continue;
		}
		
		pLSRigs.Add(reLSRig::Ref::New((deBaseRigModule*)loadableModule.GetModule()));
	}
}

reRig::Ref reLoadSaveSystem::LoadRig(const char *filename){
	reLSRig &lsrig = pLSRigs.FindOrDefault([&](const reLSRig &l){
		return decString::StringMatchesPattern(filename, l.GetPattern());
	});
	
	const reRig::Ref rig(reRig::Ref::New(&pWindowMain.GetEnvironment()));
	
	lsrig.LoadRig(rig, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForReading(decPath::CreatePathUnix(filename)));
	
	rig->SetFilePath(filename);
	rig->SetChanged(false);
	rig->SetSaved(true);
	
	return rig;
}

void reLoadSaveSystem::SaveRig(reRig *rig, const char *filename){
	reLSRig &lsrig = pLSRigs.FindOrDefault([&](const reLSRig &l){
		return decString::StringMatchesPattern(filename, l.GetPattern());
	});
	
	lsrig.SaveRig(rig, pWindowMain.GetEnvironment().GetFileSystemGame()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
}
