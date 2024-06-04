/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
