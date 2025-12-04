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

#include "igdeLabel.h"
#include "igdeContainer.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFont.h"
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
