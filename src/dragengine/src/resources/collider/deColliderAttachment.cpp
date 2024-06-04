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
