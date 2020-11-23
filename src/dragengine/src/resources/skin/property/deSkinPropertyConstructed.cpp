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

#include "deSkinPropertyConstructed.h"
#include "deSkinPropertyVisitor.h"
#include "node/deSkinPropertyNodeGroup.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyConstructed
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyConstructed::deSkinPropertyConstructed( const char *type ) :
deSkinProperty( type ),
pContent( NULL ),
pTileX( false ),
pTileY( false ),
pBitCount( 8 )
{
	pContent = new deSkinPropertyNodeGroup;
	pContent->SetSize( decVector( 256.0f, 256.0f, 1.0f ) );
}

deSkinPropertyConstructed::~deSkinPropertyConstructed(){
	if( pContent ){
		delete pContent;
	}
}



// Management
///////////////

void deSkinPropertyConstructed::SetColor( const decColor &color ){
	pColor = color;
}

void deSkinPropertyConstructed::SetTileX( bool tileX ){
	pTileX = tileX;
}

void deSkinPropertyConstructed::SetTileY( bool tileY ){
	pTileY = tileY;
}

void deSkinPropertyConstructed::SetBitCount( int bitCount ){
	switch( bitCount ){
	case 8:
	case 16:
	case 32:
		pBitCount = bitCount;
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



// Visiting
/////////////

void deSkinPropertyConstructed::Visit( deSkinPropertyVisitor &visitor ){
	visitor.VisitConstructed( *this );
}
