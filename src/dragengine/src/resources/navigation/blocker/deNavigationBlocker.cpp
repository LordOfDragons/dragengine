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
#include <string.h>

#include "deNavigationBlocker.h"
#include "deNavigationBlockerManager.h"
#include "../space/deNavigationSpace.h"
#include "../../../systems/modules/ai/deBaseAINavigationBlocker.h"
#include "../../../common/exceptions.h"



// Class deNavigationBlocker
//////////////////////////////

// Constructor, destructor
////////////////////////////

deNavigationBlocker::deNavigationBlocker( deNavigationBlockerManager *manager ) :
deResource( manager ),
pBlockingPriority( 0 )
{
	pScaling.Set( 1.0f, 1.0f, 1.0f );
	
	pLayer = 0;
	pSpaceType = deNavigationSpace::estGrid;
	pEnabled = true;
	
	pPeerAI = NULL;
	
	pParentWorld = NULL;
	pLLWorldPrev = NULL;
	pLLWorldNext = NULL;
}

deNavigationBlocker::~deNavigationBlocker(){
	if( pPeerAI ){
		delete pPeerAI;
		pPeerAI = NULL;
	}
}



// Management
///////////////

void deNavigationBlocker::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		if( pPeerAI ){
			pPeerAI->PositionChanged();
		}
	}
}

void deNavigationBlocker::SetOrientation( const decQuaternion &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		if( pPeerAI ){
			pPeerAI->OrientationChanged();
		}
	}
}

void deNavigationBlocker::SetScaling( const decVector &scaling ){
	if( ! scaling.IsEqualTo( pScaling ) ){
		pScaling = scaling;
		if( pPeerAI ){
			pPeerAI->ScalingChanged();
		}
	}
}

void deNavigationBlocker::SetLayer( int layer ){
	if( layer != pLayer ){
		pLayer = layer;
		if( pPeerAI ){
			pPeerAI->LayerChanged();
		}
	}
}

void deNavigationBlocker::SetSpaceType( deNavigationSpace::eSpaceTypes spaceType ){
	if( spaceType < deNavigationSpace::estGrid || spaceType > deNavigationSpace::estVolume ){
		DETHROW( deeInvalidParam );
	}
	
	if( spaceType != pSpaceType ){
		pSpaceType = spaceType;
		if( pPeerAI ){
			pPeerAI->SpaceTypeChanged();
		}
	}
}

void deNavigationBlocker::SetBlockingPriority( int priority ){
	if( priority == pBlockingPriority ){
		return;
	}
	
	pBlockingPriority = priority;
	
	if( pPeerAI ){
		pPeerAI->BlockingPriorityChanged();
	}
}



void deNavigationBlocker::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	
	if( pPeerAI ){
		pPeerAI->EnabledChanged();
	}
}



void deNavigationBlocker::NotifyShapeListChanged(){
	if( pPeerAI ){
		pPeerAI->ShapeChanged();
	}
}



// System Peers
/////////////////

void deNavigationBlocker::SetPeerAI( deBaseAINavigationBlocker *peer ){
	if( pPeerAI ){
		delete pPeerAI;
	}
	pPeerAI = peer;
}



// Linked List
////////////////

void deNavigationBlocker::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deNavigationBlocker::SetLLWorldPrev( deNavigationBlocker *blocker ){
	pLLWorldPrev = blocker;
}

void deNavigationBlocker::SetLLWorldNext( deNavigationBlocker *blocker ){
	pLLWorldNext = blocker;
}
