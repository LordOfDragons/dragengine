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
#include <dragengine/systems/deVRSystem.h>
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
	pAddSystem( GetEngine()->GetVRSystem() );
	
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
				system.GetSystemName().GetString() );
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
