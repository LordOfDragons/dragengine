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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxMenuCascade.h"
#include "../../../igdeContainer.h"
#include "../../../igdeWidgetReference.h"
#include "../../../igdeWindow.h"
#include "../../../menu/igdeMenuCascade.h"
#include "../../../resources/igdeIcon.h"
#include "../../../../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>


namespace {

class igdeNativeFoxMenuCascadeCascade : public FXMenuCascade{
	FXDECLARE( igdeNativeFoxMenuCascadeCascade )
	
private:
	igdeMenuCascade *pOwner;
	
protected:
	igdeNativeFoxMenuCascadeCascade();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuCascade::ID_LAST,
	};
	
public:
	igdeNativeFoxMenuCascadeCascade( igdeMenuCascade &powner, FXComposite *pparent, FXWindow *paneParent );
	virtual ~igdeNativeFoxMenuCascadeCascade();
	
	void DestroyMenuPane();
	
	long onMenuAction( FXObject*, FXSelector, void* );
	long updateMenuAction( FXObject*, FXSelector, void* );
	
	static FXString BuildConstrText( igdeMenuCascade &powner );
};


FXDEFMAP( igdeNativeFoxMenuCascadeCascade ) igdeNativeFoxMenuCascadeCascadeMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxMenuCascadeCascade::ID_SELF, igdeNativeFoxMenuCascadeCascade::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxMenuCascadeCascade::ID_SELF, igdeNativeFoxMenuCascadeCascade::updateMenuAction )
};


FXIMPLEMENT( igdeNativeFoxMenuCascadeCascade, FXMenuCascade, igdeNativeFoxMenuCascadeCascadeMap, ARRAYNUMBER( igdeNativeFoxMenuCascadeCascadeMap ) )

igdeNativeFoxMenuCascadeCascade::igdeNativeFoxMenuCascadeCascade(){ }

igdeNativeFoxMenuCascadeCascade::igdeNativeFoxMenuCascadeCascade( igdeMenuCascade &powner,
	FXComposite *pparent, FXWindow *paneParent ) :
FXMenuCascade( pparent, BuildConstrText( powner ), powner.GetIcon()
	? ( FXIcon* ) powner.GetIcon()->GetNativeIcon() : NULL, new FXMenuPane( paneParent ) ),
pOwner( &powner )
{
	if( ! powner.GetEnabled() ){
		disable();
	}
	//getMenu()->create(); // FXMenuCascade constructor calls create()
}

igdeNativeFoxMenuCascadeCascade::~igdeNativeFoxMenuCascadeCascade(){
	DestroyMenuPane();
}

void igdeNativeFoxMenuCascadeCascade::DestroyMenuPane(){
	if( getMenu() ){
		delete getMenu();
		setMenu( NULL );
	}
}

long igdeNativeFoxMenuCascadeCascade::onMenuAction( FXObject*, FXSelector, void* ){
	if( pOwner->GetEnabled() ){
// 		pOwner->OnAction();
	}
	return 1;
}

long igdeNativeFoxMenuCascadeCascade::updateMenuAction( FXObject *sender, FXSelector, void* ){
	return sender->handle( this, FXSEL( SEL_COMMAND, pOwner->GetEnabled()
		? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE ), NULL);
}

FXString igdeNativeFoxMenuCascadeCascade::BuildConstrText( igdeMenuCascade &powner ){
	return igdeUIFoxHelper::MnemonizeString( powner.GetText(), powner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( powner.GetHotKey() )
		+ "\t" + powner.GetDescription().GetString();
}



class igdeNativeFoxMenuCascadeTitle : public FXMenuTitle{
	FXDECLARE( igdeNativeFoxMenuCascadeTitle )
	
private:
	igdeMenuCascade *pOwner;
	
protected:
	igdeNativeFoxMenuCascadeTitle();
	
public:
	enum eFoxIDs{
		ID_SELF = FXMenuTitle::ID_LAST,
	};
	
public:
	igdeNativeFoxMenuCascadeTitle( igdeMenuCascade &powner, FXComposite *pparent, FXWindow *paneParent );
	virtual ~igdeNativeFoxMenuCascadeTitle();
	
	void DestroyMenuPane();
	
	long onMenuAction( FXObject*, FXSelector, void* );
	long updateMenuAction( FXObject*, FXSelector, void* );
};


FXDEFMAP( igdeNativeFoxMenuCascadeTitle ) igdeNativeFoxMenuCascadeTitleMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxMenuCascadeTitle::ID_SELF, igdeNativeFoxMenuCascadeTitle::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxMenuCascadeTitle::ID_SELF, igdeNativeFoxMenuCascadeTitle::updateMenuAction )
};


