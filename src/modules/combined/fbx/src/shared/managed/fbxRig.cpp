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

fbxRig::fbxRig( fbxScene &scene, fbxNode *nodePose ) :
pScene( scene ),
pNodePose( nodePose )
{
	if( nodePose ){
		pMatrix = nodePose->GetTransformation().QuickMultiply( scene.GetTransformation() );
		pMatrixInverse = pMatrix.QuickInvert();
	}
	
	/*
	const int childCount = scene.GetNodeObjects()->GetNodeCount();
	int i;
	
	for( i=0; i<childCount; i++ ){
		fbxNode &nodePoseBone = *scene.GetNodeObjects()->GetNodeAt( i );
		if( nodePoseBone.GetName() != "Model" || nodePoseBone.GetPropertyAt( 2 )->CastString().GetValue() != "LimbNode" ){
			continue;
		}
		
		const fbxRigBone::Ref bone( fbxRigBone::Ref::New( new fbxRigBone( *this, nodePoseBone, nodePoseBone ) ) );
		bone->SetIndex( pBones.GetCount() );
		
		if( ! GetBoneNamed( bone->GetName() ) ){
			pBones.Add( bone );
		}
	}
	*/
	
	int i;
	
	if( nodePose ){
		const int childCount = nodePose->GetNodeCount();
		
		for( i=0; i<childCount; i++ ){
			fbxNode &nodePoseBone = *nodePose->GetNodeAt( i );
			if( nodePoseBone.GetName() != "PoseNode" ){
				continue;
			}
			
			fbxNode &nodeModel = *scene.NodeWithID( nodePoseBone.FirstNodeNamed( "Node" )->GetID() );
			if( nodeModel.GetPropertyAt( 2 )->CastString().GetValue() == "LimbNode" ){
				const fbxRigBone::Ref bone( fbxRigBone::Ref::New( new fbxRigBone( *this, nodePoseBone, nodeModel ) ) );
				bone->SetIndex( pBones.GetCount() );
				
				if( ! GetBoneNamed( bone->GetName() ) ){
					pBones.Add( bone );
				}
				
			}else if( nodeModel.GetPropertyAt( 2 )->CastString().GetValue() == "Root" ){
				const fbxRigBone::Ref bone( fbxRigBone::Ref::New( new fbxRigBone( *this, nodeModel, nodeModel ) ) );
				bone->SetIndex( pBones.GetCount() );
				
				if( ! GetBoneNamed( bone->GetName() ) ){
					pBones.Add( bone );
				}
				
				pAddRootBone( scene, nodeModel );
			}
		}
		
	}else{
		const int childCount = scene.GetNodeObjects()->GetNodeCount();
		
		for( i=0; i<childCount; i++ ){
			fbxNode &nodeModel = *scene.GetNodeObjects()->GetNodeAt( i );
			if( nodeModel.GetName() != "Model" ){
				continue;
			}
			
			if( nodeModel.GetPropertyAt( 2 )->CastString().GetValue() != "Root" ){
				continue;
			}
			
			const fbxRigBone::Ref bone( fbxRigBone::Ref::New( new fbxRigBone( *this, nodeModel, nodeModel ) ) );
			bone->SetIndex( pBones.GetCount() );
			
			if( ! GetBoneNamed( bone->GetName() ) ){
				pBones.Add( bone );
			}
			
			pAddRootBone( scene, nodeModel );
		}
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



// Private Functions
//////////////////////

void fbxRig::pAddRootBone( fbxScene &scene, fbxNode &nodeRoot ){
	decPointerList cons;
	scene.FindConnections( nodeRoot.GetID(), cons );
	
	const int childCount = cons.GetCount();
	int i;
	
	for( i=0; i<childCount; i++ ){
		const fbxConnection &con = *( fbxConnection* )cons.GetAt( i );
		if( con.GetTarget() != nodeRoot.GetID() ){
			continue;
		}
		
		fbxNode &nodeModel = *scene.NodeWithID( con.GetSource() );
		if( nodeModel.GetName() != "Model" ){
			continue;
		}
		
		if( nodeModel.GetPropertyAt( 2 )->CastString().GetValue() != "LimbNode"
		&& nodeModel.GetPropertyAt( 2 )->CastString().GetValue() != "Root" ){
			continue;
		}
		
		const fbxRigBone::Ref bone( fbxRigBone::Ref::New( new fbxRigBone( *this, nodeModel, nodeModel ) ) );
		bone->SetIndex( pBones.GetCount() );
		
		if( ! GetBoneNamed( bone->GetName() ) ){
			pBones.Add( bone );
		}
		
		pAddRootBone( scene, nodeModel );
	}
}
