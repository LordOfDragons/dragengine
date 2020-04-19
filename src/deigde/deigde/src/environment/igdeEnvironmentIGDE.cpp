/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
#include <deigde/gamedefinition/igdeGameDefinitionList.h>
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
pWindowMain( NULL )
{
	pLogger.TakeOver( new deLoggerConsoleColor );
}

igdeEnvironmentIGDE::~igdeEnvironmentIGDE(){
}



// Management
///////////////

void igdeEnvironmentIGDE::SetWindowMain( igdeWindowMain *windowMain ){
	pWindowMain = windowMain;
}

void igdeEnvironmentIGDE::SetLogger( deLogger *logger ){
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

igdeGameDefinition *igdeEnvironmentIGDE::GetSharedGameDefinition( const char *id ){
	return pWindowMain->GetSharedGameDefinitions().GetWithID( id );
}

igdeGDPreviewManager *igdeEnvironmentIGDE::GetGDPreviewManager(){
	return pWindowMain->GetGDPreviewManager();
}

const igdeTexturePropertyList *igdeEnvironmentIGDE::GetTexturePropertyList(){
	return pWindowMain->GetTexturePropertyList();
}

igdeGameProject *igdeEnvironmentIGDE::GetGameProject(){
	return pWindowMain->GetGameProject();
}

const igdeFilePatternList *igdeEnvironmentIGDE::GetOpenFilePatternList( eFilePatternListTypes type ){
	igdeLoadSaveSystem &lssys = *pWindowMain->GetLoadSaveSystem();
	
	switch( type ){
	case efpltAll:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplAll );
		
	case efpltAnimation:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplAnimation );
		
	case efpltAnimator:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplAnimator );
		
	case efpltFont:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplFont );
		
	case efpltImage:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplImage );
		
	case efpltLanguagePack:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplLanguagePack );
		
	case efpltModel:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplModel );
		
	case efpltNavigationSpace:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplNavigationSpace );
		
	case efpltOcclusionMesh:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplOcclusionMesh );
		
	case efpltParticleEmitter:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplParticleEmitter );
		
	case efpltRig:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplRig );
		
	case efpltSkin:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplSkin );
		
	case efpltSky:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplSky );
		
	case efpltSound:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplSound );
		
	case efpltSynthesizer:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplSynthesizer );
		
	case efpltVideo:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplVideo );
		
	case efpltSpeechAnimation:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplSpeechAnimation );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

const igdeFilePatternList *igdeEnvironmentIGDE::GetSaveFilePatternList( eFilePatternListTypes type ){
	igdeLoadSaveSystem &lssys = *pWindowMain->GetLoadSaveSystem();
	
	switch( type ){
	case efpltAll:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplAll );
		
	case efpltModel:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplModel );
		
	case efpltSkin:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplSkin );
		
	case efpltRig:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplRig );
		
	case efpltAnimation:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplAnimation );
		
	case efpltImage:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplImage );
		
	case efpltSound:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplSound );
		
	case efpltParticleEmitter:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplParticleEmitter );
		
	case efpltOcclusionMesh:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplOcclusionMesh );
		
	case efpltNavigationSpace:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplNavigationSpace );
		
	case efpltVideo:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplVideo );
		
	case efpltLanguagePack:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplLanguagePack );
		
	case efpltFont:
		return &lssys.GetSaveFilePatternList( igdeLoadSaveSystem::efplFont );
		
	case efpltSky:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplSky );
		
	case efpltSpeechAnimation:
		return &lssys.GetOpenFilePatternList( igdeLoadSaveSystem::efplSpeechAnimation );
		
	default:
		DETHROW( deeInvalidParam );
	}
}

