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

#include "igdeGameDefinition.h"
#include "igdeGDCategory.h"
#include "igdeXMLElementClass.h"
#include "class/igdeGDClass.h"
#include "class/igdeGDClassManager.h"
#include "skin/igdeGDSkinManager.h"
#include "sky/igdeGDSkyManager.h"
#include "../environment/igdeEnvironment.h"
#include "../engine/igdeEngineController.h"
#include "../gui/filedialog/igdeFilePattern.h"
#include "../gui/filedialog/igdeFilePatternList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>
#include <dragengine/filesystem/deVFSContainer.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>



// Class igdeGameDefinition
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGameDefinition::igdeGameDefinition(igdeEnvironment &environment) :
pEnvironment(environment),

pVFSPath("/"),

pSkinManager(NULL),
pClassManager(NULL),
pSkyManager(NULL)
{
	try{
		pSkinManager = new igdeGDSkinManager;
		pClassManager = new igdeGDClassManager;
		pSkyManager = new igdeGDSkyManager;
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

igdeGameDefinition::~igdeGameDefinition(){
	pCleanUp();
}



// Management
///////////////

void igdeGameDefinition::SetFilename(const char *filename){
	pFilename = filename;
}

void igdeGameDefinition::SetID(const char *id){
	pID = id;
}

void igdeGameDefinition::SetDescription(const char *description){
	pDescription = description;
}

void igdeGameDefinition::SetBasePath(const char *path){
	pBasePath = path;
}

void igdeGameDefinition::SetVFSPath(const char *path){
	pVFSPath = path;
}

void igdeGameDefinition::SetScriptModule(const char *identifier){
	pScriptModule = identifier;
}



void igdeGameDefinition::UpdateTags(){
	pClassManager->UpdateTags();
}

void igdeGameDefinition::ResolveInheritClasses(){
	pClassManager->ResolveInheritClasses();
}



void igdeGameDefinition::UpdateWith(const igdeGameDefinition &gameDefinition){
	pFilename = gameDefinition.pFilename;
	pID = gameDefinition.pID;
	pDescription = gameDefinition.pDescription;
	pBasePath = gameDefinition.pBasePath;
	pVFSPath = gameDefinition.pVFSPath;
	
	pSkinManager->UpdateWith(*gameDefinition.pSkinManager);
	pClassManager->UpdateWith(*gameDefinition.pClassManager);
	pSkyManager->UpdateWith(*gameDefinition.pSkyManager);
	pParticleEmitterManager.UpdateWith(gameDefinition.pParticleEmitterManager);
	
	pListWorldProperties.SetToDeepCopyFrom(gameDefinition.pListWorldProperties);
	pListDecalProperties.SetToDeepCopyFrom(gameDefinition.pListDecalProperties);
}

void igdeGameDefinition::UpdateWithElementClasses(const igdeGameDefinition &gameDefinition){
	pClassManager->UpdateWithElementClasses(*gameDefinition.pClassManager);
}

void igdeGameDefinition::UpdateWithFound(const igdeGameDefinition &gameDefinition){
	pSkinManager->UpdateWithFound(*gameDefinition.pSkinManager);
	pSkyManager->UpdateWithFound(*gameDefinition.pSkyManager);
}



// Engine Objects
///////////////////

void igdeGameDefinition::SetDefaultModel(deModel *model){
	if(pDefaultModel){
		pDefaultModel = NULL;
	}
	
	pDefaultModel = model;
}

void igdeGameDefinition::SetDefaultSkin(deSkin *skin){
	if(pDefaultSkin){
		pDefaultSkin = NULL;
	}
	
	pDefaultSkin = skin;
}

void igdeGameDefinition::UpdateEngineObjects(){
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	
	// free old engine objects
	if(pDefaultModel){
		pDefaultModel = NULL;
	}
	if(pDefaultSkin){
		pDefaultSkin = NULL;
	}
	
	// load default engine objects
	try{
		pDefaultModel = engine.GetModelManager()->LoadModel("/igde/models/box/box.demodel", "/");
		pDefaultSkin = engine.GetSkinManager()->LoadSkin("/igde/models/box/materials/problem.deskin", "/");
		
	}catch(const deException &e){
		e.PrintError();
		throw;
	}
}



void igdeGameDefinition::FindClasses(deVirtualFileSystem &vfs, igdeGDClassManager &found){
	deLogger &logger = *pEnvironment.GetLogger();
	decTimer timer;
	igdeXMLElementClass loader(&logger);
	
	found.RemoveAll();
	
	const decStringList &pathList = pClassManager->GetAutoFindPath();
	const int pathCount = pathList.GetCount();
	int i;
	for(i=0; i<pathCount; i++){
		const decString &path = pathList.GetAt(i);
		logger.LogInfoFormat("IGDE", "Load XML Element Classes: %s", path.GetString());
		loader.LoadElementClasses(found, vfs, decPath::CreatePathUnix(path));
	}
	logger.LogInfoFormat("IGDE", "Load XML Element Classes done: %.1fs (%d found)",
		timer.GetElapsedTime(), found.GetCount());
	
	// DEBUG
	/*
	const igdeGDClassManager &gdClasses = *gameProject.GetXMLEClassGameDefinition()->GetClassManager();
	const int count = gdClasses.GetCount();
	int i;
	for(i=0; i<count; i++){
		const igdeGDClass &gdClass = *gdClasses.GetAt(i);
		GetLogger()->LogInfoFormat(LOGSOURCE, "XMLEClass: %s%s", gdClass.GetName().GetString(),
			gameProject.GetProjectGameDefinition()->GetClassManager()->HasNamed(gdClass.GetName())
				? "(existing in project)" : "");
	}
	*/
	// DEBUG
}

void igdeGameDefinition::FindSkins(deVirtualFileSystem &vfs, igdeGDSkinManager &found){
	const igdeFilePatternList &patterns = *pEnvironment.GetOpenFilePatternList(igdeEnvironment::efpltSkin);
	const decStringList &pathList = pSkinManager->GetAutoFindPath();
	deLogger &logger = *pEnvironment.GetLogger();
	const int pathCount = pathList.GetCount();
	decTimer timer;
	int i, j;
	
	for(i=0; i<pathCount; i++){
		const decString &path = pathList.GetAt(i);
		logger.LogInfoFormat("IGDE", "Find Skins: %s", path.GetString());
		
		const int patternCount = patterns.GetFilePatternCount();
		const decPath searchPath(decPath::CreatePathUnix(path));
		
		for(j=0; j<patternCount; j++){
			const igdeFilePattern &pattern = *patterns.GetFilePatternAt(j);
			found.FindAndAddSkins(vfs, searchPath, pattern.GetPattern());
		}
	}
	logger.LogInfoFormat("IGDE", "Find Skins done: %.1fs (%d found)", timer.GetElapsedTime(), found.GetSkinCount());
}

void igdeGameDefinition::FindSkies(deVirtualFileSystem &vfs, igdeGDSkyManager &found){
	const igdeFilePatternList &patterns = *pEnvironment.GetOpenFilePatternList(igdeEnvironment::efpltSky);
	const decStringList &pathList = pSkyManager->GetAutoFindPath();
	deLogger &logger = *pEnvironment.GetLogger();
	const int pathCount = pathList.GetCount();
	decTimer timer;
	int i, j;
	
	for(i=0; i<pathCount; i++){
		const decString &path = pathList.GetAt(i);
		logger.LogInfoFormat("IGDE", "Find Skies: %s", path.GetString());
		
		const int patternCount = patterns.GetFilePatternCount();
		const decPath searchPath(decPath::CreatePathUnix(path));
		
		for(j=0; j<patternCount; j++){
			const igdeFilePattern &pattern = *patterns.GetFilePatternAt(j);
			found.FindAndAddSkies(vfs, searchPath, pattern.GetPattern());
		}
	}
	logger.LogInfoFormat("IGDE", "Find Skies done: %.1fs (%d found)",
		timer.GetElapsedTime(), found.GetSkyList().GetCount());
}




// Private Functions
//////////////////////

void igdeGameDefinition::pCleanUp(){
	if(pSkyManager){
		delete pSkyManager;
	}
	if(pClassManager){
		delete pClassManager;
	}
	if(pSkinManager){
		delete pSkinManager;
	}
	pParticleEmitterManager.RemoveAllEmitters();
}
