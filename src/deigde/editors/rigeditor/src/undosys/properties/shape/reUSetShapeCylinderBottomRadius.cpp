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

#include "reUSetShapeCylinderBottomRadius.h"
#include "../../../rig/shape/reRigShapeCylinder.h"
#include "dragengine/common/exceptions.h"



// Class reUSetShapeCylinderBottomRadius
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

reUSetShapeCylinderBottomRadius::reUSetShapeCylinderBottomRadius( reRigShapeCylinder *shape, float bottomRadius ){
	if( ! shape ) DETHROW( deeInvalidParam );
	
	pShape = shape;
	
	pOldBottomRadius = shape->GetBottomRadius();
	pNewBottomRadius = bottomRadius;
	
	SetShortInfo( "Set Cylinder Shape Half Height" );
	
	pShape->AddReference();
}

reUSetShapeCylinderBottomRadius::~reUSetShapeCylinderBottomRadius(){
	pShape->FreeReference();
}



// Management
///////////////

void reUSetShapeCylinderBottomRadius::Undo(){
	pShape->SetBottomRadius( pOldBottomRadius );
}

void reUSetShapeCylinderBottomRadius::Redo(){
	pShape->SetBottomRadius( pNewBottomRadius );
}
