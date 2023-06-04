/* 
 * Drag[en]gine Animator Module
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

#include "dearLink.h"
#include "dearAnimatorInstance.h"
#include "dearBoneState.h"
#include "dearVPSState.h"
#include "dearControllerStates.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/resources/animator/deAnimatorLink.h>



// Class dearLink
///////////////////

// Constructors and Destructors
/////////////////////////////////

dearLink::dearLink( dearAnimatorInstance &instance, const deAnimatorLink &link,
	const decIntList &controllerMapping ) :
pInstance( instance ),
pLink( link ),
pEvaluator( link.GetCurve() ),
pBoneIndex( -1 ),
pVPSIndex( -1 ),
pWrapY( link.GetWrapY() )
{
	const int controller = pLink.GetController();
	if( controller != -1 ){
		if( controller < controllerMapping.GetCount() ){
			pLink.SetController( controllerMapping.GetAt( controller ) );
			
		}else{
			pLink.SetController( -1 );
		}
	}
	
	if( ! pLink.GetBone().IsEmpty() ){
		pBoneIndex = instance.GetBoneStateList().IndexOfStateNamed( pLink.GetBone() );
	}
	
	if( ! pLink.GetVertexPositionSet().IsEmpty() ){
		pVPSIndex = instance.GetVPSStateList().IndexOfStateNamed( pLink.GetVertexPositionSet() );
	}
}

dearLink::~dearLink(){
}



// Management
///////////////

bool dearLink::HasController() const{
	return pLink.HasController();
}

int dearLink::GetController() const{
	return pLink.GetController();
}

bool dearLink::HasBone() const{
	return pBoneIndex != -1;
}

bool dearLink::HasVPS() const{
	return pVPSIndex != -1;
}



float dearLink::GetValue( float defaultValue ) const{
	float value;
	
	if( pLink.HasController() ){
		value = pInstance.GetControllerStates().GetValueAt( pLink.GetController() );
		
	}else if( pBoneIndex != -1 ){
		const dearBoneState &state = *pInstance.GetBoneStateList().GetStateAt( pBoneIndex );
		
		switch( pLink.GetBoneParameter() ){
		case deAnimatorLink::ebpPositionX:
			value = state.GetPosition().x;
			break;
			
		case deAnimatorLink::ebpPositionY:
			value = state.GetPosition().y;
			break;
			
		case deAnimatorLink::ebpPositionZ:
			value = state.GetPosition().z;
			break;
			
		case deAnimatorLink::ebpRotationX:
			value = state.GetOrientation().GetEulerAngles().x;
			break;
			
		case deAnimatorLink::ebpRotationY:
			value = state.GetOrientation().GetEulerAngles().y;
			break;
			
		case deAnimatorLink::ebpRotationZ:
			value = state.GetOrientation().GetEulerAngles().z;
			break;
			
		case deAnimatorLink::ebpScaleX:
			value = state.GetScale().x;
			break;
			
		case deAnimatorLink::ebpScaleY:
			value = state.GetScale().y;
			break;
			
		case deAnimatorLink::ebpScaleZ:
			value = state.GetScale().z;
			break;
			
		default:
			value = pLink.GetBoneMinimumValue();
		}
		
		value = decMath::linearStep( value, pLink.GetBoneMinimumValue(),  pLink.GetBoneMaximumValue() );
		
	}else if( pVPSIndex != -1 ){
		value = decMath::linearStep( pInstance.GetVPSStateList().GetStateAt( pVPSIndex ).GetWeight(),
			pLink.GetVertexPositionSetMinimumValue(), pLink.GetVertexPositionSetMaximumValue() );
		
	}else{
		return defaultValue;
	}
	
	const int repeat = pLink.GetRepeat();
	if( repeat > 1 ){
		value *= ( float )repeat;
		value -= floorf( value );
	}
	
	if( pEvaluator.GetCurve().GetPointCount() > 0 ){
		value = pEvaluator.EvaluateAt( value );
	}
	
	if( pWrapY ){
		value -= floorf( value );
	}
	
	return value;
}

void dearLink::GetVector( decVector &vector ) const{
	if( pLink.HasController() ){
		vector = pInstance.GetControllerStates().GetVectorAt( pLink.GetController() );
	}
}

void dearLink::GetQuaternion( decQuaternion &quaternion ) const{
	if( pLink.HasController() ){
		quaternion.SetFromEuler( pInstance.GetControllerStates().GetVectorAt( pLink.GetController() ) );
	}
}