FXIMPLEMENT( igdeNativeFoxMenuCascadeTitle, FXMenuTitle, igdeNativeFoxMenuCascadeTitleMap, ARRAYNUMBER( igdeNativeFoxMenuCascadeTitleMap ) )

igdeNativeFoxMenuCascadeTitle::igdeNativeFoxMenuCascadeTitle(){ }

igdeNativeFoxMenuCascadeTitle::igdeNativeFoxMenuCascadeTitle( igdeMenuCascade &powner,
	FXComposite *pparent, FXWindow *paneParent ) :
FXMenuTitle( pparent, igdeNativeFoxMenuCascadeCascade::BuildConstrText( powner ), powner.GetIcon()
	? ( FXIcon* ) powner.GetIcon()->GetNativeIcon() : NULL, new FXMenuPane( paneParent ) ),
pOwner( &powner )
{
	if( ! powner.GetEnabled() ){
		disable();
	}
	//getMenu()->create(); // FXMenuCascade constructor calls create()
}

igdeNativeFoxMenuCascadeTitle::~igdeNativeFoxMenuCascadeTitle(){
	DestroyMenuPane();
}

void igdeNativeFoxMenuCascadeTitle::DestroyMenuPane(){
	if( getMenu() ){
		delete getMenu();
		setMenu( NULL );
	}
}

long igdeNativeFoxMenuCascadeTitle::onMenuAction( FXObject*, FXSelector, void* ){
	if( pOwner->GetEnabled() ){
// 		pOwner->OnAction();
	}
	return 0;
}

long igdeNativeFoxMenuCascadeTitle::updateMenuAction( FXObject*, FXSelector, void* ){
// 	return sender->handle( this, FXSEL( SEL_COMMAND, pOwner->GetEnabled()
// 		? FXWindow::ID_ENABLE : FXWindow::ID_DISABLE ), NULL);
	return 0;
}


