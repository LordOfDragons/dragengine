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

#include "deBillboard.h"
#include "deBillboardManager.h"
#include "../skin/deSkin.h"
#include "../skin/dynamic/deDynamicSkin.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicBillboard.h"



// Class deBillboard
//////////////////////

// Constructor, destructor
////////////////////////////

deBillboard::deBillboard( deBillboardManager *manager ) :
deResource( manager ),

pAxis( 0.0f, 1.0f, 0.0f ),
pSize( 1.0f, 1.0f ),
pLocked( true ),
pSpherical( true ),
pSizeFixedToScreen( false ),
pVisible( true ),

pPeerGraphic( NULL ),

pParentWorld( NULL ),
pLLWorldPrev( NULL ),
pLLWorldNext( NULL ){
}

deBillboard::~deBillboard(){
	SetPeerGraphic( NULL );
}



// Management
///////////////

void deBillboard::SetPosition( const decDVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	if( pPeerGraphic ){
		pPeerGraphic->PositionChanged();
	}
}

void deBillboard::SetAxis( const decVector &axis ){
	if( axis.IsEqualTo( pAxis ) ){
		return;
	}
	
	pAxis = axis;
	
	if( pPeerGraphic ){
		pPeerGraphic->AxisChanged();
	}
}

void deBillboard::SetSize( const decVector2 &size ){
	const decVector2 realSize( decVector2( FLOAT_SAFE_EPSILON, FLOAT_SAFE_EPSILON ).Largest( size ) );
	if( realSize.IsEqualTo( pSize ) ){
		return;
	}
	
	pSize = realSize;
	
	if( pPeerGraphic ){
		pPeerGraphic->SizeChanged();
	}
}

void deBillboard::SetOffset( const decVector2 &offset ){
	if( offset.IsEqualTo( pOffset ) ){
		return;
	}
	
	pOffset = offset;
	
	if( pPeerGraphic ){
		pPeerGraphic->OffsetChanged();
	}
}

void deBillboard::SetSkin( deSkin *skin ){
	if( pSkin == skin ){
		return;
	}
	
	pSkin = skin;
	
	if( pPeerGraphic ){
		pPeerGraphic->SkinChanged();
	}
}

void deBillboard::SetDynamicSkin( deDynamicSkin *dynamicSkin ){
	if( pDynamicSkin == dynamicSkin ){
		return;
	}
	
	pDynamicSkin = dynamicSkin;
	
	if( pPeerGraphic ){
		pPeerGraphic->DynamicSkinChanged();
	}
}

void deBillboard::SetLocked( bool locked ){
	if( locked == pLocked ){
		return;
	}
	
	pLocked = locked;
	
	if( pPeerGraphic ){
		pPeerGraphic->ParametersChanged();
	}
}

void deBillboard::SetSpherical( bool spherical ){
	if( spherical == pSpherical ){
		return;
	}
	
	pSpherical = spherical;
	
	if( pPeerGraphic ){
		pPeerGraphic->ParametersChanged();
	}
}

void deBillboard::SetSizeFixedToScreen( bool sizeFixedToScreen ){
	if( sizeFixedToScreen == pSizeFixedToScreen ){
		return;
	}
	
	pSizeFixedToScreen = sizeFixedToScreen;
	
	if( pPeerGraphic ){
		pPeerGraphic->ParametersChanged();
	}
}

void deBillboard::SetVisible( bool visible ){
	if( visible == pVisible ){
		return;
	}
	
	pVisible = visible;
	
	if( pPeerGraphic ){
		pPeerGraphic->VisibilityChanged();
	}
}

void deBillboard::SetLayerMask( const decLayerMask &layerMask ){
	if( layerMask == pLayerMask ){
		return;
	}
	
	pLayerMask = layerMask;
	
	if( pPeerGraphic ){
		pPeerGraphic->LayerMaskChanged();
	}
}



// System Peers
/////////////////

void deBillboard::SetPeerGraphic( deBaseGraphicBillboard *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}



// Linked List
////////////////

void deBillboard::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deBillboard::SetLLWorldPrev( deBillboard *billboard ){
	pLLWorldPrev = billboard;
}

void deBillboard::SetLLWorldNext( deBillboard *billboard ){
	pLLWorldNext = billboard;
}
