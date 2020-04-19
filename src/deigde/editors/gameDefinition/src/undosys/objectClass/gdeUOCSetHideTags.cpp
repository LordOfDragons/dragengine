/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeUOCSetHideTags.h"
#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCSetHideTags
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCSetHideTags::gdeUOCSetHideTags(
gdeObjectClass *objectClass, const decStringSet &newValue ) :
pObjectClass( NULL )
{
	if( ! objectClass ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Object class set hide tags" );
	
	pOldValue = objectClass->GetHideTags();
	pNewValue = newValue;
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCSetHideTags::~gdeUOCSetHideTags(){
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCSetHideTags::Undo(){
	pObjectClass->SetHideTags( pOldValue );
	pObjectClass->GetGameDefinition()->UpdateUsedTagsObjectClass();
}

void gdeUOCSetHideTags::Redo(){
	pObjectClass->SetHideTags( pNewValue );
	pObjectClass->GetGameDefinition()->UpdateUsedTagsObjectClass();
}
