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

#include "igdeNativeFoxMenuBar.h"
#include "../../../igdeContainer.h"
#include "../../../menu/igdeMenuBar.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxMenuBar ) igdeNativeFoxMenuBarMap[] = {
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxMenuBar::onChildLayoutFlags )
};



// Class igdeNativeFoxMenuBar
//////////////////////////////

FXIMPLEMENT( igdeNativeFoxMenuBar, FXMenuBar,
	igdeNativeFoxMenuBarMap, ARRAYNUMBER( igdeNativeFoxMenuBarMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuBar::igdeNativeFoxMenuBar(){ }

igdeNativeFoxMenuBar::igdeNativeFoxMenuBar( igdeMenuBar &powner, FXComposite *pparent, int layoutFlags ) :
FXMenuBar( pparent, layoutFlags ),
pOwner( &powner ){
}

igdeNativeFoxMenuBar::~igdeNativeFoxMenuBar(){
}

igdeNativeFoxMenuBar *igdeNativeFoxMenuBar::CreateNativeWidget( igdeMenuBar &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxMenuBar( powner, pparent, igdeUIFoxHelper::GetChildLayoutFlags( &powner ) );
}

void igdeNativeFoxMenuBar::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxMenuBar::DestroyNativeWidget(){
	delete this;
}



// Events
///////////

long igdeNativeFoxMenuBar::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	clflags.flags = LAYOUT_TOP | LAYOUT_LEFT;
	return 1;
}

#endif
