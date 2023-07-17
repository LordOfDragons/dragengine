/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglSkinMapped.h"
#include "state/deoglSkinState.h"
#include "state/deoglSkinStateMapped.h"
#include "state/deoglSkinStateRenderable.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/dynamic/renderables/render/deoglRDSRenderable.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/curve/decCurveBezierEvaluator.h>



// Class deoglSkinMapped
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinMapped::deoglSkinMapped( const deSkinMapped &mapped ) :
pMapped( mapped ),
pCurveEvaluator( mapped.GetCurve() ),
pRenderable( -1 ){
}

deoglSkinMapped::~deoglSkinMapped(){
}



// Management
///////////////

void deoglSkinMapped::SetRenderable( int renderable ){
	DEASSERT_TRUE( renderable >= -1 )
	
	pRenderable = renderable;
}

float deoglSkinMapped::Calculate( const deoglSkinState &skinState, const deoglSkinStateMapped &mapped ) const{
	return pCalculateOutputValue( pCalculateInputValue( skinState, mapped ) );
}



// Private Functions
//////////////////////

float deoglSkinMapped::pCalculateInputValue( const deoglSkinState &skinState,
const deoglSkinStateMapped &mapped ) const{
	float input = 0.0f;
	
	switch( pMapped.GetInputType() ){
	case deSkinMapped::eitTime:
		input = skinState.GetTime();
		break;
		
	case deSkinMapped::eitBonePositionX:
	case deSkinMapped::eitBonePositionY:
	case deSkinMapped::eitBonePositionZ:
	case deSkinMapped::eitBoneRotationX:
	case deSkinMapped::eitBoneRotationY:
	case deSkinMapped::eitBoneRotationZ:
	case deSkinMapped::eitBoneScaleX:
	case deSkinMapped::eitBoneScaleY:
	case deSkinMapped::eitBoneScaleZ:
		input = mapped.GetBoneInputValue();
		break;
		
	case deSkinMapped::eitRenderable:{
		if( pRenderable == -1 || pRenderable >= skinState.GetRenderableCount() ){
			break;
		}
		
		const deoglSkinStateRenderable * const renderable = skinState.GetRenderableAt( pRenderable );
		if( ! renderable ){
			break;
		}
		
		const deoglRDynamicSkin * const dynamicSkin = skinState.GetOwnerDynamicSkin();
		if( ! dynamicSkin ){
			break;
		}
		
		const int hostRenderable = renderable->GetHostRenderable();
		if( hostRenderable == -1 ){
			break;
		}
		
		deoglRDSRenderable * const dsrenderable = dynamicSkin->GetRenderableAt( hostRenderable );
		if( ! dsrenderable ){
			break;
		}
		
		if( dsrenderable->GetType() == deoglRDSRenderable::etColor ){
			const decColor color( dsrenderable->GetRenderColor( decColor() ) );
			
			switch( pMapped.GetRenderableComponent() ){
			case deSkinMapped::ercRed:
				input = color.r;
				break;
				
			case deSkinMapped::ercGreen:
				input = color.g;
				break;
				
			case deSkinMapped::ercBlue:
				input = color.b;
				break;
				
			case deSkinMapped::ercAlpha:
				input = color.a;
				break;
			}
			
		}else{
			input = dsrenderable->GetRenderValue( 0.0f );
		}
		}break;
		
	default:
		break;
	}
	
	if( ! pMapped.GetInputClamped() ){
		input = decMath::normalize( input, pMapped.GetInputLower(), pMapped.GetInputUpper() );
	}
	
	return decMath::linearStep( input, pMapped.GetInputLower(), pMapped.GetInputUpper() );
}

float deoglSkinMapped::pCalculateOutputValue( float inputValue ) const{
	const float lower = pMapped.GetOutputLower();
	const float upper = pMapped.GetOutputUpper();
	
	return lower + ( upper - lower ) * pCurveEvaluator.EvaluateAt( inputValue );
}
