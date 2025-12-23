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

#include "igdeEnvironmentIGDE.h"
#include "../configuration/igdeConfiguration.h"
#include "../engine/igdeSMCollider.h"
#include "../engine/igdeSMTouchSensor.h"
#include "../engine/igdeSMPropField.h"
#include "../loadsave/igdeLoadSaveSystem.h"
#include "../gui/igdeWindowMain.h"
#include "../gui/igdeSharedFontList.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeSwitcher.h>
#include <deigde/gui/dialog/igdeDialog.h>
#include <deigde/resourceloader/igdeResourceLoader.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/logger/deLoggerConsoleColor.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/sensor/deTouchSensor.h>



// Class igdeEnvironmentIGDE
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEnvironmentIGDE::igdeEnvironmentIGDE() :
pWindowMain(nullptr)
{
	pLogger = deLoggerConsoleColor::Ref::New();
}

igdeEnvironmentIGDE::~igdeEnvironmentIGDE(){
}



// Management
///////////////

void igdeEnvironmentIGDE::SetWindowMain(igdeWindowMain *windowMain){
	pWindowMain = windowMain;
}

void igdeEnvironmentIGDE::SetLogger(deLogger *logger){
	pLogger = logger;
}



igdeContainer *igdeEnvironmentIGDE::GetUIContainer(){
	return pWindowMain->GetContentSwitcher();
}

igdeEngineController *igdeEnvironmentIGDE::GetEngineController(){
	return pWindowMain->GetMainWindowEngineController();
}

igdeGameDefinition *igdeEnvironmentIGDE::GetGameDefinition(){
	return pWindowMain->GetIGDEGameDefinition();
}

igdeGameDefinition *igdeEnvironmentIGDE::GetSharedGameDefinition(const char *id){
	return pWindowMain->GetSharedGameDefinitions().FindOrDefault([id](const igdeGameDefinition::Ref &gd){
		return gd->GetID() == id;
	});
}

igdeGDPreviewManager *igdeEnvironmentIGDE::GetGDPreviewManager(){
	return pWindowMain->GetGDPreviewManager();
}

const igdeTextureProperty::List &igdeEnvironmentIGDE::GetTexturePropertyList(){
	return pWindowMain->GetTexturePropertyList();
}

igdeGameProject *igdeEnvironmentIGDE::GetGameProject(){
	return pWindowMain->GetGameProject();
}

const igdeFilePattern::List *igdeEnvironmentIGDE::GetOpenFilePatternList(eFilePatternListTypes type){
	igdeLoadSaveSystem &lssys = *pWindowMain->GetLoadSaveSystem();
	
	switch(type){
	case efpltAll:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplAll);
		
	case efpltAnimation:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplAnimation);
		
	case efpltAnimator:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplAnimator);
		
	case efpltFont:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplFont);
		
	case efpltImage:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplImage);
		
	case efpltLanguagePack:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplLanguagePack);
		
	case efpltModel:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplModel);
		
	case efpltNavigationSpace:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplNavigationSpace);
		
	case efpltOcclusionMesh:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplOcclusionMesh);
		
	case efpltParticleEmitter:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplParticleEmitter);
		
	case efpltRig:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplRig);
		
	case efpltSkin:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplSkin);
		
	case efpltSky:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplSky);
		
	case efpltSound:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplSound);
		
	case efpltSynthesizer:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplSynthesizer);
		
	case efpltVideo:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplVideo);
		
	case efpltSpeechAnimation:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplSpeechAnimation);
		
	case efpltCamera:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplCamera);
		
	case efpltWorld:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplWorld);
		
	default:
		DETHROW(deeInvalidParam);
	}
}

