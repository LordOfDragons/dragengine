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

#include "igdeMenuCascade.h"
#include "../native/toolkit.h"
#include "../igdeCommonDialogs.h"
#include "../igdeWindow.h"
#include "../igdeWidgetReference.h"
#include "../resources/igdeIcon.h"

#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Native Widget
//////////////////

class cNativeIgdeMenuCascade : public FXMenuCascade{
	FXDECLARE( cNativeIgdeMenuCascade )
	
private:
	igdeMenuCascade *pOwner;
	
protected:
	cNativeIgdeMenuCascade();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuCascade::ID_LAST,
	};
	
public:
	cNativeIgdeMenuCascade( igdeMenuCascade &owner, FXComposite *parent, FXWindow *paneParent );
	virtual ~cNativeIgdeMenuCascade();
	
	void DestroyMenuPane();
	
	long onMenuAction( FXObject *sender, FXSelector selector, void *data );
	long updateMenuAction( FXObject *sender, FXSelector selector, void *data );
	
	static FXString BuildConstrText( igdeMenuCascade &owner );
};


FXDEFMAP( cNativeIgdeMenuCascade ) cNativeIgdeMenuCascadeMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeMenuCascade::ID_SELF, cNativeIgdeMenuCascade::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, cNativeIgdeMenuCascade::ID_SELF, cNativeIgdeMenuCascade::updateMenuAction )
};


FXIMPLEMENT( cNativeIgdeMenuCascade, FXMenuCascade, cNativeIgdeMenuCascadeMap, ARRAYNUMBER( cNativeIgdeMenuCascadeMap ) )

cNativeIgdeMenuCascade::cNativeIgdeMenuCascade(){ }

cNativeIgdeMenuCascade::cNativeIgdeMenuCascade( igdeMenuCascade &owner,
	FXComposite *parent, FXWindow *paneParent ) :
FXMenuCascade( parent, BuildConstrText( owner ), owner.GetIcon()
	? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL, new FXMenuPane( paneParent ) ),
pOwner( &owner )
{
	if( ! owner.GetEnabled() ){
		disable();
	}
	//getMenu()->create(); // FXMenuCascade constructor calls create()
}

cNativeIgdeMenuCascade::~cNativeIgdeMenuCascade(){
	DestroyMenuPane();
}

void cNativeIgdeMenuCascade::DestroyMenuPane(){
	if( getMenu() ){
		delete getMenu();
		setMenu( NULL );
	}
}

long cNativeIgdeMenuCascade::onMenuAction( FXObject*, FXSelector, void* ){
	if( pOwner->GetEnabled() ){
// 		pOwner->OnAction();
	}
	return 1;
}

long cNativeIgdeMenuCascade::updateMenuAction( FXObject *sender, FXSelector, void* ){
	return sender->handle( this, FXSEL( SEL_COMMAND, pOwner->GetEnabled()
		? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE ), NULL);
}

