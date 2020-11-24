/* 
 * Drag[en]gine IGDE Project Editor
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

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "projPanelTestRun.h"
#include "projPanelTestRunListener.h"
#include "projWindowMain.h"
#include "../project/projProject.h"
#include "../project/profile/projProfile.h"
#include "../testrunner/projTestRunner.h"
#include "../testrunner/profile/projTRProfile.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gameproject/igdeGameProject.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionExternOpen.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/layout/igdeContainerScrollReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/gui/resources/igdeTextStyle.h>
#include <deigde/gui/resources/igdeTextStyleReference.h>



// Events
///////////

namespace {

class cActionStart : public igdeAction{
	projPanelTestRun &pPanel;
public:
	cActionStart( projPanelTestRun &panel ) : igdeAction( "Start",
		panel.GetWindowMain().GetIconStart(), "Test-Run project using selected Launcher Profile" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		pPanel.Start();
	}
	
	virtual void Update(){
		SetEnabled( ! pPanel.IsRunning() && pPanel.GetSelectedProfile() );
	}
};

class cActionQuit : public igdeAction{
	projPanelTestRun &pPanel;
public:
	cActionQuit( projPanelTestRun &panel ) : igdeAction( "Stop",
		panel.GetWindowMain().GetIconStop(), "Stop Test-Run project" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		pPanel.Quit();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.IsRunning() );
	}
};

class cActionKill : public igdeAction{
	projPanelTestRun &pPanel;
public:
	cActionKill( projPanelTestRun &panel ) : igdeAction( "Kill",
		panel.GetWindowMain().GetIconKill(), "Kill Test-Run project" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		pPanel.Quit();
	}
	
	virtual void Update(){
		SetEnabled( pPanel.GetTestRunner()->IsRunning() );
	}
};


class cComboProfile : public igdeComboBoxListener{
	projPanelTestRun &pPanel;
public:
	cComboProfile( projPanelTestRun &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		
		if( selection ){
			pPanel.SelectProfile( ( projProfile* )selection->GetData() );
			
		}else{
			pPanel.SelectProfile( NULL );
		}
		
		pPanel.UpdateWidgetEnabled();
	}
};

class cComboLaunchProfile : public igdeComboBoxListener{
	projPanelTestRun &pPanel;
public:
	cComboLaunchProfile( projPanelTestRun &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		projProject * const project = pPanel.GetProject();
		if( ! project ){
			return;
		}
		
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if( ! selection ){
			project->SetActiveLaunchProfile( "" );
			return;
		}
		
		const projTRProfile * const profile = ( const projTRProfile* )selection->GetData();
		if( ! profile ){
			project->SetActiveLaunchProfile( "" );
			return;
		}
		
		project->SetActiveLaunchProfile( profile->GetName() );
	}
};

}



// Class projPanelTestRun
///////////////////////////

const char *projPanelTestRun::styleWarning = "warning";
const char *projPanelTestRun::styleError = "error";

// Constructor, destructor
////////////////////////////

projPanelTestRun::projPanelTestRun( projWindowMain &windowMain ) :
igdeContainerSplitted( windowMain.GetEnvironment(), igdeContainerSplitted::espLeft, 250 ),

pWindowMain( windowMain ),

pProject( NULL ),
pListener( NULL ),

pTestRunner( NULL ),
pIsRunning( false ),
pMaxLines( 500 )
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	
	pListener = new projPanelTestRunListener( *this );
	
	
	// create actions
	pActionStart.TakeOver( new cActionStart( *this ) );
	pActionQuit.TakeOver( new cActionQuit( *this ) );
	pActionKill.TakeOver( new cActionKill( *this ) );
	
	
	
	// side panel
	igdeContainerScrollReference scroll;
	igdeContainerReference sidePanel;
	helper.SidePanel( scroll, sidePanel, false, 5 );
	sidePanel->SetWidgetGuiThemeName( "" );
	AddChild( scroll, eaSide );
	
	
	igdeContainerReference groupBox;
	groupBox.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	helper.Label( groupBox, "Profile:" );
	helper.ComboBox( groupBox, "Distribution profile to run.",
		pCBProfile, new cComboProfile( *this ) );
	pCBProfile->SetDefaultSorter();
	sidePanel->AddChild( groupBox );
	
	groupBox.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	helper.Label( groupBox, "Launch Profile:" );
	helper.ComboBox( groupBox, "Launcher profile to use for testing.",
		pCBLaunchProfile, new cComboLaunchProfile( *this ) );
	pCBLaunchProfile->SetDefaultSorter();
	sidePanel->AddChild( groupBox );
	
	helper.Button( sidePanel, pBtnStart, pActionStart );
	helper.Button( sidePanel, pBtnQuit, pActionQuit );
	helper.Button( sidePanel, pBtnKill, pActionKill );
	
	helper.GroupBoxStaticFlow( sidePanel, groupBox, "Logs / Debug:" );
	helper.Button( groupBox, windowMain.GetActionShowLogs() );
	helper.Button( groupBox, windowMain.GetActionShowConfig() );
	helper.Button( groupBox, windowMain.GetActionShowOverlay() );
	helper.Button( groupBox, windowMain.GetActionShowCapture() );
	
	
	// logs widget
	pEditLogs.TakeOver( new igdeTextArea( env, 60, 10, false ) );
	
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
	
	AddChild( pEditLogs, eaCenter );
	
	
	// update
	pTestRunner = new projTestRunner( windowMain );
}

projPanelTestRun::~projPanelTestRun(){
	SetProject( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void projPanelTestRun::SetProject( projProject *project ){
	if( project == pProject ){
		return;
	}
	
	pTestRunner->Kill();
	ClearLogs();
	pIsRunning = false;
	pSelectedProfile = NULL;
	
	if( pProject ){
		pProject->RemoveListener( pListener );
		pProject->FreeReference();
	}
	
	pEditLogs->ClearText();
	
	pProject = project;
	
	if( project ){
		project->AddReference();
		project->AddListener( pListener );
	}
	
	UpdateWidgetEnabled();
	
	UpdateProfiles();
	pUpdateLaunchProfiles();
	
	if( project && project->GetActiveLaunchProfile().IsEmpty() ){
		project->SetActiveLaunchProfile( pTestRunner->GetDefaultLauncherProfileName() );
	}
	
	SelectLauncherProfile();
	
	UpdateLogs( true );
	MoveToBottomLine();
}



bool projPanelTestRun::IsRunning(){
	return pIsRunning;
}

void projPanelTestRun::Start(){
	if( pIsRunning || ! pProject ){
		return;
	}
	
	projProfile * const profile = GetSelectedProfile();
	if( ! profile ){
		return;
	}
	
	if( pProject->GetChanged() ){
		pWindowMain.SaveProject();
	}
	
	if( ! GetEnvironment().RequestSaveDocuments( "Start Test-Running",
	"Unsaved changes are present. To start Test-Running it is recommended to save them" ) ){
		return;
	}
	
	projTRProfile *launchProfile = NULL;
	if( ! pProject->GetActiveLaunchProfile().IsEmpty() ){
		launchProfile = pTestRunner->GetLauncherProfiles().GetNamed(
			pProject->GetActiveLaunchProfile() );
	}
	
	try{
		pTestRunner->Start( profile, launchProfile );
		
	}catch( const deException &e ){
		try{
			pTestRunner->Kill();
			
		}catch( const deException & ){
		}
		UpdateLogs( false );
		
		igdeCommonDialogs::Exception( &pWindowMain, e );
	}
	
	pEditLogs->ClearText();
	pIsRunning = true;
	UpdateWidgetEnabled();
}

void projPanelTestRun::Quit(){
	if( ! pIsRunning ){
		return;
	}
	
	try{
		pTestRunner->SendQuit();
		pIsRunning = false;
		
	}catch( const deException &e ){
		igdeCommonDialogs::Exception( &pWindowMain, e );
	}
	
	UpdateWidgetEnabled();
}

void projPanelTestRun::Kill(){
	if( ! pTestRunner->IsRunning() ){
		return;
	}
	
	pIsRunning = false;
	
	try{
		pTestRunner->Kill();
		
	}catch( const deException &e ){
		igdeCommonDialogs::Exception( &pWindowMain, e );
	}
	
	UpdateLogs( false );
	UpdateWidgetEnabled();
}

void projPanelTestRun::Update( float elapsed ){
	CheckRunning();
	
	if( pTestRunner->IsRunning() ){
		UpdateLogs( false );
	}
}

void projPanelTestRun::UpdateLogs( bool lastLogs ){
	decString logs( lastLogs ? pGetLastLogsMaxLines() : pTestRunner->ReadNextLogData() );
	if( logs.IsEmpty() ){
		return;
	}
	
	if( logs.GetAt( 0 ) == '\n' ){
		logs = logs.GetMiddle( 1 );
	}
	if( logs.IsEmpty() ){
		return;
	}
	
	if( logs.GetAt( -1 ) != '\n' ){
		logs.AppendCharacter( '\n' );
	}
	
	const bool atBottom = pEditLogs->GetBottomLine() == pEditLogs->GetLineCount() - 1;
	
	// append text
	const int startPosition = pEditLogs->GetText().GetLength();
	pEditLogs->AppendText( logs );
	
	// style text
	const int logEnd = logs.GetLength();
	const char *logPtr = logs.GetString();
	const char *style = NULL;
	int logPosition = 0;
	int styleBegin = 0;
	int mode = 0;
	
	while( logPosition < logEnd ){
		switch( mode ){
		case 0: // begin of line
			if( logPtr[ logPosition ] == '\n' ){
				break; // skip empty line
			}
			
			if( strncmp( logPtr + logPosition, "II ", 3 ) == 0 ){
				style = NULL;
				
			}else if( strncmp( logPtr + logPosition, "WW ", 3 ) == 0 ){
				style = styleWarning;
				
			}else if( strncmp( logPtr + logPosition, "EE ", 3 ) == 0 ){
				style = styleError;
				
			}else{
				style = NULL;
			}
			
			styleBegin = logPosition;
			mode = 1;
			break;
			
		case 1: // find end of line
			if( logPtr[ logPosition ] == '\n' || logPtr[ logPosition + 1 ] == 0 ){
				if( style ){
					pEditLogs->SetTextSegment( startPosition + styleBegin,
						startPosition + logPosition, style, NULL );
				}
				mode = 0;
			}
			break;
		}
		
		logPosition++;
	}
	
	pRemoveOldLines();
	
	if( atBottom ){
		pEditLogs->SetBottomLine( pEditLogs->GetLineCount() - 1 );
	}
}

void projPanelTestRun::ClearLogs(){
	pEditLogs->ClearText();
}

void projPanelTestRun::CheckRunning(){
	if( pTestRunner->IsRunning() ){
		return;
	}
	
	try{
		pTestRunner->Stop();
		
	}catch( const deException & ){
		pTestRunner->Kill();
	}
	
	UpdateLogs();
	pIsRunning = false;
	UpdateWidgetEnabled();
}

void projPanelTestRun::UpdateWidgetEnabled(){
	pCBProfile->SetEnabled( ! pIsRunning );
	pActionStart->Update();
	pActionQuit->Update();
	pActionKill->Update();
}

void projPanelTestRun::UpdateProfiles(){
	pCBProfile->RemoveAllItems();
	
	if( pProject ){
		const projProfileList &list = pProject->GetProfiles();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			projProfile * const profile = list.GetAt( i );
			pCBProfile->AddItem( profile->GetName(), NULL, profile );
		}
		
		pCBProfile->SortItems();
	}
	
	SelectProfile( ( projProfile* )( deObject* )pSelectedProfile );
	
	if( ! pSelectedProfile && pCBProfile->GetItemCount() > 0 ){
		SelectProfile( ( projProfile* )pCBProfile->GetItemAt( 0 )->GetData() );
	}
}

projProfile *projPanelTestRun::GetSelectedProfile() const{
	return ( projProfile* )( deObject* )pSelectedProfile;
}

void projPanelTestRun::SelectProfile( projProfile *profile ){
	pSelectedProfile = profile;
	pCBProfile->SetSelectionWithData( profile );
}


void projPanelTestRun::SelectLauncherProfile(){
	if( ! pProject ){
		return;
	}
	
	if( ! pProject->GetActiveLaunchProfile().IsEmpty() ){
		pCBLaunchProfile->SetSelectionWithData( pTestRunner->GetLauncherProfiles()
			.GetNamed( pProject->GetActiveLaunchProfile() ) );
		
	}else{
		pCBLaunchProfile->SetSelectionWithData( NULL );
	}
}

void projPanelTestRun::MoveToBottomLine(){
	const int lastLine = pEditLogs->GetLineCount() - 1;
	if( lastLine > 0 ){
		pEditLogs->SetBottomLine( lastLine );
	}
}



// Private Functions
//////////////////////

void projPanelTestRun::pUpdateLaunchProfiles(){
	pCBLaunchProfile->RemoveAllItems();
	pCBLaunchProfile->AddItem( "< IGDE Default >", NULL, NULL );
	
	pTestRunner->LoadEngineConfiguration();
	
	const projTRProfileList &list = pTestRunner->GetLauncherProfiles();
	const int count = list.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		projTRProfile * const profile = list.GetAt( i );
		pCBLaunchProfile->AddItem( profile->GetName(), NULL, profile );
	}
	
	SelectLauncherProfile();
}

void projPanelTestRun::pRemoveOldLines(){
	const char * const logs = pEditLogs->GetText();
	const int length = strlen( logs );
	int count = pMaxLines;
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

decString projPanelTestRun::pGetLastLogsMaxLines() const{
	// log files can be potentially very long with many lines (over 100k while debugging).
	// using Split('\n') is the simple solution but building arrays of such length just to
	// use the last pMaxLines lines can slow down a lot. for this reason the last pMaxLines
	// lines are searched using a backwards search on the full text. this is less simple
	// but a lot faster
	
	/*
	const decStringList lines( pTestRunner->GetLastLogContent().Split( '\n' ) );
	const int count = lines.GetCount();
	if( count == 0 ){
		return decString();
	}
	
	return lines.Splice( decMath::max( count - 1 - pMaxLines, 0 ) ).Join( "\n" );
	*/
	
	const decString logContent( pTestRunner->GetLastLogContent() );
	int i, index = logContent.GetLength();
	for( i=0; i<pMaxLines; i++ ){
		const int nextIndex = logContent.FindReverse( '\n', 0, index );
		if( nextIndex == -1 ){
			return logContent;
		}
		index = nextIndex;
	}
	
	return logContent.GetMiddle( index + 1 );
}
