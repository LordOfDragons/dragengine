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

#include "igdeMenuCommand.h"
#include "../igdeContainer.h"
#include "../igdeCommonDialogs.h"
#include "../igdeWidgetReference.h"
#include "../event/igdeAction.h"
#include "../native/toolkit.h"
#include "../resources/igdeIcon.h"
#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeMenuCommand
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuCommand::igdeMenuCommand( igdeEnvironment &environment ) :
igdeWidget( environment ),
pMnemonic( deInputEvent::ekcUndefined ),
pEnabled( true ){
}

igdeMenuCommand::igdeMenuCommand( igdeEnvironment &environment, igdeAction *action ) :
igdeWidget( environment ),
pMnemonic( deInputEvent::ekcUndefined ),
pEnabled( true )
{
	SetAction( action );
}

igdeMenuCommand::~igdeMenuCommand(){
	igdeMenuCommand::DestroyNativeWidget();
	SetAction( NULL );
}



// Management
///////////////

void igdeMenuCommand::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeMenuCommand::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeMenuCommand::SetHotKey( const igdeHotKey &hotKey ){
	if( pHotKey == hotKey ){
		return;
	}
	
	pHotKey = hotKey;
	OnHotKeyChanged();
}

void igdeMenuCommand::SetMnemonic( deInputEvent::eKeyCodes mnemonic ){
	if( pMnemonic == mnemonic ){
		return;
	}
	
	pMnemonic = mnemonic;
	OnMnemonicChanged();
}

void igdeMenuCommand::SetIcon( igdeIcon *icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}

void igdeMenuCommand::SetEnabled( bool enabled ){
	if( pEnabled == enabled ){
		return;
	}
	
	pEnabled = enabled;
	OnEnabledChanged();
}

void igdeMenuCommand::SetAction( igdeAction *action ){
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


void igdeMenuCommand::OnAction(){
	if( pAction ){
		pAction->OnAction();
	}
}

void igdeMenuCommand::OnParameterChanged( igdeAction *action ){
	SetText( action->GetText() );
	SetDescription( action->GetDescription() );
	SetHotKey( action->GetHotKey() );
	SetMnemonic( action->GetMnemonic() );
	SetIcon( action->GetIcon() );
	SetEnabled( action->GetEnabled() );
}

void igdeMenuCommand::OnDestroyed( igdeAction *action ){
	GetLogger()->LogWarnFormat( "IGDE", "igdeMenuCommand::OnDestroyed: "
		"Action(%s) destroyed while still listening on it", action->GetText().GetString() );
	pAction = NULL;
}



void igdeMenuCommand::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeMenuCommand * const native = igdeNativeMenuCommand::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeMenuCommand::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeMenuCommand* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeMenuCommand::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeMenuCommand* )GetNativeWidget() )->UpdateText();
	}
}

void igdeMenuCommand::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeMenuCommand* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeMenuCommand::OnHotKeyChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeMenuCommand* )GetNativeWidget() )->UpdateHotKey();
	}
}

void igdeMenuCommand::OnMnemonicChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
}

void igdeMenuCommand::OnIconChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeMenuCommand* )GetNativeWidget() )->UpdateIcon();
	}
}

void igdeMenuCommand::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeMenuCommand* )GetNativeWidget() )->UpdateEnabled();
	}
}
