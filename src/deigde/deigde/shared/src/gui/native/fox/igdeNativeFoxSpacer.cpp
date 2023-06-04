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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "igdeNativeFoxSpacer.h"
#include "../../igdeContainer.h"
#include "../../igdeSpacer.h"

#include <dragengine/common/exceptions.h>



// Event map
//////////////

FXIMPLEMENT( igdeNativeFoxSpacer, FXFrame, nullptr, 0 )



// Class igdeNativeFoxSpacer
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxSpacer::igdeNativeFoxSpacer(){
}

igdeNativeFoxSpacer::igdeNativeFoxSpacer( igdeSpacer &powner, FXComposite *pparent, int childFlags ) :
FXFrame( pparent, LayoutFlags( childFlags ), 0, 0, powner.GetSize().x, powner.GetSize().y ),
pOwner( &powner ),
pWidth( width ),
pHeight( height ){
}

igdeNativeFoxSpacer::~igdeNativeFoxSpacer(){
	pOwner = NULL;
}

igdeNativeFoxSpacer *igdeNativeFoxSpacer::CreateNativeWidget( igdeSpacer &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxSpacer( powner, pparent, igdeUIFoxHelper::GetChildLayoutFlags( &powner ) );
}

void igdeNativeFoxSpacer::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxSpacer::DestroyNativeWidget(){
	delete this;
}



// Management
///////////////

void igdeNativeFoxSpacer::SetSize( int wwidth, int hheight ){
	pWidth = wwidth;
	pHeight = hheight;
	resize( wwidth, hheight );
}

int igdeNativeFoxSpacer::getDefaultWidth(){
	return pWidth;
}

int igdeNativeFoxSpacer::getDefaultHeight(){
	return pHeight;
}

int igdeNativeFoxSpacer::LayoutFlags( int childFlags ){
	childFlags &= ~( LAYOUT_FILL_COLUMN | LAYOUT_FILL_ROW );
	childFlags &= ~LAYOUT_FILL;
	childFlags |= LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT;
	return childFlags;
}

#endif
