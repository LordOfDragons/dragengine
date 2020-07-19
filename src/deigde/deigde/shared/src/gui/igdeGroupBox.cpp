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
