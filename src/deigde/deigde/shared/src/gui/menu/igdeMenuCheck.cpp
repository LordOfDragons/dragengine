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

#include "igdeMenuCheck.h"
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


// Class igdeMenuCheck
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeMenuCheck::igdeMenuCheck( igdeEnvironment &environment ) :
igdeMenuCommand( environment ),
pChecked( false ){
}

igdeMenuCheck::igdeMenuCheck( igdeEnvironment &environment, igdeAction *action ) :
igdeMenuCommand( environment, action ),
pChecked( false ){
}

igdeMenuCheck::~igdeMenuCheck(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeMenuCheck::SetChecked( bool checked ){
	if( pChecked == checked ){
		return;
	}
	
	pChecked = checked;
	OnCheckedChanged();
}



void igdeMenuCheck::OnParameterChanged( igdeAction *action ){
	igdeMenuCommand::OnParameterChanged( action );
	
	SetChecked( action->GetSelected() );
}



void igdeMenuCheck::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeMenuCheck * const native = igdeNativeMenuCheck::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
}

void igdeMenuCheck::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeMenuCheck* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}



void igdeMenuCheck::OnCheckedChanged(){
	if( GetNativeWidget() ){
		( ( igdeNativeMenuCheck* )GetNativeWidget() )->UpdateChecked();
	}
}
