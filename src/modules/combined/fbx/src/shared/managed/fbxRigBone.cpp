/* 
 * FBX Model Module
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

#include "fbxRig.h"
#include "fbxRigBone.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxRigBone
/////////////////////

// Constructor, destructor
////////////////////////////

fbxRigBone::fbxRigBone( fbxRig &rig, fbxNode &nodePoseBone, fbxNode &nodeModel ) :
pRig( rig ),
pNodePoseBone( nodePoseBone ),
pNodeModel( nodeModel ),
pNodeArmature( NULL ),
pNodeModelID( nodeModel.GetID() ),
pIndex( 0 ),
pName( nodeModel.GetPropertyAt( 1 )->CastString().GetValue() ),
pParent( NULL ),
pDirty( true ){
}

fbxRigBone::~fbxRigBone(){
}



// Management
///////////////

void fbxRigBone::SetIndex( int index ){
	pIndex = index;
}

void fbxRigBone::SetName( const char *name ){
	pName = name;
}



void fbxRigBone::Prepare(){
	if( ! pDirty ){
		return;
	}
	
	decPointerList connections;
	pRig.GetScene().FindConnections( pNodeModelID, connections );
	const int conCount = connections.GetCount();
	int i;
	
	for( i=0; i<conCount; i++ ){
		const fbxConnection &connection = *( ( fbxConnection* )connections.GetAt( i ) );
		if( connection.GetSource() != pNodeModelID || connection.GetTarget() == 0 ){
			continue;
		}
		
		fbxRigBone * const bone = pRig.GetBoneWithModelID( connection.GetTarget() );
		if( bone ){
			pParent = bone;
			break;
		}
		
		// not a PoseBone which means it has to be the armature node
		pNodeArmature = pRig.GetScene().NodeWithID( connection.GetTarget() );
		break;
	}
	
	const decMatrix modelMatrix( pNodeModel.CalcTransformMatrix() );
	
	if( pParent ){
		pParent->Prepare();
		pMatrix = modelMatrix * pParent->GetBoneMatrix();
		pBoneMatrix = decMatrix::CreateScale( modelMatrix.GetScale() ) * pParent->GetBoneMatrix();
		
	}else if( pNodeArmature ){
		pMatrix = modelMatrix * pNodeArmature->CalcTransformMatrix() * pRig.GetScene().GetTransformation();
		pBoneMatrix.SetScale( pMatrix.GetScale() );
		
	}else{
		pMatrix = modelMatrix * pRig.GetScene().GetTransformation();
		pBoneMatrix.SetScale( pMatrix.GetScale() );
	}
	
	pMatrixInverse = pMatrix.QuickInvert();
	pPosition = pMatrix.GetPosition();
	
	decVector scale( pMatrix.GetScale() );
	if( fabsf( scale.x ) > 0.001f && fabsf( scale.y ) > 0.001f && fabsf( scale.z ) > 0.001f ){
		scale.x = 1.0f / scale.x;
		scale.y = 1.0f / scale.y;
		scale.z = 1.0f / scale.z;
		pOrientation = ( pMatrix * decMatrix::CreateScale( scale ) ).ToQuaternion();
	}
	
	pDirty = false;
}



void fbxRigBone::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose) const{
	const decVector rotation( pOrientation.GetEulerAngles() * RAD2DEG );
	module.LogInfoFormat( "%sBone %d '%s': pos=(%g,%g,%g) rot=(%g,%g,%g) parent=(%s) scale=(%g,%g)",
		prefix.GetString(), pIndex, pName.GetString(), pPosition.x, pPosition.y, pPosition.z,
		rotation.x, rotation.y, rotation.z, pParent ? pParent->GetName().GetString() : "<null>",
		pMatrix.GetScale().x, pBoneMatrix.GetScale().x );
}
