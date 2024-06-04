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