class igdeNativeFoxMenuCascade_PopupWindow : public igdeWindow{
private:
	igdeMenuCascade &pMenu;
	
public:
	igdeNativeFoxMenuCascade_PopupWindow( igdeMenuCascade &menu );
	
protected:
	virtual ~igdeNativeFoxMenuCascade_PopupWindow();
	
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


igdeNativeFoxMenuCascade_PopupWindow::igdeNativeFoxMenuCascade_PopupWindow( igdeMenuCascade &menu ) :
igdeWindow( menu.GetEnvironment(), "", NULL, false ),
pMenu( menu ){
}

igdeNativeFoxMenuCascade_PopupWindow::~igdeNativeFoxMenuCascade_PopupWindow(){
	// make sure native widget is gone. required due to igdeWindow::~igdeWindow()
	DropNativeWidget();
}


void igdeNativeFoxMenuCascade_PopupWindow::Popup( const decPoint &position ){
	if( ! pMenu.GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	
	FXMenuPane * const native = ( FXMenuPane* )pMenu.GetNativeWidget();
	SetNativeWidget( native );
	native->popup( NULL, position.x, position.y );
	
	GetEnvironment().RunModalWhileShown( *this );
}


void igdeNativeFoxMenuCascade_PopupWindow::CreateNativeWidget(){
	DETHROW( deeInvalidParam );
}

void igdeNativeFoxMenuCascade_PopupWindow::DestroyNativeWidget(){
	DropNativeWidget();
}

void igdeNativeFoxMenuCascade_PopupWindow::OnTitleChanged(){}
void igdeNativeFoxMenuCascade_PopupWindow::OnSizeChanged(){}
void igdeNativeFoxMenuCascade_PopupWindow::OnPositionChanged(){}
void igdeNativeFoxMenuCascade_PopupWindow::OnVisibleChanged(){}
void igdeNativeFoxMenuCascade_PopupWindow::OnEnabledChanged(){}

}


// Class igdeNativeFoxMenuCascade
///////////////////////////////////

// Constructor, destructor
////////////////////////////

void *igdeNativeFoxMenuCascade::CreateNativeWidget( igdeMenuCascade &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
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
	FXWindow * const paneParent = pparent->getShell();
	if( ! paneParent ){
		DETHROW( deeInvalidParam );
	}
	
	if( pparent->isMemberOf( FXMetaClass::getMetaClassFromName( "FXMenuBar" ) ) ){
		return new igdeNativeFoxMenuCascadeTitle( powner, pparent, paneParent );
		
	}else{
		return new igdeNativeFoxMenuCascadeCascade( powner, pparent, paneParent );
	}
}

void igdeNativeFoxMenuCascade::PostCreateNativeWidget( igdeMenuCascade &powner, void *native ){
	FXComposite &pparent = *( ( FXComposite* ) powner.GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		( ( FXMenuCaption* )native )->create();
	}
}

void igdeNativeFoxMenuCascade::DestroyNativeWidget( igdeMenuCascade&, void *native ){
	const FXchar * const className = ( ( FXMenuCaption* )native )->getClassName();
	if( strcmp( className, "igdeNativeFoxMenuCascadeTitle" ) == 0 ){
		delete ( igdeNativeFoxMenuCascadeTitle* )native;
		
	}else if( strcmp( className, "igdeNativeFoxMenuCascadeCascade" ) == 0 ){
		delete ( igdeNativeFoxMenuCascadeCascade* )native;
		
	}else if( strcmp( className, "FXMenuPane" ) == 0 ){
		delete ( FXMenuPane* )native;
		
	}else{
		delete ( FXMenuCaption* )native;
	}
}

void *igdeNativeFoxMenuCascade::GetNativeContainer( const igdeMenuCascade&, void *native ){
	// yes I know, this is ugly, but FOX reacts very badly to multi-class sub-classing
	// leaving no other solution than either compare the class name or use isMemberOf
	const FXchar * const className = ( ( FXMenuCaption* )native )->getClassName();
	if( strcmp( className, "igdeNativeFoxMenuCascadeTitle" ) == 0 ){
		return ( ( igdeNativeFoxMenuCascadeTitle* )native )->getMenu();
		
	}else if( strcmp( className, "igdeNativeFoxMenuCascadeCascade" ) == 0 ){
		return ( ( igdeNativeFoxMenuCascadeCascade* )native )->getMenu();
		
	}else if( strcmp( className, "FXMenuPane" ) == 0 ){
		return ( FXMenuPane* )native;
		
	}else{
		return ( FXMenuPane* )native;
	}
}

void igdeNativeFoxMenuCascade::UpdateTitle( const igdeMenuCascade &powner, void *native ){
	( ( FXMenuCaption* )native )->setText( powner.GetText().GetString() );
}

void igdeNativeFoxMenuCascade::UpdateDescription( const igdeMenuCascade &powner, void *native ){
	const char * const description = powner.GetDescription();
	FXMenuCaption &widget = *( ( FXMenuCaption* )native );
	widget.setTipText( description );
	widget.setHelpText( description );
}

/*
UpdateHotKey:
	//( ( FXMenuCascade* )GetNativeWidget() )->setAccelText( igdeUIFoxHelper::AccelString( pHotKey ), true );

UpdateMnemonic:
	//( ( FXMenuCascade* )GetNativeWidget() )->setKey( igdeUIFoxHelper::MnemonicKey( pMnemonic ) );
*/

void igdeNativeFoxMenuCascade::UpdateIcon( const igdeMenuCascade &powner, void *native ){
	FXIcon *iicon = NULL;
	if( powner.GetIcon() ){
		iicon = ( FXIcon* ) powner.GetIcon()->GetNativeIcon();
	}
	
	( ( FXMenuCaption* )native )->setIcon( iicon );
}

void igdeNativeFoxMenuCascade::UpdateEnabled( const igdeMenuCascade&, void *native ){
	( ( FXMenuCaption* )native )->update();
}

void *igdeNativeFoxMenuCascade::CreateNativePopup( igdeMenuCascade&, igdeWidget &widgetOwner ){
	// this is ugly, I know. the problem is that we can not create a popup widget as a window
	// out in the blue. we sort of use an "injection hack" to get this working. we create for
	// the menu a native widget of just FXMenuPane type and assign it to the menu as if
	// CreateNativeWidget has been called on the menu. then we assign the native widgets
	// also to this class knowning very well we are not allowed to delete them. we can not
	// simply create igdeNativeFoxMenuCascadeCascade and use the getMenu() of it since
	// fox then fails
	
	FXComposite * const nativeParent = ( FXComposite* )widgetOwner.GetNativeWidget();
	if( ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	if( nativeParent->isMemberOf( FXMetaClass::getMetaClassFromName( "FXMenuBar" ) ) ){
		DETHROW( deeInvalidParam );
	}
	
	return new FXMenuPane( nativeParent );
}

void igdeNativeFoxMenuCascade::PostCreateNativePopup( igdeMenuCascade&, void *native ){
	( ( FXMenuPane* )native )->create();
}

void igdeNativeFoxMenuCascade::ShowPopupWindow( igdeMenuCascade &powner,
igdeWidget &widgetOwner, const decPoint &position ){
	igdeWidgetReference window;
	window.TakeOver( new igdeNativeFoxMenuCascade_PopupWindow( powner ) );
	( ( igdeNativeFoxMenuCascade_PopupWindow& )( igdeWidget& )window ).Popup( position );
}

void igdeNativeFoxMenuCascade::DestroyNativePopup( igdeMenuCascade&, void *native ){
	delete ( FXMenuPane* )native;
}

#endif
