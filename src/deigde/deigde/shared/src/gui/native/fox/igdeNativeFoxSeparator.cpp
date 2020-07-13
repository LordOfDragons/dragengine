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

#include "igdeNativeFoxSeparator.h"
#include "foxIcons.h"
#include "../../igdeSeparator.h"
#include "../../igdeContainer.h"
#include "../../../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeNativeFoxSeparator
/////////////////////////////////

void *igdeNativeFoxSeparator::CreateNativeWidget( igdeSeparator &owner ){
	if( ! owner.GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const parent = ( FXComposite* )owner.GetParent()->GetNativeContainer();
	if( ! parent ){
		DETHROW( deeInvalidParam );
	}
	
	const int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( &owner ) | SEPARATOR_GROOVE;
	
	switch( owner.GetOrientation() ){
	case igdeSeparator::eoHorizontal:
		return new FXHorizontalSeparator( parent, layoutFlags );
		
	case igdeSeparator::eoVertical:
		return new FXVerticalSeparator( parent, layoutFlags );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void igdeNativeFoxSeparator::PostCreateNativeWidget( igdeSeparator &owner, void *native ){
	FXComposite &parent = *( ( FXComposite* )owner.GetParent()->GetNativeContainer() );
	if( parent.id() ){
		( ( FXSeparator* )native )->create();
	}
}

void igdeNativeFoxSeparator::DestroyNativeWidget( igdeSeparator &owner, void *native ){
	delete ( FXSeparator* )native;
}
