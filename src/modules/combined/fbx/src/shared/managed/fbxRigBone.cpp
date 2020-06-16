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
pNodeModelID( nodeModel.GetPropertyAt( 0 )->GetValueAsLong() ),
pIndex( 0 ),
pName( nodeModel.GetPropertyAt( 1 )->CastString().GetValue() ),
pParent( NULL ),
pWorldMatrix( nodeModel.CalcTransformMatrix() * rig.GetScene().GetTransformation() ),
pWorldMatrixInverse( pWorldMatrix.QuickInvert() ),
pRigMatrix( pWorldMatrix ){
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
	decPointerList connections;
	pRig.GetScene().FindConnections( pNodeModelID, connections );
	const int conCount = connections.GetCount();
	int i;
	
	for( i=0; i<conCount; i++ ){
		const fbxConnection &connection = *( ( fbxConnection* )connections.GetAt( i ) );
		if( connection.GetSource() != pNodeModelID || connection.GetTarget() == 0 ){
			continue;
		}
		
		pParent = pRig.GetBoneWithModelID( connection.GetTarget() );
		if( ! pParent ){
			DETHROW( deeInvalidParam );
		}
		
		break;
	}
}



void fbxRigBone::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose) const{
	const decVector pos( pRigMatrix.GetPosition() );
	const decVector rot( pRigMatrix.GetEulerAngles() * RAD2DEG );
	module.LogInfoFormat( "%sBone %d '%s': pos=(%g,%g,%g) rot=(%g,%g,%g) parent=(%s)",
		prefix.GetString(), pIndex, pName.GetString(), pos.x, pos.y, pos.z, rot.x, rot.y, rot.z,
		pParent ? pParent->GetName().GetString() : "<null>" );
}
