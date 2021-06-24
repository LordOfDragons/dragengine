/* 
 * Drag[en]gine IGDE World Editor
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

#include "meWorld.h"
#include "meWorldGuiParameters.h"

#include <dragengine/common/exceptions.h>



// Class meWorldGuiParameters
///////////////////////////////

// Constructor, destructor
////////////////////////////

meWorldGuiParameters::meWorldGuiParameters( meWorld &world ) :
pWorld( world ),

pElementMode( eemObject ),
pWorkMode( ewmSelect ),
pLockAxisX( false ),
pLockAxisY( false ),
pLockAxisZ( false ),
pUseLocal( false ),
pSnapToSnapPoints( true ),

pShowOcclusionMeshes( false ),
pShowOcclusionMeshesSelected( false ),
pShowNavigationSpaces( false ),
pShowNavigationSpacesSelected( false ),

pHPDrawMode( ehpdmRaise ),
pHPRadius( 2.5f ),
pHPDrawStrength( 0.2f ),
pHPLevelStrength( 0.5f ),
pHPSmoothStrength( 0.5f ),

pMPDrawMode( empdmDraw ),
pMPRadius( 10.0f ),

pVPDrawMode( evpdmInvisible ),
pVPRadius( 10.0f ),

pRotationPivotCenter( erpcActive ),

pAutoUpdateVegetation( false ),

pAddFilterObjectInclusive( false ),

pRectSelDistance( 25.0f ),
pRectSelDistanceStep( 1.0f ),
pRectSelDragThreshold( 5 ){
}

meWorldGuiParameters::~meWorldGuiParameters(){
}



// Editing
////////////

void meWorldGuiParameters::SetElementMode( eElementModes mode ){
	if( mode == pElementMode ){
		return;
	}
	
	pElementMode = mode;
	pWorld.ElementModeChanged();
}

void meWorldGuiParameters::SetWorkMode( eWorkModes mode ){
	if( mode == pWorkMode ){
		return;
	}
	
	pWorkMode = mode;
	pWorld.ElementModeChanged();
}



void meWorldGuiParameters::SetLockAxisX( bool lock ){
	if( lock == pLockAxisX ){
		return;
	}
	
	pLockAxisX = lock;
	pWorld.NotifyEditingChanged();
}

void meWorldGuiParameters::SetLockAxisY( bool lock ){
	if( lock == pLockAxisY ){
		return;
	}
	
	pLockAxisY = lock;
	pWorld.NotifyEditingChanged();
}

void meWorldGuiParameters::SetLockAxisZ( bool lock ){
	if( lock == pLockAxisZ ){
		return;
	}
	
	pLockAxisZ = lock;
	pWorld.NotifyEditingChanged();
}

void meWorldGuiParameters::SetUseLocal( bool useLocal ){
	if( useLocal == pUseLocal ){
		return;
	}
	
	pUseLocal = useLocal;
	pWorld.NotifyEditingChanged();
}

void meWorldGuiParameters::SetRotationPivotCenter( eRotationPivotCenters pivotCenter ){
	if( pivotCenter == pRotationPivotCenter ){
		return;
	}
	
	pRotationPivotCenter = pivotCenter;
	pWorld.NotifyEditingChanged();
}



void meWorldGuiParameters::SetSnapToSnapPoints( bool snapToSnapPoints ){
	if( snapToSnapPoints == pSnapToSnapPoints ){
		return;
	}
	
	pSnapToSnapPoints = snapToSnapPoints;
	pWorld.NotifyEditingChanged();
}



void meWorldGuiParameters::SetRectSelDistance( float distance ){
	distance = decMath::max( distance, 0.01f );
	if( fabsf( distance - pRectSelDistance ) <= FLOAT_EPSILON ){
		return;
	}
	
	pRectSelDistance = distance;
	pWorld.NotifyEditingChanged();
}

void meWorldGuiParameters::SetRectSelDistanceStep( float step ){
	step = decMath::max( step, 0.001f );
	if( fabsf( step - pRectSelDistanceStep ) <= FLOAT_EPSILON ){
		return;
	}
	
	pRectSelDistanceStep = step;
	pWorld.NotifyEditingChanged();
}

void meWorldGuiParameters::SetRectSelDragThreshold( int threshold ){
	threshold = decMath::max( threshold, 1 );
	if( threshold == pRectSelDragThreshold ){
		return;
	}
	
	pRectSelDragThreshold = threshold;
	pWorld.NotifyEditingChanged();
}



void meWorldGuiParameters::SetShowOcclusionMeshes( bool show ){
	if( show == pShowOcclusionMeshes ){
		return;
	}
	
	pShowOcclusionMeshes = show;
	pWorld.ElementVisibilityChanged();
}

void meWorldGuiParameters::SetShowOcclusionMeshesSelected( bool show ){
	if( show == pShowOcclusionMeshesSelected ){
		return;
	}
	
	pShowOcclusionMeshesSelected = show;
	pWorld.ElementVisibilityChanged();
}

void meWorldGuiParameters::SetShowNavigationSpaces( bool show ){
	if( show == pShowNavigationSpaces ){
		return;
	}
	
	pShowNavigationSpaces = show;
	pWorld.ElementVisibilityChanged();
}

void meWorldGuiParameters::SetShowNavigationSpacesSelected( bool show ){
	if( show == pShowNavigationSpacesSelected ){
		return;
	}
	
	pShowNavigationSpacesSelected = show;
	pWorld.ElementVisibilityChanged();
}



// Painting
/////////////

void meWorldGuiParameters::SetHPDrawMode( eHPDrawModes mode ){
	if( mode < ehpdmRaise || mode > ehpdmSmooth ){
		DETHROW( deeInvalidParam );
	}
	
	if( mode == pHPDrawMode ){
		return;
	}
	
	pHPDrawMode = mode;
	pWorld.NotifyHeightPaintChanged();
}

void meWorldGuiParameters::SetHPRadius( float radius ){
	radius = decMath::clamp( radius, 0.1f, 100.0f );
	if( fabsf( radius - pHPRadius ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pHPRadius = radius;
	pWorld.NotifyHeightPaintChanged();
}

void meWorldGuiParameters::SetHPDrawStrength( float strength ){
	strength = decMath::clamp( strength, 0.01f, 10.0f );
	if( fabsf( strength - pHPDrawStrength ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pHPDrawStrength = strength;
	pWorld.NotifyHeightPaintChanged();
}

void meWorldGuiParameters::SetHPLevelStrength( float strength ){
	strength = decMath::clamp( strength, 0.01f, 1.0f );
	if( fabsf( strength - pHPLevelStrength ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pHPLevelStrength = strength;
	pWorld.NotifyHeightPaintChanged();
}

void meWorldGuiParameters::SetHPSmoothStrength( float strength ){
	strength = decMath::clamp( strength, 0.01f, 1.0f );
	if( fabsf( strength - pHPSmoothStrength ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pHPSmoothStrength = strength;
	pWorld.NotifyHeightPaintChanged();
}



void meWorldGuiParameters::SetMPDrawMode( eMPDrawModes mode ){
	if( mode == pMPDrawMode ){
		return;
	}
	
	pMPDrawMode = mode;
	pWorld.NotifyMaskPaintChanged();
}

void meWorldGuiParameters::SetMPRadius( float radius ){
	radius = decMath::clamp( radius, 0.1f, 100.0f );
	if( fabsf( radius - pMPRadius ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pMPRadius = radius;
	pWorld.NotifyMaskPaintChanged();
}



void meWorldGuiParameters::SetVPDrawMode( eVPDrawModes mode ){
	if( mode == pVPDrawMode ){
		return;
	}
	
	pVPDrawMode = mode;
	pWorld.NotifyVisibilityPaintChanged();
}

void meWorldGuiParameters::SetVPRadius( float radius ){
	radius = decMath::clamp( radius, 0.1f, 100.0f );
	if( fabsf( radius - pVPRadius ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pVPRadius = radius;
	pWorld.NotifyVisibilityPaintChanged();
}



// Vegetation
///////////////

void meWorldGuiParameters::SetAutoUpdateVegetation( bool autoUpdate ){
	if( autoUpdate == pAutoUpdateVegetation ){
		return;
	}
	
	pAutoUpdateVegetation = autoUpdate;
	pWorld.NotifyModeChanged(); // not the right notify, but enough hack for the time being
}



// Browsing
/////////////

void meWorldGuiParameters::SetBrowseClass( const char *name ){
	if( pBrowseClass == name ){
		return;
	}
	
	pBrowseClass = name;
	pWorld.NotifyEditingChanged(); // new notify please: NotifyBrowseChanged
}

void meWorldGuiParameters::SetBrowseSkin( const char *name ){
	if( pBrowseSkin == name ){
		return;
	}
	
	pBrowseSkin = name;
	pWorld.NotifyEditingChanged(); // new notify please: NotifyBrowseChanged
}



// Adding
///////////

void meWorldGuiParameters::SetAddFilterObjectSet( const decStringSet &set ){
	pAddFilterObjectSet = set;
	pWorld.NotifyEditingChanged();
}

void meWorldGuiParameters::SetAddFilterObjectInclusive( bool inclusive ){
	if( inclusive == pAddFilterObjectInclusive ){
		return;
	}
	
	pAddFilterObjectInclusive = inclusive;
	pWorld.NotifyEditingChanged();
}
