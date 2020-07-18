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

#include "igdeSwitcher.h"
#include "native/toolkit.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>


// Class igdeSwitcher
///////////////////////

// Constructor, destructor
////////////////////////////

igdeSwitcher::igdeSwitcher( igdeEnvironment &environment ) :
igdeContainer( environment ),
pCurrent( -1 ){
}

igdeSwitcher::~igdeSwitcher(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeSwitcher::SetCurrent( int index ){
	if( index == pCurrent ){
		return;
	}
	
	if( GetChildCount() > 0 ){
		if( index < 0 || index >= GetChildCount() ){
			DETHROW( deeInvalidParam );
		}
		
	}else{
		if( index != -1 ){
			DETHROW( deeInvalidParam );
		}
	}
	
	pCurrent = index;
	
	OnCurrentChanged();
}



void igdeSwitcher::AddChild( igdeWidget *child ){
	igdeContainer::AddChild( child );
	pCurrent = 0;
}

void igdeSwitcher::RemoveChild( igdeWidget *child ){
	const int index = IndexOfChild( child );
	igdeContainer::RemoveChild( child );
	
	int current = pCurrent;
	
	if( index < current ){
		current--;
		
	}else if( current >= GetChildCount() ){
		current = GetChildCount() - 1;
	}
	
	SetCurrent( current );
}

void igdeSwitcher::RemoveAllChildren(){
	igdeContainer::RemoveAllChildren();
	pCurrent = -1;
}



void igdeSwitcher::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeSwitcher * const native = igdeNativeSwitcher::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
	
	native->UpdateCurrent();
}

void igdeSwitcher::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeSwitcher* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



// Protected Functions
////////////////////////

void igdeSwitcher::OnCurrentChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeSwitcher* )GetNativeWidget() )->UpdateCurrent();
	}
}
