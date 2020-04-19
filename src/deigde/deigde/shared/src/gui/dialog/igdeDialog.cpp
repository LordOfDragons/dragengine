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



// Native Widget
//////////////////

namespace{

class igdeNativeFoxDialog : public FXDialogBox{
	FXDECLARE( igdeNativeFoxDialog )
	
protected:
	igdeNativeFoxDialog();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
	};
	
private:
	igdeDialog *pOwner;
	
public:
	igdeNativeFoxDialog( igdeDialog &owner, FXWindow *parent, const igdeGuiTheme &guitheme );
	virtual ~igdeNativeFoxDialog();
	
	virtual void UpdateEnabled();
	virtual void UpdatePosition();
	virtual void UpdateIcon();
	
	long onClose( FXObject*, FXSelector, void* );
	long onChildLayoutFlags( FXObject*, FXSelector, void* );
	long onFrameUpdate( FXObject*, FXSelector, void* );
	
	static int DialogFlags( const igdeDialog &owner );
	
	static int DialogPadLeft( const igdeGuiTheme &guitheme );
	static int DialogPadRight( const igdeGuiTheme &guitheme );
	static int DialogPadTop( const igdeGuiTheme &guitheme );
	static int DialogPadBottom( const igdeGuiTheme &guitheme );
	static int DialogPadContent( const igdeGuiTheme &guitheme );
	static int DialogPadButtons( const igdeGuiTheme &guitheme );
};


FXDEFMAP( igdeNativeFoxDialog ) igdeNativeFoxDialogMap[] = {
	FXMAPFUNC( SEL_CLOSE, 0, igdeNativeFoxDialog::onClose ),
	FXMAPFUNC( SEL_COMMAND, FXDialogBox::ID_ACCEPT, igdeNativeFoxDialog::onCmdAccept ),
	FXMAPFUNC( SEL_COMMAND, FXDialogBox::ID_CANCEL, igdeNativeFoxDialog::onCmdCancel ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxDialog::onChildLayoutFlags ),
	FXMAPFUNC( SEL_IGDE_FRAME_UPDATE, 0, igdeNativeFoxDialog::onFrameUpdate )
};


FXIMPLEMENT( igdeNativeFoxDialog, FXDialogBox, igdeNativeFoxDialogMap, ARRAYNUMBER( igdeNativeFoxDialogMap ) )

igdeNativeFoxDialog::igdeNativeFoxDialog(){ }

igdeNativeFoxDialog::igdeNativeFoxDialog( igdeDialog &owner, FXWindow *parent, const igdeGuiTheme &guitheme ) :
FXDialogBox( parent, owner.GetTitle().GetString(), DialogFlags( owner ), 0, 0, 0, 0,
	DialogPadLeft( guitheme ), DialogPadRight( guitheme ),
	DialogPadTop( guitheme ), DialogPadBottom( guitheme ),
	0, 0 ),
pOwner( &owner )
{
	UpdateIcon();
}

igdeNativeFoxDialog::~igdeNativeFoxDialog(){
}


void igdeNativeFoxDialog::UpdateEnabled(){
	if( pOwner->GetEnabled() ){
		enable();
		
	}else{
		disable();
	}
}

void igdeNativeFoxDialog::UpdatePosition(){
	if( pOwner->GetPosition().x == getX() && pOwner->GetPosition().y == getY() ){
		return;
	}
	
	move( pOwner->GetPosition().x, pOwner->GetPosition().y );
}

void igdeNativeFoxDialog::UpdateIcon(){
	FXIcon * const icon = pOwner->GetIcon() ? ( FXIcon* )pOwner->GetIcon()->GetNativeIcon() : NULL;
	setIcon( icon );
	setMiniIcon( icon );
}


long igdeNativeFoxDialog::onClose( FXObject*, FXSelector, void* ){
	return pOwner->Cancel() ? 0 : 1;
}

long igdeNativeFoxDialog::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y;
	return 1;
}

long igdeNativeFoxDialog::onFrameUpdate( FXObject*, FXSelector, void* ){
	pOwner->OnFrameUpdate();
	return 1;
}


int igdeNativeFoxDialog::DialogFlags( const igdeDialog &owner ){
	int flags = DECOR_TITLE | DECOR_BORDER | DECOR_CLOSE | DECOR_MENU;
	
	if( owner.GetCanResize() ){
		flags |=  DECOR_RESIZE; //DECOR_STRETCHABLE, DECOR_SHRINKABLE
	}
	
	return flags;
}


int igdeNativeFoxDialog::DialogPadLeft( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingLeft, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadRight( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingRight, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadTop( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingTop, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadBottom( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingBottom, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadContent( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingContent, DEFAULT_PAD );
}

int igdeNativeFoxDialog::DialogPadButtons( const igdeGuiTheme &guitheme ){
	return guitheme.GetIntProperty( igdeGuiThemePropertyNames::dialogPaddingButtons, DEFAULT_PAD );
}

}



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
	const int spacing = igdeNativeFoxDialog::DialogPadButtons( *GetGuiTheme() );
	
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
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	if( ! foxOwner ){
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
		
		igdeNativeFoxDialog * const native = ( igdeNativeFoxDialog* )GetNativeWidget();
		if( pInitialSize != decPoint() ){
			native->resize( decMath::max( native->getWidth(), pInitialSize.x ),
				decMath::max( native->getHeight(), pInitialSize.y ) );
		}
		native->show( PLACEMENT_OWNER );
// 		native->getApp()->refresh();
		
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
	igdeNativeFoxDialog * const dialog = ( igdeNativeFoxDialog* )GetNativeWidget();
	if( ! dialog ){
		DETHROW( deeInvalidParam );
	}
	
	pAccepted = accepted;
	
	if( accepted ){
		dialog->tryHandle( dialog, FXSEL( SEL_COMMAND, FXDialogBox::ID_ACCEPT ), NULL );
		
	}else{
		dialog->tryHandle( dialog, FXSEL( SEL_COMMAND, FXDialogBox::ID_CANCEL ), NULL );
	}
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
	
	if( GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow *foxOwnerWindow = NULL;
	if( pOwner ){
		foxOwnerWindow = ( FXWindow* )pOwner->GetNativeWidget();
	}
	
	igdeNativeFoxDialog * const foxWidget = new igdeNativeFoxDialog( *this, foxOwnerWindow, *GetGuiTheme() );
	SetNativeWidget( foxWidget );
	CreateChildWidgetNativeWidgets();
	foxWidget->create();
}

void igdeDialog::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( igdeNativeFoxDialog* )GetNativeWidget();
	DropNativeWidget();
}



// Protected Functions
////////////////////////

void igdeDialog::pAddContent( igdeWidget *content, igdeWidget *header, igdeWidget *leftPanel,
igdeWidget *rightPanel, igdeWidget *buttonBar ){
	igdeEnvironment &env = GetEnvironment();
	const int spacing = igdeNativeFoxDialog::DialogPadContent( *GetGuiTheme() );
	
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
		( ( igdeNativeFoxDialog* )GetNativeWidget() )->setTitle( GetTitle().GetString() );
	}
}

void igdeDialog::OnIconChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxDialog* )GetNativeWidget() )->UpdateIcon();
	}
}

void igdeDialog::OnSizeChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxDialog* )GetNativeWidget() )->resize( GetSize().x, GetSize().y );
	}
}

void igdeDialog::OnPositionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxDialog* )GetNativeWidget() )->UpdatePosition();
	}
}

void igdeDialog::OnVisibleChanged(){
	// has no effect
}

void igdeDialog::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxDialog* )GetNativeWidget() )->UpdateEnabled();
	}
}
