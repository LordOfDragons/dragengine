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
#include <string.h>
#include <stdint.h>

#include "debpCollisionObject.h"

#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/CollisionDispatch/btCollisionObjectWrapper.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/NarrowPhaseCollision/btManifoldPoint.h"

#include <dragengine/common/exceptions.h>



// Class debpCollisionObject
//////////////////////////////

// Constructor, destructor
////////////////////////////

debpCollisionObject::debpCollisionObject(){
	pOwnerCollider = NULL;
	pOwnerBone = -1;
	pOwnerHTSector = NULL;
	pOwnerTouchSensor = NULL;
	pDirtyAABB = true;
}

debpCollisionObject::~debpCollisionObject(){
}



// Management
///////////////

void debpCollisionObject::SetOwnerCollider( debpCollider *collider, int bone ){
	pOwnerCollider = collider;
	pOwnerBone = bone;
	pOwnerHTSector = NULL;
	pOwnerTouchSensor = NULL;
}

void debpCollisionObject::SetOwnerHTSector( debpHTSector *htsector ){
	pOwnerCollider = NULL;
	pOwnerBone = -1;
	pOwnerHTSector = htsector;
	pOwnerTouchSensor = NULL;
}

void debpCollisionObject::SetOwnerTouchSensor( debpTouchSensor *touchSensor ){
	pOwnerCollider = NULL;
	pOwnerBone = -1;
	pOwnerHTSector = NULL;
	pOwnerTouchSensor = touchSensor;
}



bool debpCollisionObject::IsOwnerCollider() const{
	return pOwnerCollider != NULL;
}

bool debpCollisionObject::IsOwnerHTSector() const{
	return pOwnerHTSector != NULL;
}

bool debpCollisionObject::IsOwnerTouchSensor() const{
	return pOwnerTouchSensor != NULL;
}



void debpCollisionObject::SetDirtyAABB( bool dirty ){
	pDirtyAABB = dirty;
}



bool debpCollisionObject::CallbackAddContact( btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap,
int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1 ){
	const btCollisionObject &colObj0 = *colObj0Wrap->getCollisionObject();
	const btCollisionObject &colObj1 = *colObj1Wrap->getCollisionObject();
	
	CallbackAddContactFixParameters( cp, *colObj0Wrap, *colObj1Wrap );
	
	// for the time being average the two frictions. a proper table will be implementated later on
	const btScalar friction0 = colObj0.getFriction();
	const btScalar friction1 = colObj1.getFriction();
	const btScalar friction = ( friction0 + friction1 ) * 0.5f;
	const btScalar maxFriction = btScalar( 10.0f );
	
	if( friction < 0.0f ){
		cp.m_combinedFriction = ( btScalar )0.0f;
		
	}else if( friction > maxFriction ){
		cp.m_combinedFriction = maxFriction;
		
	}else{
		cp.m_combinedFriction = friction;
	}
	
	// same for rolling friction
	const btScalar rollingFriction0 = colObj0.getRollingFriction();
	const btScalar rollingFriction1 = colObj1.getRollingFriction();
	const btScalar rollingFriction = ( rollingFriction0 + rollingFriction1 ) * 0.5f;
	const btScalar maxRollingFriction = btScalar( 10.0f );
	
	if( rollingFriction < 0.0f ){
		cp.m_combinedRollingFriction = ( btScalar )0.0f;
		
	}else if( rollingFriction > maxRollingFriction ){
		cp.m_combinedRollingFriction = maxRollingFriction;
		
	}else{
		cp.m_combinedRollingFriction = rollingFriction;
	}
	
	// restitution is the sum of both restitution values
	const btScalar restitution0 = colObj0.getRestitution();
	const btScalar restitution1 = colObj1.getRestitution();
	const btScalar restitution = restitution0 + restitution1;
	const btScalar maxRestitution = btScalar( 1.0f );
	
	if( restitution < 0.0f ){
		cp.m_combinedRestitution = ( btScalar )0.0f;
		
	}else if( restitution > maxRestitution ){
		cp.m_combinedRestitution = maxRestitution;
		
	}else{
		cp.m_combinedRestitution = restitution;
	}
	
	// return value not used
	return true;
}

void debpCollisionObject::CallbackAddContactFixParameters( btManifoldPoint &cp,
const btCollisionObjectWrapper &colObj0Wrap, const btCollisionObjectWrapper &colObj1Wrap ){
	// the default value for partId and index is -1 if no additional information is available.
	// this case is kept since there we can not provide any useful information
	// 
	// the second case is for child shapes. these are set like this:
	//   m_resultOut->setShapeIdentifiersA(-1,childIndex0)
	// whereas the colObj0 is:
	//   (this->m_compoundColObjWrap,childShape,m_compoundColObjWrap->getCollisionObject(),newChildWorldTrans,-1,index);
	// 
	// the third case is for triangle shapes. these are set like this:
	//   m_resultOut->setShapeIdentifiersA(partId,triangleIndex)
	// partId is in this some value larger than -1
	// 
	// using these values the correct parameters can be calculated and the point updated
	
	// set shape index from user pointer
	cp.m_index0 = ( int )( intptr_t )colObj0Wrap.getCollisionShape()->getUserPointer() - 1;
	cp.m_index1 = ( int )( intptr_t )colObj1Wrap.getCollisionShape()->getUserPointer() - 1;
	
	// adjust triangle index from. there's no solution for this yet
}
