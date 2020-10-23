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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deColliderAttachment.h"
#include "../deResource.h"
#include "../deResourceManager.h"
#include "../../common/exceptions.h"



// Class deColliderAttachment
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deColliderAttachment::deColliderAttachment( deResource *resource ) :
pAttachType( eatStatic ),
pResource( resource ),
pScaling( 1.0f, 1.0f, 1.0f ),
pNoScaling( false ),
pWeights( NULL ),
pWeightCount( 0 )
{
	if( ! resource || ! CanAttachResource( *resource ) ){
		DETHROW( deeInvalidParam );
	}
}

deColliderAttachment::~deColliderAttachment(){
	if( pWeights ){
		delete [] pWeights;
	}
}



// Management
///////////////

void deColliderAttachment::SetAttachType( deColliderAttachment::eAttachType attachType ){
	pAttachType = attachType;
}

void deColliderAttachment::SetPosition( const decVector &position ){
	pPosition = position;
}

void deColliderAttachment::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void deColliderAttachment::SetScaling( const decVector &scaling ){
	pScaling = scaling;
}

void deColliderAttachment::SetNoScaling( bool noScaling ){
	pNoScaling = noScaling;
}

void deColliderAttachment::SetTrackBone( const char *bone ){
	pTrackBone = bone;
}



void deColliderAttachment::SetWeightCount( int count ){
	if( count == pWeightCount ){
		return;
	}
	
	sWeight *newArray = NULL;
	
	if( count > 0 ){
		newArray = new sWeight[ count ];
	}
	
	if( pWeights ){
		delete [] pWeights;
	}
	
	pWeights = newArray;
	pWeightCount = count;
}



bool deColliderAttachment::CanAttachResource( const deResource &resource ){
	switch( resource.GetResourceManager()->GetResourceType() ){
	case deResourceManager::ertCollider:
	case deResourceManager::ertBillboard:
	case deResourceManager::ertCamera:
	case deResourceManager::ertComponent:
	case deResourceManager::ertDebugDrawer:
	case deResourceManager::ertEnvMapProbe:
	case deResourceManager::ertForceField:
	case deResourceManager::ertLight:
	case deResourceManager::ertLumimeter:
	case deResourceManager::ertMicrophone:
	case deResourceManager::ertNavigationSpace:
	case deResourceManager::ertNavigationBlocker:
	case deResourceManager::ertNavigator:
	case deResourceManager::ertParticleEmitterInstance:
	case deResourceManager::ertPropField:
	case deResourceManager::ertSpeaker:
	case deResourceManager::ertTouchSensor:
	case deResourceManager::ertSoundLevelMeter:
		return true;
		
	default:
		return false;
	}
}
