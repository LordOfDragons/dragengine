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

#include "igdeAction.h"
#include "igdeActionListener.h"

#include <dragengine/common/exceptions.h>



// Class igdeAction
/////////////////////

// Constructor, destructor
////////////////////////////

igdeAction::igdeAction() :
pMnemonic( deInputEvent::ekcUndefined ),
pEnabled( true ),
pSelected( false ),
pDefault( false ){
}

igdeAction::igdeAction( const char *text, const char *description ) :
pText( text ),
pDescription( description ),
pMnemonic( deInputEvent::ekcUndefined ),
pEnabled( true ),
pSelected( false ),
pDefault( false ){
}

igdeAction::igdeAction( const char *text, const char *description, const igdeHotKey &hotKey ) :
pText( text ),
pDescription( description ),
pHotKey( hotKey ),
pMnemonic( deInputEvent::ekcUndefined ),
pEnabled( true ),
pSelected( false ),
pDefault( false ){
}

igdeAction::igdeAction( const char *text, const char *description, deInputEvent::eKeyCodes mnemonic ) :
pText( text ),
pDescription( description ),
pMnemonic( mnemonic ),
pEnabled( true ),
pSelected( false ),
pDefault( false ){
}

igdeAction::igdeAction( const char *text, const char *description,
	deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey ) :
pText( text ),
pDescription( description ),
pHotKey( hotKey ),
pMnemonic( mnemonic ),
pEnabled( true ),
pSelected( false ),
pDefault( false ){
}

igdeAction::igdeAction( const char *text, igdeIcon *icon, const char *description ) :
pText( text ),
pDescription( description ),
pMnemonic( deInputEvent::ekcUndefined ),
pIcon( icon ),
pEnabled( true ),
pSelected( false ),
pDefault( false ){
}

igdeAction::igdeAction( const char *text, igdeIcon *icon, const char *description,
	const igdeHotKey &hotKey ) :
pText( text ),
pDescription( description ),
pHotKey( hotKey ),
pMnemonic( deInputEvent::ekcUndefined ),
pIcon( icon ),
pEnabled( true ),
pSelected( false ),
pDefault( false ){
}

igdeAction::igdeAction( const char *text, igdeIcon *icon, const char *description,
	deInputEvent::eKeyCodes mnemonic ) :
pText( text ),
pDescription( description ),
pMnemonic( mnemonic ),
pIcon( icon ),
pEnabled( true ),
pSelected( false ),
pDefault( false ){
}

igdeAction::igdeAction( const char *text, igdeIcon *icon, const char *description,
	deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey ) :
pText( text ),
pDescription( description ),
pHotKey( hotKey ),
pMnemonic( mnemonic ),
pIcon( icon ),
pEnabled( true ),
pSelected( false ),
pDefault( false ){
}

igdeAction::~igdeAction(){
	const decPointerOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeActionListener* )listeners.GetAt( i ) )->OnDestroyed( this );
	}
}



// Management
///////////////

void igdeAction::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	NotifyParametersChanged();
}

void igdeAction::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	NotifyParametersChanged();
}

void igdeAction::SetHotKey( const igdeHotKey &hotKey ){
	if( pHotKey == hotKey ){
		return;
	}
	
	pHotKey = hotKey;
	NotifyParametersChanged();
}

void igdeAction::SetMnemonic( deInputEvent::eKeyCodes mnemonic ){
	if( pMnemonic == mnemonic ){
		return;
	}
	
	pMnemonic = mnemonic;
	NotifyParametersChanged();
}

void igdeAction::SetIcon( igdeIcon *icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	NotifyParametersChanged();
}

void igdeAction::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	NotifyParametersChanged();
}

void igdeAction::SetSelected( bool selected ){
	if( pSelected == selected ){
		return;
	}
	
	pSelected = selected;
	NotifyParametersChanged();
}

void igdeAction::SetDefault( bool isdefault ){
	if( pDefault == isdefault ){
		return;
	}
	
	pDefault = isdefault;
	NotifyParametersChanged();
}



void igdeAction::AddListener( igdeActionListener *listener ){
	if( ! listener || pListeners.Has( listener ) ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeAction::RemoveListener( igdeActionListener *listener ){
	pListeners.Remove( listener );
}

void igdeAction::NotifyParametersChanged(){
	const decPointerOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeActionListener* )listeners.GetAt( i ) )->OnParameterChanged( this );
	}
}



void igdeAction::Update(){
}