FXString cNativeIgdeMenuCascade::BuildConstrText( igdeMenuCascade &owner ){
	return igdeUIFoxHelper::MnemonizeString( owner.GetText(), owner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( owner.GetHotKey() )
		+ "\t" + owner.GetDescription().GetString();
}



class cNativeIgdeMenuTitle : public FXMenuTitle{
	FXDECLARE( cNativeIgdeMenuTitle )
	
private:
	igdeMenuCascade *pOwner;
	
protected:
	cNativeIgdeMenuTitle();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuTitle::ID_LAST,
	};
	
public:
	cNativeIgdeMenuTitle( igdeMenuCascade &owner, FXComposite *parent, FXWindow *paneParent );
	virtual ~cNativeIgdeMenuTitle();
	
	void DestroyMenuPane();
	
	long onMenuAction( FXObject *sender, FXSelector selector, void *data );
	long updateMenuAction( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( cNativeIgdeMenuTitle ) cNativeIgdeMenuTitleMap[] = {
	FXMAPFUNC( SEL_COMMAND, cNativeIgdeMenuTitle::ID_SELF, cNativeIgdeMenuTitle::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, cNativeIgdeMenuTitle::ID_SELF, cNativeIgdeMenuTitle::updateMenuAction )
};


FXIMPLEMENT( cNativeIgdeMenuTitle, FXMenuTitle, cNativeIgdeMenuTitleMap, ARRAYNUMBER( cNativeIgdeMenuTitleMap ) )

cNativeIgdeMenuTitle::cNativeIgdeMenuTitle(){ }

cNativeIgdeMenuTitle::cNativeIgdeMenuTitle( igdeMenuCascade &owner,
	FXComposite *parent, FXWindow *paneParent ) :
FXMenuTitle( parent, cNativeIgdeMenuCascade::BuildConstrText( owner ), owner.GetIcon()
	? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL, new FXMenuPane( paneParent ) ),
pOwner( &owner )
{
	if( ! owner.GetEnabled() ){
		disable();
	}
	//getMenu()->create(); // FXMenuCascade constructor calls create()
}

cNativeIgdeMenuTitle::~cNativeIgdeMenuTitle(){
	DestroyMenuPane();
}

void cNativeIgdeMenuTitle::DestroyMenuPane(){
	if( getMenu() ){
		delete getMenu();
		setMenu( NULL );
	}
}

long cNativeIgdeMenuTitle::onMenuAction( FXObject*, FXSelector, void* ){
	if( pOwner->GetEnabled() ){
// 		pOwner->OnAction();
	}
	return 0;
}

long cNativeIgdeMenuTitle::updateMenuAction( FXObject*, FXSelector, void* ){
// 	return sender->handle( this, FXSEL( SEL_COMMAND, pOwner->GetEnabled()
// 		? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE ), NULL);
	return 0;
}



// Show popup class
/////////////////////

class igdeMenuCascade_Popup : public igdeWindow{
private:
	igdeMenuCascade &pMenu;
	
public:
	igdeMenuCascade_Popup( igdeMenuCascade &menu );
	
protected:
	virtual ~igdeMenuCascade_Popup();
	
public:
	virtual void Popup( const decPoint &position );
	
	virtual void CreateNativeWidget();
	virtual void DestroyNativeWidget();
	
protected:
	virtual void OnTitleChanged();
	virtual void OnSizeChanged();
	virtual void OnPositionChanged();
	virtual void OnVisibleChanged();
	virtual void OnEnabledChanged();
};


igdeMenuCascade_Popup::igdeMenuCascade_Popup( igdeMenuCascade &menu ) :
igdeWindow( menu.GetEnvironment(), "", NULL, false ),
pMenu( menu ){
}

igdeMenuCascade_Popup::~igdeMenuCascade_Popup(){}


void igdeMenuCascade_Popup::Popup( const decPoint &position ){
	if( ! pMenu.GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	
	FXMenuPane * const native = ( FXMenuPane* )pMenu.GetNativeWidget();
	SetNativeWidget( native );
	native->popup( NULL, position.x, position.y );
	
	GetEnvironment().RunModalWhileShown( *this );
}


void igdeMenuCascade_Popup::CreateNativeWidget(){
	DETHROW( deeInvalidParam );
}

void igdeMenuCascade_Popup::DestroyNativeWidget(){
	DropNativeWidget();
}

void igdeMenuCascade_Popup::OnTitleChanged(){}
void igdeMenuCascade_Popup::OnSizeChanged(){}
void igdeMenuCascade_Popup::OnPositionChanged(){}
void igdeMenuCascade_Popup::OnVisibleChanged(){}
void igdeMenuCascade_Popup::OnEnabledChanged(){}



// Class igdeMenuCascade
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuCascade::igdeMenuCascade( igdeEnvironment &environment ) :
igdeContainer( environment ),
pMnemonic( deInputEvent::ekcUndefined ),
pEnabled( true ){
}

igdeMenuCascade::igdeMenuCascade( igdeEnvironment &environment, const char *text ) :
igdeContainer( environment ),
pText( text ),
pMnemonic( deInputEvent::ekcUndefined ),
pEnabled( true ){
}

igdeMenuCascade::igdeMenuCascade( igdeEnvironment &environment, const char *text,
	igdeIcon *icon ) :
igdeContainer( environment ),
pText( text ),
pMnemonic( deInputEvent::ekcUndefined ),
pIcon( icon ),
pEnabled( true ){
}

igdeMenuCascade::igdeMenuCascade( igdeEnvironment &environment, const char *text,
	deInputEvent::eKeyCodes mnemonic ) :
igdeContainer( environment ),
pText( text ),
pMnemonic( mnemonic ),
pEnabled( true ){
}

igdeMenuCascade::igdeMenuCascade( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, const char *description ) :
igdeContainer( environment ),
pText( text ),
pDescription( description ),
pMnemonic( deInputEvent::ekcUndefined ),
pIcon( icon ),
pEnabled( true ){
}

igdeMenuCascade::igdeMenuCascade( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, const char *description, const igdeHotKey &hotKey ) :
igdeContainer( environment ),
pText( text ),
pDescription( description ),
pHotKey( hotKey ),
pMnemonic( deInputEvent::ekcUndefined ),
pIcon( icon ),
pEnabled( true ){
}

igdeMenuCascade::igdeMenuCascade( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, const char *description, deInputEvent::eKeyCodes mnemonic ) :
igdeContainer( environment ),
pText( text ),
pDescription( description ),
pMnemonic( mnemonic ),
pIcon( icon ),
pEnabled( true ){
}

igdeMenuCascade::igdeMenuCascade( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, const char *description, deInputEvent::eKeyCodes mnemonic,
	const igdeHotKey &hotKey ) :
igdeContainer( environment ),
pText( text ),
pDescription( description ),
pHotKey( hotKey ),
pMnemonic( mnemonic ),
pIcon( icon ),
pEnabled( true ){
}

igdeMenuCascade::~igdeMenuCascade(){
	igdeMenuCascade::DestroyNativeWidget();
}



// Management
///////////////

void igdeMenuCascade::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeMenuCascade::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeMenuCascade::SetHotKey( const igdeHotKey &hotKey ){
	if( pHotKey == hotKey ){
		return;
	}
	
	pHotKey = hotKey;
	OnHotKeyChanged();
}

void igdeMenuCascade::SetMnemonic( deInputEvent::eKeyCodes mnemonic ){
	if( pMnemonic == mnemonic ){
		return;
	}
	
	pMnemonic = mnemonic;
	OnMnemonicChanged();
}

void igdeMenuCascade::SetIcon( igdeIcon *icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}

void igdeMenuCascade::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}



void igdeMenuCascade::Popup( igdeWidget &owner ){
	if( ! owner.GetNativeWidget() ){
		return;
	}
	
	FXWindow &window = *( ( FXWindow* )owner.GetNativeWidget() );
	FXint x, y;
	FXuint buttons;
	
	if( window.getCursorPosition( x, y, buttons) ){
		PopupAt( owner, decPoint( x, y ) );
	}
}

void igdeMenuCascade::Popup( igdeWidget &owner, const decPoint &position ){
	PopupAt( owner, owner.WidgetToScreen( position ) );
}

void igdeMenuCascade::PopupBottom( igdeWidget &owner ){
	if( ! owner.GetNativeWidget() ){
		return;
	}
	
	FXWindow &window = *( ( FXWindow* )owner.GetNativeWidget() );
	
	PopupAt( owner, owner.WidgetToScreen( decPoint( 0, window.getHeight() ) ) );
}



void igdeMenuCascade::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const nativeParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	
	// according to FOX source code the situation looks like this:
	// FXMenuPane popup(owner)
	// FXMenuPane submenu(owner)
	// new FXMenuCascade(popup, submenu)
	// 
	// all FXMenuPane have the same owner which is the popup owner. interestingly all menu
	// panes are stack allocated which means the FXMenuCascade does not destroy them and
	// they are destroyed after runModalWhileShown is finished.
	// 
	// unfortunately things seem to crash if cascade menu is used with popup. no idea
	// what is causing this problem.
	// 
	// since FXMenuPane is an FXPopup which is an FXShell using getShell() is enough to
	// get the top level popup window. this is pretty much "owner" in the example above
	// 
	// the top level popup menu is destroyed by going out of scope. so all FXMenuPane
	// have to be destroyed manually with fox dealing with the rest
	FXWindow * const foxPaneParent = nativeParent->getShell();
	if( ! foxPaneParent ){
		DETHROW( deeInvalidParam );
	}
	
	FXMenuCaption *native;
	
	if( nativeParent->isMemberOf( FXMetaClass::getMetaClassFromName( "FXMenuBar" ) ) ){
		native = new cNativeIgdeMenuTitle( *this, nativeParent, foxPaneParent );
		
	}else{
		native = new cNativeIgdeMenuCascade( *this, nativeParent, foxPaneParent );
	}
	
	SetNativeWidget( native );
	if( nativeParent->id() ){
		native->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeMenuCascade::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXMenuCaption * const native = ( FXMenuCaption* )GetNativeWidget();
	if( strcmp( native->getClassName(), "cNativeIgdeMenuTitle" ) == 0 ){
		delete ( cNativeIgdeMenuTitle* )native;
		
	}else if( strcmp( native->getClassName(), "cNativeIgdeMenuCascade" ) == 0 ){
		delete ( cNativeIgdeMenuCascade* )native;
		
	}else if( strcmp( native->getClassName(), "FXMenuPane" ) == 0 ){
		delete ( FXMenuPane* )native;
		
	}else{
		delete native;
	}
	
	DropNativeWidget();
}

void *igdeMenuCascade::GetNativeContainer() const{
	const FXMenuCaption * const native = ( FXMenuCaption* )GetNativeWidget();
	if( ! native ){
		return NULL;
	}
	
	// yes I know, this is ugly, but FOX reacts very badly to multi-class sub-classing
	// leaving no other solution than either compare the class name or use isMemberOf
	if( strcmp( native->getClassName(), "cNativeIgdeMenuTitle" ) == 0 ){
		return ( ( cNativeIgdeMenuTitle* )native )->getMenu();
		
	}else if( strcmp( native->getClassName(), "cNativeIgdeMenuCascade" ) == 0 ){
		return ( ( cNativeIgdeMenuCascade* )native )->getMenu();
		
	}else if( strcmp( native->getClassName(), "FXMenuPane" ) == 0 ){
		return ( FXMenuPane* )native;
		
	}else{
		return ( FXMenuPane* )native;
	}
}



void igdeMenuCascade::OnTextChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( FXMenuCascade* )GetNativeWidget() )->setText( pText.GetString() );
}

