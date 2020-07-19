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

#include "igdeDialog.h"
#include "../native/toolkit.h"
#include "../igdeCommonDialogs.h"
#include "../igdeContainerReference.h"
#include "../igdeSeparator.h"
#include "../igdeButton.h"
#include "../igdeWidget.h"
#include "../igdeWidgetReference.h"
#include "../event/igdeActionReference.h"
#include "../layout/igdeContainerBorder.h"
#include "../layout/igdeContainerBorderReference.h"
#include "../layout/igdeContainerFlow.h"
#include "../layout/igdeContainerBox.h"
#include "../resources/igdeIcon.h"
#include "../theme/igdeGuiTheme.h"
#include "../theme/propertyNames.h"
#include "../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// class igdeDialog::AcceptDialog
///////////////////////////////////

igdeDialog::AcceptDialog::AcceptDialog( igdeDialog &dialog, const char *text, igdeIcon *icon ) :
igdeAction( text, icon ),
pDialog( dialog )
{
	SetDefault( true );
}

igdeDialog::AcceptDialog::~AcceptDialog(){
}

void igdeDialog::AcceptDialog::OnAction(){
	try{
		pDialog.Accept();
		
	}catch( const deException &e ){
		pDialog.GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( &pDialog, e );
	}
}



// class igdeDialog::CancelDialog
///////////////////////////////////

igdeDialog::CancelDialog::CancelDialog( igdeDialog &dialog, const char *text, igdeIcon *icon ) :
igdeAction( text, icon ),
pDialog( dialog ){
}

igdeDialog::CancelDialog::~CancelDialog(){
}



void igdeDialog::CancelDialog::OnAction(){
	try{
		pDialog.Cancel();
		
	}catch( const deException &e ){
		pDialog.GetLogger()->LogException( "IGDE", e );
		igdeCommonDialogs::Exception( &pDialog, e );
	}
}




// Class igdeDialog
/////////////////////

// Constructor, destructor
////////////////////////////

igdeDialog::igdeDialog( igdeEnvironment &environment, const char *title, igdeIcon *icon, bool canResize ) :
igdeWindow( environment, title, icon, canResize ),
pOwner( NULL ),
pAccepted( false ){
}

igdeDialog::~igdeDialog(){
}



// Management
///////////////

void igdeDialog::SetInitialSize( const decPoint &initialSize ){
	pInitialSize = initialSize.Largest( decPoint() );
}

void igdeDialog::DefaultActionChanged(){
	// TODO find a way to tell FOX to update
}

void igdeDialog::AddChild( igdeWidget *child ){
	if( GetChildCount() > 0 ){
		DETHROW( deeInvalidParam );
	}
	igdeContainer::AddChild( child );
}

void igdeDialog::AddContent( igdeWidget *content, igdeWidget *buttonBar ){
	if( ! content || ! buttonBar ){
		DETHROW( deeInvalidParam );
	}
	pAddContent( content, NULL, NULL, NULL, buttonBar );
}

void igdeDialog::AddContent( igdeWidget *content, igdeWidget *header, igdeWidget *buttonBar ){
	if( ! content || ! header || ! buttonBar ){
		DETHROW( deeInvalidParam );
	}
	pAddContent( content, header, NULL, NULL, buttonBar );
}

void igdeDialog::AddContent( igdeWidget *content, igdeWidget *header, igdeWidget *leftPanel,
igdeWidget *rightPanel, igdeWidget *buttonBar ){
	if( ! content || ! header || ! buttonBar ){
		DETHROW( deeInvalidParam );
	}
	pAddContent( content, header, leftPanel, rightPanel, buttonBar );
}

void igdeDialog::CreateButtonBar( igdeContainerReference &buttonBar, igdeAction *action ){
	igdeAction *actions[ 1 ] = { action };
	CreateButtonBar( buttonBar, &actions[ 0 ], 1 );
}

void igdeDialog::CreateButtonBar( igdeContainerReference &buttonBar,
igdeAction *action1, igdeAction *action2 ){
	igdeAction *actions[ 2 ] = { action1, action2 };
	CreateButtonBar( buttonBar, &actions[ 0 ], 2 );
}

void igdeDialog::CreateButtonBar( igdeContainerReference &buttonBar,
igdeAction *action1, igdeAction *action2, igdeAction *action3 ){
	igdeAction *actions[ 3 ] = { action1, action2, action3 };
	CreateButtonBar( buttonBar, &actions[ 0 ], 3 );
}

