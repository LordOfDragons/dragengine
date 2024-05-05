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

#include "native/toolkit.h"
#include "igdeGroupBox.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class igdeGroupBox
///////////////////////

// Constructor, destructor
////////////////////////////

igdeGroupBox::igdeGroupBox( igdeEnvironment &environment, const char *title,
	eTitleAlignment titleAlignment ) :
igdeContainer( environment ),
pTitle( title ),
pTitleAlignment( titleAlignment ),
pCanCollapse( false ),
pCollapsed( false ),
pStretchLast( false ){
}

igdeGroupBox::igdeGroupBox( igdeEnvironment &environment, const char *title,
	const char *description, eTitleAlignment titleAlignment ) :
igdeContainer( environment ),
pTitle( title ),
pTitleAlignment( titleAlignment ),
pDescription( description ),
pCanCollapse( false ),
pCollapsed( false ),
pStretchLast( false ){
}

igdeGroupBox::igdeGroupBox( igdeEnvironment &environment, const char *title,
	bool collapsed, eTitleAlignment titleAlignment ) :
igdeContainer( environment ),
pTitle( title ),
pTitleAlignment( titleAlignment ),
pCanCollapse( true ),
pCollapsed( collapsed ),
pStretchLast( false ){
}

igdeGroupBox::igdeGroupBox( igdeEnvironment &environment, const char *title,
	const char *description, bool collapsed, eTitleAlignment titleAlignment ) :
igdeContainer( environment ),
pTitle( title ),
pTitleAlignment( titleAlignment ),
pDescription( description ),
pCanCollapse( true ),
pCollapsed( collapsed ),
pStretchLast( false ){
}

igdeGroupBox::~igdeGroupBox(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeGroupBox::SetTitle( const char *title ){
	if( pTitle == title ){
		return;
	}
	
	pTitle = title;
	OnTitleChanged();
}

void igdeGroupBox::SetTitleAlignment( eTitleAlignment titleAlignment ){
	if( pTitleAlignment == titleAlignment ){
		return;
	}
	
	pTitleAlignment = titleAlignment;
	OnTitleAlignmentChanged();
}

void igdeGroupBox::SetDescription( const char *description ){
	if( pDescription == description ){
		return;
	}
	
	pDescription = description;
	OnDescriptionChanged();
}

void igdeGroupBox::SetCanCollapse( bool canCollapse ){
	if( pCanCollapse == canCollapse ){
		return;
	}
	
	if( ! canCollapse ){
		SetCollapsed( false );
	}
	
	pCanCollapse = canCollapse;
	OnCanCollapseChanged();
}

void igdeGroupBox::SetCollapsed( bool collapsed ){
	if( pCollapsed == collapsed ){
		return;
	}
	
	pCollapsed = collapsed;
	OnCollapsedChanged();
}

void igdeGroupBox::SetStretchLast( bool stretchLast ){
	if( stretchLast == pStretchLast ){
		return;
	}
	pStretchLast = stretchLast;
	OnStretchLastChanged();
}



void igdeGroupBox::AddChild( igdeWidget *child ){
	if( GetChildCount() > 0 ){
		DETHROW( deeInvalidParam );
	}
	igdeContainer::AddChild( child );
}

void igdeGroupBox::RemoveChild( igdeWidget *child ){
	igdeContainer::RemoveChild( child );
	
	/* // TODO if stretching last widget is supported activate this
	if( GetChildCount() > 0 ){
		igdeUIFoxHelper::UpdateLayoutFlags( GetChildAt( GetChildCount() - 1 ) );
		( ( FXPacker* )GetNativeContainer() )->recalc();
	}
	*/
}



void igdeGroupBox::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeGroupBox * const native = igdeNativeGroupBox::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
}

void igdeGroupBox::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeGroupBox* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeGroupBox::OnTitleChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeGroupBox* )GetNativeWidget() )->UpdateTitle();
}

void igdeGroupBox::OnTitleAlignmentChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeGroupBox* )GetNativeWidget() )->UpdateTitleAlignment();
}

void igdeGroupBox::OnDescriptionChanged(){
	/* // not supported
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxGroupBox &groupbox = *( ( igdeNativeFoxGroupBox* )GetNativeWidget() );
	groupbox.setTipText( pDescription.GetString() );
	groupbox.setHelpText( pDescription.GetString() );
	*/
}

void igdeGroupBox::OnCanCollapseChanged(){
}

void igdeGroupBox::OnCollapsedChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeGroupBox* )GetNativeWidget() )->UpdateCollapsed();
}

void igdeGroupBox::OnStretchLastChanged(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeGroupBox* )GetNativeWidget() )->UpdateStretchLast();
}
