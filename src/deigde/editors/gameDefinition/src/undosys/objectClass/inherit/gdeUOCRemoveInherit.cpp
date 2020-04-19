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

#include "gdeUOCRemoveInherit.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/inherit/gdeOCInherit.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCRemoveInherit
///////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCRemoveInherit::gdeUOCRemoveInherit( gdeObjectClass *objectClass, gdeOCInherit *inherit ) :
pObjectClass( NULL ),
pInherit( NULL )
{
	if( ! objectClass || ! inherit ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! objectClass->GetInherits().Has( inherit ) ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove inherit" );
	
	pInherit = inherit;
	inherit->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCRemoveInherit::~gdeUOCRemoveInherit(){
	if( pInherit ){
		pInherit->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCRemoveInherit::Undo(){
	pObjectClass->GetInherits().Add( pInherit );
	pObjectClass->NotifyInheritsChanged();
}

void gdeUOCRemoveInherit::Redo(){
	pObjectClass->GetInherits().Remove( pInherit );
	pObjectClass->NotifyInheritsChanged();
}
