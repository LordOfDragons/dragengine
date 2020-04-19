/* 
 * Drag[en]gine IGDE Animator Editor
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
