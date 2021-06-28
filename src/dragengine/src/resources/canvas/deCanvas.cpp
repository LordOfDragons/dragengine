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

#include "deCanvas.h"
#include "deCanvasManager.h"
#include "deCanvasVisitor.h"
#include "../../systems/modules/graphic/deBaseGraphicCanvas.h"
#include "../../common/exceptions.h"



// Class deCanvas
///////////////////

// Constructors and Destructors
/////////////////////////////////

deCanvas::deCanvas( deCanvasManager *manager ) :
deResource( manager ),

pSize( 20, 20 ),
pVisible( true ),
pOrder( 0.0f ),
pTransparency( 1.0f ),
pBlendMode( ebmBlend ),

pPeerGraphic( NULL ),

pParentMask( NULL ),
pParentView( NULL ),
pLLViewPrev( NULL ),
pLLViewNext( NULL ){
}

deCanvas::~deCanvas(){
	SetPeerGraphic( NULL );
}



// Management
///////////////

void deCanvas::SetPosition( const decPoint &position ){
	if( position == pPosition ){
		return;
	}
	
	pPosition = position;
	if( pPeerGraphic ){
		pPeerGraphic->PositionChanged();
	}
}

void deCanvas::SetSize( const decPoint &size ){
	if( size == pSize ){
		return;
	}
	
	pSize = size;
	if( pPeerGraphic ){
		pPeerGraphic->SizeChanged();
	}
}

void deCanvas::SetTransform( const decTexMatrix2 &transform ){
	if( transform.IsEqualTo( pTransform ) ){
		return;
	}
	
	pTransform = transform;
	if( pPeerGraphic ){
		pPeerGraphic->TransformChanged();
	}
}

void deCanvas::SetColorTransform( const decColorMatrix &transform ){
	if( transform.IsEqualTo( pColorTransform ) ){
		return;
	}
	
	pColorTransform = transform;
	if( pPeerGraphic ){
		pPeerGraphic->ColorTransformChanged();
	}
}

void deCanvas::SetVisible( bool visible ){
	if( visible == pVisible ){
		return;
	}
	
	pVisible = visible;
	if( pPeerGraphic ){
		pPeerGraphic->VisibleChanged();
	}
}

void deCanvas::SetOrder( float order ){
	if( fabsf( order - pOrder ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pOrder = order;
	if( pPeerGraphic ){
		pPeerGraphic->OrderChanged();
	}
}

void deCanvas::SetTransparency( float transparency ){
	if( fabsf( transparency - pTransparency ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pTransparency = transparency;
	if( pPeerGraphic ){
		pPeerGraphic->TransparencyChanged();
	}
}

void deCanvas::SetBlendMode( eBlendModes blendMode ){
	if( blendMode < ebmBlend || blendMode > ebmAdd ){
		DETHROW( deeInvalidParam );
	}
	
	if( blendMode == pBlendMode ){
		return;
	}
	
	pBlendMode = blendMode;
	if( pPeerGraphic ){
		pPeerGraphic->BlendModeChanged();
	}
}

void deCanvas::SetMask( deCanvas *mask ){
	if( pMask == mask ){
		return;
	}
	
	if( mask && ( mask->GetParentMask() || mask->GetParentView() ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( pMask ){
		pMask->pParentMask = NULL;
	}
	
	pMask = mask;
	
	if( mask ){
		mask->pParentMask = this;
	}
	
	if( pPeerGraphic ){
		pPeerGraphic->MaskChanged();
	}
}



void deCanvas::NotifyContentChanged(){
	if( pPeerGraphic ){
		pPeerGraphic->ContentChanged();
	}
}



// System Peers
/////////////////

void deCanvas::SetPeerGraphic( deBaseGraphicCanvas *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	pPeerGraphic = peer;
}



// Visiting
/////////////

void deCanvas::Visit( deCanvasVisitor &visitor ){
	visitor.VisitCanvas( *this );
}



// Linked List
////////////////

void deCanvas::SetParentMask( deCanvas *canvas ){
	pParentMask = canvas;
}

void deCanvas::SetParentView( deCanvasView *view ){
	pParentView = view;
}

void deCanvas::SetLLViewPrev( deCanvas *canvas ){
	pLLViewPrev = canvas;
}

void deCanvas::SetLLViewNext( deCanvas *canvas ){
	pLLViewNext = canvas;
}
