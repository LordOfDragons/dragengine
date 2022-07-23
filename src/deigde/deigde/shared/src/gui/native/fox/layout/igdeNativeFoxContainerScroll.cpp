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

#ifdef IGDE_TOOLKIT_FOX

#include "igdeNativeFoxContainerScroll.h"
#include "../../../layout/igdeContainerScroll.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxContainerScroll ) igdeNativeFoxContainerScrollMap[] = {
	FXMAPFUNC( SEL_CONFIGURE, 0, igdeNativeFoxContainerScroll::onResize ),
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerScroll::onChildLayoutFlags )
};



// Class igdeNativeFoxContainerScroll
///////////////////////////////////////

FXIMPLEMENT( igdeNativeFoxContainerScroll, FXScrollWindow,
	igdeNativeFoxContainerScrollMap, ARRAYNUMBER( igdeNativeFoxContainerScrollMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxContainerScroll::igdeNativeFoxContainerScroll(){ }

igdeNativeFoxContainerScroll::igdeNativeFoxContainerScroll( igdeContainerScroll &powner,
	FXComposite *pparent, int layoutFlags ) :
FXScrollWindow( pparent, layoutFlags, 0, 0, 0, 0 ),
pOwner( &powner ){
}

igdeNativeFoxContainerScroll::~igdeNativeFoxContainerScroll(){
}

igdeNativeFoxContainerScroll *igdeNativeFoxContainerScroll::CreateNativeWidget( igdeContainerScroll &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( &powner ) | SCROLLERS_NORMAL | SCROLLERS_TRACK;
	if( ! powner.GetCanScrollX() ){
		layoutFlags |= HSCROLLING_OFF;
	}
	if( ! powner.GetCanScrollY() ){
		layoutFlags |= VSCROLLING_OFF;
	}
	
	return new igdeNativeFoxContainerScroll( powner, pparent, layoutFlags );
}

void igdeNativeFoxContainerScroll::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxContainerScroll::DestroyNativeWidget(){
	delete this;
}



// Events
///////////

long igdeNativeFoxContainerScroll::onResize( FXObject*, FXSelector, void* ){
	pOwner->OnResize();
	return 1;
}

long igdeNativeFoxContainerScroll::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	clflags.flags = LAYOUT_SIDE_TOP;
	
	//if( ! pOwner->GetCanScrollX() ){
		clflags.flags |= LAYOUT_FILL_X;
	//}
	//if( ! pOwner->GetCanScrollY() ){
		clflags.flags |= LAYOUT_FILL_Y;
	//}
	
	return 1;
}

#endif
