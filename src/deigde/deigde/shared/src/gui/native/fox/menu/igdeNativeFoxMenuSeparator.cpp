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

#include "igdeNativeFoxMenuSeparator.h"
#include "../../../igdeContainer.h"
#include "../../../menu/igdeMenuSeparator.h"

#include <dragengine/common/exceptions.h>



// Class igdeNativeFoxMenuSeparator
/////////////////////////////////////

FXIMPLEMENT( igdeNativeFoxMenuSeparator, FXMenuSeparator, nullptr, 0 )

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuSeparator::igdeNativeFoxMenuSeparator(){ }

igdeNativeFoxMenuSeparator::igdeNativeFoxMenuSeparator( igdeMenuSeparator &powner, FXComposite *pparent ) :
FXMenuSeparator( pparent ),
pOwner( &powner ){
}

igdeNativeFoxMenuSeparator::~igdeNativeFoxMenuSeparator(){
}

igdeNativeFoxMenuSeparator *igdeNativeFoxMenuSeparator::CreateNativeWidget( igdeMenuSeparator &powner ){
	if( ! powner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const pparent = ( FXComposite* ) powner.GetParent()->GetNativeContainer();
	if( ! pparent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxMenuSeparator( powner, pparent );
}

void igdeNativeFoxMenuSeparator::PostCreateNativeWidget(){
	FXComposite &pparent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( pparent.id() ){
		create();
	}
}

void igdeNativeFoxMenuSeparator::DestroyNativeWidget(){
	delete this;
}

#endif
