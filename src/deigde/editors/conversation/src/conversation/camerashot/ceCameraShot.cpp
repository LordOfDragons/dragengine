/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceCameraShot.h"
#include "../ceConversation.h"

#include <dragengine/common/exceptions.h>



// Class ceCameraShot
///////////////////////

// Constructor, destructor
////////////////////////////

ceCameraShot::ceCameraShot( const char *name ) :
pName( name )
{
	pConversation = NULL;
	pActorCount = 1;
	pOffsetCameraFrom.Set( 0.0f, 1.65f, 0.0f );
	pCameraDistanceFrom = 0.5f;
	pOffsetLookAtFrom.Set( 0.0f, 1.65f, 0.0f );
	pTiltFrom = 0.0f;
	pFovFrom = 90.0f;
	pOffsetCameraTo = pOffsetCameraFrom;
	pCameraDistanceTo = pCameraDistanceFrom;
	pOffsetLookAtTo = pOffsetLookAtFrom;
	pTiltTo = pTiltFrom;
	pFovTo = pFovFrom;
	pLockUpAxis = false;
	pAlignTargets = false;
	pRelativeToLookAt = false;
	pLockCameraTarget = true;
	pLockLookAtTarget = true;
}

ceCameraShot::ceCameraShot( const ceCameraShot &cameraShot ){
	int i;
	
	pConversation = NULL;
	pName = cameraShot.pName;
	pActorCount = cameraShot.pActorCount;
	
	pCameraTarget = cameraShot.pCameraTarget;
	pOffsetCameraFrom = cameraShot.pOffsetCameraFrom;
	pOffsetCameraTo = cameraShot.pOffsetCameraTo;
	pCameraOrbitFrom = cameraShot.pCameraOrbitFrom;
	pCameraOrbitTo = cameraShot.pCameraOrbitTo;
	pCameraDistanceFrom = cameraShot.pCameraDistanceFrom;
	pCameraDistanceTo = cameraShot.pCameraDistanceTo;
	
	pLookAtTarget = cameraShot.pLookAtTarget;
	pOffsetLookAtFrom = cameraShot.pOffsetLookAtFrom;
	pOffsetLookAtTo = cameraShot.pOffsetLookAtTo;
	
	pPositionFrom = cameraShot.pPositionFrom;
	pPositionTo = cameraShot.pPositionTo;
	pRotationFrom = cameraShot.pRotationFrom;
	pRotationTo = cameraShot.pRotationTo;
	
	pTiltFrom = cameraShot.pTiltFrom;
	pTiltTo = cameraShot.pTiltTo;
	pFovFrom = cameraShot.pFovFrom;
	pFovTo = cameraShot.pFovTo;
	
	pLockUpAxis = cameraShot.pLockUpAxis;
	pAlignTargets = cameraShot.pAlignTargets;
	pRelativeToLookAt = cameraShot.pRelativeToLookAt;
	pLockCameraTarget = cameraShot.pLockCameraTarget;
	pLockLookAtTarget = cameraShot.pLockLookAtTarget;
	
	for( i=0; i<EP_COUNT; i++ ){
		pCurve[ i ] = cameraShot.pCurve[ i ];
	}
}

ceCameraShot::~ceCameraShot(){
}



// Management
///////////////

void ceCameraShot::SetConversation( ceConversation *conversation ){
	pConversation = conversation;
}

