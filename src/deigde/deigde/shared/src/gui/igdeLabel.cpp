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

#include "igdeLabel.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeLabel
/////////////////////

// Constructor, destructor
////////////////////////////

igdeLabel::igdeLabel( igdeEnvironment &environment, const char *text, int alignment ) :
igdeWidget( environment ),
pText( text ),
pAlignment( alignment ){
}

igdeLabel::igdeLabel( igdeEnvironment &environment, const char *text,
	const char *description, int alignment ) :
igdeWidget( environment ),
pText( text ),
pAlignment( alignment ),
pDescription( description ){
}

igdeLabel::igdeLabel( igdeEnvironment &environment, const char *text,
	igdeIcon *icon, int alignment ) :
igdeWidget( environment ),
pText( text ),
pAlignment( alignment ),
pIcon( icon ){
}

igdeLabel::igdeLabel( igdeEnvironment &environment, const char *text,
	const char *description, igdeIcon *icon, int alignment ) :
igdeWidget( environment ),
pText( text ),
pAlignment( alignment ),
pDescription( description ),
pIcon( icon ){
}

igdeLabel::~igdeLabel(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeLabel::SetText( const char *text ){
	if( pText == text ){
		return;
	}
	
	pText = text;
	OnTextChanged();
}

void igdeLabel::SetAlignment( int alignment ){
	if( pAlignment == alignment ){
		return;
	}
	
	pAlignment = alignment;
	OnAlignmentChanged();
}

void igdeLabel::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeLabel::SetIcon( igdeIcon *icon ){
	if( pIcon == icon ){
		return;
	}
	
	pIcon = icon;
	OnIconChanged();
}



void igdeLabel::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeLabel * const native = igdeNativeLabel::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeLabel::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeLabel* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeLabel::OnTextChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeLabel* )GetNativeWidget() )->UpdateText();
	}
}

void igdeLabel::OnAlignmentChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeLabel* )GetNativeWidget() )->UpdateAlignment();
	}
}

void igdeLabel::OnDescriptionChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeLabel* )GetNativeWidget() )->UpdateDescription();
	}
}

void igdeLabel::OnIconChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeLabel* )GetNativeWidget() )->UpdateIcon();
	}
}
