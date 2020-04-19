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
#include "deEnvMapProbe.h"
#include "deEnvMapProbeManager.h"
#include "../world/deWorld.h"
#include "../image/deImage.h"
#include "../../deEngine.h"
#include "../../systems/modules/graphic/deBaseGraphicEnvMapProbe.h"
#include "../../common/shape/decShape.h"
#include "../../common/exceptions.h"



// Class deEnvMapProbe
////////////////////////

// Constructor, destructor
////////////////////////////

deEnvMapProbe::deEnvMapProbe( deEnvMapProbeManager *manager ) : deResource( manager ){
	pScaling.Set( 1.0f, 1.0f, 1.0f );
	pShapeReflection = NULL;
	pInfluenceBorderSize = 0.1f;
	pInfluencePriority = 0;
	
	pLayerMask.SetBit( 0 );
	
	pPeerGraphic = NULL;
	
	pParentWorld = NULL;
	pLLWorldPrev = NULL;
	pLLWorldNext = NULL;
}

deEnvMapProbe::~deEnvMapProbe(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if( pShapeReflection ){
		delete pShapeReflection;
	}
}



// Management
///////////////

void deEnvMapProbe::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		
		if( pPeerGraphic ){
			pPeerGraphic->PositionChanged();
		}
	}
}

void deEnvMapProbe::SetOrientation( const decQuaternion &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		
		if( pPeerGraphic ){
			pPeerGraphic->OrientationChanged();
		}
	}
}

void deEnvMapProbe::SetScaling( const decVector &scaling ){
	if( ! scaling.IsEqualTo( pScaling ) ){
		pScaling = scaling;
		
		if( pPeerGraphic ){
			pPeerGraphic->ScalingChanged();
		}
	}
}



void deEnvMapProbe::NotifyShapeListInfluenceChanged(){
	if( pPeerGraphic ){
		pPeerGraphic->ShapeListInfluenceChanged();
	}
}



void deEnvMapProbe::SetShapeReflection( decShape *shape ){
	if( shape != pShapeReflection ){
		if( pShapeReflection ){
			delete pShapeReflection;
		}
		
		pShapeReflection = shape;
		
		NotifyShapeReflectionChanged();
	}
}

void deEnvMapProbe::NotifyShapeReflectionChanged(){
	if( pPeerGraphic ){
		pPeerGraphic->ShapeReflectionChanged();
	}
}



void deEnvMapProbe::SetInfluenceBorderSize( float borderSize ){
	if( borderSize < 0.0f ){
		borderSize = 0.0f;
	}
	
	if( fabsf( borderSize - pInfluenceBorderSize ) > FLOAT_SAFE_EPSILON ){
		pInfluenceBorderSize = borderSize;
		
		if( pPeerGraphic ){
			pPeerGraphic->InfluenceBorderSizeChanged();
		}
	}
}

void deEnvMapProbe::SetInfluencePriority( int priority ){
	if( priority < 0 ){
		priority = 0;
	}
	
	if( priority != pInfluencePriority ){
		pInfluencePriority = priority;
		
		if( pPeerGraphic ){
			pPeerGraphic->InfluencePriorityChanged();
		}
	}
}

void deEnvMapProbe::SetLayerMask( const decLayerMask &layerMask ){
	if( layerMask == pLayerMask ){
		return;
	}
	
	pLayerMask = layerMask;
	
	if( pPeerGraphic ){
		pPeerGraphic->LayerMaskChanged();
	}
}



void deEnvMapProbe::SetImage( deImage *image ){
	if( image == pImage ){
		return;
	}
	
	pImage = image;
	
	if( pPeerGraphic ){
		pPeerGraphic->ImageChanged();
	}
}



// System Peers
/////////////////

void deEnvMapProbe::SetPeerGraphic( deBaseGraphicEnvMapProbe *peer ){
	if( peer != pPeerGraphic ){
		if( pPeerGraphic ){
			delete pPeerGraphic;
		}
		
		pPeerGraphic = peer;
	}
}



// Linked List
////////////////

void deEnvMapProbe::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deEnvMapProbe::SetLLWorldPrev( deEnvMapProbe *instance ){
	pLLWorldPrev = instance;
}

void deEnvMapProbe::SetLLWorldNext( deEnvMapProbe *instance ){
	pLLWorldNext = instance;
}
