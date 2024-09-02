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
