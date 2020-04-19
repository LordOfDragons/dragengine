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

#include "igdeContainerFixed.h"
#include "../native/toolkit.h"
#include "../igdeWidget.h"

#include <dragengine/common/exceptions.h>



// Native Widget
//////////////////

class igdeNativeFoxLayoutFixed : public FXPacker{
	FXDECLARE( igdeNativeFoxLayoutFixed )
	
protected:
	igdeNativeFoxLayoutFixed();
	
private:
	igdeContainerFixed *pOwner;
	
public:
	igdeNativeFoxLayoutFixed( igdeContainerFixed &owner, FXComposite *parent, int layoutFlags );
	virtual ~igdeNativeFoxLayoutFixed();
	
	long onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data );
};


FXDEFMAP( igdeNativeFoxLayoutFixed ) igdeNativeFoxLayoutFixedMap[] = {
	FXMAPFUNC( SEL_IGDE_CHILD_LAYOUT_FLAGS, 0, igdeNativeFoxLayoutFixed::onChildLayoutFlags )
};

FXIMPLEMENT( igdeNativeFoxLayoutFixed, FXPacker,
	igdeNativeFoxLayoutFixedMap, ARRAYNUMBER( igdeNativeFoxLayoutFixedMap ) )

igdeNativeFoxLayoutFixed::igdeNativeFoxLayoutFixed(){ }

igdeNativeFoxLayoutFixed::igdeNativeFoxLayoutFixed( igdeContainerFixed &owner,
	FXComposite *parent, int layoutFlags ) :
FXPacker( parent, layoutFlags, 0, 0, 0, 0, 0, 0, 0, 0 ),
pOwner( &owner ){
}

igdeNativeFoxLayoutFixed::~igdeNativeFoxLayoutFixed(){
}

long igdeNativeFoxLayoutFixed::onChildLayoutFlags( FXObject *sender, FXSelector selector, void *data ){
	igdeUIFoxHelper::sChildLayoutFlags &clflags = *( ( igdeUIFoxHelper::sChildLayoutFlags* )data );
	clflags.flags = 0;
	clflags.canResizeHorizontal = true;
	clflags.canResizeVertical = true;
	return 1;
}



// Class igdeContainerFixed
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerFixed::igdeContainerFixed( igdeEnvironment &environment ) :
igdeContainer( environment ){
}

igdeContainerFixed::~igdeContainerFixed(){
}



// Management
///////////////

void igdeContainerFixed::CreateNativeWidget(){
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
	
	FXPacker * const nativeWidget = new igdeNativeFoxLayoutFixed( *this, nativeParent,
		igdeUIFoxHelper::GetChildLayoutFlags( this ) );
	
	SetNativeWidget( nativeWidget );
	if( nativeParent->id() ){
		nativeWidget->create();
	}
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerFixed::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	FXPacker * const native = ( FXPacker* )GetNativeWidget();
	DropNativeWidget();
	delete native;
}
