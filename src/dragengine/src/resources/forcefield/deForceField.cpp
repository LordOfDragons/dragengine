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

#include "deForceField.h"
#include "deForceFieldManager.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/physics/deBasePhysicsForceField.h"



// Class deForceField
///////////////////////

// Constructor, destructor
////////////////////////////

deForceField::deForceField( deForceFieldManager *manager ) :
deResource( manager ),

pRadius( 1.0f ),
pExponent( 1.0f ),
pFieldType( eftRadial ),
pApplicationType( eatDirect ),
pForce( 1.0f ),
pFluctuationDirection( 0.0f ),
pFluctuationForce( 0.0f ),
pEnabled( true ),
pPeerPhysics ( NULL ),

pParentWorld( NULL ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL ){
}

deForceField::~deForceField(){
	if( pPeerPhysics ){
		delete pPeerPhysics;
		pPeerPhysics = NULL;
	}
}



// Management
///////////////

void deForceField::SetPosition( const decDVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	if( pPeerPhysics ){
		pPeerPhysics->PositionChanged();
	}
}

void deForceField::SetOrientation( const decQuaternion &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	if( pPeerPhysics ){
		pPeerPhysics->OrientationChanged();
	}
}



void deForceField::SetInfluenceArea( const decShapeList &area ){
	pInfluenceArea = area;
	if( pPeerPhysics ){
		pPeerPhysics->InfluenceAreaChanged();
	}
}

void deForceField::SetRadius( float radius ){
	radius = decMath::max( radius, 0.0f );
	if( fabsf( radius - pRadius ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pRadius = radius;
	if( pPeerPhysics ){
		pPeerPhysics->RadiusChanged();
	}
}

void deForceField::SetExponent( float exponent ){
	exponent = decMath::max( exponent, 0.0f );
	if( fabsf( exponent - pExponent ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pExponent = exponent;
	if( pPeerPhysics ){
		pPeerPhysics->ExponentChanged();
	}
}



void deForceField::SetFieldType( eFieldTypes type ){
	if( type == pFieldType ){
		return;
	}
	
	if( type < eftRadial || type > eftVortex ){
		DETHROW( deeInvalidParam );
	}
	
	pFieldType = type;
	if( pPeerPhysics ){
		pPeerPhysics->TypeChanged();
	}
}

void deForceField::SetApplicationType( eApplicationTypes type ){
	if( type == pApplicationType ){
		return;
	}
	
	if( type < eatDirect || type > eatSpeed ){
		DETHROW( deeInvalidParam );
	}
	
	pApplicationType = type;
	if( pPeerPhysics ){
		pPeerPhysics->TypeChanged();;
	}
}

void deForceField::SetDirection( const decVector &direction ){
	if( direction.IsEqualTo( pDirection ) ){
		return;
	}
	
	pDirection = direction;
	if( pPeerPhysics ){
		pPeerPhysics->DirectionChanged();
	}
}

void deForceField::SetForce( float force ){
	if( fabsf( force - pForce ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pForce = force;
	if( pPeerPhysics ){
		pPeerPhysics->ForceChanged();
	}
}

void deForceField::SetFluctuationDirection( float fluctuation ){
	if( fabsf( fluctuation - pFluctuationDirection ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pFluctuationDirection = fluctuation;
	if( pPeerPhysics ){
		pPeerPhysics->FluctuationChanged();
	}
}

void deForceField::SetFluctuationForce( float fluctuation ){
	if( fabsf( fluctuation - pFluctuationForce ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pFluctuationForce = fluctuation;
	if( pPeerPhysics ){
		pPeerPhysics->FluctuationChanged();
	}
}



void deForceField::SetShape( const decShapeList &shape ){
	pShape = shape;
	
	if( pPeerPhysics ){
		pPeerPhysics->ShapeChanged();
	}
}

void deForceField::SetCollisionFilter( const decCollisionFilter &filter ){
	if( filter == pCollisionFilter ){
		return;
	}
	
	pCollisionFilter = filter;
	if( pPeerPhysics ){
		pPeerPhysics->CollisionFilterChanged();;
	}
}

void deForceField::SetEnabled( bool enabled ){
	if( enabled == pEnabled ){
		return;
	}
	
	pEnabled = enabled;
	if( pPeerPhysics ){
		pPeerPhysics->EnabledChanged();
	}
}



// System Peers
/////////////////

void deForceField::SetPeerPhysics( deBasePhysicsForceField *peer ){
	if( peer == pPeerPhysics ){
		return;
	}
	
	if( pPeerPhysics ){
		delete pPeerPhysics;
	}
	pPeerPhysics = peer;
}



// Linked List
////////////////

void deForceField::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deForceField::SetLLWorldPrev( deForceField *forceField ){
	pLLWorldPrev = forceField;
}

void deForceField::SetLLWorldNext( deForceField *forceField ){
	pLLWorldNext = forceField;
}
