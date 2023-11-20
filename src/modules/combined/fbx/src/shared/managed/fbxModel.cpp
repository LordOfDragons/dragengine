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

#include "fbxModel.h"
#include "fbxModelCluster.h"
#include "fbxRig.h"
#include "fbxRigBone.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxModel
///////////////////

// Constructor, destructor
////////////////////////////

fbxModel::fbxModel( fbxScene &scene, fbxNode &nodeGeometry ) :
pScene( scene ),
pNodeGeomtry( nodeGeometry ),
pNodeModel( NULL ),
pNodeDeformer( NULL ),
pGeometryID( nodeGeometry.GetID() ),
pModelID( 0 ),
pDeformerID( 0 ),
pVertexWeights( NULL ),
pVertexWeightCount( 0 ),
pVertexWeightSize( 0 ),
pWeights( NULL ),
pWeightCount( 0 ),
pWeightSize( 0 ),
pWeightMatchThreshold( 0.001f ),
pVertices( NULL ),
pVertexCount( 0 ),
pCulling( true )
{
	decPointerList consGeometry;
	scene.FindConnections( pGeometryID, consGeometry );
	
	const int conGeomCount = consGeometry.GetCount();
	int i;
	
	for( i=0; i<conGeomCount; i++ ){
		const fbxConnection &connection = *( ( fbxConnection* )consGeometry.GetAt( i ) );
		fbxNode * const node = scene.NodeWithID( connection.OtherID( pGeometryID ) );
		
		if( node->GetName() == "Model" ){
			if( connection.GetSource() == pGeometryID
			&& node->GetPropertyAt( 2 )->CastString().GetValue() == "Mesh" ){
				if( pNodeModel ){
					DETHROW_INFO( deeInvalidParam, "duplicate model" );
				}
				pNodeModel = node;
				pModelID = connection.GetTarget();
			}
			
		}else if( node->GetName() == "Deformer" ){
			if( connection.GetTarget() == pGeometryID
			&& node->GetPropertyAt( 2 )->CastString().GetValue() == "Skin" ){
				if( pNodeDeformer ){
					DETHROW_INFO( deeInvalidParam, "duplicate deformer" );
				}
				pNodeDeformer = node;
				pDeformerID = connection.GetSource();
			}
		}
	}
	
	if( ! pNodeModel ){
		DETHROW_INFO( deeInvalidParam, "model not found" );
	}
	
	pMatrix = pNodeModel->GetTransformation();
	if( pNodeDeformer ){
		pMatrix = pMatrix.QuickMultiply( pNodeDeformer->GetTransformation() );
	}
	pMatrix = pMatrix.QuickMultiply( scene.GetTransformation() );
	
	if( pNodeDeformer ){
		deObjectReference refCluster;
		decPointerList consDeformer;
		scene.FindConnections( pDeformerID, consDeformer );
		
		const int conDeformCount = consDeformer.GetCount();
		for( i=0; i<conDeformCount; i++ ){
			const fbxConnection &connection = *( ( fbxConnection* )consDeformer.GetAt( i ) );
			if( connection.GetTarget() != pDeformerID ){
				continue;
			}
			
			fbxNode &node = *scene.NodeWithID( connection.GetSource() );
			if( node.GetName() == "Deformer"
			&& node.GetPropertyAt( 2 )->CastString().GetValue() == "Cluster" ){
				refCluster.TakeOver( new fbxModelCluster( *this, node ) );
				pClusters.Add( refCluster );
			}
		}
	}
	
	const int clusterCount = pClusters.GetCount();
	for( i=0; i<clusterCount; i++ ){
		GetClusterAt( i )->Prepare();
	}
	
	// create vertices
	const fbxProperty &propVertices = *nodeGeometry.FirstNodeNamed( "Vertices" )->GetPropertyAt( 0 );
	const int count = propVertices.GetValueCount() / 3;
	
	if( count > 0 ){
		pVertices = new sVertex[ count ];
		
		int j;
		for( i=0, j=0; i<count; i++, j+=3 ){
			pVertices[ i ].weightSet = -1;
			pVertices[ i ].position = pMatrix * decVector(
				propVertices.GetValueAtAsFloat( j ),
				propVertices.GetValueAtAsFloat( j + 1 ),
				propVertices.GetValueAtAsFloat( j + 2 ) );
		}
		pVertexCount = count;
	}
	
	// properties
	const fbxNode * const nodeCulling = pNodeModel->FirstNodeNamedOrNull( "Culling" );
	if( nodeCulling ){
		pCulling = nodeCulling->GetPropertyAt( 0 )->CastString().GetValue() != "CullingOff";
	}
}