void igdeDialog::CreateButtonBar( igdeContainerReference &buttonBar,
igdeAction **actions, int actionCount ){
	if( ! actions || actionCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<actionCount; i++ ){
		if( ! actions[ i ] ){
			DETHROW( deeInvalidParam );
		}
	}
	
	igdeEnvironment &env = GetEnvironment();
	const int spacing = igdeNativeDialog::DialogPadButtons( *GetGuiTheme() );
	
	buttonBar.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaX, spacing ) );
	
	igdeWidgetReference button;
	for( i=0; i<actionCount; i++ ){
		button.TakeOver( new igdeButton( env, actions[ i ] ) );
		buttonBar->AddChild( button );
	}
}

void igdeDialog::CreateButtonBar( igdeContainerReference &buttonBar, const char *text ){
	igdeActionReference action;
	action.TakeOver( new CancelDialog( *this, text ) );
	CreateButtonBar( buttonBar, action );
}

void igdeDialog::CreateButtonBar( igdeContainerReference &buttonBar,
const char *textAccept, const char *textCancel ){
	igdeActionReference actionAccept;
	actionAccept.TakeOver( new AcceptDialog( *this, textAccept ) );
	
	igdeActionReference actionCancel;
	actionCancel.TakeOver( new CancelDialog( *this, textCancel ) );
	
	CreateButtonBar( buttonBar, actionAccept, actionCancel );
}



bool igdeDialog::Run( igdeWidget *owner ){
	if( ! owner->GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	if( GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	
	pOwner = owner;
	
	// FXDialogBox box basically runs getApp()->runModalFor(this). this results though in the
	// engine update code to be skipped. the code below modifies the FXDialogBox::execute
	// behavior to run igdeEnvironment::RunModalWhileShown instead of FXApp::runModalFor
	pAccepted = false;
	
	try{
		CreateNativeWidget();
		
		( ( igdeNativeDialog* )GetNativeWidget() )->ShowDialog();
		
		OnDialogShown();
		GetEnvironment().RunModalWhileShown( *this );
		
		DestroyNativeWidget();
		
	}catch( const deException & ){
		DestroyNativeWidget();
		throw;
	}
	
	return pAccepted;
}

bool igdeDialog::Accept(){
	CloseDialog( true );
	return true;
}

bool igdeDialog::Cancel(){
	CloseDialog( false );
	return true;
}

void igdeDialog::CloseDialog( bool accepted ){
	if( ! GetNativeContainer() ){
		DETHROW( deeInvalidParam );
	}
	
	pAccepted = accepted;
	
	( ( igdeNativeDialog* )GetNativeWidget() )->CloseDialog( accepted );
}



void igdeDialog::OnDialogShown(){
}

void igdeDialog::OnFrameUpdate(){
}



// IGDE Internal Use Only
///////////////////////////

void igdeDialog::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeDialog * const native = igdeNativeDialog::CreateNativeWidget( *this, pOwner );
	SetNativeWidget( native );
	CreateChildWidgetNativeWidgets();
	native->PostCreateNativeWidget();
}

void igdeDialog::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeDialog* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



// Protected Functions
////////////////////////

void igdeDialog::pAddContent( igdeWidget *content, igdeWidget *header, igdeWidget *leftPanel,
igdeWidget *rightPanel, igdeWidget *buttonBar ){
	igdeEnvironment &env = GetEnvironment();
	const int spacing = igdeNativeDialog::DialogPadContent( *GetGuiTheme() );
	
	igdeContainerBorderReference border;
	border.TakeOver( new igdeContainerBorder( env, spacing ) );
	AddChild( border );
	
	if( header ){
		border->AddChild( header, igdeContainerBorder::eaTop );
	}
	if( leftPanel ){
		border->AddChild( leftPanel, igdeContainerBorder::eaLeft );
	}
	if( rightPanel ){
		border->AddChild( rightPanel, igdeContainerBorder::eaRight );
	}
	
	border->AddChild( content, igdeContainerBorder::eaCenter );
	
	igdeContainerReference bottom;
	bottom.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY,
		igdeContainerFlow::esNone, spacing ) );
	border->AddChild( bottom, igdeContainerBorder::eaBottom );
	
	igdeWidgetReference separator;
	separator.TakeOver( new igdeSeparator( env, igdeSeparator::eoHorizontal ) );
	bottom->AddChild( separator );
	bottom->AddChild( buttonBar );
}



void igdeDialog::OnTitleChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeDialog* )GetNativeWidget() )->UpdateTitle();
	}
}

void igdeDialog::OnIconChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeDialog* )GetNativeWidget() )->UpdateIcon();
	}
}

void igdeDialog::OnSizeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeDialog* )GetNativeWidget() )->UpdateSize();
	}
}

void igdeDialog::OnPositionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeDialog* )GetNativeWidget() )->UpdatePosition();
	}
}

void igdeDialog::OnVisibleChanged(){
	// has no effect
}

void igdeDialog::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeDialog* )GetNativeWidget() )->UpdateEnabled();
	}
}
