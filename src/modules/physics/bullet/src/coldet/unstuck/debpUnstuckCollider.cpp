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
#include <string.h>

#include "debpUnstuckCollider.h"
#include "../../collider/debpCollider.h"
#include "../../world/debpWorld.h"

#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Class debpUnstuckCollider
//////////////////////////////

// Constructor, destructor
////////////////////////////

debpUnstuckCollider::debpUnstuckCollider(debpWorld &world) :
pWorld(world),

pStuckCollider(NULL),

pNearbyColliders(NULL),
pNearbyColliderCount(0),
pNearbyColliderSize(0),

pBlockerNormals(NULL),
pBlockerNormalCount(0),
pBlockerNormalSize(0)
{
	(void)pWorld; // will be used in the future
}

debpUnstuckCollider::~debpUnstuckCollider(){
	if(pBlockerNormals){
		delete [] pBlockerNormals;
	}
	if(pNearbyColliders){
		delete [] pNearbyColliders;
	}
}



// Management
///////////////

void debpUnstuckCollider::SetStuckCollider(debpCollider *collider){
	pStuckCollider = collider;
}

void debpUnstuckCollider::SetDisplacement(const decDVector &displacement){
	pDisplacement = displacement;
}

void debpUnstuckCollider::Reset(){
	pStuckCollider = NULL;
	pNearbyColliderCount = 0;
	pBlockerNormalCount = 0;
}

void debpUnstuckCollider::FindNearbyColliders(){
	pNearbyColliderCount = 0;
	
	// TODO visit world to find colliders
}

void debpUnstuckCollider::FindBlockerNormals(){
	pBlockerNormalCount = 0;
	
	// TODO test nearby colliders for collision with the stuck collider and
	// store the normals for them
}

bool debpUnstuckCollider::UnstuckCollider(){
	return true;
}



debpCollider *debpUnstuckCollider::GetNearbyColliderAt(int index) const{
	if(index < 0 || index >= pNearbyColliderCount) DETHROW(deeInvalidParam);
	
	return pNearbyColliders[index];
}

void debpUnstuckCollider::AddNearbyCollider(debpCollider *collider){
	if(!collider) DETHROW(deeInvalidParam);
	
	if(pNearbyColliderCount == pNearbyColliderSize){
		debpCollider **newArray = new debpCollider*[pNearbyColliderSize + 1];
		if(pNearbyColliders){
			memcpy(newArray, pNearbyColliders, sizeof(debpCollider*) * pNearbyColliderSize);
			delete [] pNearbyColliders;
		}
		pNearbyColliders = newArray;
		pNearbyColliderSize++;
	}
	
	pNearbyColliders[pNearbyColliderCount++] = collider;
}

void debpUnstuckCollider::RemoveAllNearbyColliders(){
	pNearbyColliderCount = 0;
}



const decDVector& debpUnstuckCollider::GetBlockerNormalAt(int index) const{
	if(index < 0 || index >= pBlockerNormalCount) DETHROW(deeInvalidParam);
	
	return pBlockerNormals[index];
}

void debpUnstuckCollider::AddBlockerNormal(const decDVector &normal){
	if(pBlockerNormalCount == pBlockerNormalSize){
		decDVector *newArray = new decDVector[pBlockerNormalSize + 1];
		if(pBlockerNormals){
			memcpy(newArray, pBlockerNormals, sizeof(decDVector) * pBlockerNormalSize);
			delete [] pBlockerNormals;
		}
		pBlockerNormals = newArray;
		pBlockerNormalSize++;
	}
	
	pBlockerNormals[pBlockerNormalCount++] = normal;
}

void debpUnstuckCollider::RemoveAllBlockerNormals(){
	pBlockerNormalCount = 0;
}
