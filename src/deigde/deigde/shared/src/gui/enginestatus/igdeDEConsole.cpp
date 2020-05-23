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

#include "igdeDEConsole.h"
#include "igdeDialogEngine.h"
#include "../igdeUIHelper.h"
#include "../igdeComboBox.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../event/igdeComboBoxListener.h"
#include "../event/igdeTextFieldListener.h"
#include "../model/igdeListItem.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/common/string/unicode/decUnicodeLineBuffer.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/systems/deModuleSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/deLoadableModule.h>



// Actions
////////////

class igdeDEConsole_TextCommand : public igdeTextFieldListener{
	igdeDEConsole &pPanel;
	
public:
	igdeDEConsole_TextCommand( igdeDEConsole &panel ) : pPanel( panel ){}
	
	virtual void OnEnterKey( igdeTextField* ){
		pPanel.SendCommand();
	}
};



// Class igdeDEConsole
////////////////////////
	
// Constructor, destructor
////////////////////////////

igdeDEConsole::igdeDEConsole( igdeDialogEngine &windowEngine ) :
igdeContainerFlow( windowEngine.GetEnvironment(), igdeContainerFlow::eaY, igdeContainerFlow::esFirst ),
pDialogEngine( windowEngine ),
pLog( 1000 )
{
	igdeEnvironment &env = GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	
	helper.EditString( *this, "Console Logs", pEditLogs, 10, NULL );
	
	igdeContainerReference line;
	line.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst ) );
	AddChild( line );
	
	helper.EditString( line, "Command to send", pEditCommand, new igdeDEConsole_TextCommand( *this ) );
	
	helper.ComboBox( line, "Module to send command to", pCBModule, NULL );
	pCBModule->SetDefaultSorter();
	
	
	UpdateModulesList();
}

igdeDEConsole::~igdeDEConsole(){
}



// Management
///////////////

void igdeDEConsole::UpdateModulesList(){
	const deModuleSystem &moduleSystem = *GetEngine()->GetModuleSystem();
	const int count = moduleSystem.GetModuleCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deLoadableModule * const module = moduleSystem.GetModuleAt( i );
		if( ! pCBModule->HasItem( module->GetName() ) ){
			pCBModule->AddItem( module->GetName() );
		}
	}
	pCBModule->SortItems();
}

void igdeDEConsole::AddToConsole( decUnicodeString &text ){
	const bool atBottom = pEditLogs->GetBottomLine() == pEditLogs->GetLineCount() - 1;
	
	pLog.AddMultipleLines( text );
	
	decUnicodeString output;
	pLog.FillLinesInto( output );
	pEditLogs->SetText( output.ToUTF8() );
	
	if( atBottom ){
		pEditLogs->SetBottomLine( pEditLogs->GetLineCount() - 1 );
	}
}

void igdeDEConsole::ClearConsole(){
	pLog.Clear();
	pEditLogs->ClearText();
}

void igdeDEConsole::SendCommand(){
	if( ! pCBModule->GetSelectedItem() ){
		return;
	}
	
	// if this is a special command do it
	if( pEditCommand->GetText() == "/clear" ){
		ClearConsole();
		return;
	}
	
	// determine which module to send the command to
	deLoadableModule * const loadableModule = GetEngine()->GetModuleSystem()->
		GetModuleNamed( pCBModule->GetSelectedItem()->GetText() );
	if( ! loadableModule ){
		return;
	}
	
	deBaseModule * const module = loadableModule->GetModule();
	if( ! module ){
		return;
	}
	
	// get the command and build an argument list from it
	decUnicodeArgumentList argList;
	decUnicodeString command;
	
	command.SetFromUTF8( pEditCommand->GetText() );
	argList.ParseCommand( command );
	if( argList.GetArgumentCount() == 0){
		return;
	}
	
	// add commend to send
	decUnicodeString answer;
	answer.SetFromUTF8( loadableModule->GetName() );
	answer.AppendFromUTF8( " > " );
	answer += command;
	AddToConsole( answer );
	
	// send command to the selected module
	try{
		answer.SetFromUTF8( "" );
		module->SendCommand( argList, answer );
		
	}catch( const deException &e ){
		answer.SetFromUTF8( e.FormatOutput().Join( "\n" ) );
	}
	
	if( answer.GetLength() > 0 && answer.GetAt( answer.GetLength() - 1 ) != '\n' ){
		answer.AppendFromUTF8( "\n" );
	}
	
	AddToConsole( answer );
}
