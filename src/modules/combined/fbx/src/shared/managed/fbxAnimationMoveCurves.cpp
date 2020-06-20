/* 
 * FBX Modules
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
#include <inttypes.h>

#include "fbxAnimation.h"
#include "fbxAnimationCurve.h"
#include "fbxAnimationMove.h"
#include "fbxAnimationMoveCurves.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxAnimationMoveCurves
// ////////////////////////

// Constructor, destructor
////////////////////////////

fbxAnimationMoveCurves::fbxAnimationMoveCurves( fbxAnimationMove &move, fbxNode &nodeCurves ) :
pMove( move ),
pNodeCurves( nodeCurves ),
pNodeCurvesID( nodeCurves.GetID() ),
pNodeModel( NULL ),
pNodeModelID( 0 ),
pTargetProperty( etpUnsupported ),
pRigBone( NULL )
{
	// find curves and model
	decPointerList connections;
	move.GetAnimation().GetScene().FindConnections( pNodeCurvesID, connections );
	int i, conCount = connections.GetCount();
	deObjectReference nodeCurve;
	
	for( i=0; i<conCount; i++ ){
		const fbxConnection &connection = *( ( fbxConnection* )connections.GetAt( i ) );
		if( connection.GetTarget() == pNodeCurvesID ){
			fbxNode &node = *move.GetAnimation().GetScene().NodeWithID( connection.GetSource() );
			if( node.GetName() == "AnimationCurve" ){
				if( connection.GetProperty() == "d|X" ){
					pCurveX.TakeOver( new fbxAnimationCurve( node ) );
					
				}else if( connection.GetProperty() == "d|Y" ){
					pCurveY.TakeOver( new fbxAnimationCurve( node ) );
					
				}else if ( connection.GetProperty() == "d|Z" ){
					pCurveZ.TakeOver( new fbxAnimationCurve( node ) );
				}
			}
			
		}else{
			fbxNode &node = *move.GetAnimation().GetScene().NodeWithID( connection.GetTarget() );
			if( node.GetName() == "Model" ){
				pNodeModel = &node;
				pNodeModelID = node.GetID();
				pBoneName = node.GetPropertyAt( 1 )->CastString().GetValue();
				
				if( connection.GetProperty() == "Lcl Translation" ){
					pTargetProperty = etpPosition;
					pDefaultValue = node.GetLocalTranslation();
					
				}else if( connection.GetProperty() == "Lcl Rotation" ){
					pTargetProperty = etpRotation;
					pDefaultValue = node.GetLocalRotation();
					
				}else if( connection.GetProperty() == "Lcl Scaling" ){
					pTargetProperty = etpScale;
					pDefaultValue = node.GetLocalScaling();
				}
			}
		}
	}
	
	pDefaultValue.x = nodeCurves.GetPropertyFloat( "d|X", ( float )pDefaultValue.x );
	pDefaultValue.y = nodeCurves.GetPropertyFloat( "d|Y", ( float )pDefaultValue.y );
	pDefaultValue.z = nodeCurves.GetPropertyFloat( "d|Z", ( float )pDefaultValue.z );
}

fbxAnimationMoveCurves::~fbxAnimationMoveCurves(){
}



// Management
///////////////

void fbxAnimationMoveCurves::SetTargetProperty( eTargetProperty targetProperty ){
	pTargetProperty = targetProperty;
}

void fbxAnimationMoveCurves::SetBoneName( const char *name ){
	pBoneName = name;
}

void fbxAnimationMoveCurves::SetRigBone( fbxRigBone *bone ){
	pRigBone = bone;
}

void fbxAnimationMoveCurves::SetDefaultValue( const decVector &defaultValue ){
	pDefaultValue = defaultValue;
}



void fbxAnimationMoveCurves::Prepare(){
}



void fbxAnimationMoveCurves::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose) const{
	static const char * const tpnames[ 4 ] = { "position", "rotation", "scale", "unsupported" };
	module.LogInfoFormat( "%sCurves '%s': bone='%s'(%p) targetProperty=%s, curves(%p %p %p)",
		prefix.GetString(), pNodeModel->GetPropertyAt( 1 )->CastString().GetValue().GetString(),
		pBoneName.GetString(), pRigBone, tpnames[ pTargetProperty ], ( deObject* )pCurveX,
		( deObject* )pCurveY, ( deObject* )pCurveZ );
}
