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

#include "igdeContainerForm.h"
#include "../native/toolkit.h"
#include "../igdeWidget.h"
#include "../native/fox/igdeNativeFoxContainerForm.h"

#include <dragengine/common/exceptions.h>



// Class igdeContainerForm
////////////////////////////

// Constructor, destructor
////////////////////////////

void igdeContainerForm::RemoveChild( igdeWidget *child ){
	igdeContainer::RemoveChild( child );
	
	if( ! GetNativeContainer() ){
		return;
	}
	
	const int count = GetChildCount();
	if( count < 1 ){
		return;
	}
	
	if( pStretching == esLast ){
		const int index = count - ( count % 2 );
		igdeUIFoxHelper::UpdateLayoutFlags( GetChildAt( index ) );
		if( index + 1 < count ){
			igdeUIFoxHelper::UpdateLayoutFlags( GetChildAt( index + 1 ) );
		}
		( ( igdeNativeFoxContainerForm* )GetNativeContainer() )->recalc();
	}
}



igdeContainerForm::igdeContainerForm( igdeEnvironment &environment, eStretching stretching,
	int columnSpacing, int rowSpacing ) :
igdeContainer( environment ),
pStretching( stretching ),
pColumnSpacing( columnSpacing ),
pRowSpacing( rowSpacing )
{
	if( columnSpacing < 0 || rowSpacing < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeContainerForm::~igdeContainerForm(){
}



// Management
///////////////

void igdeContainerForm::CreateNativeWidget(){
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
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	FXPacker * const native = new igdeNativeFoxContainerForm( *this, nativeParent, layoutFlags );
	
	SetNativeWidget( native );
	if( nativeParent->id() ){
		native->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerForm::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	igdeNativeFoxContainerForm * const native = ( igdeNativeFoxContainerForm* )GetNativeWidget();
	DropNativeWidget();
	delete native;
}
