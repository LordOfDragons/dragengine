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

#include "igdeDEStatus.h"
#include "igdeDialogEngine.h"

#include "../igdeUIHelper.h"
#include "../igdeTextArea.h"
#include "../igdeButton.h"
#include "../igdeIconListBox.h"
#include "../igdeContainerReference.h"
#include "../igdeMainWindow.h"
#include "../event/igdeAction.h"
#include "../layout/igdeContainerForm.h"
#include "../layout/igdeContainerBox.h"
#include "../model/igdeListItem.h"
#include "../../engine/igdeEngineController.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/deScriptingSystem.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/deCrashRecoverySystem.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/systems/deSynthesizerSystem.h>



// Actions
////////////

class igdeDEStatus_ActionStart : public igdeAction{
	igdeDEStatus &pPanel;
	
public:
	igdeDEStatus_ActionStart( igdeDEStatus &panel ) :
		igdeAction( "Start Engine", NULL, "Start game engine" ), pPanel( panel ){}
	
	virtual void OnAction(){
		pPanel.StartEngine();
	}
	
	virtual void Update(){
		SetEnabled( ! pPanel.GetEngineController().GetRunning() );
	}
};

class igdeDEStatus_ActionStop : public igdeAction{
	igdeDEStatus &pPanel;
	
public:
	igdeDEStatus_ActionStop( igdeDEStatus &panel ) :
		igdeAction( "Stop Engine", NULL, "Stop game engine" ), pPanel( panel ){}
	
	virtual void OnAction(){
		pPanel.StopEngine();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetEngineController().GetRunning() );
	}
};



// Class igdeDEStatus
///////////////////////
	
// Constructor, destructor
////////////////////////////

igdeDEStatus::igdeDEStatus( igdeDialogEngine &dialogEngine ) :
igdeContainerFlow( dialogEngine.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esLast ),
pDialogEngine( dialogEngine )
{
	igdeEnvironment &env = dialogEngine.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainerReference groupBox, panel;
	
	
	helper.GroupBoxStaticFlow( *this, groupBox, "Engine Status:" );
	helper.EditString( groupBox, "Engine status", pTextStatus, 3, NULL );
	
	panel.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaX ) );
	groupBox->AddChild( panel );
	helper.Button( panel, pBtnStart, new igdeDEStatus_ActionStart( *this ), true );
	helper.Button( panel, pBtnStop, new igdeDEStatus_ActionStop( *this ), true );
	
	
	helper.GroupBoxStaticFlow( *this, groupBox, "System Status:", true );
	const igdeUIHelper::sColumnHeader columns[ 3 ] = {
		igdeUIHelper::sColumnHeader( "System Name", NULL, 150 ),
		igdeUIHelper::sColumnHeader( "Active Module", NULL, 200 ),
		igdeUIHelper::sColumnHeader( "Status", NULL, 150 )
	};
	helper.IconListBox( groupBox, pListSystems, decPoint( 100, 150 ), columns, 3, "System Status", NULL );
	
	pAddSystem( GetEngine()->GetGraphicSystem() );
	pAddSystem( GetEngine()->GetAudioSystem() );
	pAddSystem( GetEngine()->GetInputSystem() );
	pAddSystem( GetEngine()->GetPhysicsSystem() );
	pAddSystem( GetEngine()->GetScriptingSystem() );
	pAddSystem( GetEngine()->GetCrashRecoverySystem() );
	pAddSystem( GetEngine()->GetAnimatorSystem() );
	pAddSystem( GetEngine()->GetSynthesizerSystem() );
	
	UpdateStatus();
}

igdeDEStatus::~igdeDEStatus(){
}



// Management
///////////////

void igdeDEStatus::UpdateStatus(){
	const int count = pListSystems->GetItemCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeListItem &item = *pListSystems->GetItemAt( i );
		const deBaseSystem &system = *( ( deBaseSystem* )item.GetData() );
		const deLoadableModule * const loadedModule = system.GetActiveLoadableModule();
		
		if( loadedModule ){
			item.GetDetails().SetAt( 0, loadedModule->GetName() );
			
		}else{
			item.GetDetails().SetAt( 0, "<No Module Loaded>" );
		}
		
		if( system.GetIsRunning() ){
			item.GetDetails().SetAt( 1, "Running" );
			
		}else{
			item.GetDetails().SetAt( 1, "Stopped" );
		}
		
		pListSystems->ItemChangedAt( i );
	}
	
	if( pTextStatus->GetText().IsEmpty() ){
		if( GetEngineController().GetRunning() ){
			pTextStatus->SetText( "Engine is running" );
			
		}else{
			pTextStatus->SetText( "Engine is not running" );
		}
	}
	
	pBtnStart->GetAction()->Update();
	pBtnStop->GetAction()->Update();
}

void igdeDEStatus::StartEngine(){
	if( GetEngineController().GetRunning() ){
		return;
	}
	
	pTextStatus->ClearText();
	
	const int count = pListSystems->GetItemCount();
	decString message;
	int i;
	
	try{
		// test if we can start all required systems
		for( i=0; i<count; i++ ){
			deBaseSystem &system = *( ( deBaseSystem* )pListSystems->GetItemAt( i )->GetData() );
			if( system.CanStart() ){
				continue;
			}
			
			message.Format( "Engine can not be started because system %s is not possible to start."
				" Verify that a module is selected and that this module is runnable on this system.",
				system.GetSystemName() );
			pTextStatus->SetText( message );
			break;
		}
		
		// do the rest and mark as running
		pDialogEngine.GetMainWindow().StartEngine();
		
	}catch( const deException &e ){
		pTextStatus->SetText( e.FormatOutput().Join( "\n" ) );
	}
	
	UpdateStatus();
}

void igdeDEStatus::StopEngine(){
	if( ! GetEngineController().GetRunning() ){
		return;
	}
	
	pTextStatus->ClearText();
	
	try{
		pDialogEngine.GetMainWindow().StopEngine();
		
	}catch( const deException &e ){
		pTextStatus->SetText( e.FormatOutput().Join( "\n" ) );
	}
	
	UpdateStatus();
}



// Private Functions
//////////////////////

void igdeDEStatus::pAddSystem( deBaseSystem *system ){
	decStringList details;
	details.Add( "-" );
	details.Add( "-" );
	pListSystems->AddItem( system->GetSystemName(), details, NULL, system );
}