fbxModel::~fbxModel(){
	if( pVertices ){
		delete [] pVertices;
	}
	if( pWeights ){
		delete [] pWeights;
	}
	if( pVertexWeights ){
		delete [] pVertexWeights;
	}
}



// Management
///////////////

int fbxModel::GetClusterCount() const{
	return pClusters.GetCount();
}

fbxModelCluster *fbxModel::GetClusterAt( int index ) const{
	return ( fbxModelCluster* )pClusters.GetAt( index );
}

fbxModelCluster *fbxModel::GetClusterNamed( const char *name ) const{
	const int count = pClusters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		fbxModelCluster * const cluster = ( fbxModelCluster* )pClusters.GetAt( i );
		if( cluster->GetRigBone() && cluster->GetRigBone()->GetName() == name ){
			return cluster;
		}
	}
	return NULL;
}

void fbxModel::MatchClusters( const fbxRig &rig ){
	const int count = pClusters.GetCount();
	decPointerList connections;
	int i, j;
	
	for( i=0; i<count; i++ ){
		fbxModelCluster &cluster = *( ( fbxModelCluster* )( deObject* )pClusters.GetAt( i ) );
		
		connections.RemoveAll();
		
		pScene.FindConnections( cluster.GetNodeClusterID(), connections );
		const int conCount = connections.GetCount();
		
		for( j=0; j<conCount; j++ ){
			const fbxConnection &connection = *( ( fbxConnection* )connections.GetAt( j ) );
			if( connection.GetTarget() != cluster.GetNodeClusterID() || connection.GetSource() == 0 ){
				continue;
			}
			
			cluster.SetRigBone( rig.GetBoneWithModelID( connection.GetSource() ) );
			if( cluster.GetRigBone() ){
				break;
			}
		}
	}
}

void fbxModel::BuildWeights(){
	const int count = pClusters.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		fbxModelCluster &cluster = *( ( fbxModelCluster* )( deObject* )pClusters.GetAt( i ) );
		if( ! cluster.GetRigBone() ){
			continue;
		}
		
		//const fbxScene::eWeightMode weightMode = fbxScene::ConvWeightMode( cluster.GetNodeCluster() );
		
		const fbxNode * const nodeIndex = cluster.GetNodeCluster().FirstNodeNamedOrNull( "Indexes" );
		const fbxNode * const nodeWeight = cluster.GetNodeCluster().FirstNodeNamedOrNull( "Weights" );
		if( ! nodeIndex || ! nodeWeight ){
			continue; // rig affects nobody
		}
		
		const fbxProperty &propIndex = *nodeIndex->GetPropertyAt( 0 );
		const fbxProperty &propWeight = *nodeWeight->GetPropertyAt( 0 );
		const int entryCount = propIndex.GetValueCount();
		const int bone = cluster.GetRigBone()->GetIndex();
		
		for( j=0; j<entryCount; j++ ){
			const int index = propIndex.GetValueAtAsInt( j );
			const float weight = propWeight.GetValueAtAsFloat( j );
			pVertices[ index ].weights.Add( pAddVertexWeight( bone, weight ) );
		}
	}
	
	for( i=0; i<pVertexCount; i++ ){
		decIntList &weights = pVertices[ i ].weights;
		const int weightCount = weights.GetCount();
		float weightSum = 0.0f;
		
		for( j=0; j<weightCount; j++ ){
			weightSum += pVertexWeights[ weights.GetAt( j ) ].GetWeight();
		}
		
		if( weightSum < 0.001f ){
			pVertices[ i ].weightSet = -1;
			continue;
		}
		
		const float factor = 1.0f / weightSum;
		for( j=0; j<weightCount; j++ ){
			const deModelWeight &weight = pVertexWeights[ weights.GetAt( j ) ];
			pVertices[ i ].weights.SetAt( j, pAddWeight( weight.GetBone(), weight.GetWeight() * factor ) );
		}
	}
	
	int maxWGWeightCount = 0;
	for( i=0; i<pVertexCount; i++ ){
		maxWGWeightCount = decMath::max( maxWGWeightCount, pVertices[ i ].weights.GetCount() );
	}
	
	for( i=0; i<maxWGWeightCount; i++ ){
		const int firstWeightSet = pWeightSetsFirstWeight.GetCount();
		const int wgWeightCount = i + 1;
		
		for( j=0; j<pVertexCount; j++ ){
			if( wgWeightCount == pVertices[ j ].weights.GetCount() ){
				pVertices[ j ].weightSet = pAddWeightSet( pVertices[ j ].weights );
			}
		}
		
		pWeightGroupsSetCount.Add( pWeightSetsFirstWeight.GetCount() - firstWeightSet );
	}
}

