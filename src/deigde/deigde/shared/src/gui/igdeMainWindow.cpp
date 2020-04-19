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

#include "igdeMainWindow.h"
#include "native/fox/igdeNativeFoxMainWindow.h"
#include "../engine/igdeEngineController.h"
#include "../engine/igdeNullScriptModule.h"
#include "../engine/igdeNullInputModule.h"
#include "../engine/igdeNullCrashRecoveryModule.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeMainWindow
/////////////////////////

// Constructor, destructor
////////////////////////////

igdeMainWindow::igdeMainWindow( igdeEnvironment &environment, const char *title ) :
igdeWindow( environment, title, NULL, true ),
pEngineController( NULL ),
pWindowState( ewsNormal ){
}

igdeMainWindow::~igdeMainWindow(){
	igdeMainWindow::DestroyNativeWidget();
	
	if( pEngineController ){
		delete pEngineController;
	}
}



// Management
///////////////

void igdeMainWindow::SetInitialSize( const decPoint &initialSize ){
	pInitialSize = initialSize.Largest( decPoint() );
}

void igdeMainWindow::SetWindowState( eWindowStates windowState ){
	if( windowState == pWindowState ){
		return;
	}
	
	pWindowState = windowState;
	OnWindowStateChanged();
	
	OnWindowState();
}



void igdeMainWindow::StartEngine(){
	if( pEngineController->GetRunning() ){
		return;
	}
	
	OnBeforeEngineStart();
	
	// TODO: There is no check yet like in the engine window. This has to be added
	//       at some time to avoid troubles.
	pEngineController->StartEngine();
	
	OnAfterEngineStart();
}

void igdeMainWindow::StopEngine(){
	if( ! pEngineController || ! pEngineController->GetRunning() ){
		return;
	}
	
	OnBeforeEngineStop();
	pEngineController->StopEngine();
	OnAfterEngineStop();
}

void igdeMainWindow::OnBeforeEngineStart(){
}

void igdeMainWindow::OnAfterEngineStart(){
}

void igdeMainWindow::OnBeforeEngineStop(){
}

void igdeMainWindow::OnAfterEngineStop(){
}

void igdeMainWindow::OnFrameUpdate(){
}

void igdeMainWindow::OnWindowState(){
}



void igdeMainWindow::Close(){
	DestroyNativeWidget();
}



// Protected Functions
////////////////////////

void igdeMainWindow::CreateEngineController(){
	pEngineController = new igdeEngineController( *this );
	AddNullModules();
	ActivateNullModules();
}

void igdeMainWindow::AddNullModules(){
	deObjectReference module;
	
	module.TakeOver( new igdeNullScriptModule::cModule( pEngineController->GetEngine()->GetModuleSystem() ) );
	pEngineController->AddInternalModule( ( deInternalModule* )( deObject* )module );
	
	module.TakeOver( new igdeNullInputModule::cModule( pEngineController->GetEngine()->GetModuleSystem() ) );
	pEngineController->AddInternalModule( ( deInternalModule* )( deObject* )module );
	
	module.TakeOver( new igdeNullCrashRecoveryModule::cModule( pEngineController->GetEngine()->GetModuleSystem() ) );
	pEngineController->AddInternalModule( ( deInternalModule* )( deObject* )module );
}

void igdeMainWindow::ActivateNullModules(){
	pEngineController->ActivateModule( igdeEngineController::esInput, "NullInput" );
	pEngineController->ActivateModule( igdeEngineController::esScripting, "NullScript" );
	pEngineController->ActivateModule( igdeEngineController::esCrashRecovery, "NullCrashRecovery" );
}



void igdeMainWindow::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxMainWindow * const native = new igdeNativeFoxMainWindow( *this );
	SetNativeWidget( native );
	CreateChildWidgetNativeWidgets();
	
	// NOTE we need to fix the maximize problem during showing the window using PLACEMENT_* .
	//      if we try to maximize(true) here things become the opposite. no idea how FOX
	//      manages to mess up so hard. calling maximize at the end does work albeit looking
	//      ugly while doing so. fixing this when main window is converted to new UI system
	native->create();
	
	// here maximize seems to work
	native->maximize( true );
	
	native->raise();
}

void igdeMainWindow::DropNativeWidget(){
	if( pEngineController ){
		pEngineController->UnparentMainRenderWindow();
	}
	
	igdeWindow::DropNativeWidget();
}

void igdeMainWindow::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	// we use close() on purpose instead of delete because fox requires this
	//delete ( igdeNativeFoxMainWindow* )GetNativeWidget();
	igdeNativeFoxMainWindow * const native = ( igdeNativeFoxMainWindow* )GetNativeWidget();
	DropNativeWidget();
	native->close( false );
}



void igdeMainWindow::OnTitleChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxMainWindow* )GetNativeWidget() )->setTitle( GetTitle().GetString() );
	}
}

void igdeMainWindow::OnIconChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxMainWindow* )GetNativeWidget() )->UpdateIcon();
	}
}

void igdeMainWindow::OnSizeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxMainWindow* )GetNativeWidget() )->resize( GetSize().x, GetSize().y );
	}
}

void igdeMainWindow::OnPositionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxMainWindow* )GetNativeWidget() )->UpdatePosition();
	}
}

void igdeMainWindow::OnVisibleChanged(){
	// never hide main window
}

void igdeMainWindow::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxMainWindow* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeMainWindow::OnWindowStateChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxMainWindow* )GetNativeWidget() )->UpdateWindowState();
	}
}
