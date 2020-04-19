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

#include "debpSharedCollisionFiltering.h"
#include "../debpCollisionObject.h"
#include "../debpMotionState.h"
#include "../terrain/heightmap/debpHeightTerrain.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../collider/debpCollider.h"
#include "../collider/debpCollider.h"
#include "../collider/debpColliderVolume.h"
#include "../collider/debpColliderComponent.h"
#include "../collider/debpColliderRig.h"
#include "../collider/debpColliderBone.h"
#include "../collider/debpColliderBones.h"
#include "../touchsensor/debpTouchSensor.h"

#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderVisitorIdentify.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/resources/sensor/deTouchSensor.h>



// Class debpSharedCollisionFiltering
///////////////////////////////////////

// Constructor, Destructor
////////////////////////////

debpSharedCollisionFiltering::debpSharedCollisionFiltering( debpWorld &world ) :
pWorld( world ){
}

debpSharedCollisionFiltering::~debpSharedCollisionFiltering(){
}



// Management
///////////////

bool debpSharedCollisionFiltering::CanBodyHitBody( const btCollisionObject &body0,
const btCollisionObject &body1 ) const{
	// ghost objects can never collider. for some reason bullet can let this one slip through
	if( body0.getInternalType() == btCollisionObject::CO_GHOST_OBJECT
	&& body1.getInternalType() == btCollisionObject::CO_GHOST_OBJECT ){
		return false;
	}
	
	// check collision between collision objects
	return CanCObjHitCObj(
		*( ( debpCollisionObject* )body0.getUserPointer() ),
		*( ( debpCollisionObject* )body1.getUserPointer() ) );
}


bool debpSharedCollisionFiltering::CanCObjHitCObj( const debpCollisionObject &colObj0,
const debpCollisionObject &colObj1 ) const{
	if( colObj0.IsOwnerCollider() ){
		return CanColliderHitCObj( colObj0, *colObj0.GetOwnerCollider(), colObj1 );
		
	}else if( colObj0.IsOwnerHTSector() ){
		return CanHTerrainHitCObj( *colObj0.GetOwnerHTSector(), colObj1 );
		
	}else if( colObj0.IsOwnerTouchSensor() ){
		return CanTSensorHitCObj( *colObj0.GetOwnerTouchSensor(), colObj1 );
	}
	
	return false;
}


bool debpSharedCollisionFiltering::CanColliderHitCObj( const debpCollisionObject &colObj0,
const debpCollider &collider0, const debpCollisionObject &colObj1 ) const{
	// collider versus collider
	if( colObj1.IsOwnerCollider() ){
		return CanColliderHitCollider( colObj0, collider0, colObj1, *colObj1.GetOwnerCollider() );
		
	// collider versus height terrain sector
	}else if( colObj1.IsOwnerHTSector() ){
		const deHeightTerrainSector &htsector = *colObj1.GetOwnerHTSector()->GetSector();
		
		// no collision with height terrains without height image or textures
		if( ! htsector.GetHeightImage() || htsector.GetTextureCount() == 0 ){
			return false;
		}
		
		return collider0.GetCollider().GetCollisionFilter().Collides(
			colObj1.GetOwnerHTSector()->GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter() );
		
	// collider versus touch sensor shape
	}else if( colObj1.IsOwnerTouchSensor() ){
		return colObj1.GetOwnerTouchSensor()->Collides( collider0 );
	}
	
	// everything else is no collision
	return false;
}


bool debpSharedCollisionFiltering::CanHTerrainHitCObj( const debpHTSector &htsector,
const debpCollisionObject &colObj1 ) const{
	const deHeightTerrainSector &engHTSector = *htsector.GetSector();
	
	// no collision with height terrains without height image or textures
	if( ! engHTSector.GetHeightImage() || engHTSector.GetTextureCount() == 0 ){
		return false;
	}
	
	const deHeightTerrain &engHTerrain = *htsector.GetHeightTerrain()->GetHeightTerrain();
	const decCollisionFilter &cfHT = engHTerrain.GetCollisionFilter();
	
	// height terrain sector versus collider
	if( colObj1.IsOwnerCollider() ){
		return cfHT.Collides( colObj1.GetOwnerCollider()->GetCollider().GetCollisionFilter() );
		
	// height terrain versus height terrain sector is no collision
	//}else if( colObj2.IsOwnerHTSector() ){
		
	// height terrain versus touch sensor
	}else if( colObj1.IsOwnerTouchSensor() ){
		const deTouchSensor &touchSensor = colObj1.GetOwnerTouchSensor()->GetTouchSensor();
		return cfHT.Collides( touchSensor.GetCollisionFilter() );
	}
	
	// everything else is no collision
	return false;
}


bool debpSharedCollisionFiltering::CanTSensorHitCObj( const debpTouchSensor &touchSensor,
const debpCollisionObject &colObj1 ) const{
	// touch sensor shape versus collider
	if( colObj1.IsOwnerCollider() ){
		return touchSensor.Collides( *colObj1.GetOwnerCollider() );
		
	// touch sensor shape versus height terrain sector
	}else if( colObj1.IsOwnerHTSector() ){
		// no collision with height terrains without height image or textures
		const deHeightTerrainSector &htsector = *colObj1.GetOwnerHTSector()->GetSector();
		if( ! htsector.GetHeightImage() || htsector.GetTextureCount() == 0 ){
			return false;
		}
		
		const deHeightTerrain &heightTerrain = *colObj1.GetOwnerHTSector()->GetHeightTerrain()->GetHeightTerrain();
		const decCollisionFilter &cfTS = touchSensor.GetTouchSensor().GetCollisionFilter();
		return cfTS.Collides( heightTerrain.GetCollisionFilter() );
		
	// touch sensor shape versus touch sensor shape is no collision
	//}else if( colObj2.IsOwnerTouchSensor() ){
	}
	
	// everything else is no collision
	return false;
}


bool debpSharedCollisionFiltering::CanColliderHitCollider(
const debpCollisionObject &colObj0, const debpCollider &collider0,
const debpCollisionObject &colObj1, const debpCollider &collider1 ) const{
	const debpCollider * const bpCollider1 = colObj1.GetOwnerCollider();
	
	// collider versus collider volume
	if( collider0.IsVolume() ){
		return collider0.Collides( *bpCollider1 );
		
	// collider versus collider component
	}else if( collider0.IsComponent() ){
		if( bpCollider1 == &collider0 ){
			const debpColliderBones * const bones = collider0.CastToComponent()->GetBones();
			return bones && bones->CanBonesCollide( colObj0.GetOwnerBone(), colObj1.GetOwnerBone() );
			
		}else{
			return collider0.Collides( *bpCollider1 );
		}
		
	// collider versus rigged collider
	}else if( collider0.IsRigged() ){
		if( bpCollider1 == &collider0 ){
			const debpColliderBones * const bones = bpCollider1->CastToRigged()->GetBones();
			if( bones ){
				return bones->CanBonesCollide( colObj0.GetOwnerBone(), colObj1.GetOwnerBone() );
			}
			
		}else{
			return collider0.Collides( *bpCollider1 );
		}
	}
	
	// no collision
	return false;
}
