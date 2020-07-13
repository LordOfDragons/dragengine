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

#include "igdeToggleButton.h"
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


// Class igdeToggleButton
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeToggleButton::igdeToggleButton( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, eButtonStyle style ) :
igdeButton( environment, text, icon, style ),
pToggled( false ){
}

igdeToggleButton::igdeToggleButton( igdeEnvironment &environment, const char *text,
	const char *description, igdeIcon *icon, eButtonStyle style ) :
igdeButton( environment, text, description, icon, style ),
pToggled( false ){
}

igdeToggleButton::igdeToggleButton( igdeEnvironment &environment, igdeAction *action,
	eButtonStyle style ) :
igdeButton( environment, action, style ),
pToggled( false ){
}

igdeToggleButton::~igdeToggleButton(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeToggleButton::SetToggled( bool toggled ){
	if( pToggled == toggled ){
		return;
	}
	
	pToggled = toggled;
	OnToggledChanged();
}


void igdeToggleButton::OnAction(){
	SetToggled( ! pToggled );
	igdeButton::OnAction();
}

void igdeToggleButton::OnParameterChanged( igdeAction *action ){
	igdeButton::OnParameterChanged( action );
	SetToggled( action->GetSelected() );
}



void igdeToggleButton::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeToggleButton * const native = igdeNativeToggleButton::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeToggleButton::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeToggleButton* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeToggleButton::OnStyleChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeToggleButton* )GetNativeWidget() )->UpdateStyle();
	}
}

void igdeToggleButton::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxToggleButton* )GetNativeWidget() )->UpdateText();
	}
}

void igdeToggleButton::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeToggleButton* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeToggleButton::OnIconChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeToggleButton* )GetNativeWidget() )->UpdateIcons();
	}
}

void igdeToggleButton::OnEnabledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeToggleButton* )GetNativeWidget() )->UpdateEnabled();
	}
}

void igdeToggleButton::OnDefaultChanged(){
	// only used during widget construction
}

void igdeToggleButton::OnToggledChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeToggleButton* )GetNativeWidget() )->UpdateToggled();
	}
}
