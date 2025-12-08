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

#include <stdlib.h>
#include <string.h>

#include "igdeEditorModule.h"
#include "../environment/igdeEnvironment.h"
#include "../engine/igdeEngineController.h"
#include "../gui/igdeEditorWindow.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeEditorModule
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditorModule::igdeEditorModule(igdeEnvironment &environment) :
pEnvironment(environment){
}

igdeEditorModule::~igdeEditorModule(){
}



// Management
///////////////

void igdeEditorModule::SetEditorDirectory(const char *directory){
	pEditorDirectory = directory;
}

void igdeEditorModule::SetEditorPathLib(const char *path){
	pEditorPathLib = path;
}

void igdeEditorModule::SetEditorWindow(igdeEditorWindow *editorWindow){
	if(editorWindow && editorWindow->GetParent()){
		DETHROW(deeInvalidParam);
	}
	
	if(pEditorWindow == editorWindow){
		return;
	}
	
	igdeContainer * const uiContainer = pEnvironment.GetUIContainer();
	
	if(pEditorWindow){
		if(pEditorWindow->GetParent() == uiContainer){
			uiContainer->RemoveChild(pEditorWindow);
		}
	}
	
	pEditorWindow = editorWindow;
	
	if(editorWindow){
		// not adding a reference since we take over the reference!
		uiContainer->AddChild(pEditorWindow);
	}
}



void igdeEditorModule::Start(){
}

void igdeEditorModule::Stop(){
	SetEditorWindow(NULL);
}

void igdeEditorModule::OnBeforeEngineStart(){
	if(pEditorWindow){
		pEditorWindow->OnBeforeEngineStart();
	}
}

void igdeEditorModule::OnAfterEngineStart(){
	if(pEditorWindow){
		pEditorWindow->OnAfterEngineStart();
	}
}

void igdeEditorModule::OnBeforeEngineStop(){
	if(pEditorWindow){
		pEditorWindow->OnBeforeEngineStop();
	}
}

void igdeEditorModule::OnAfterEngineStop(){
	if(pEditorWindow){
		pEditorWindow->OnAfterEngineStop();
	}
}

void igdeEditorModule::OnActivate(){
	if(pEditorWindow){
		pEditorWindow->OnActivate();
	}
}

void igdeEditorModule::OnDeactivate(){
	if(pEditorWindow){
		pEditorWindow->OnDeactivate();
	}
}

void igdeEditorModule::OnFrameUpdate(float elapsed){
	if(pEditorWindow){
		pEditorWindow->OnFrameUpdate(elapsed);
	}
}

void igdeEditorModule::GetChangedDocuments(decStringList &list){
	if(pEditorWindow){
		pEditorWindow->GetChangedDocuments(list);
	}
}

bool igdeEditorModule::SaveDocument(const char *filename){
	if(pEditorWindow){
		return pEditorWindow->SaveDocument(filename);
		
	}else{
		return false;
	}
}

void igdeEditorModule::LoadDocument(const char *filename){
	if(pEditorWindow){
		pEditorWindow->LoadDocument(filename);
	}
}

void igdeEditorModule::OnGameProjectChanged(){
	if(pEditorWindow){
		pEditorWindow->OnGameProjectChanged();
	}
}

igdeStepableTask *igdeEditorModule::OnGameDefinitionChanged(){
	return pEditorWindow ? pEditorWindow->OnGameDefinitionChanged() : nullptr;
}

bool igdeEditorModule::ProcessCommandLine(decUnicodeStringList&){
	return true;
}



// Debugging
//////////////

deEngine *igdeEditorModule::GetEngine() const{
	return pEnvironment.GetEngineController()->GetEngine();
}

deLogger *igdeEditorModule::GetLogger() const{
	return GetEngine()->GetLogger();
}

const decString &igdeEditorModule::GetLoggingName() const{
	return pLoggingName;
}

void igdeEditorModule::SetLoggingName(const char *name){
	pLoggingName = name;
}



void igdeEditorModule::LogInfo(const char *message) const{
	GetLogger()->LogInfo(pLoggingName.GetString(), message);
}

void igdeEditorModule::LogInfoFormat(const char *message, ...) const{
	va_list list;
	va_start(list, message);
	GetLogger()->LogInfoFormatUsing(pLoggingName.GetString(), message, list);
	va_end(list);
}

void igdeEditorModule::LogInfoFormatUsing(const char *message, va_list args) const{
	GetLogger()->LogInfoFormatUsing(pLoggingName.GetString(), message, args);
}

void igdeEditorModule::LogWarn(const char *message) const{
	GetLogger()->LogWarn(pLoggingName.GetString(), message);
}

void igdeEditorModule::LogWarnFormat(const char *message, ...) const{
	va_list list;
	va_start(list, message);
	GetLogger()->LogWarnFormatUsing(pLoggingName.GetString(), message, list);
	va_end(list);
}

void igdeEditorModule::LogWarnFormatUsing(const char *message, va_list args) const{
	GetLogger()->LogWarnFormatUsing(pLoggingName.GetString(), message, args);
}

void igdeEditorModule::LogError(const char *message) const{
	GetLogger()->LogError(pLoggingName.GetString(), message);
}

void igdeEditorModule::LogErrorFormat(const char *message, ...) const{
	va_list list;
	va_start(list, message);
	GetLogger()->LogErrorFormatUsing(pLoggingName.GetString(), message, list);
	va_end(list);
}

void igdeEditorModule::LogErrorFormatUsing(const char *message, va_list args) const{
	GetLogger()->LogErrorFormatUsing(pLoggingName.GetString(), message, args);
}

void igdeEditorModule::LogException(const deException &exception) const{
	GetLogger()->LogException(pLoggingName.GetString(), exception);
}
