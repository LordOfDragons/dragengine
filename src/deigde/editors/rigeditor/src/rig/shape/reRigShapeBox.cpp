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
#include <string.h>
#include <stdio.h>

#include "reRigShapeBox.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/exceptions.h>



// Class reRigShapeBox
////////////////////////

// Constructor, destructor
////////////////////////////

reRigShapeBox::reRigShapeBox( deEngine *engine ) : reRigShape( engine, estBox ){
	pHalfExtends.Set( 0.5f, 0.5f, 0.5f );
}

reRigShapeBox::~reRigShapeBox(){
}



// Management
///////////////

void reRigShapeBox::SetHalfExtends( const decVector &halfExtends ){
	if( ! halfExtends.IsEqualTo( pHalfExtends ) ){
		pHalfExtends = halfExtends;
		NotifyShapeChanged();
	}
}

reRigShape *reRigShapeBox::Duplicate() const{
	reRigShapeBox *shape = NULL;
	
	try{
		shape = new reRigShapeBox( GetEngine() );
		if( ! shape ) DETHROW( deeOutOfMemory );
		
		shape->SetPosition( GetPosition() );
		shape->SetOrientation( GetOrientation() );
		shape->SetHalfExtends( GetHalfExtends() );
		
	}catch( const deException & ){
		if( shape ) shape->FreeReference();
		throw;
	}
	
	return shape;
}

void reRigShapeBox::Scale( float scale ){
	SetPosition( GetPosition() * scale );
	pHalfExtends *= scale;
}

decShape *reRigShapeBox::CreateShape(){
	return new decShapeBox( pHalfExtends, GetPosition(), decQuaternion::CreateFromEuler( GetOrientation() * DEG2RAD ) );
}
