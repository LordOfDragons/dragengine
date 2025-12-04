/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeMenuCommand.h"
#include "../igdeContainer.h"
#include "../igdeCommonDialogs.h"
#include "../igdeWidget::Ref.h"
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