igdeIcon *igdeEnvironmentIGDE::GetStockIcon( eStockIcons icon ){
	return pWindowMain->GetStockIcon( icon );
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

void igdeEnvironmentIGDE::GetApplicationFont( igdeFont::sConfiguration &config ){
	igdeApplication::app().GetAppFontConfig( config );
}

igdeFont *igdeEnvironmentIGDE::GetSharedFont( const igdeFont::sConfiguration &configuration ){
	return pWindowMain->GetSharedFontList().GetFontWith( configuration );
}

decColor igdeEnvironmentIGDE::GetSystemColor( eSystemColors color ){
	return igdeApplication::app().GetSystemColor( color );
}

deRig *igdeEnvironmentIGDE::GetSharedModelCollisionRig(){
	return pWindowMain->GetSharedModelCollisionRig();
}

void igdeEnvironmentIGDE::AsyncLoadResource( const char *filename,
deResourceLoader::eResourceType resourceType, igdeResourceLoaderListener *listener ){
	pWindowMain->GetResourceLoader().RequestResource( filename, resourceType, listener );
}

igdeUIHelper &igdeEnvironmentIGDE::GetUIHelper(){
	return pWindowMain->GetUIHelper();
}

igdeUIHelper &igdeEnvironmentIGDE::GetUIHelperProperties(){
	return pWindowMain->GetUIHelperProperties();
}

deSkin *igdeEnvironmentIGDE::GetErrorSkin(){
	return pWindowMain->GetErrorSkin();
}

igdeGuiTheme *igdeEnvironmentIGDE::GetGuiThemeNamed( const char *name ){
	igdeGuiTheme *guitheme = pWindowMain->GetGuiThemeNamed( name );
	return guitheme ? guitheme : pWindowMain->GetDefaultGuiTheme();
}

igdeGuiTheme *igdeEnvironmentIGDE::GetDefaultGuiTheme(){
	return pWindowMain->GetDefaultGuiTheme();
}



void igdeEnvironmentIGDE::SetProgressVisible( bool visible ){
	pWindowMain->SetProgressVisible( visible );
}

void igdeEnvironmentIGDE::SetProgress( float progress ){
	pWindowMain->SetProgress( progress );
}

void igdeEnvironmentIGDE::SetProgressText( const char *text ){
	pWindowMain->SetProgressText( text );
}



void igdeEnvironmentIGDE::ActiveModuleSharedMenusChanged(){
	pWindowMain->ActiveModuleSharedMenusChanged();
}

void igdeEnvironmentIGDE::ActiveModuleSharedToolBarsChanged(){
	pWindowMain->ActiveModuleSharedToolBarsChanged();
}



void igdeEnvironmentIGDE::SetColliderDelegee( deCollider *collider,
deBaseScriptingCollider *delegee ){
	if( ! collider ){
		DETHROW( deeInvalidParam );
	}
	
	igdeSMCollider * const peer = ( igdeSMCollider* )collider->GetPeerScripting();
	if( ! peer ){
		DETHROW( deeInvalidParam );
	}
	
	peer->SetDelegee( delegee );
}

void *igdeEnvironmentIGDE::GetColliderUserPointer( deCollider *collider ){
	if( ! collider ){
		DETHROW( deeInvalidParam );
	}
	
	const igdeSMCollider * const peer = ( const igdeSMCollider * )collider->GetPeerScripting();
	if( ! peer ){
		DETHROW( deeInvalidParam );
	}
	
	return peer->GetUserPointer();
}

void igdeEnvironmentIGDE::SetColliderUserPointer( deCollider *collider, void *userPointer ){
	if( ! collider ){
		DETHROW( deeInvalidParam );
	}
	
	igdeSMCollider * const peer = ( igdeSMCollider* )collider->GetPeerScripting();
	if( ! peer ){
		DETHROW( deeInvalidParam );
	}
	
	peer->SetUserPointer( userPointer );
}

void igdeEnvironmentIGDE::SetTouchSensorDelegee( deTouchSensor *touchSensor,
deBaseScriptingTouchSensor *delegee ){
	if( ! touchSensor ){
		DETHROW( deeInvalidParam );
	}
	
	igdeSMTouchSensor * const peer = ( igdeSMTouchSensor* )touchSensor->GetPeerScripting();
	if( ! peer ){
		DETHROW( deeInvalidParam );
	}
	
	peer->SetDelegee( delegee );
}

void igdeEnvironmentIGDE::SetPropFieldDelegee( dePropField *propField,
deBaseScriptingPropField *delegee ){
	if( ! propField ){
		DETHROW( deeInvalidParam );
	}
	
	igdeSMPropField * const peer = ( igdeSMPropField* )propField->GetPeerScripting();
	if( ! peer ){
		DETHROW( deeInvalidParam );
	}
	
	peer->SetDelegee( delegee );
}



void igdeEnvironmentIGDE::RunModalWhileShown( igdeWindow &window ){
	igdeApplication::app().RunModalWhileShown( window );
}



void igdeEnvironmentIGDE::ProjecGameDefinitionChanged(){
	pWindowMain->OnProjectGameDefinitionChanged();
}

void igdeEnvironmentIGDE::ReloadXMLElementClasses(){
	pWindowMain->ReloadXMLElementClasses();
}

bool igdeEnvironmentIGDE::OpenFileInEditor( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	return false;
}

bool igdeEnvironmentIGDE::RequestSaveDocuments( const char *title, const char* message ){
	return pWindowMain->RequestSaveDocuments( title, message );
}

const decStringList &igdeEnvironmentIGDE::GetRecentEditorFiles(){
	return pWindowMain->GetConfigurationLocal().GetRecentEditorFiles().GetFiles();
}

void igdeEnvironmentIGDE::AddRecentEditorFile( const char *filename ){
	igdeConfigurationLocal &configuration = pWindowMain->GetConfigurationLocal();
	const int index = configuration.GetRecentEditorFiles().GetFiles().IndexOf( filename );
	configuration.GetRecentEditorFiles().AddFile( filename );
	if( index != 0 ){
		configuration.SaveConfiguration();
	}
}
/*@}*/
