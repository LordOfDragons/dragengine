/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpTouchSensor.h"
#include "debpTSConvexResultCallback.h"
#include "../debpCollisionObject.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../terrain/heightmap/debpHeightTerrain.h"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>



// Class debpTSConvexResultCallback
/////////////////////////////////////

// Constructors
/////////////////

debpTSConvexResultCallback::debpTSConvexResultCallback(
deCollisionInfo *colinfo, const debpTouchSensor &touchSensor ) :
debpConvexResultCallback( colinfo ),
pTouchSensor( touchSensor ){
}



// Bullet
///////////

bool debpTSConvexResultCallback::needsCollision( btBroadphaseProxy *proxy0 ) const{
	const btCollisionObject &collisionObject = *( ( btCollisionObject* )proxy0->m_clientObject );
	const debpCollisionObject &colObj = *( ( debpCollisionObject* )collisionObject.getUserPointer() );
	
	if( colObj.IsOwnerCollider() ){
		if( pTouchSensor.CollidesNot( *colObj.GetOwnerCollider() ) ){
			return false;
		}
		
	}else if( colObj.IsOwnerHTSector() ){
		if( pTouchSensor.GetTouchSensor().GetCollisionFilter().CollidesNot(
		colObj.GetOwnerHTSector()->GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter() ) ){
			return false;
		}
	}
	
	return debpConvexResultCallback::needsCollision( proxy0 );
}
