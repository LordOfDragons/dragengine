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
#include <string.h>

#include "igdeEditorWindow.h"
#include "igdeCommonDialogs.h"
#include "resources/igdeFont.h"
#include "event/igdeAction.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"
#include "../gameproject/igdeGameProject.h"
#include "../gui/igdeToolBar.h"
#include "../gui/menu/igdeMenuCascade.h"
#include "../module/igdeEditorModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeEditorWindow
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditorWindow::igdeEditorWindow( igdeEditorModule &editorModule ) :
igdeContainerBox( editorModule.GetEnvironment(), igdeContainerBox::eaY ),
pEditorModule( editorModule ),
pActiveModule( false ),
pRecentFiles( *this, true, 10 ){
}

igdeEditorWindow::~igdeEditorWindow(){
	DestroyNativeWidget();
	
	RemoveAllSharedToolBars();
	RemoveAllSharedMenus();
}



// Management
///////////////

void igdeEditorWindow::OnBeforeEngineStart(){
}

void igdeEditorWindow::OnAfterEngineStart(){
}

void igdeEditorWindow::OnBeforeEngineStop(){
}

void igdeEditorWindow::OnAfterEngineStop(){
}

void igdeEditorWindow::OnActivate(){
	pActiveModule = true;
	
#ifdef OS_W32
	CreateChildWidgetNativeWidgets();
#endif
}

void igdeEditorWindow::OnDeactivate(){
	pActiveModule = false;
}

#ifdef OS_W32
void igdeEditorWindow::SaveResourceHandlers(){
	const int count = GetChildCount();
	int i;
	for( i=0; i<count; i++ ){
		GetChildAt( i )->DestroyNativeWidget();
	}
}
#endif

void igdeEditorWindow::OnFrameUpdate( float ){
}

void igdeEditorWindow::GetChangedDocuments( decStringList& ){
}

void igdeEditorWindow::LoadDocument( const char * ){
}

bool igdeEditorWindow::SaveDocument( const char * ){
	return false;
}

void igdeEditorWindow::RecentFilesChanged(){
}

void igdeEditorWindow::OnGameProjectChanged(){
}

igdeStepableTask *igdeEditorWindow::OnGameDefinitionChanged(){
	return NULL;
}

void igdeEditorWindow::DisplayException( const deException &exception ){
	GetLogger()->LogException( pEditorModule.GetLoggingName(), exception );
	igdeCommonDialogs::Exception( this, exception );
}



// Shared Menus
/////////////////

int igdeEditorWindow::GetSharedMenuCount() const{
	return pSharedMenus.GetCount();
}

igdeMenuCascade *igdeEditorWindow::GetSharedMenuAt( int index ) const{
	return ( igdeMenuCascade* )pSharedMenus.GetAt( index );
}

void igdeEditorWindow::AddSharedMenu( igdeMenuCascade *menu ){
	if( ! menu ){
		DETHROW( deeInvalidParam );
	}
	pSharedMenus.Add( menu );
	
	if( pActiveModule ){
		GetEnvironment().ActiveModuleSharedMenusChanged();
	}
}

void igdeEditorWindow::RemoveSharedMenu( igdeMenuCascade *menu ){
	pSharedMenus.Remove( menu );
	
	if( pActiveModule ){
		GetEnvironment().ActiveModuleSharedMenusChanged();
	}
}

void igdeEditorWindow::RemoveAllSharedMenus(){
	pSharedMenus.RemoveAll();
	
	if( pActiveModule ){
		GetEnvironment().ActiveModuleSharedMenusChanged();
	}
}



// Shared ToolBars
////////////////////

int igdeEditorWindow::GetSharedToolBarCount() const{
	return pSharedToolBars.GetCount();
}

igdeToolBar *igdeEditorWindow::GetSharedToolBarAt( int index ) const{
	return ( igdeToolBar* )pSharedToolBars.GetAt( index );
}

void igdeEditorWindow::AddSharedToolBar( igdeToolBar *toolbar ){
	if( ! toolbar ){
		DETHROW( deeInvalidParam );
	}
	pSharedToolBars.Add( toolbar );
	
	if( pActiveModule ){
		GetEnvironment().ActiveModuleSharedToolBarsChanged();
	}
}

void igdeEditorWindow::RemoveSharedToolBar( igdeToolBar *toolbar ){
	pSharedToolBars.Remove( toolbar );
	
	if( pActiveModule ){
		GetEnvironment().ActiveModuleSharedToolBarsChanged();
	}
}

void igdeEditorWindow::RemoveAllSharedToolBars(){
	pSharedToolBars.RemoveAll();
	
	if( pActiveModule ){
		GetEnvironment().ActiveModuleSharedToolBarsChanged();
	}
}



// Update Actions
///////////////////

void igdeEditorWindow::AddUpdateAction( igdeAction *action ){
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
	pUpdateActions.AddIfAbsent( action );
}

void igdeEditorWindow::RemoveUpdateAction( igdeAction *action ){
	pUpdateActions.RemoveIfPresent( action );
}

void igdeEditorWindow::RemoveAllUpdateActions(){
	pUpdateActions.RemoveAll();
}

void igdeEditorWindow::UpdateAllActions(){
	const int count = pUpdateActions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeAction* )pUpdateActions.GetAt( i ) )->Update();
	}
}



// Internal
/////////////

void igdeEditorWindow::CreateNativeWidget(){
	igdeContainerBox::CreateNativeWidget();
	
#ifdef OS_W32
	if( ! pActiveModule ){
		SaveResourceHandlers();
	}
#endif
}
