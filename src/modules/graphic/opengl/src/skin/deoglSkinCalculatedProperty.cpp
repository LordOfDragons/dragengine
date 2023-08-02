/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglSkinCalculatedProperty.h"
#include "state/deoglSkinState.h"
#include "state/deoglSkinStateCalculated.h"
#include "state/deoglSkinStateMapped.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkinCalculatedProperty
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinCalculatedProperty::deoglSkinCalculatedProperty() :
pRequiresTexture( false )
{
	int i;
	for( i=0; i<4; i++ ){
		pMappedComponent[ i ] = -1;
	}
}

deoglSkinCalculatedProperty::~deoglSkinCalculatedProperty(){
}



// Management
///////////////

int deoglSkinCalculatedProperty::GetMappedComponent( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index <= 3 )
	return pMappedComponent[ index ];
}

void deoglSkinCalculatedProperty::SetMappedComponent( int index, int mapped ){
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index <= 3 )
	DEASSERT_TRUE( mapped >= -1 )
	pMappedComponent[ index ] = mapped;
}

void deoglSkinCalculatedProperty::SetRequiresTexture( bool requiresTexture ){
	pRequiresTexture = requiresTexture;
}

decColor deoglSkinCalculatedProperty::Calculate( const deoglSkinState &skinState ) const{
	decColor color;
	
	if( pMappedComponent[ 0 ] != -1 ){
		color.r = skinState.GetMappedAt( pMappedComponent[ 0 ] ).GetValue();
	}
	
	if( pMappedComponent[ 1 ] != -1 ){
		color.g = skinState.GetMappedAt( pMappedComponent[ 1 ] ).GetValue();
		
	}else{
		color.g = color.r;
	}
	
	if ( pMappedComponent[ 2 ] != -1 ){
		color.b = skinState.GetMappedAt( pMappedComponent[ 2 ] ).GetValue();
		
	}else{
		color.b = color.r;
	}
	
	if( pMappedComponent[ 3 ] != -1 ){
		color.a = skinState.GetMappedAt( pMappedComponent[ 3 ] ).GetValue();
		
	}else{
		color.a = color.r;
	}
	
	return color;
}
