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
	igdeNativeFoxMenuCascadeCascade( igdeMenuCascade &owner, FXComposite *parent, FXWindow *paneParent );
	virtual ~igdeNativeFoxMenuCascadeCascade();
	
	void DestroyMenuPane();
	
	long onMenuAction( FXObject *sender, FXSelector selector, void *data );
	long updateMenuAction( FXObject *sender, FXSelector selector, void *data );
	
	static FXString BuildConstrText( igdeMenuCascade &owner );
};


FXDEFMAP( igdeNativeFoxMenuCascadeCascade ) igdeNativeFoxMenuCascadeCascadeMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxMenuCascadeCascade::ID_SELF, igdeNativeFoxMenuCascadeCascade::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxMenuCascadeCascade::ID_SELF, igdeNativeFoxMenuCascadeCascade::updateMenuAction )
};


FXIMPLEMENT( igdeNativeFoxMenuCascadeCascade, FXMenuCascade, igdeNativeFoxMenuCascadeCascadeMap, ARRAYNUMBER( igdeNativeFoxMenuCascadeCascadeMap ) )

igdeNativeFoxMenuCascadeCascade::igdeNativeFoxMenuCascadeCascade(){ }

igdeNativeFoxMenuCascadeCascade::igdeNativeFoxMenuCascadeCascade( igdeMenuCascade &owner,
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

FXString igdeNativeFoxMenuCascadeCascade::BuildConstrText( igdeMenuCascade &owner ){
	return igdeUIFoxHelper::MnemonizeString( owner.GetText(), owner.GetMnemonic() )
		+ "\t" + igdeUIFoxHelper::AccelString( owner.GetHotKey() )
		+ "\t" + owner.GetDescription().GetString();
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
	igdeNativeFoxMenuCascadeTitle( igdeMenuCascade &owner, FXComposite *parent, FXWindow *paneParent );
	virtual ~igdeNativeFoxMenuCascadeTitle();
	
	void DestroyMenuPane();
	
	long onMenuAction( FXObject *sender, FXSelector selector, void *data );
	long updateMenuAction( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( igdeNativeFoxMenuCascadeTitle ) igdeNativeFoxMenuCascadeTitleMap[] = {
	FXMAPFUNC( SEL_COMMAND, igdeNativeFoxMenuCascadeTitle::ID_SELF, igdeNativeFoxMenuCascadeTitle::onMenuAction ),
	FXMAPFUNC( SEL_UPDATE, igdeNativeFoxMenuCascadeTitle::ID_SELF, igdeNativeFoxMenuCascadeTitle::updateMenuAction )
};


FXIMPLEMENT( igdeNativeFoxMenuCascadeTitle, FXMenuTitle, igdeNativeFoxMenuCascadeTitleMap, ARRAYNUMBER( igdeNativeFoxMenuCascadeTitleMap ) )

igdeNativeFoxMenuCascadeTitle::igdeNativeFoxMenuCascadeTitle(){ }

igdeNativeFoxMenuCascadeTitle::igdeNativeFoxMenuCascadeTitle( igdeMenuCascade &owner,
	FXComposite *parent, FXWindow *paneParent ) :
FXMenuTitle( parent, igdeNativeFoxMenuCascadeCascade::BuildConstrText( owner ), owner.GetIcon()
	? ( FXIcon* )owner.GetIcon()->GetNativeIcon() : NULL, new FXMenuPane( paneParent ) ),
pOwner( &owner )
{
	if( ! owner.GetEnabled() ){
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

}


// Class igdeNativeFoxMenuCascade
///////////////////////////////////

// Constructor, destructor
////////////////////////////

void *igdeNativeFoxMenuCascade::CreateNativeWidget( igdeMenuCascade &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
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
	FXWindow * const paneParent = parent->getShell();
	if( ! paneParent ){
		DETHROW( deeInvalidParam );
	}
	
	if( parent->isMemberOf( FXMetaClass::getMetaClassFromName( "FXMenuBar" ) ) ){
		return new igdeNativeFoxMenuCascadeTitle( owner, parent, paneParent );
		
	}else{
		return new igdeNativeFoxMenuCascadeCascade( owner, parent, paneParent );
	}
}

void igdeNativeFoxMenuCascade::PostCreateNativeWidget( igdeMenuCascade &owner, void *native ){
	FXComposite &parent = *( ( FXComposite* )owner.GetParent()->GetNativeContainer() );
	if( parent.id() ){
		( ( FXMenuCaption* )native )->create();
	}
}

void igdeNativeFoxMenuCascade::DestroyNativeWidget( igdeMenuCascade &owner, void *native ){
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

void *igdeNativeFoxMenuCascade::GetNativeContainer( const igdeMenuCascade &owner, void *native ){
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

void igdeNativeFoxMenuCascade::UpdateTitle( const igdeMenuCascade &owner, void *native ){
	( ( FXMenuCaption* )native )->setText( owner.GetText().GetString() );
}

void igdeNativeFoxMenuCascade::UpdateDescription( const igdeMenuCascade &owner, void *native ){
	const char * const description = owner.GetDescription();
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

void igdeNativeFoxMenuCascade::UpdateIcon( const igdeMenuCascade &owner, void *native ){
	FXIcon *icon = NULL;
	if( owner.GetIcon() ){
		icon = ( FXIcon* )owner.GetIcon()->GetNativeIcon();
	}
	
	( ( FXMenuCaption* )native )->setIcon( icon );
}

void igdeNativeFoxMenuCascade::UpdateEnabled( const igdeMenuCascade &owner, void *native ){
	( ( FXMenuCaption* )native )->update();
}

void *igdeNativeFoxMenuCascade::CreateNativePopup( igdeMenuCascade &owner, igdeWidget &widgetOwner ){
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

void igdeNativeFoxMenuCascade::PostCreateNativePopup( igdeMenuCascade &owner, void *native ){
	( ( FXMenuPane* )native )->create();
}

void igdeNativeFoxMenuCascade::DestroyNativePopup( igdeMenuCascade &owner, void *native ){
	delete ( FXMenuPane* )native;
}
