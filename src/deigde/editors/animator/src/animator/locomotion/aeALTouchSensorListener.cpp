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

#include "aeAnimatorLocomotion.h"
#include "aeALTouchSensorListener.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/collider/deColliderManager.h>
#include <dragengine/resources/sensor/deTouchSensor.h>
#include <dragengine/resources/sensor/deTouchSensorManager.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/world/deWorldManager.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Animator Editor"



// Class aeALTouchSensorListener
//////////////////////////////////

// Constructor, destructor
////////////////////////////

aeALTouchSensorListener::aeALTouchSensorListener(){
	pLocomotion = NULL;
}

aeALTouchSensorListener::~aeALTouchSensorListener(){
}



// Management
///////////////

void aeALTouchSensorListener::SetAnimatorLocomotion( aeAnimatorLocomotion *locomotion ){
	pLocomotion = locomotion;
}



// Notifications
//////////////////

void aeALTouchSensorListener::ColliderEntered( deCollider *collider ){
// 	pLocomotion->GetLogger()->LogInfoFormat( LOGSOURCE, "Touch Sensor Listener: Collider %p entered shape %i", collider, shape );
	pLocomotion->UpdateTSColors();
}

void aeALTouchSensorListener::ColliderLeft( deCollider *collider ){
// 	pLocomotion->GetLogger()->LogInfoFormat( LOGSOURCE, "Touch Sensor Listener: Collider %p left shape %i", collider, shape );
	pLocomotion->UpdateTSColors();
}
