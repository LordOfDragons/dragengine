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

#include "reUSetShapeBoxHalfExtends.h"
#include "../../../rig/shape/reRigShapeBox.h"
#include "dragengine/common/exceptions.h"



// Class reUSetShapeBoxHalfExtends
////////////////////////////////////

// Constructor, destructor
////////////////////////////

reUSetShapeBoxHalfExtends::reUSetShapeBoxHalfExtends( reRigShapeBox *shape, const decVector &halfExtends ){
	if( ! shape ) DETHROW( deeInvalidParam );
	
	pShape = shape;
	
	pOldHalfExtends = shape->GetHalfExtends();
	pNewHalfExtends = halfExtends;
	
	SetShortInfo( "Set Box Shape HalfExtends" );
	
	pShape->AddReference();
}

reUSetShapeBoxHalfExtends::~reUSetShapeBoxHalfExtends(){
	pShape->FreeReference();
}



// Management
///////////////

void reUSetShapeBoxHalfExtends::Undo(){
	pShape->SetHalfExtends( pOldHalfExtends );
}

void reUSetShapeBoxHalfExtends::Redo(){
	pShape->SetHalfExtends( pNewHalfExtends );
}
