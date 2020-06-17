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

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxRig
/////////////////

// Constructor, destructor
////////////////////////////

fbxRig::fbxRig( fbxScene &scene, fbxNode &nodePose ) :
pScene( scene ),
pNodePose( nodePose ),
pMatrix( nodePose.CalcTransformMatrix() * scene.GetTransformation() ),
pMatrixInverse( pMatrix.QuickInvert() ),
pBoneMatrix( decMatrix::CreateScale( pMatrix.GetScale() ) )
{
	const int childCount = nodePose.GetNodeCount();
	deObjectReference refBone;
	int i;
	
	for( i=0; i<childCount; i++ ){
		fbxNode &nodePoseBone = *nodePose.GetNodeAt( i );
		if( nodePoseBone.GetName() != "PoseNode" ){
			continue;
		}
		
		fbxNode &nodeModel = *scene.NodeWithID( nodePoseBone.FirstNodeNamed( "Node" )
			->GetPropertyAt( 0 )->GetValueAsLong() );
		if( nodeModel.GetPropertyAt( 2 )->CastString().GetValue() != "LimbNode" ){
			continue;
		}
		
		refBone.TakeOver( new fbxRigBone( *this, nodePoseBone, nodeModel ) );
		fbxRigBone &bone = ( fbxRigBone& )( deObject& )refBone;
		bone.SetIndex( pBones.GetCount() );
		
		if( GetBoneNamed( bone.GetName() ) ){
			decString message;
			message.Format( "duplicate rig bone '%s'", bone.GetName().GetString() );
			DETHROW_INFO( deeInvalidParam, message );
		}
		
		pBones.Add( refBone );
	}
	
	const int boneCount = pBones.GetCount();
	for( i=0; i<boneCount; i++ ){
		GetBoneAt( i )->Prepare();
	}
}

fbxRig::~fbxRig(){
}



// Management
///////////////

int fbxRig::GetBoneCount() const{
	return pBones.GetCount();
}

fbxRigBone *fbxRig::GetBoneAt( int index ) const{
	return ( fbxRigBone* )pBones.GetAt( index );
}

fbxRigBone *fbxRig::GetBoneNamed( const char *name ) const{
	const int count = pBones.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		fbxRigBone * const bone = ( fbxRigBone* )pBones.GetAt( i );
		if( bone->GetName() == name ){
			return bone;
		}
	}
	return NULL;
}

fbxRigBone *fbxRig::GetBoneWithModelID( int64_t id ) const{
	const int count = pBones.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		fbxRigBone * const bone = ( fbxRigBone* )pBones.GetAt( i );
		if( bone->GetNodeModelID() == id ){
			return bone;
		}
	}
	return NULL;
}



void fbxRig::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose ) const{
	const int count = pBones.GetCount();
	int i;
	
	module.LogInfoFormat( "%sRig:", prefix.GetString() );
	
	const decString childPrefix( prefix + "  " );
	for( i=0; i<count; i++ ){
		GetBoneAt( i )->DebugPrintStructure( module, childPrefix, verbose );
	}
}
