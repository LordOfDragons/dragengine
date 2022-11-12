/* 
 * Drag[en]gine Game Engine
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

#include "deColliderCollisionTest.h"
#include "deCollider.h"
#include "deCollisionInfo.h"
#include "../sensor/deTouchSensor.h"
#include "../component/deComponent.h"
#include "../../common/exceptions.h"




// Class deColliderCollisionTest
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deColliderCollisionTest::deColliderCollisionTest() :
pLocalDirection( true ),
pEnabled( true ){
}

deColliderCollisionTest::deColliderCollisionTest( const deColliderCollisionTest &collisionTest ) :
pTouchSensor( collisionTest.pTouchSensor ),
pCollisionFilter( collisionTest.pCollisionFilter ),
pComponent( collisionTest.pComponent ),
pBone( collisionTest.pBone ),
pOrigin( collisionTest.pOrigin ),
pDirection( collisionTest.pDirection ),
pLocalDirection( collisionTest.pLocalDirection ),
pEnabled( collisionTest.pEnabled ){
}

deColliderCollisionTest::~deColliderCollisionTest(){
}



// Management
///////////////

void deColliderCollisionTest::SetTouchSensor( deTouchSensor *touchSensor ){
	pTouchSensor = touchSensor;
}



void deColliderCollisionTest::SetCollider( deCollider *collider ){
	if( collider == pCollider ){
		return;
	}
	
	pCollider = collider;
	
	if( collider ){
		collider->SetCollisionFilter( pCollisionFilter );
	}
}

void deColliderCollisionTest::SetCollisionFilter( const decCollisionFilter &filter ){
	if( filter == pCollisionFilter ){
		return;
	}
	
	pCollisionFilter = filter;
	
	if( pCollider ){
		pCollider->SetCollisionFilter( filter );
	}
}



void deColliderCollisionTest::SetComponent( deComponent *component ){
	pComponent = component;
}

void deColliderCollisionTest::SetBone( const char *name ){
	pBone = name;
}

void deColliderCollisionTest::SetOrigin( const decVector &origin ){
	pOrigin = origin;
}

void deColliderCollisionTest::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void deColliderCollisionTest::SetDirection( const decVector &direction ){
	pDirection = direction;
}

void deColliderCollisionTest::SetLocalDirection( bool localDirection ){
	pLocalDirection = localDirection;
}

void deColliderCollisionTest::SetEnabled( bool enabled ){
	pEnabled = enabled;
}



int deColliderCollisionTest::GetCollisionInfoCount() const{
	return pCollisionInfo.GetCount();
}

deCollisionInfo *deColliderCollisionTest::GetCollisionInfoAt( int index ) const{
	return ( deCollisionInfo* )pCollisionInfo.GetAt( index );
}

void deColliderCollisionTest::AddCollisionInfo( deCollisionInfo *info ){
	pCollisionInfo.Add( info );
}

void deColliderCollisionTest::RemoveAllCollisionInfo(){
	pCollisionInfo.RemoveAll();
}



void deColliderCollisionTest::SetTestOrigin( const decDVector &testOrigin ){
	pTestOrigin = testOrigin;
}

void deColliderCollisionTest::SetTestOrientation( const decQuaternion &orientation ){
	pTestOrientation = orientation;
}

void deColliderCollisionTest::SetTestDirection( const decDVector &testDirection ){
	pTestDirection = testDirection;
}



// Operators
//////////////

deColliderCollisionTest &deColliderCollisionTest::operator=( const deColliderCollisionTest &collisionTest ){
	pTouchSensor = collisionTest.pTouchSensor;
	pCollisionFilter = collisionTest.pCollisionFilter;
	pComponent = collisionTest.pComponent;
	pLocalDirection = collisionTest.pLocalDirection;
	pEnabled = collisionTest.pEnabled;
	pBone = collisionTest.pBone;
	pOrigin = collisionTest.pOrigin;
	pDirection = collisionTest.pDirection;
	
	SetCollider( collisionTest.pCollider );
	return *this;
}
