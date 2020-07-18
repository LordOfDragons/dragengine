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

#include "igdeCheckBox.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "event/igdeAction.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeCheckBox
///////////////////////

// Constructor, destructor
////////////////////////////

igdeCheckBox::igdeCheckBox( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, eButtonStyle style ) :
igdeButton( environment, text, icon, style ),
pChecked( false ){
}

igdeCheckBox::igdeCheckBox( igdeEnvironment &environment, const char *text,
	const char *description, igdeIcon *icon, eButtonStyle style ) :
igdeButton( environment, text, description, icon, style ),
pChecked( false ){
}

igdeCheckBox::igdeCheckBox( igdeEnvironment &environment, igdeAction *action, eButtonStyle style ) :
igdeButton( environment, action, style ),
pChecked( false )
{
	SetAction( action );
}

igdeCheckBox::~igdeCheckBox(){
}



// Management
///////////////

void igdeCheckBox::SetChecked( bool checked ){
	if( pChecked == checked ){
		return;
	}
	
	pChecked = checked;
	OnCheckedChanged();
}



void igdeCheckBox::OnParameterChanged( igdeAction *action ){
	igdeButton::OnParameterChanged( action );
	
	SetChecked( action->GetSelected() );
}



void igdeCheckBox::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeCheckBox * const native = igdeNativeCheckBox::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeCheckBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeCheckBox* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeCheckBox::OnCheckedChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeCheckBox* )GetNativeWidget() )->UpdateChecked();
}

void igdeCheckBox::OnStyleChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeCheckBox* )GetNativeWidget() )->UpdateStyle();
}

void igdeCheckBox::OnTextChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeCheckBox* )GetNativeWidget() )->UpdateText();
}

void igdeCheckBox::OnDescriptionChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeCheckBox* )GetNativeWidget() )->UpdateDescription();
}

void igdeCheckBox::OnIconChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeCheckBox* )GetNativeWidget() )->UpdateIcon();
}

void igdeCheckBox::OnEnabledChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeCheckBox* )GetNativeWidget() )->UpdateEnabled();
}
