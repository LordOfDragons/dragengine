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

#include "igdeStatusBar.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "native/fox/igdeNativeFoxStatusBar.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeStatusBar
///////////////////////

// Constructor, destructor
////////////////////////////

igdeStatusBar::igdeStatusBar( igdeEnvironment &environment ) :
igdeContainer( environment ){
}

igdeStatusBar::~igdeStatusBar(){
	igdeStatusBar::DestroyNativeWidget();
}



// Management
///////////////

void igdeStatusBar::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeStatusBar::ClearText(){
	if( pText.IsEmpty() ){
		return;
	}
	
	pText.Empty();
	OnTextChanged();
}



void igdeStatusBar::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeContainer * const parent = GetParent();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const nativeParent = ( FXComposite* )parent->GetNativeContainer();
	if( ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeFoxStatusBar * const native = new igdeNativeFoxStatusBar( *this,
		nativeParent, igdeUIFoxHelper::GetChildLayoutFlagsAll( this ), *GetGuiTheme() );
	SetNativeWidget( native );
	if( nativeParent->id() ){
		native->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeStatusBar::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( igdeNativeFoxStatusBar* )GetNativeWidget();
	DropNativeWidget();
}

void igdeStatusBar::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxStatusBar* )GetNativeWidget() )->UpdateText();
	}
}
