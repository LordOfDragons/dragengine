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

#include "igdeNativeFoxContainerFixed.h"
#include "../../../layout/igdeContainerFixed.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxContainerFixed ) igdeNativeFoxContainerFixedMap[] = {
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxContainerFixed::onChildLayoutFlags )
};



// Class igdeNativeFoxContainerFixed
//////////////////////////////////////

FXIMPLEMENT( igdeNativeFoxContainerFixed, FXPacker,
	igdeNativeFoxContainerFixedMap, ARRAYNUMBER( igdeNativeFoxContainerFixedMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxContainerFixed::igdeNativeFoxContainerFixed(){ }

igdeNativeFoxContainerFixed::igdeNativeFoxContainerFixed( igdeContainerFixed &powner,
	FXComposite *pparent, int layoutFlags ) :
FXPacker( pparent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &powner ){
}

igdeNativeFoxContainerFixed::~igdeNativeFoxContainerFixed(){
}

igdeNativeFoxContainerFixed *igdeNativeFoxContainerFixed::CreateNativeWidget( igdeContainerFixed &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxContainerFixed( powner, pparent,
		igdeUIFoxHelper::GetChildLayoutFlags( &powner ) );
}

void igdeNativeFoxContainerFixed::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxContainerFixed::DestroyNativeWidget(){
	delete this;
}



// Events
///////////

long igdeNativeFoxContainerFixed::onChildLayoutFlags( FXObject*, FXSelector, void *pdata ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )pdata );
	clflags.flags = 0;
	clflags.canResizeHorizontal = true;
	clflags.canResizeVertical = true;
	return 1;
}

#endif
