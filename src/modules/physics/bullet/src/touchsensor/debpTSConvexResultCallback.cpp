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
deCollisionInfo *colinfo, const debpTouchSensor &touchSensor) :
debpConvexResultCallback(colinfo),
pTouchSensor(touchSensor){
}



// Bullet
///////////

bool debpTSConvexResultCallback::needsCollision(btBroadphaseProxy *proxy0) const{
	const btCollisionObject &collisionObject = *((btCollisionObject*)proxy0->m_clientObject);
	const debpCollisionObject &colObj = *((debpCollisionObject*)collisionObject.getUserPointer());
	
	if(colObj.IsOwnerCollider()){
		if(pTouchSensor.CollidesNot(*colObj.GetOwnerCollider())){
			return false;
		}
		
	}else if(colObj.IsOwnerHTSector()){
		if(pTouchSensor.GetTouchSensor().GetCollisionFilter().CollidesNot(
		colObj.GetOwnerHTSector()->GetHeightTerrain()->GetHeightTerrain()->GetCollisionFilter())){
			return false;
		}
	}
	
	return debpConvexResultCallback::needsCollision(proxy0);
}
