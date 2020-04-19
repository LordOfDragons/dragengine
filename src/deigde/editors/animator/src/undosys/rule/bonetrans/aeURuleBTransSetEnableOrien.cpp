/* 
 * Drag[en]gine IGDE Animator Editor
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
#include <string.h>
#include <stdlib.h>

#include "aeURuleBTransSetEnableOrien.h"
#include "../../../animator/rule/aeRuleBoneTransformator.h"

#include <dragengine/common/exceptions.h>



// Class aeURuleBTransSetEnableOrien
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeURuleBTransSetEnableOrien::aeURuleBTransSetEnableOrien( aeRuleBoneTransformator *rule ){
	if( ! rule ){
		DETHROW( deeInvalidParam );
	}
	
	pRule = NULL;
	
	SetShortInfo( "Bone transformator toggle enable orientation" );
	
	pRule = rule;
	pRule->AddReference();
}

aeURuleBTransSetEnableOrien::~aeURuleBTransSetEnableOrien(){
	if( pRule ){
		pRule->FreeReference();
	}
}



// Management
///////////////

void aeURuleBTransSetEnableOrien::Undo(){
	Redo();
}

void aeURuleBTransSetEnableOrien::Redo(){
	pRule->SetEnableOrientation( ! pRule->GetEnableOrientation() );
}
