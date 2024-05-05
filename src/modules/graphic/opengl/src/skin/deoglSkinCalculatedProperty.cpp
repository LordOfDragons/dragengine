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
