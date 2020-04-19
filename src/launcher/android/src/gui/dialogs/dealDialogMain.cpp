/* 
 * Drag[en]gine Android Launcher
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

#include "dealDialogMain.h"
#include "../dealDisplay.h"
#include "../dealImage.h"
#include "../widgets/dealWidgetButton.h"
#include "../widgets/dealWidgetCheckBox.h"
#include "../widgets/dealWidgetOptionBox.h"
#include "../widgets/dealWidgetOptionGroup.h"
#include "../widgets/dealWidgetBorderImage.h"
#include "../widgets/dealWidgetLayoutFlow.h"
#include "../widgets/dealWidgetLogFile.h"
#include "../widgets/dealWidgetTaskStatus.h"
#include "../../dealLauncher.h"
#include "../../common/exceptions.h"
#include "../../logger/deLogger.h"

#define LOGSOURCE "DEAndroidLauncher"



// Button launching game
//////////////////////////

class ButtonLaunchGame : public dealWidgetButton{
private:
	/** \brief Dialog. */
	dealDialogMain &pDialog;
	
public:
	/** \brief Create button. */
	ButtonLaunchGame( dealDialogMain &dialog ) :
	dealWidgetButton( dialog.GetDisplay(), NULL, "Run" ),
	pDialog( dialog ){
		SetFromGuiTheme( *dialog.GetDisplay().GetDefaultGuiTheme() );
	}
	
	/** \brief Button action. */
	virtual void OnAction(){
		try{
			GetDisplay().GetLauncher().LaunchGame();
			
		}catch( const deException &e ){
			GetDisplay().GetLauncher().GetLogger().LogException( LOGSOURCE, e );
		}
	}
};



// Class dealDialogMain
/////////////////////////

// Constructors, destructors
//////////////////////////////

dealDialogMain::dealDialogMain( dealDisplay &display ) :
dealDialog( display, "Drag[en]gine Launcher" ),
pTaskCheckEngine( NULL ),
pButtonRun( NULL )
{
	pBuildContent();
}

dealDialogMain::~dealDialogMain(){
	if( pButtonRun ){
		pButtonRun->FreeReference();
	}
}



// Management
///////////////

void dealDialogMain::OnActivate(){
}

void dealDialogMain::OnDeactivate(){
}



void dealDialogMain::SetRunEnabled( bool enabled ){
	if( ! pButtonRun ){
		return;
	}
	pButtonRun->SetEnabled( enabled );
}



// Private functions
//////////////////////

