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

igdeEditorModule::igdeEditorModule( igdeEnvironment &environment ) :
pEnvironment( environment ),
pEditorWindow( NULL ){
}

igdeEditorModule::~igdeEditorModule(){
	if( pEditorWindow ){
		pEditorWindow->FreeReference();
	}
}



// Management
///////////////

void igdeEditorModule::SetEditorDirectory( const char *directory ){
	pEditorDirectory = directory;
}

void igdeEditorModule::SetEditorPathLib( const char *path ){
	pEditorPathLib = path;
}

void igdeEditorModule::SetEditorWindow( igdeEditorWindow *editorWindow ){
	if( editorWindow && editorWindow->GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	if( editorWindow == pEditorWindow ){
		return;
	}
	
	igdeContainer * const uiContainer = pEnvironment.GetUIContainer();
	
	if( pEditorWindow ){
		if( pEditorWindow->GetParent() == uiContainer ){
			uiContainer->RemoveChild( pEditorWindow );
		}
		pEditorWindow->FreeReference();
	}
	
	pEditorWindow = editorWindow;
	
	if( editorWindow ){
		// not adding a reference since we take over the reference!
		uiContainer->AddChild( pEditorWindow );
	}
}



void igdeEditorModule::Start(){
}

void igdeEditorModule::Stop(){
	SetEditorWindow( NULL );
}

void igdeEditorModule::OnBeforeEngineStart(){
	if( pEditorWindow ){
		pEditorWindow->OnBeforeEngineStart();
	}
}

void igdeEditorModule::OnAfterEngineStart(){
	if( pEditorWindow ){
		pEditorWindow->OnAfterEngineStart();
	}
}

void igdeEditorModule::OnBeforeEngineStop(){
	if( pEditorWindow ){
		pEditorWindow->OnBeforeEngineStop();
	}
}

void igdeEditorModule::OnAfterEngineStop(){
	if( pEditorWindow ){
		pEditorWindow->OnAfterEngineStop();
	}
}

void igdeEditorModule::OnActivate(){
	if( pEditorWindow ){
		pEditorWindow->OnActivate();
	}
}

void igdeEditorModule::OnDeactivate(){
	if( pEditorWindow ){
		pEditorWindow->OnDeactivate();
	}
}

void igdeEditorModule::OnFrameUpdate( float elapsed ){
	if( pEditorWindow ){
		pEditorWindow->OnFrameUpdate( elapsed );
	}
}

void igdeEditorModule::GetChangedDocuments( decStringList &list ){
	if( pEditorWindow ){
		pEditorWindow->GetChangedDocuments( list );
	}
}

bool igdeEditorModule::SaveDocument( const char *filename ){
	if( pEditorWindow ){
		return pEditorWindow->SaveDocument( filename );
		
	}else{
		return false;
	}
}

void igdeEditorModule::LoadDocument( const char *filename ){
	if( pEditorWindow ){
		pEditorWindow->LoadDocument( filename );
	}
}

void igdeEditorModule::OnGameProjectChanged(){
	if( pEditorWindow ){
		pEditorWindow->OnGameProjectChanged();
	}
}

igdeStepableTask *igdeEditorModule::OnGameDefinitionChanged(){
	if( pEditorWindow ){
		return pEditorWindow->OnGameDefinitionChanged();
		
	}else{
		return NULL;
	}
}

bool igdeEditorModule::ProcessCommandLine( decUnicodeStringList& ){
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

void igdeEditorModule::SetLoggingName( const char *name ){
	pLoggingName = name;
}



void igdeEditorModule::LogInfo( const char *message ) const{
	GetLogger()->LogInfo( pLoggingName.GetString(), message );
}

void igdeEditorModule::LogInfoFormat( const char *message, ... ) const{
	va_list list;
	va_start( list, message );
	GetLogger()->LogInfoFormatUsing( pLoggingName.GetString(), message, list );
	va_end( list );
}

void igdeEditorModule::LogInfoFormatUsing( const char *message, va_list args ) const{
	GetLogger()->LogInfoFormatUsing( pLoggingName.GetString(), message, args );
}

void igdeEditorModule::LogWarn( const char *message ) const{
	GetLogger()->LogWarn( pLoggingName.GetString(), message );
}

void igdeEditorModule::LogWarnFormat( const char *message, ... ) const{
	va_list list;
	va_start( list, message );
	GetLogger()->LogWarnFormatUsing( pLoggingName.GetString(), message, list );
	va_end( list );
}

void igdeEditorModule::LogWarnFormatUsing( const char *message, va_list args ) const{
	GetLogger()->LogWarnFormatUsing( pLoggingName.GetString(), message, args );
}

void igdeEditorModule::LogError( const char *message ) const{
	GetLogger()->LogError( pLoggingName.GetString(), message );
}

void igdeEditorModule::LogErrorFormat( const char *message, ... ) const{
	va_list list;
	va_start( list, message );
	GetLogger()->LogErrorFormatUsing( pLoggingName.GetString(), message, list );
	va_end( list );
}

void igdeEditorModule::LogErrorFormatUsing( const char *message, va_list args ) const{
	GetLogger()->LogErrorFormatUsing( pLoggingName.GetString(), message, args );
}

void igdeEditorModule::LogException( const deException &exception ) const{
	GetLogger()->LogException( pLoggingName.GetString(), exception );
}