void igdeMenuCascade::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXMenuCascade &command = *( ( FXMenuCascade* )GetNativeWidget() );
	command.setTipText( pText.GetString() );
	command.setHelpText( pText.GetString() );
}

void igdeMenuCascade::OnHotKeyChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	//( ( FXMenuCascade* )GetNativeWidget() )->setAccelText( igdeUIFoxHelper::AccelString( pHotKey ), true );
}

void igdeMenuCascade::OnMnemonicChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	//( ( FXMenuCascade* )GetNativeWidget() )->setKey( igdeUIFoxHelper::MnemonicKey( pMnemonic ) );
}

void igdeMenuCascade::OnIconChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXIcon *icon = NULL;
	if( pIcon ){
		icon = ( FXIcon* )pIcon->GetNativeIcon();
	}
	
	( ( FXMenuCascade* )GetNativeWidget() )->setIcon( icon );
}

void igdeMenuCascade::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( FXMenuCascade* )GetNativeWidget() )->update();
}

void igdeMenuCascade::PopupAt( igdeWidget &owner, const decPoint &position ){
	// this is ugly, I know. the problem is that we can not create a popup widget as a window
	// out in the blue. we sort of use an "injection hack" to get this working. we create for
	// the menu a native widget of just FXMenuPane type and assign it to the menu as if
	// CreateNativeWidget has been called on the menu. then we assign the native widgets
	// also to this class knowning very well we are not allowed to delete them. we can not
	// simply create cNativeIgdeMenuCascade and use the getMenu() of it since fox then fails
	if( GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const nativeParent = ( FXComposite* )owner.GetNativeWidget();
	if( ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	if( nativeParent->isMemberOf( FXMetaClass::getMetaClassFromName( "FXMenuBar" ) ) ){
		DETHROW( deeInvalidParam );
	}
	
	FXMenuPane *nativePopup = NULL;
	
	try{
		nativePopup = new FXMenuPane( nativeParent );
		SetNativeWidget( nativePopup );
		CreateChildWidgetNativeWidgets();
		nativePopup->create();
		
		igdeWidgetReference popup;
		popup.TakeOver( new igdeMenuCascade_Popup( *this ) );
		( ( igdeMenuCascade_Popup& )( igdeWidget& )popup ).Popup( position );
		
		DestroyNativeWidget();
		
	}catch( const deException & ){
		DestroyNativeWidget();
		throw;
	}
}
