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

#include "igdeButton.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeAction.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
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
		igdeAction::Ref( pAction )->OnAction();
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
