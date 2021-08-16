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

#include "igdeButton.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeAction.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeButton
/////////////////////

// Constructor, destructor
////////////////////////////

igdeButton::igdeButton( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, eButtonStyle style ) :
igdeWidget( environment ),
pStyle( style ),
pText( text ),
pIcon( icon ),
pEnabled( true ),
pDefault( false ){
}

igdeButton::igdeButton( igdeEnvironment &environment, const char *text,
	const char *description, igdeIcon *icon, eButtonStyle style ) :
igdeWidget( environment ),
pStyle( style ),
pText( text ),
pDescription( description ),
pIcon( icon ),
pEnabled( true ),
pDefault( false ){
}

igdeButton::igdeButton( igdeEnvironment &environment, igdeAction *action, eButtonStyle style ) :
igdeWidget( environment ),
pStyle( style ),
pEnabled( true ),
pDefault( false )
{
	SetAction( action );
}

igdeButton::~igdeButton(){
	DestroyNativeWidget();
	SetAction( NULL );
}



// Management
///////////////

void igdeButton::SetStyle( eButtonStyle style ){
	if( pStyle == style ){
		return;
	}
	
	pStyle = style;
	OnStyleChanged();
}

void igdeButton::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeButton::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeButton::SetIcon( igdeIcon *icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}

void igdeButton::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeButton::SetDefault( bool isdefault ){
	if( pDefault == isdefault ){
		return;
	}
	
	pDefault = isdefault;
	OnDefaultChanged();
}

void igdeButton::SetAction( igdeAction *action ){
	if( pAction == action ){
		return;
	}
	
	if( pAction ){
		pAction->RemoveListener( this );
	}
	
	pAction = action;
	
	if( action ){
		action->AddListener( this );
		OnParameterChanged( action );
	}
}

void igdeButton::Focus(){
	if( GetNativeWidget() ){
		( ( igdeNativeButton* )GetNativeWidget() )->Focus();
	}
}


void igdeButton::OnAction(){
	if( pAction ){
		// guard against action being deleted while in use
		igdeActionReference( pAction )->OnAction();
	}
}

void igdeButton::OnParameterChanged( igdeAction *action ){
	SetText( action->GetText() );
	SetDescription( action->GetDescription() );
	SetIcon( action->GetIcon() );
	SetEnabled( action->GetEnabled() );
	SetDefault( action->GetDefault() );
}

void igdeButton::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeButton::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



void igdeButton::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeButton * const native = igdeNativeButton::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeButton::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeButton* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeButton::OnStyleChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeButton* )GetNativeWidget() )->UpdateStyle();
}

void igdeButton::OnTextChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeButton* )GetNativeWidget() )->UpdateText();
}

void igdeButton::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeButton* )GetNativeWidget() )->UpdateDescription();
}

void igdeButton::OnIconChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeButton* )GetNativeWidget() )->UpdateIcon();
}

void igdeButton::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeButton* )GetNativeWidget() )->UpdateEnabled();
}

void igdeButton::OnDefaultChanged(){
	// only used during widget construction
}
