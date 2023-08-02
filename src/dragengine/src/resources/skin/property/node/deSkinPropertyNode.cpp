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

#include "deSkinPropertyNode.h"
#include "deSkinPropertyNodeVisitor.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNode
/////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNode::deSkinPropertyNode() :
pSize( 256, 256, 1 ),
pRotation( 0.0f ),
pShear( 0.0f ),

pBrightness( 0.0f ),
pContrast( 1.0f ),
pGamma( 1.0f ),
pColorize( 1.0f, 1.0f, 1.0f ),

pTransparency( 1.0f ),
pMask( NULL ),
pCombineMode( ecmBlend )
{
	int i;
	for( i=0; i<MappedCount; i++ ){
		pMapped[ i ] = -1;
	}
}

deSkinPropertyNode::~deSkinPropertyNode(){
	if( pMask ){
		delete pMask;
	}
}



// Management
///////////////

void deSkinPropertyNode::SetPosition( const decPoint3 &position ){
	pPosition = position;
}

void deSkinPropertyNode::SetSize( const decPoint3 &size ){
	pSize = size;
}

void deSkinPropertyNode::SetRotation( float rotation ){
	pRotation = rotation;
}

void deSkinPropertyNode::SetShear( float shear ){
	pShear = shear;
}



void deSkinPropertyNode::SetBrightness( float brightness ){
	pBrightness = brightness;
}

void deSkinPropertyNode::SetContrast( float contrast ){
	pContrast = contrast;
}

void deSkinPropertyNode::SetGamma( float gamma ){
	pGamma = gamma;
}

void deSkinPropertyNode::SetColorize( const decColor &color ){
	pColorize = color;
}



void deSkinPropertyNode::SetTransparency( float transparency ){
	pTransparency = transparency;
}

void deSkinPropertyNode::SetMask( deSkinPropertyNode *mask ){
	if( mask == pMask ){
		return;
	}
	
	if( pMask ){
		delete pMask;
	}
	pMask = mask;
}

void deSkinPropertyNode::SetCombineMode( eCombineModes mode ){
	pCombineMode = mode;
}



int deSkinPropertyNode::GetMappedFor( eMapped mapped ) const{
	return pMapped[ mapped ];
}

void deSkinPropertyNode::SetMappedFor( eMapped mapped, int index ){
	DEASSERT_TRUE( index >= -1 )
	pMapped[ mapped ] = index;
}



// Visiting
/////////////

void deSkinPropertyNode::Visit( deSkinPropertyNodeVisitor &visitor ){
	visitor.VisitNode( *this );
}
