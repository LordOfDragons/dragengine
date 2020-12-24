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

#include "gdeUOCRemoveAllInherits.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/inherit/gdeOCInherit.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCRemoveAllInherits
//////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCRemoveAllInherits::gdeUOCRemoveAllInherits( gdeObjectClass *objectClass ) :
pObjectClass( NULL )
{
	if( ! objectClass || objectClass->GetInherits().GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove all inherits" );
	
	pInherits = objectClass->GetInherits();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCRemoveAllInherits::~gdeUOCRemoveAllInherits(){
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCRemoveAllInherits::Undo(){
	const int count = pInherits.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pObjectClass->GetInherits().Add( pInherits.GetAt( i ) );
	}
	pObjectClass->NotifyInheritsChanged();
}

void gdeUOCRemoveAllInherits::Redo(){
	pObjectClass->GetInherits().RemoveAll();
	pObjectClass->NotifyInheritsChanged();
}
