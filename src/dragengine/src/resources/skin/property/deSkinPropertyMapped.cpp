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

#include "deSkinPropertyMapped.h"
#include "deSkinPropertyVisitor.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyMapped
///////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyMapped::deSkinPropertyMapped( const char *type ) :
deSkinProperty( type )
{
	int i;
	for( i=0; i<4; i++ ){
		pComponents[ i ] = -1;
	}
}

deSkinPropertyMapped::~deSkinPropertyMapped(){
}



// Management
///////////////

void deSkinPropertyMapped::SetRed( int mapped ){
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ 0 ] = mapped;
}

void deSkinPropertyMapped::SetGreen( int mapped ){
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ 1 ] = mapped;
}

void deSkinPropertyMapped::SetBlue( int mapped ){
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ 2 ] = mapped;
}

void deSkinPropertyMapped::SetAlpha( int mapped ){
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ 3 ] = mapped;
}

int deSkinPropertyMapped::GetComponent( int component ) const{
	DEASSERT_TRUE( component >= 0 )
	DEASSERT_TRUE( component <= 3 )
	return pComponents[ component ];
}

void deSkinPropertyMapped::SetComponent( int component, int mapped ){
	DEASSERT_TRUE( component >= 0 )
	DEASSERT_TRUE( component <= 3 )
	DEASSERT_TRUE( mapped >= -1 )
	pComponents[ component ] = mapped;
}



// Visiting
/////////////

void deSkinPropertyMapped::Visit( deSkinPropertyVisitor &visitor ){
	visitor.VisitMapped( *this );
}
