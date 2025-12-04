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

#include "igdeToggleButton.h"
#include "igdeContainer.h"
#include "igdeCommonDialogs.h"
#include "event/igdeAction.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFont.h"
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
		( ( igdeNativeToggleButton* )GetNativeWidget() )->UpdateText();
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
