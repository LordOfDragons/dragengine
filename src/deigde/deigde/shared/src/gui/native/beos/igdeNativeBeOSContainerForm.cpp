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

#ifdef IGDE_TOOLKIT_BEOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeBeOSContainerForm.h"
#include "../../layout/igdeContainerForm.h"

#include <dragengine/common/exceptions.h>



// Class igdeNativeBeOSContainerForm
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeBeOSContainerForm::igdeNativeBeOSContainerForm( igdeContainerForm &owner ) :
BGridView( owner.GetColumnSpacing(), owner.GetRowSpacing() ),
pOwner( owner )
{
	if( ! owner.GetVisible() ){
		Hide();
	}
}

igdeNativeBeOSContainerForm::~igdeNativeBeOSContainerForm(){
}

igdeNativeBeOSContainerForm *igdeNativeBeOSContainerForm::CreateNativeWidget( igdeContainerForm &owner ){
	igdeNativeBeOSContainerForm * const native = new igdeNativeBeOSContainerForm( owner );
	igdeUIBeOSHelper::AddView( native, owner.GetParent() );
	return native;
}

void igdeNativeBeOSContainerForm::PostCreateNativeWidget(){
}

void igdeNativeBeOSContainerForm::DestroyNativeWidget(){
	igdeUIBeOSHelper::DestroyView( this );
}



// Management
///////////////

void igdeNativeBeOSContainerForm::ChildRemoved(){
	if( pOwner.GetStretching() != igdeContainerForm::esLast ){
		return;
	}
	
	// due to the way the BGridLayout works we have to remove all views
	// re-adding them to arrange them new.
	while( CountChildren() > 0 ){
		RemoveChild( ChildAt( CountChildren() - 1 ) );
	}
	
	const int count = pOwner.GetChildCount();
	int i;
	for( i=0; i<count; i++ ){
		BView * const child = ( BView* )pOwner.GetChildAt( i )->GetNativeWidget();
		if( child ){
			Perform( igdeUIBeOSHelper::epcAddView, child );
		}
	}
}



void igdeNativeBeOSContainerForm::FrameResized( float newWidth, float newHeight ){
	pOwner.OnResize();
	BGridView::FrameResized( newWidth, newHeight );
}

status_t igdeNativeBeOSContainerForm::Perform( perform_code d, void *arg ){
	switch( d ){
	case igdeUIBeOSHelper::epcAddView:{
		const int index = CountChildren();
		GridLayout()->AddView( ( BView* )arg, index % 2, index / 2 );
		}return B_OK;
		
	default:
		return BGridView::Perform( d, arg );
	}
}

#endif
