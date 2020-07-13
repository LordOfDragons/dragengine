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

igdeNativeFoxContainerFixed::igdeNativeFoxContainerFixed( igdeContainerFixed &owner,
	FXComposite *parent, int layoutFlags ) :
FXPacker( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ){
}

igdeNativeFoxContainerFixed::~igdeNativeFoxContainerFixed(){
}

igdeNativeFoxContainerFixed *igdeNativeFoxContainerFixed::CreateNativeWidget( igdeContainerFixed &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxContainerFixed( owner, parent,
		igdeUIFoxHelper::GetChildLayoutFlags( &owner ) );
}

void igdeNativeFoxContainerFixed::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxContainerFixed::DestroyNativeWidget(){
	delete this;
}



// Events
///////////

long igdeNativeFoxContainerFixed::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = 0;
	clflags.canResizeHorizontal = true;
	clflags.canResizeVertical = true;
	return 1;
}
