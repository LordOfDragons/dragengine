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

#include "igdeMenuOption.h"
#include "igdeMenuCascade.h"
#include "../igdeCommonDialogs.h"
#include "../event/igdeAction.h"
#include "../native/toolkit.h"
#include "../resources/igdeIcon.h"
#include "../../engine/igdeEngineController.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>


// Class igdeMenuOption
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuOption::igdeMenuOption( igdeEnvironment &environment ) :
igdeMenuCommand( environment ),
pSelected( false ){
}

igdeMenuOption::igdeMenuOption( igdeEnvironment &environment, igdeAction *action ) :
igdeMenuCommand( environment, action ),
pSelected( false ){
}

igdeMenuOption::~igdeMenuOption(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeMenuOption::SetSelected( bool selected ){
	if( pSelected == selected ){
		return;
	}
	
	pSelected = selected;
	OnSelectedChanged();
}



void igdeMenuOption::OnParameterChanged( igdeAction *action ){
	igdeMenuCommand::OnParameterChanged( action );
	
	SetSelected( action->GetSelected() );
}



void igdeMenuOption::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeMenuOption * const native = igdeNativeMenuOption::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeMenuOption::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeMenuOption* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeMenuOption::OnSelectedChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeMenuOption* )GetNativeWidget() )->UpdateSelected();
	}
}
