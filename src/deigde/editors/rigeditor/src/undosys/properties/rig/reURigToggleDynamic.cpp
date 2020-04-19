/* 
 * Drag[en]gine IGDE Rig Editor
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

#include <stdlib.h>

#include "reURigToggleDynamic.h"

#include "../../../rig/reRig.h"
#include "../../../rig/bone/reRigBone.h"

#include <dragengine/common/exceptions.h>



// Class reURigToggleDynamic
//////////////////////////////

// Constructor, destructor
////////////////////////////

reURigToggleDynamic::reURigToggleDynamic( reRig *rig ){
	if( ! rig ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Toggle Rig Dynamic" );
	
	pRig = rig;
	pRig->AddReference();
}

reURigToggleDynamic::~reURigToggleDynamic(){
	if( pRig ) pRig->FreeReference();
}



// Management
///////////////

void reURigToggleDynamic::Undo(){
	pRig->SetDynamic( ! pRig->GetDynamic() );
}

void reURigToggleDynamic::Redo(){
	pRig->SetDynamic( ! pRig->GetDynamic() );
}
