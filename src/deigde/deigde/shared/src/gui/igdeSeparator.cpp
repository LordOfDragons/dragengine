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

#include "igdeSeparator.h"
#include "igdeContainer.h"
#include "native/toolkit.h"

#include <dragengine/common/exceptions.h>



// Class igdeSeparator
////////////////////////

// Constructor, destructor
////////////////////////////

igdeSeparator::igdeSeparator( igdeEnvironment &environment, eOrientation orientation ) :
igdeWidget( environment ),
pOrientation( orientation ){
}

igdeSeparator::~igdeSeparator(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeSeparator::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! foxParent ){
		DETHROW( deeInvalidParam );
	}
	
	int layoutFlags = igdeUIFoxHelper::GetChildLayoutFlags( this );
	FXWindow *foxWidget = NULL;
	
	switch( pOrientation ){
	case eoHorizontal:
		foxWidget = new FXHorizontalSeparator( foxParent, SEPARATOR_GROOVE | layoutFlags );
		break;
		
	case eoVertical:
		foxWidget = new FXVerticalSeparator( foxParent, SEPARATOR_GROOVE | layoutFlags );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	SetNativeWidget( foxWidget );
	if( foxParent->id() ){
		foxWidget->create();
	}
}

void igdeSeparator::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( FXWindow* )GetNativeWidget();
	DropNativeWidget();
}