void ceCameraShot::SetName( const char *name ){
	if( ! name ) DETHROW( deeInvalidParam );
	
	if( ! pName.Equals( name ) ){
		if( pConversation && pConversation->GetCameraShotList().HasNamed( name ) ) DETHROW( deeInvalidParam );
		
		pName = name;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetActorCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count != pActorCount ){
		pActorCount = count;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}



void ceCameraShot::SetCameraTarget( const char *target ){
	if( ! target ){
		DETHROW( deeInvalidParam );
	}
	
	if( pCameraTarget != target ){
		pCameraTarget = target;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetOffsetCameraFrom( const decVector& offset ){
	if( ! offset.IsEqualTo( pOffsetCameraFrom ) ){
		pOffsetCameraFrom = offset;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetOffsetCameraTo( const decVector& offset ){
	if( ! offset.IsEqualTo( pOffsetCameraTo ) ){
		pOffsetCameraTo = offset;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}



void ceCameraShot::SetLookAtTarget( const char *target ){
	if( ! target ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLookAtTarget != target ){
		pLookAtTarget = target;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetOffsetLookAtFrom( const decVector& offset ){
	if( ! offset.IsEqualTo( pOffsetLookAtFrom ) ){
		pOffsetLookAtFrom = offset;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetOffsetLookAtTo( const decVector& offset ){
	if( ! offset.IsEqualTo( pOffsetLookAtTo ) ){
		pOffsetLookAtTo = offset;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}



void ceCameraShot::SetCameraOrbitFrom( const decVector &rotation ){
	if( ! rotation.IsEqualTo( pCameraOrbitFrom ) ){
		pCameraOrbitFrom = rotation;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetCameraOrbitTo( const decVector &rotation ){
	if( ! rotation.IsEqualTo( pCameraOrbitTo ) ){
		pCameraOrbitTo = rotation;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetCameraDistanceFrom( float distance ){
	if( fabsf( distance - pCameraDistanceFrom ) > 1e-6f ){
		pCameraDistanceFrom = distance;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetCameraDistanceTo( float distance ){
	if( fabsf( distance - pCameraDistanceTo ) > 1e-6f ){
		pCameraDistanceTo = distance;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}



void ceCameraShot::SetPositionFrom( const decVector &position ){
	if( ! position.IsEqualTo( pPositionFrom ) ){
		pPositionFrom = position;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetPositionTo( const decVector &position ){
	if( ! position.IsEqualTo( pPositionTo ) ){
		pPositionTo = position;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetRotationFrom( const decVector &rotation ){
	if( ! rotation.IsEqualTo( pRotationFrom ) ){
		pRotationFrom = rotation;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetRotationTo( const decVector &rotation ){
	if( ! rotation.IsEqualTo( pRotationTo ) ){
		pRotationTo = rotation;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetTiltFrom( float tilt ){
	if( fabsf( tilt - pTiltFrom ) > 1e-6f ){
		pTiltFrom = tilt;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetTiltTo( float tilt ){
	if( fabsf( tilt - pTiltTo ) > 1e-6f ){
		pTiltTo = tilt;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetFovFrom( float fov ){
	if( fabsf( fov - pFovFrom ) > 1e-6f ){
		pFovFrom = fov;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetFovTo( float fov ){
	if( fabsf( fov - pFovTo ) > 1e-6f ){
		pFovTo = fov;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}



void ceCameraShot::SetLockUpAxis( bool lockUpAxis ){
	if( lockUpAxis != pLockUpAxis ){
		pLockUpAxis = lockUpAxis;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetAlignTargets( bool alignTargets ){
	if( alignTargets != pAlignTargets ){
		pAlignTargets = alignTargets;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetRelativeToLookAt( bool relativeToLookAt ){
	if( relativeToLookAt != pRelativeToLookAt ){
		pRelativeToLookAt = relativeToLookAt;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetLockCameraTarget( bool lock ){
	if( lock != pLockCameraTarget ){
		pLockCameraTarget = lock;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}

void ceCameraShot::SetLockLookAtTarget( bool lock ){
	if( lock != pLockLookAtTarget ){
		pLockLookAtTarget = lock;
		
		if( pConversation ){
			pConversation->NotifyCameraShotChanged( this );
		}
	}
}



const decCurveBezier &ceCameraShot::GetParameterCurve( int parameter ) const{
	if( parameter < 0 || parameter >= EP_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return pCurve[ parameter ];
}

void ceCameraShot::SetParameterCurve( int parameter, const decCurveBezier &curve ){
	if( parameter < 0 || parameter >= EP_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	pCurve[ parameter ] = curve;
	
	if( pConversation ){
		pConversation->NotifyCameraShotChanged( this );
	}
}
