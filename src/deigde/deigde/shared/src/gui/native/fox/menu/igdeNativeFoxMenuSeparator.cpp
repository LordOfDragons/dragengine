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

#include "igdeNativeFoxMenuSeparator.h"
#include "../../../igdeContainer.h"
#include "../../../menu/igdeMenuSeparator.h"

#include <dragengine/common/exceptions.h>



// Events
///////////

FXDEFMAP( igdeNativeFoxMenuSeparator ) igdeNativeFoxMenuSeparatorMap[] = { };



// Class igdeNativeFoxMenuSeparator
/////////////////////////////////////

FXIMPLEMENT( igdeNativeFoxMenuSeparator, FXMenuSeparator,
	igdeNativeFoxMenuSeparatorMap, ARRAYNUMBER( igdeNativeFoxMenuSeparatorMap ) )

// Constructor, destructor
////////////////////////////

igdeNativeFoxMenuSeparator::igdeNativeFoxMenuSeparator(){ }

igdeNativeFoxMenuSeparator::igdeNativeFoxMenuSeparator( igdeMenuSeparator &owner, FXComposite *parent ) :
FXMenuSeparator( parent ),
pOwner( &owner ){
}

igdeNativeFoxMenuSeparator::~igdeNativeFoxMenuSeparator(){
}

igdeNativeFoxMenuSeparator *igdeNativeFoxMenuSeparator::CreateNativeWidget( igdeMenuSeparator &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	return new igdeNativeFoxMenuSeparator( owner, parent );
}

void igdeNativeFoxMenuSeparator::PostCreateNativeWidget(){
	FXComposite &parent = *( ( FXComposite* )pOwner->GetParent()->GetNativeContainer() );
	if( parent.id() ){
		create();
	}
}

void igdeNativeFoxMenuSeparator::DestroyNativeWidget(){
	delete this;
}
