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
#include <stdint.h>

#include "igdeNativeFoxContainerForm.h"
#include "../../layout/igdeContainerForm.h"

#include <dragengine/common/exceptions.h>



// Event map
//////////////

FXDEFMAP( igdeNativeFoxContainerForm ) igdeNativeFoxContainerFormMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerForm::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerForm::onChildLayoutFlags )
};


FXIMPLEMENT( igdeNativeFoxContainerForm, FXMatrix,
	igdeNativeFoxContainerFormMap, ARRAYNUMBER( igdeNativeFoxContainerFormMap ) )



// Class igdeNativeFoxContainerForm
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxContainerForm::igdeNativeFoxContainerForm(){
}

igdeNativeFoxContainerForm::igdeNativeFoxContainerForm(
	igdeContainerForm &owner, FXComposite *parent, int layoutFlags ) :
FXMatrix( parent, 2, MATRIX_BY_COLUMNS | layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0,
	owner.GetColumnSpacing(), owner.GetRowSpacing() ),
pOwner( &owner )
{
	if( ! pOwner->GetVisible() ){
		hide();
	}
}

igdeNativeFoxContainerForm::~igdeNativeFoxContainerForm(){
}

igdeNativeFoxContainerForm *igdeNativeFoxContainerForm::CreateNativeWidget( igdeContainerForm &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxContainerForm( owner, parent, igdeUIFoxHelper::GetChildLayoutFlags( &owner ) );
}

void igdeNativeFoxContainerForm::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxContainerForm::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxContainerForm::ChildRemoved(){
	if( pOwner->GetStretching() != igdeContainerForm::esLast ){
		return;
	}
	
	const int count = pOwner->GetChildCount();
	const int index = count - ( count % 2 );
	igdeUIFoxHelper::UpdateLayoutFlags( pOwner->GetChildAt( index ) );
	if( index + 1 < count ){
		igdeUIFoxHelper::UpdateLayoutFlags( pOwner->GetChildAt( index + 1 ) );
	}
	
	recalc();
}



// Events
///////////

long igdeNativeFoxContainerForm::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerForm::onChildLayoutFlags( FXObject*, FXSelector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = LAYOUT_FILL_X | LAYOUT_FILL_Y;
	
	const int index = pOwner->IndexOfChild( clflags.widget );
	if( index == -1 ){
		return 1;
	}
	
	// right side widgets fill column but not left side widgets. this makes the right
	// column to stretch but keeps the left column at minimal size
	clflags.canResizeHorizontal = true;
	
	if( index % 2 == 1 ){
		clflags.flags |= LAYOUT_FILL_COLUMN;
		clflags.canResizeHorizontal = false;
	}
	
	// stretch rows
	clflags.canResizeVertical = true;
	
	switch( pOwner->GetStretching() ){
	case igdeContainerForm::esFirst:
		if( index / 2 == 0 ){
			clflags.flags |= LAYOUT_FILL_ROW;
			clflags.canResizeVertical = false;
		}
		break;
		
	case igdeContainerForm::esLast:
		if( index / 2 == pOwner->GetChildCount() / 2 - 1 ){
			clflags.flags |= LAYOUT_FILL_ROW;
			clflags.canResizeVertical = false;
		}
		break;
		
	case igdeContainerForm::esNone:
	default:
		break;
	}
	
	return 1;
}
