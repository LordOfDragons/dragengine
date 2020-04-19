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

#include "igdeWindowLogger.h"
#include "igdeWindowLoggerListener.h"
#include "../igdeCommonDialogs.h"
#include "../igdeTextArea.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionReference.h"
#include "../resources/igdeTextStyle.h"
#include "../resources/igdeTextStyleReference.h"
#include "../../environment/igdeEnvironment.h"
#include "../../logger/igdeLoggerHistory.h"
#include "../../logger/igdeLoggerHistoryEntry.h"

#include <dragengine/common/exceptions.h>



// Actions
////////////

class igdeWindowLogger_ActionTest : public igdeAction{
protected:
	igdeWindowLogger &pWindow;
	
public:
	igdeWindowLogger_ActionTest( igdeWindowLogger &window ) :
	igdeAction( "Test", NULL, "Test" ),
	pWindow( window ){
	}
	
	virtual void OnAction(){
		igdeCommonDialogs::Information( &pWindow, "Clicked", "Clicked." );
	}
};



// Class igdeWindowLogger
///////////////////////////

const char *igdeWindowLogger::styleWarning = "warning";
const char *igdeWindowLogger::styleError = "error";

// Constructor, destructor
////////////////////////////

igdeWindowLogger::igdeWindowLogger( igdeEnvironment &environment ) :
igdeWindow( environment, "Logging History" ),
pListener( NULL ),
pLogger( NULL ),
pPendingAddedLogs( 0 ),
pPendingClearLogs( false )
{
	pListener = new igdeWindowLoggerListener( *this );
	
	SetPosition( decPoint( 10, 50 ) );
	SetSize( decPoint( 800, 600 ) );
	
	pEditLogs.TakeOver( new igdeTextArea( environment, 60, 10, false ) );
	
	igdeTextStyleReference style;
	style.TakeOver( new igdeTextStyle( styleWarning ) );
	style->SetColor( decColor( 0.0f, 0.0f, 0.0f ) );
	style->SetBgColor( decColor( 1.0f, 0.815f, 0.0f ) );
	pEditLogs->AddStyle( style );
	
	style.TakeOver( new igdeTextStyle( styleError ) );
	style->SetColor( decColor( 1.0f, 1.0f, 0.5f ) );
	style->SetBgColor( decColor( 0.75f, 0.0f, 0.0f ) );
// 	style->SetBold( true );
	pEditLogs->AddStyle( style );
	
	AddChild( pEditLogs );
}

igdeWindowLogger::~igdeWindowLogger(){
	SetLogger( NULL );
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void igdeWindowLogger::SetLogger( igdeLoggerHistory *logger ){
	if( logger == pLogger ){
		return;
	}
	
	if( pLogger ){
		pLogger->RemoveListener( pListener );
		pLogger->FreeReference();
	}
	
	pLogger = logger;
	
	if( logger ){
		logger->AddReference();
		logger->AddListener( pListener );
	}
	
	if( GetVisible() ){
		OnLogsCleared();
	}
}



void igdeWindowLogger::OnFrameUpdate(){
	if( ! pLogger ){
		return;
	}
	
	pLogger->GetMutex().Lock();
	
	try{
		if( pPendingAddedLogs >= pLogger->GetHistorySize() ){
			pPendingAddedLogs = 0;
			pPendingClearLogs = true;
		}
		
		if( pPendingClearLogs ){
			pPendingClearLogs = false;
			pClearLogs();
			pPendingAddedLogs = pLogger->GetEntryCount();
		}
		
		while( pPendingAddedLogs > 0 ){
			const igdeLoggerHistoryEntry &entry = pLogger->GetEntryAt(
				pLogger->GetEntryCount() - pPendingAddedLogs );
			pPendingAddedLogs--;
			pAddLog( entry );
		}
		
		pLogger->GetMutex().Unlock();
		
	}catch( const deException &e ){
		pLogger->GetMutex().Unlock();
		e.PrintError();
	}
}



bool igdeWindowLogger::CloseWindow(){
	SetVisible( false );
	return false;
}



void igdeWindowLogger::OnVisibleChanged(){
	igdeWindow::OnVisibleChanged();
	
	if( GetVisible() ){
		OnLogsCleared();
	}
}



void igdeWindowLogger::OnLogAdded(){
	// potentially called from threads. logger mutex is required to be locked
	if( ! pPendingClearLogs ){
		pPendingAddedLogs++;
	}
}

void igdeWindowLogger::OnLogsCleared(){
	// potentially called from threads. logger mutex is required to be locked
	pPendingClearLogs = true;
	pPendingAddedLogs = 0;
}



// Private Functions
//////////////////////

void igdeWindowLogger::pRemoveOldLines(){
	const char * const logs = pEditLogs->GetText();
	const int length = strlen( logs );
	int count = pLogger->GetHistorySize();
	int i;
	
	for( i=length-1; i>=0; i-- ){
		if( logs[ i ] != '\n' ){
			continue;
		}
		
		count--;
		if( count > 0 ){
			continue;
		}
		
		i++;
		break;
	}
	
	if( i <= 0 ){
		return;
	}
	
	if( i == length ){
		pEditLogs->ClearText();
		
	}else{
		pEditLogs->DeleteText( 0, i );
	}
}

void igdeWindowLogger::pAddLog( const igdeLoggerHistoryEntry &entry ){
	const bool atBottom = pEditLogs->GetBottomLine() == pEditLogs->GetLineCount() - 1;
	
	const char *typeMarker = "";
	const char *style = NULL;
	
	switch( entry.GetType() ){
	case igdeLoggerHistoryEntry::emtWarn:
		style = styleWarning;
		typeMarker = "WW ";
		break;
		
	case igdeLoggerHistoryEntry::emtError:
		style = styleError;
		typeMarker = "EE ";
		break;
		
	case igdeLoggerHistoryEntry::emtInfo:
	default:
		style = NULL;
		typeMarker = "II ";
		break;
	}
	
	decString text;
	text.Format( "%s[%s] %s\n", typeMarker, entry.GetSource().GetString(), entry.GetMessage().GetString() );
	
	if( style ){
		igdeActionReference action;
// 		action.TakeOver( new igdeWindowLogger_ActionTest( *this ) );
		pEditLogs->AppendText( text, style, action );
		
	}else{
		pEditLogs->AppendText( text );
	}
	
	pRemoveOldLines();
	
	if( atBottom ){
		pEditLogs->SetBottomLine( pEditLogs->GetLineCount() - 1 );
	}
}

void igdeWindowLogger::pClearLogs(){
	pEditLogs->ClearText();
}
