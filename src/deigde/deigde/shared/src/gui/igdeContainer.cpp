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
#include "igdeContainer.h"
#include "igdeWidget.h"

#include <dragengine/common/exceptions.h>



// Native Widget
//////////////////



// Class igdeContainer
////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainer::igdeContainer( igdeEnvironment &environment ) :
igdeWidget( environment ){
}

igdeContainer::~igdeContainer(){
	RemoveAllChildren();
}



// Management
///////////////

int igdeContainer::GetChildCount() const{
	return pChildren.GetCount();
}

igdeWidget *igdeContainer::GetChildAt( int index ) const{
	return ( igdeWidget* )pChildren.GetAt( index );
}

int igdeContainer::IndexOfChild( igdeWidget *widget ) const{
	const int count = pChildren.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( igdeWidget* )pChildren.GetAt( i ) == widget ){
			return i;
		}
	}
	
	return -1;
}

void igdeContainer::AddChild( igdeWidget *child ){
	if( ! child || child->GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	pChildren.Add( child );
	child->SetParent( this );
	if( GetNativeContainer() ){
		child->CreateNativeWidget();
	}
}

void igdeContainer::RemoveChild( igdeWidget *child ){
	if( ! child || child->GetParent() != this || ! pChildren.Has( child ) ){
		DETHROW( deeInvalidParam );
	}
	
	child->DestroyNativeWidget();
	child->SetParent( NULL );
	pChildren.Remove( child );
}

void igdeContainer::RemoveAllChildren(){
	while( pChildren.GetCount() > 0 ){
		const int index = pChildren.GetCount() - 1;
		igdeWidget &child = *( ( igdeWidget* )pChildren.GetAt( index ) );
		
		child.DestroyNativeWidget();
		child.SetParent( NULL );
		
		pChildren.RemoveFrom( index );
	}
}



void igdeContainer::DropNativeWidget(){
	const int count = pChildren.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeWidget* )pChildren.GetAt( i ) )->DropNativeWidget();
	}
	
	igdeWidget::DropNativeWidget();
}

void igdeContainer::CreateChildWidgetNativeWidgets(){
	const int count = pChildren.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeWidget* )pChildren.GetAt( i ) )->CreateNativeWidget();
	}
}

void igdeContainer::OnGuiThemeChanged(){
	igdeWidget::OnGuiThemeChanged();
	NotifyChildrenGuiThemeChanged();
}

void igdeContainer::NotifyChildrenGuiThemeChanged(){
	igdeGuiTheme * const guitheme = GetGuiTheme();
	const int count = pChildren.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		igdeWidget * const widget = ( igdeWidget* )pChildren.GetAt( i );
		if( widget->GetGuiTheme() != guitheme ){
			widget->OnGuiThemeChanged();
		}
	}
}

void *igdeContainer::GetNativeContainer() const{
	return GetNativeWidget();
}
