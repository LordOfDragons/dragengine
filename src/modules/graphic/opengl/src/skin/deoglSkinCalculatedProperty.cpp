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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglSkinCalculatedProperty.h"
#include "state/deoglSkinState.h"
#include "state/deoglSkinStateCalculated.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>



// Class deoglSkinCalculatedProperty
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinCalculatedProperty::deoglSkinCalculatedProperty() :
pRequiresTexture( false )
{
	int i;
	for( i=0; i<4; i++ ){
		pMappedComponentEvaluator[ i ] = NULL;
	}
	for( i=0; i<3; i++ ){
		pCopyFromFirst[ i ] = true;
	}
}

deoglSkinCalculatedProperty::~deoglSkinCalculatedProperty(){
	int i;
	for( i=0; i<4; i++ ){
		if( pMappedComponentEvaluator[ i ] ){
			delete pMappedComponentEvaluator[ i ];
		}
	}
}



// Management
///////////////

deSkinPropertyMapped::cComponent &deoglSkinCalculatedProperty::GetMappedComponent( int index ){
	if( index < 0 || index > 3 ){
		DETHROW( deeInvalidParam );
	}
	return pMappedComponent[ index ];
}

const deSkinPropertyMapped::cComponent &deoglSkinCalculatedProperty::GetMappedComponent( int index ) const{
	if( index < 0 || index > 3 ){
		DETHROW( deeInvalidParam );
	}
	return pMappedComponent[ index ];
}

void deoglSkinCalculatedProperty::SetRequiresTexture( bool requiresTexture ){
	pRequiresTexture = requiresTexture;
}

void deoglSkinCalculatedProperty::Prepare(){
	int i;
	for( i=0; i<3; i++ ){
		pCopyFromFirst[ i ] = true;
	}
	for( i=0; i<4; i++ ){
		if( pMappedComponentEvaluator[ i ] ){
			delete pMappedComponentEvaluator[ i ];
			pMappedComponentEvaluator[ i ] = NULL;
		}
		
		if( pMappedComponent[ i ].GetCurve().GetPointCount() > 0 ){
			pMappedComponentEvaluator[ i ] = new decCurveBezierEvaluator( pMappedComponent[ i ].GetCurve() );
			if( i > 0 ){
				pCopyFromFirst[ i - 1 ] = false;
			}
		}
	}
}

decColor deoglSkinCalculatedProperty::Calculate( const deoglSkinState &skinState,
const deoglSkinStateCalculated &calculated ){
	decColor color;
	
	color.r = pCalculateOutputValue( 0, pCalculateInputValue( 0, skinState, calculated ) );
	
	if( pCopyFromFirst[ 0 ] ){
		color.g = color.r;
		
	}else{
		color.g = pCalculateOutputValue( 1, pCalculateInputValue( 1, skinState, calculated ) );
	}
	
	if( pCopyFromFirst[ 1 ] ){
		color.b = color.r;
		
	}else{
		color.b = pCalculateOutputValue( 2, pCalculateInputValue( 2, skinState, calculated ) );
	}
	
	if( pCopyFromFirst[ 2 ] ){
		color.a = color.r;
		
	}else{
		color.a = pCalculateOutputValue( 3, pCalculateInputValue( 3, skinState, calculated ) );
	}
	
	return color;
}



// Private Functions
//////////////////////

float deoglSkinCalculatedProperty::pCalculateInputValue( int index, const deoglSkinState &skinState,
const deoglSkinStateCalculated &calculated ){
	float input = 0.0f;
	
	switch( pMappedComponent[ index ].GetInputType() ){
	case deSkinPropertyMapped::eitTime:
		input = skinState.GetTime();
		break;
		
	case deSkinPropertyMapped::eitBonePositionX:
	case deSkinPropertyMapped::eitBonePositionY:
	case deSkinPropertyMapped::eitBonePositionZ:
	case deSkinPropertyMapped::eitBoneRotationX:
	case deSkinPropertyMapped::eitBoneRotationY:
	case deSkinPropertyMapped::eitBoneRotationZ:
	case deSkinPropertyMapped::eitBoneScaleX:
	case deSkinPropertyMapped::eitBoneScaleY:
	case deSkinPropertyMapped::eitBoneScaleZ:
		input = calculated.GetBoneInputValue( index );
		break;
		
	default:
		break;
	}
	
	if( ! pMappedComponent[ index ].GetInputClamped() ){
		input = decMath::normalize( input, pMappedComponent[ index ].GetInputLower(),
			pMappedComponent[ index ].GetInputUpper() );
	}
	
	return decMath::linearStep( input, pMappedComponent[ index ].GetInputLower(),
		pMappedComponent[ index ].GetInputUpper() );
}

float deoglSkinCalculatedProperty::pCalculateOutputValue( int index, float inputValue ){
	if( pMappedComponentEvaluator[ index ] ){
		return pMappedComponentEvaluator[ index ]->EvaluateAt( inputValue );
		
	}else{
		return 0.0f;
	}
}
