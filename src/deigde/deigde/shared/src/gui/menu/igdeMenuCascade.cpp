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
#include "../igdeCommonDialogs.h"
#include "../igdeWindow.h"
#include "../igdeWidgetReference.h"
#include "../native/toolkit.h"
#include "../resources/igdeIcon.h"

#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


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
	if( owner.GetNativeWidget() ){
		PopupAt( owner, igdeNativeWidget::GetCursorPosition( owner ) );
	}
}

void igdeMenuCascade::Popup( igdeWidget &owner, const decPoint &position ){
	PopupAt( owner, owner.WidgetToScreen( position ) );
}

void igdeMenuCascade::PopupBottom( igdeWidget &owner ){
	if( ! owner.GetNativeWidget() ){
		return;
	}
	
	PopupAt( owner, owner.WidgetToScreen( decPoint( 0, igdeNativeWidget::GetSize( owner ).y ) ) );
}



void igdeMenuCascade::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	void * const native = igdeNativeMenuCascade::CreateNativeWidget( *this );
	SetNativeWidget( native );
	igdeNativeMenuCascade::PostCreateNativeWidget( *this, native );
	
	CreateChildWidgetNativeWidgets();
}

void igdeMenuCascade::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeMenuCascade::DestroyNativeWidget( *this, GetNativeWidget() );
	DropNativeWidget();
}

void *igdeMenuCascade::GetNativeContainer() const{
	if( ! GetNativeWidget() ){
		return NULL;
	}
	return igdeNativeMenuCascade::GetNativeContainer( *this, GetNativeWidget() );
}



void igdeMenuCascade::OnTextChanged(){
	if( GetNativeWidget() ){
		igdeNativeMenuCascade::UpdateTitle( *this, GetNativeWidget() );
	}
}

void igdeMenuCascade::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		igdeNativeMenuCascade::UpdateDescription( *this, GetNativeWidget() );
	}
}

void igdeMenuCascade::OnHotKeyChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
}

void igdeMenuCascade::OnMnemonicChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
}

void igdeMenuCascade::OnIconChanged(){
	if( GetNativeWidget() ){
		igdeNativeMenuCascade::UpdateIcon( *this, GetNativeWidget() );
	}
}

void igdeMenuCascade::OnEnabledChanged(){
	if( GetNativeWidget() ){
		igdeNativeMenuCascade::UpdateEnabled( *this, GetNativeWidget() );
	}
}

void igdeMenuCascade::PopupAt( igdeWidget &owner, const decPoint &position ){
	// this is ugly, I know. the problem is that we can not create a popup widget as a window
	// out in the blue. we sort of use an "injection hack" to get this working. we create for
	// the menu a native widget of just FXMenuPane type and assign it to the menu as if
	// CreateNativeWidget has been called on the menu. then we assign the native widgets
	// also to this class knowning very well we are not allowed to delete them. we can not
	// simply create igdeNativeMenuCascadeCascade and use the getMenu() of it since
	// toolkits then fail
	if( GetNativeWidget() ){
		DETHROW( deeInvalidParam );
	}
	
	void * const nativePopup = igdeNativeMenuCascade::CreateNativePopup( *this, owner );
	try{
		SetNativeWidget( nativePopup );
		CreateChildWidgetNativeWidgets();
		igdeNativeMenuCascade::PostCreateNativePopup( *this, nativePopup );
		
		igdeNativeMenuCascade::ShowPopupWindow( *this, owner, position );
		
		DropNativeWidget();
		igdeNativeMenuCascade::DestroyNativePopup( *this, nativePopup );
		
	}catch( const deException & ){
		DropNativeWidget();
		igdeNativeMenuCascade::DestroyNativePopup( *this, nativePopup );
		throw;
	}
}
