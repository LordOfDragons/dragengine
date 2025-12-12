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

#include "igdeLSGameProject.h"
#include "igdeLoadSaveSystem.h"
#include "../gui/igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../../shared/src/gameproject/igdeXMLGameProject.h"

#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gamedefinition/igdeXMLGameDefinition.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decDiskFileReader.h>



// Definitions
////////////////

#define LOGSOURCE "IGDE"



// Class igdeLSGameProject
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLSGameProject::igdeLSGameProject(igdeLoadSaveSystem *lssys){
	if(!lssys){
		DETHROW(deeInvalidParam);
	}
	
	pLoadSaveSystem = lssys;
	
	pName = "Drag[en]gine Game Project";
	pPattern = "*.degp";
	pDefaultExtension = ".degp";
}

igdeLSGameProject::~igdeLSGameProject(){
}



// Management
///////////////

void igdeLSGameProject::SetName(const char *name){
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	pName = name;
}

void igdeLSGameProject::SetPattern(const char *pattern){
	if(!pattern){
		DETHROW(deeInvalidParam);
	}
	
	pPattern = pattern;
}

void igdeLSGameProject::SetDefaultExtension(const char *extension){
	if(!extension){
		DETHROW(deeInvalidParam);
	}
	
	pDefaultExtension = extension;
}



// Loading and saving
///////////////////////

void igdeLSGameProject::Load(const char *filename, igdeGameProject *project, decBaseFileReader *file){
	if(!project || !file){
		DETHROW(deeInvalidParam);
	}
	
	igdeWindowMain &windowMain = *pLoadSaveSystem->GetWindowMain();
	igdeEnvironment &environment = windowMain.GetEnvironment();
	deLogger * const logger = windowMain.GetLogger();
	
	// load game project file
	igdeXMLGameProject xmlGameProject(logger);
	
	xmlGameProject.Load(*file, *project);
	project->SetFilePath(filename);
	
	// locate base game definitions
	const igdeGameDefinitionList &sharedGameDefs = windowMain.GetSharedGameDefinitions();
	const decStringList &baseGameDefs = project->GetBaseGameDefinitionIDList();
	const int baseGameDefCount = baseGameDefs.GetCount();
	int i;
	
	for(i=0; i<baseGameDefCount; i++){
		const decString &id = baseGameDefs.GetAt(i);
		igdeGameDefinition * const baseGameDef = sharedGameDefs.GetWithID(id);
		if(baseGameDef){
			project->GetBaseGameDefinitionList().Add(baseGameDef);
			
		}else{
			logger->LogWarnFormat(LOGSOURCE, "Can not find base game definition '%s', skipped",
				id.GetString());
		}
	}
	
	// load the project game definition specified
	decPath path;
	path.SetFromNative(project->GetDirectoryPath());
	path.AddUnixPath(project->GetPathProjectGameDefinition());
	
	const igdeGameDefinition::Ref gameDef(igdeGameDefinition::Ref::New(environment));
	gameDef->SetFilename(path.GetPathNative());
	
	igdeXMLGameDefinition(environment, logger).Load(
		decDiskFileReader::Ref::New(path.GetPathNative()), gameDef);
	
	project->SetProjectGameDefinition(gameDef);
}

void igdeLSGameProject::Save(igdeGameProject *project, decBaseFileWriter *file){
	if(!project || !file){
		DETHROW(deeInvalidParam);
	}
	
	igdeXMLGameProject xmlGameProject(pLoadSaveSystem->GetWindowMain()->GetLogger());
	
	xmlGameProject.Save(*file, *project);
}
