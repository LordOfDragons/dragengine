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

#include "reRigShapeCapsule.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/exceptions.h>



// Class reRigShapeCapsule
////////////////////////

// Constructor, destructor
////////////////////////////

reRigShapeCapsule::reRigShapeCapsule( deEngine *engine ) : reRigShape( engine, estCapsule ){
	pTopRadius = 0.5f;
	pBottomRadius = 0.5f;
	pHalfHeight = 0.5f;
}

reRigShapeCapsule::~reRigShapeCapsule(){
}



// Management
///////////////

void reRigShapeCapsule::SetHalfHeight( float halfHeight ){
	if( fabs( halfHeight - pHalfHeight ) > 1e-5f ){
		pHalfHeight = halfHeight;
		NotifyShapeChanged();
	}
}

void reRigShapeCapsule::SetTopRadius( float topRadius ){
	if( fabs( topRadius - pTopRadius ) > 1e-5f ){
		pTopRadius = topRadius;
		NotifyShapeChanged();
	}
}

void reRigShapeCapsule::SetBottomRadius( float bottomRadius ){
	if( fabs( bottomRadius - pBottomRadius ) > 1e-5f ){
		pBottomRadius = bottomRadius;
		NotifyShapeChanged();
	}
}

reRigShape *reRigShapeCapsule::Duplicate() const{
	reRigShapeCapsule *shape = NULL;
	
	try{
		shape = new reRigShapeCapsule( GetEngine() );
		if( ! shape ) DETHROW( deeOutOfMemory );
		
		shape->SetPosition( GetPosition() );
		shape->SetOrientation( GetOrientation() );
		shape->SetHalfHeight( GetHalfHeight() );
		shape->SetTopRadius( GetTopRadius() );
		shape->SetBottomRadius( GetBottomRadius() );
		
	}catch( const deException & ){
		if( shape ) shape->FreeReference();
		throw;
	}
	
	return shape;
}

void reRigShapeCapsule::Scale( float scale ){
	SetPosition( GetPosition() * scale );
	pHalfHeight *= scale;
	pTopRadius *= scale;
	pBottomRadius *= scale;
}

decShape *reRigShapeCapsule::CreateShape(){
	return new decShapeCapsule( pHalfHeight, pTopRadius, pBottomRadius,
		GetPosition(), decQuaternion::CreateFromEuler( GetOrientation() * DEG2RAD ) );
}