const deModelWeight &fbxModel::GetWeightAt( int index ) const{
	if( index < 0 || index >= pWeightCount ){
		DETHROW( deeInvalidParam );
	}
	return pWeights[ index ];
}



void fbxModel::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose ) const{
	const int count = pClusters.GetCount();
	int i;
	
	module.LogInfoFormat( "%sModel", prefix.GetString() );
	
	const decString childPrefix( prefix + "  " );
	for( i=0; i<count; i++ ){
		GetClusterAt( i )->DebugPrintStructure( module, childPrefix, verbose );
	}
}



// Private Functions
//////////////////////

int fbxModel::pAddVertexWeight( int bone, float weight ){
	if( pVertexWeightCount == pVertexWeightSize ){
		const int newSize = pVertexWeightSize * 3 / 2 + 1;
		deModelWeight * const newArray = new deModelWeight[ newSize ];
		if( pVertexWeights ){
			memcpy( newArray, pVertexWeights, sizeof( deModelWeight ) * pVertexWeightSize );
			delete [] pVertexWeights;
		}
		pVertexWeights = newArray;
		pVertexWeightSize = newSize;
	}
	
	const int index = pVertexWeightCount++;
	pVertexWeights[ index ].SetBone( bone );
	pVertexWeights[ index ].SetWeight( weight );
	return index;
}

int fbxModel::pAddWeight( int bone, float weight ){
	// find matching weight
	int i;
	for( i=0; i<pWeightCount; i++ ){
		if( pWeights[ i ].GetBone() == bone
		&& fabsf( pWeights[ i ].GetWeight() - weight ) <= pWeightMatchThreshold ){
			return i;
		}
	}
	
	// add weight
	if( pWeightCount == pWeightSize ){
		const int newSize = pWeightSize * 3 / 2 + 1;
		deModelWeight * const newArray = new deModelWeight[ newSize ];
		if( pWeights ){
			memcpy( newArray, pWeights, sizeof( deModelWeight ) * pWeightSize );
			delete [] pWeights;
		}
		pWeights = newArray;
		pWeightSize = newSize;
	}
	
	const int index = pWeightCount++;
	pWeights[ index ].SetBone( bone );
	pWeights[ index ].SetWeight( weight );
	return index;
}

int fbxModel::pAddWeightSet( const decIntList &weights ){
	// find matching weight set
	const int count = pWeightSetsFirstWeight.GetCount();
	const int weightCount = weights.GetCount();
	int i, j;
	
	for( i=0; i<count; i++ ){
		if( weightCount != pWeightSetsWeightsCount.GetAt( i ) ){
			continue;
		}
		
		const int setFirstWeight = pWeightSetsFirstWeight.GetAt( i );
		for( j=0; j<weightCount; j++ ){
			if( ! weights.Has( pWeightSetWeights.GetAt( setFirstWeight + j ) ) ){
				break;
			}
		}
		
		if( j == weightCount ){
			return i;
		}
	}
	
	// add weight set
	const int firstWeight = pWeightSetWeights.GetCount();
	for( i=0; i<weightCount; i++ ){
		pWeightSetWeights.Add( weights.GetAt( i ) );
	}
	pWeightSetsFirstWeight.Add( firstWeight );
	pWeightSetsWeightsCount.Add( weightCount );
	return count;
}