const igdeFilePattern::List *igdeEnvironmentIGDE::GetSaveFilePatternList(eFilePatternListTypes type){
	igdeLoadSaveSystem &lssys = *pWindowMain->GetLoadSaveSystem();
	
	switch(type){
	case efpltAll:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplAll);
		
	case efpltAnimation:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplAnimation);
		
	case efpltAnimator:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplAnimator);
		
	case efpltFont:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplFont);
		
	case efpltImage:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplImage);
		
	case efpltLanguagePack:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplLanguagePack);
		
	case efpltModel:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplModel);
		
	case efpltNavigationSpace:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplNavigationSpace);
		
	case efpltOcclusionMesh:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplOcclusionMesh);
		
	case efpltParticleEmitter:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplParticleEmitter);
		
	case efpltRig:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplRig);
		
	case efpltSky:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplSky);
		
	case efpltSkin:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplSkin);
		
	case efpltSound:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplSound);
		
	case efpltSynthesizer:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplSynthesizer);
		
	case efpltVideo:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplVideo);
		
	case efpltSpeechAnimation:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplSpeechAnimation);
		
	case efpltCamera:
		return &lssys.GetOpenFilePatternList(igdeLoadSaveSystem::efplCamera);
		
	case efpltWorld:
		return &lssys.GetSaveFilePatternList(igdeLoadSaveSystem::efplWorld);
		
	default:
		DETHROW(deeInvalidParam);
	}
}

igdeIcon *igdeEnvironmentIGDE::GetStockIcon(eStockIcons icon){
	return pWindowMain->GetStockIcon(icon);
}

deSkin::Ref igdeEnvironmentIGDE::GetStockSkin(eStockSkins skin){
	return pWindowMain->GetStockSkin(skin);
}

deRig::Ref igdeEnvironmentIGDE::GetStockRig(eStockRigs rig){
	return pWindowMain->GetStockRig(rig);
}

deModel::Ref igdeEnvironmentIGDE::GetStockModel(eStockModels model){
	return pWindowMain->GetStockModel(model);
}

deLogger *igdeEnvironmentIGDE::GetLogger(){
	return pLogger;
}

deVirtualFileSystem *igdeEnvironmentIGDE::GetFileSystemGame(){
	return pWindowMain->GetEngine()->GetVirtualFileSystem();
}

deVirtualFileSystem *igdeEnvironmentIGDE::GetFileSystemIGDE(){
	return pWindowMain->GetVirtualFileSystem();
}

void igdeEnvironmentIGDE::GetApplicationFont(igdeFont::sConfiguration &config){
	igdeApplication::app().GetAppFontConfig(config);
}

igdeFont *igdeEnvironmentIGDE::GetSharedFont(const igdeFont::sConfiguration &configuration){
	return pWindowMain->GetSharedFontList().GetFontWith(configuration);
}

decColor igdeEnvironmentIGDE::GetSystemColor(eSystemColors color){
	return igdeApplication::app().GetSystemColor(color);
}

deRig *igdeEnvironmentIGDE::GetSharedModelCollisionRig(){
	return pWindowMain->GetSharedModelCollisionRig();
}

void igdeEnvironmentIGDE::AsyncLoadResource(const char *filename,
deResourceLoader::eResourceType resourceType, igdeResourceLoaderListener *listener){
	pWindowMain->GetResourceLoader().RequestResource(filename, resourceType, listener);
}

igdeUIHelper &igdeEnvironmentIGDE::GetUIHelper(){
	return pWindowMain->GetUIHelper();
}

igdeUIHelper &igdeEnvironmentIGDE::GetUIHelperProperties(){
	return pWindowMain->GetUIHelperProperties();
}

igdeGuiTheme *igdeEnvironmentIGDE::GetGuiThemeNamed(const char *name){
	return pWindowMain->GetGuiThemes().GetAtOrDefault(name, pWindowMain->GetDefaultGuiTheme());
}

igdeGuiTheme *igdeEnvironmentIGDE::GetDefaultGuiTheme(){
	return pWindowMain->GetDefaultGuiTheme();
}



void igdeEnvironmentIGDE::SetProgressVisible(bool visible){
	pWindowMain->SetProgressVisible(visible);
}

void igdeEnvironmentIGDE::SetProgress(float progress){
	pWindowMain->SetProgress(progress);
}

