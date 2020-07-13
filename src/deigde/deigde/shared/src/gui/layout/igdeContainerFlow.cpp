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

#include "igdeContainerFlow.h"
#include "../igdeWidget.h"
#include "../native/toolkit.h"

#include <dragengine/common/exceptions.h>


// Class igdeContainerFlow
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeContainerFlow::igdeContainerFlow( igdeEnvironment &environment, eAxis axis,
	eStretching stretching, int spacing ) :
igdeContainer( environment ),
pAxis( axis ),
pStretching( stretching ),
pSpacing( spacing )
{
	if( spacing < 0 ){
		DETHROW( deeInvalidParam );
	}
}

igdeContainerFlow::~igdeContainerFlow(){
}



// Management
///////////////

void igdeContainerFlow::RemoveChild( igdeWidget *child ){
	igdeContainer::RemoveChild( child );
	
	if( GetNativeWidget() ){
		igdeNativeContainerFlow::ChildRemoved( *this, GetNativeWidget() );
	}
}



void igdeContainerFlow::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	void * const native = igdeNativeContainerFlow::CreateNativeWidget( *this );
	SetNativeWidget( native );
	igdeNativeContainerFlow::PostCreateNativeWidget( *this, native );
	
	CreateChildWidgetNativeWidgets();
}

void igdeContainerFlow::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	void * const native = GetNativeWidget();
	DropNativeWidget();
	igdeNativeContainerFlow::DestroyNativeWidget( *this, native );
}

void *igdeContainerFlow::GetNativeContainer() const{
	if( ! GetNativeWidget() ){
		return NULL;
	}
	return igdeNativeContainerFlow::GetNativeContainer( *this, GetNativeWidget() );
}
