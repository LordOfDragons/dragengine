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

pStuckCollider(nullptr)
{
	(void)pWorld; // will be used in the future
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
	pStuckCollider = nullptr;
	pNearbyColliders.RemoveAll();
	pBlockerNormals.RemoveAll();
}

void debpUnstuckCollider::FindNearbyColliders(){
	pNearbyColliders.RemoveAll();
	
	// TODO visit world to find colliders
}

void debpUnstuckCollider::FindBlockerNormals(){
	pBlockerNormals.RemoveAll();
	
	// TODO test nearby colliders for collision with the stuck collider and
	// store the normals for them
}

bool debpUnstuckCollider::UnstuckCollider(){
	return true;
}



debpCollider *debpUnstuckCollider::GetNearbyColliderAt(int index) const{
	return pNearbyColliders.GetAt(index);
}

void debpUnstuckCollider::AddNearbyCollider(debpCollider *collider){
	DEASSERT_NOTNULL(collider)
	
	pNearbyColliders.Add(collider);
}

void debpUnstuckCollider::RemoveAllNearbyColliders(){
	pNearbyColliders.RemoveAll();
}



const decDVector& debpUnstuckCollider::GetBlockerNormalAt(int index) const{
	return pBlockerNormals.GetAt(index);
}

void debpUnstuckCollider::AddBlockerNormal(const decDVector &normal){
	pBlockerNormals.Add(normal);
}

void debpUnstuckCollider::RemoveAllBlockerNormals(){
	pBlockerNormals.RemoveAll();
}
