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

#include "seLoadSaveSystem.h"
#include "seLoadSaveSkin.h"
#include "../skin/seSkin.h"
#include "../gui/seWindowMain.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/modules/skin/deBaseSkinModule.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decDiskFileWriter.h>
#include <dragengine/common/exceptions.h>



// Class seLoadSaveSystem
///////////////////////////

// Constructor, destructor
////////////////////////////

seLoadSaveSystem::seLoadSaveSystem(seWindowMain &windowMain) :
pWindowMain(windowMain)
{
	UpdateLSSkins();
}

seLoadSaveSystem::~seLoadSaveSystem(){
	RemoveAllLSSkins();
}



// Management
///////////////

seLoadSaveSkin *seLoadSaveSystem::GetLSSkinMatching(const char *filename){
	return pLSSkins.FindOrDefault([&](const seLoadSaveSkin &lsskin){
		return decString::StringMatchesPattern(filename, lsskin.GetPattern());
	});
}

void seLoadSaveSystem::AddLSSkin(seLoadSaveSkin *lsSkin){
	DEASSERT_FALSE(pLSSkins.Has(lsSkin))
	pLSSkins.Add(lsSkin);
}

void seLoadSaveSystem::RemoveLSSkin(seLoadSaveSkin *lsSkin){
	pLSSkins.Remove(lsSkin);
}

void seLoadSaveSystem::RemoveAllLSSkins(){
	pLSSkins.RemoveAll();
}

void seLoadSaveSystem::UpdateLSSkins(){
	deModuleSystem &modSys = *pWindowMain.GetEngineController().GetEngine()->GetModuleSystem();
	int m, moduleCount = modSys.GetModuleCount();
	
	RemoveAllLSSkins();
	
	// add a new load save skin for each skin module found in the engine that is also
	// running and usable therefore
	for(m=0; m<moduleCount; m++){
		deLoadableModule &loadableModule = *modSys.GetModuleAt(m);
		
		if(loadableModule.GetType() != deModuleSystem::emtSkin){
			continue;
		}
		if(!loadableModule.IsLoaded()){
			continue;
		}
		
		AddLSSkin(seLoadSaveSkin::Ref::New((deBaseSkinModule*)loadableModule.GetModule()));
	}
}



seSkin::Ref seLoadSaveSystem::LoadSkin(const char *filename, igdeGameDefinition *gameDefinition){
	DEASSERT_NOTNULL(filename)
	DEASSERT_NOTNULL(gameDefinition)
	
	seLoadSaveSkin &lsskin = pLSSkins.FindOrDefault([&](const seLoadSaveSkin &l){
		return decString::StringMatchesPattern(filename, l.GetPattern());
	});
	
	const seSkin::Ref skin(seSkin::Ref::New(&pWindowMain.GetEnvironment()));
	skin->SetFilePath(filename); // required here so the relative path can be resolved properly
	
	lsskin.LoadSkin(skin, pWindowMain.GetEngineController().GetEngine()->GetVirtualFileSystem()->
		OpenFileForReading(decPath::CreatePathUnix(filename)),
		pWindowMain.GetEnvironment().GetTexturePropertyList());
	
	skin->SetSaved(true);
	skin->SetChanged(false);
	
	return skin;
}

void seLoadSaveSystem::SaveSkin(seSkin *skin, const char *filename){
	DEASSERT_NOTNULL(skin)
	DEASSERT_NOTNULL(filename)
	
	seLoadSaveSkin &lsskin = pLSSkins.FindOrDefault([&](const seLoadSaveSkin &l){
		return decString::StringMatchesPattern(filename, l.GetPattern());
	});
	
	skin->SetFilePath(filename); // required here so the relative path can be resolved properly
	lsskin.SaveSkin(skin, pWindowMain.GetEngineController().GetEngine()->GetVirtualFileSystem()->
		OpenFileForWriting(decPath::CreatePathUnix(filename)));
	
	skin->SetSaved(true);
	skin->SetChanged(false);
}
