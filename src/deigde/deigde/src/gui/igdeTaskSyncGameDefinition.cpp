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

#include "igdeTaskSyncGameDefinition.h"
#include "igdeWindowMain.h"
#include "../environment/igdeEnvironmentIGDE.h"
#include "../module/igdeEditorModuleManager.h"
#include "../module/igdeEditorModuleDefinition.h"

#include <deigde/module/igdeEditorModule.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gamedefinition/igdeXMLGameDefinition.h>
#include <deigde/gameproject/igdeGameProject.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/file/decPath.h>



// Class igdeTaskSyncGameDefinition
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeTaskSyncGameDefinition::igdeTaskSyncGameDefinition(igdeWindowMain &windowMain) :
pWindowMain(windowMain),
pState(esReloadProjectGameDef),
pTaskIndex(0),
pLastProgress(-1.0f),
pBaseProgress(0.0f),
pProgressFactor(0.0f),
pReloadXMLElementClasses(false)
{
	try{
		SetMessage("Reloading Project Game Definition");
		
		windowMain.SetProgressVisible(true);
		windowMain.SetProgress(0.0f);
		windowMain.SetProgressText(GetMessage());
		
		// NOTE looks like a bug in FOX but disable() seems to have no effect. No idea how
		//      to solve this problem
		windowMain.SetEnabled(false);
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

igdeTaskSyncGameDefinition::~igdeTaskSyncGameDefinition(){
	pWindowMain.SetEnabled(true);
	
	pWindowMain.SetProgressVisible(false);
	pWindowMain.SetProgress(0.0f);
	pWindowMain.SetProgressText("");
	
	pCleanUp();
}



// Management
///////////////

void igdeTaskSyncGameDefinition::SetReloadXMLElementClasses(bool reloadXMLElementClasses){
	pReloadXMLElementClasses = reloadXMLElementClasses;
}

bool igdeTaskSyncGameDefinition::Step(){
	switch(pState){
	case esReloadProjectGameDef:{
		igdeGameProject &project = *pWindowMain.GetGameProject();
		
		pOldProjectGameDef = project.GetProjectGameDefinition();
		
		pOldGameDef = project.GetGameDefinition();
		
		if(!pReloadXMLElementClasses){
			pLoadProjectGameDefinition();
		}
		
		project.MergeGameDefinitions();
		project.GetGameDefinition()->UpdateEngineObjects();
		
		pCreateEditorTasks();
		
		if(pEditorTasks.GetCount() == 0){
			pState = esFinished;
			return false;
		}
		
		pProgressFactor = 1.0f / (float)(pEditorTasks.GetCount() + 1);
		pBaseProgress = pProgressFactor;
		pUpdateProgress(true);
		
		pState = esEditorUpdateTasks;
		break;
		}
		
	case esEditorUpdateTasks:{
		// step 2..2+N: process module update tasks
		const int taskCount = pEditorTasks.GetCount();
		if(pTaskIndex == taskCount){
			pState = esFinished;
			return false;
		}
		
		igdeStepableTask &activeTask = *((igdeStepableTask*)pEditorTasks.GetAt(pTaskIndex));
		if(activeTask.Step()){
			pUpdateProgress(false);
			break;
		}
		
		delete (igdeStepableTask*)pEditorTasks.GetAt(pTaskIndex);
		pEditorTasks.SetAt(pTaskIndex, NULL);
		
		pTaskIndex++;
		if(pTaskIndex == taskCount){
			pState = esFinished;
			return false;
		}
		
		pBaseProgress = (float)(pTaskIndex + 1) / (float)(taskCount + 1);
		pUpdateProgress(true);
		break;
		}
		
	case esFinished:
	default:
		return false;
	}
	
	pWindowMain.SetProgress(GetProgress());
	pWindowMain.SetProgressText(GetMessage());
	
	return true;
}



// Private Functions
//////////////////////

void igdeTaskSyncGameDefinition::pCleanUp(){
	const int taskCount = pEditorTasks.GetCount();
	
	while(pTaskIndex<taskCount){
		delete (igdeStepableTask*)pEditorTasks.GetAt(pTaskIndex++);
	}
}

void igdeTaskSyncGameDefinition::pUpdateProgress(bool force){
	const igdeStepableTask &task = *((const igdeStepableTask*)pEditorTasks.GetAt(pTaskIndex));
	
	if(force || fabsf(task.GetProgress() - pLastProgress) > 0.1f){
		pLastProgress = task.GetProgress();
		SetProgress(pBaseProgress + pProgressFactor * pLastProgress);
	}
	
	if(force || task.GetMessage() != pLastMessage){
		pLastMessage = task.GetMessage();
		SetMessage(pLastMessage);
	}
}

void igdeTaskSyncGameDefinition::pLoadProjectGameDefinition(){
	igdeGameProject &project = *pWindowMain.GetGameProject();
	decPath path;
	path.SetFromNative(project.GetDirectoryPath());
	path.AddUnixPath(project.GetPathProjectGameDefinition());
	
	const igdeGameDefinition::Ref gamedef(
		igdeGameDefinition::Ref::NewWith(pWindowMain.GetEnvironment()));
	gamedef->SetFilename(path.GetPathNative());
	
	igdeXMLGameDefinition(pWindowMain.GetEnvironment(), pWindowMain.GetLogger()).Load(
		decDiskFileReader::Ref::NewWith(path.GetPathNative()), gamedef);
	
	project.SetProjectGameDefinition(gamedef);
}

void igdeTaskSyncGameDefinition::pCreateEditorTasks(){
	const igdeEditorModuleManager &moduleManager = pWindowMain.GetModuleManager();
	const int editorCount = moduleManager.GetModuleCount();
	igdeStepableTask *editorTask = NULL;
	int i;
	
	try{
		for(i=0; i<editorCount; i++){
			const igdeEditorModuleDefinition &editorDefinition = *moduleManager.GetModuleAt(i);
			if(!editorDefinition.IsModuleRunning()){
				continue;
			}
			
			igdeEditorModule * const editor = editorDefinition.GetModule();
			if(!editor){
				continue;
			}
			
			editorTask = editor->OnGameDefinitionChanged();
			if(!editorTask){
				continue;
			}
			
			pEditorTasks.Add(editorTask);
			editorTask = NULL;
		}
		
	}catch(const deException &){
		if(editorTask){
			delete editorTask;
		}
		throw;
	}
}