void igdeEnvironmentIGDE::SetProgressText(const char *text){
	pWindowMain->SetProgressText(text);
}



void igdeEnvironmentIGDE::ActiveModuleSharedMenusChanged(){
	pWindowMain->ActiveModuleSharedMenusChanged();
}

void igdeEnvironmentIGDE::ActiveModuleSharedToolBarsChanged(){
	pWindowMain->ActiveModuleSharedToolBarsChanged();
}

void igdeEnvironmentIGDE::ActivateEditor(igdeEditorModule *editor){
	pWindowMain->ActivateEditor(editor);
}



void igdeEnvironmentIGDE::SetColliderDelegee(deCollider *collider,
deBaseScriptingCollider *delegee){
	DEASSERT_NOTNULL(collider)
	
	igdeSMCollider * const peer = (igdeSMCollider*)collider->GetPeerScripting();
	DEASSERT_NOTNULL(peer)
	
	peer->SetDelegee(delegee);
}

void *igdeEnvironmentIGDE::GetColliderUserPointer(deCollider *collider){
	DEASSERT_NOTNULL(collider)
	
	const igdeSMCollider * const peer = (const igdeSMCollider *)collider->GetPeerScripting();
	DEASSERT_NOTNULL(peer)
	
	return peer->GetUserPointer();
}

void igdeEnvironmentIGDE::SetColliderUserPointer(deCollider *collider, void *userPointer){
	DEASSERT_NOTNULL(collider)
	
	igdeSMCollider * const peer = (igdeSMCollider*)collider->GetPeerScripting();
	DEASSERT_NOTNULL(peer)
	
	peer->SetUserPointer(userPointer);
}

void igdeEnvironmentIGDE::SetTouchSensorDelegee(deTouchSensor *touchSensor,
deBaseScriptingTouchSensor *delegee){
	DEASSERT_NOTNULL(touchSensor)
	
	igdeSMTouchSensor * const peer = (igdeSMTouchSensor*)touchSensor->GetPeerScripting();
	DEASSERT_NOTNULL(peer)
	
	peer->SetDelegee(delegee);
}

void igdeEnvironmentIGDE::SetPropFieldDelegee(dePropField *propField,
deBaseScriptingPropField *delegee){
	DEASSERT_NOTNULL(propField)
	
	igdeSMPropField * const peer = (igdeSMPropField*)propField->GetPeerScripting();
	DEASSERT_NOTNULL(peer)
	
	peer->SetDelegee(delegee);
}



void igdeEnvironmentIGDE::RunModalWhileShown(igdeWindow &window){
	igdeApplication::app().RunModalWhileShown(window);
}



void igdeEnvironmentIGDE::ProjecGameDefinitionChanged(){
	pWindowMain->OnProjectGameDefinitionChanged();
}

void igdeEnvironmentIGDE::ReloadXMLElementClasses(){
	pWindowMain->ReloadXMLElementClasses();
}

bool igdeEnvironmentIGDE::OpenFileInEditor(const char *filename){
	if(!filename){
		DETHROW(deeInvalidParam);
	}
	
	return false;
}

bool igdeEnvironmentIGDE::RequestSaveDocuments(const char *title, const char* message){
	return pWindowMain->RequestSaveDocuments(title, message);
}

const decStringList &igdeEnvironmentIGDE::GetRecentEditorFiles(){
	return pWindowMain->GetConfigurationLocal().GetRecentEditorFiles().GetFiles();
}

void igdeEnvironmentIGDE::AddRecentEditorFile(const char *filename){
	igdeConfigurationLocal &configuration = pWindowMain->GetConfigurationLocal();
	const int index = configuration.GetRecentEditorFiles().GetFiles().IndexOf(filename);
	configuration.GetRecentEditorFiles().AddFile(filename);
	if(index != 0){
		configuration.SaveConfiguration();
	}
}

void igdeEnvironmentIGDE::CloseApplication(){
	if(pWindowMain->CloseWindow()){
		pWindowMain->Close();
	}
}

/*@}*/