void dealDialogMain::pBuildContent(){
	dealDisplay &display = GetDisplay();
	const int fontSize = display.GetDefaultFontSize();
	
	dealWidgetLayoutFlow * const content = GetContent();
	content->SetLayoutY( dealWidgetLayoutFlow::eltTopFill );
	content->SetGap( fontSize / 2 );
	content->SetPadding( fontSize / 2, fontSize/ 2 );
// 	content->Set = new dealWidgetLayoutFlow( display, false, 0,
// 		dealWidgetLayoutFlow::eltFill, dealWidgetLayoutFlow::eltFill );
// 	content->SetSize( decPoint( display.GetWidth(), display.GetHeight() ) );
// 	content->SetBackgroundColor( decColor( 0.9f, 0.9f, 0.9f ) );
	
	pTaskCheckEngine = new dealWidgetTaskStatus( display );
	pTaskCheckEngine->SetName( "Engine Installation" );
	pTaskCheckEngine->SetStatus( "Verifying" );
	pTaskCheckEngine->SetProgressRange( 100 );
	pTaskCheckEngine->SetProgress( 0 );
	content->AddWidget( pTaskCheckEngine );
	
	// TEST
	dealWidgetLayoutFlow * const buttons = new dealWidgetLayoutFlow( display, true,
		display.GetDefaultFontSize() / 2, dealWidgetLayoutFlow::eltFill, dealWidgetLayoutFlow::eltFill );
	
	dealWidgetButton * const button1 = new dealWidgetButton( display, NULL, "Button 1" );
	button1->SetFromGuiTheme( *display.GetDefaultGuiTheme() );
	buttons->AddWidget( button1 );
	button1->FreeReference();
	
	dealWidgetButton * const button2 = new dealWidgetButton( display, NULL, "Button 2" );
	button2->SetEnabled( false );
	button2->SetFromGuiTheme( *display.GetDefaultGuiTheme() );
	buttons->AddWidget( button2 );
	button2->FreeReference();
	
	pButtonRun = new ButtonLaunchGame( *this );
	pButtonRun->SetEnabled( false );
	buttons->AddWidget( pButtonRun );
	
	content->AddWidget( buttons );
	
	dealWidgetLayoutFlow * const checkboxes = new dealWidgetLayoutFlow( display, true,
		display.GetDefaultFontSize() / 2, dealWidgetLayoutFlow::eltFill, dealWidgetLayoutFlow::eltFill );
	
	dealWidgetCheckBox * const checkbox1 = new dealWidgetCheckBox( display, NULL, "Check Box 1" );
	checkbox1->SetFromGuiTheme( *display.GetDefaultGuiTheme() );
	checkboxes->AddWidget( checkbox1 );
	checkbox1->FreeReference();
	
	dealWidgetCheckBox * const checkbox2 = new dealWidgetCheckBox( display, NULL, "Check Box 2" );
	checkbox2->SetEnabled( false );
	checkbox2->SetChecked( true );
	checkbox2->SetFromGuiTheme( *display.GetDefaultGuiTheme() );
	checkboxes->AddWidget( checkbox2 );
	checkbox2->FreeReference();
	
	content->AddWidget( checkboxes );
	
	dealWidgetLayoutFlow * const optionboxes = new dealWidgetLayoutFlow( display, true,
		display.GetDefaultFontSize() / 4, dealWidgetLayoutFlow::eltFill, dealWidgetLayoutFlow::eltFill );
	dealWidgetOptionGroup * const optionGroup = new dealWidgetOptionGroup;
	
	dealWidgetOptionBox * const optionbox1 = new dealWidgetOptionBox( display, NULL, "A" );
	optionbox1->SetFromGuiTheme( *display.GetDefaultGuiTheme() );
	optionbox1->SetSelected( true );
	optionbox1->SetGroup( optionGroup );
	optionboxes->AddWidget( optionbox1 );
	optionbox1->FreeReference();
	
	dealWidgetOptionBox * const optionbox2 = new dealWidgetOptionBox( display, NULL, "B" );
	optionbox2->SetFromGuiTheme( *display.GetDefaultGuiTheme() );
	optionbox2->SetGroup( optionGroup );
	optionboxes->AddWidget( optionbox2 );
	optionbox2->FreeReference();
	
	dealWidgetOptionBox * const optionbox3 = new dealWidgetOptionBox( display, NULL, "C" );
	optionbox3->SetFromGuiTheme( *display.GetDefaultGuiTheme() );
	optionbox3->SetGroup( optionGroup );
	optionboxes->AddWidget( optionbox3 );
	optionbox3->FreeReference();
	
	dealWidgetOptionBox * const optionbox4 = new dealWidgetOptionBox( display, NULL, "D" );
	optionbox4->SetEnabled( false );
	optionbox4->SetFromGuiTheme( *display.GetDefaultGuiTheme() );
	optionbox4->SetGroup( optionGroup );
	optionboxes->AddWidget( optionbox4 );
	optionbox4->FreeReference();
	
	optionGroup->FreeReference();
	
	content->AddWidget( optionboxes );
	
	/*
	// TEST
	dealImage * const borderImageImage = new dealImage( display, "guitheme/button_normal.tga" );
	dealWidgetBorderImage * const borderImage = new dealWidgetBorderImage( display, borderImageImage, 4, 4, 4, 4 );
	borderImageImage->FreeReference();
	borderImage->SetScaling( 2.0f );
	borderImage->SetExplicitMinimumSize( decPoint( 100, 100 ) );
	content->AddWidget( borderImage );
	borderImage->FreeReference();
	*/
	
	dealWidgetLogFile * const logWidget = new dealWidgetLogFile( display );
	logWidget->SetFilename( "/logs/delauncherandroid.log" );
	logWidget->SetVisibleLineCount( 5 ); // if too large clips outside view range
	content->AddWidget( logWidget );
	logWidget->FreeReference();
}
