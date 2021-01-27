/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglOcclusionTracing.h"
#include "mesh/deoglROcclusionMesh.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../utils/collision/deoglCollisionBox.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>


// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionTracing::deoglOcclusionTracing( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pProbeSpacing( 1.0f ),
pProbeCount( 32, 4, 32 ),
pOccMeshes( NULL ),
pOccMeshCount( 0 ),
pOccMeshSize( 0 ),
pOccMeshInstances( NULL ),
pOccMeshInstanceCount( 0 ),
pOccMeshInstanceSize( 0 ),
pBVHBuildPrimitives( NULL ),
pBVHBuildPrimitiveSize( 0 ),
pTBONodeBox( renderThread, 4 ),
pTBOIndex( renderThread, 2 ),
pTBOInstance( renderThread, 1 ),
pTBOMatrix( renderThread, 4 ),
pTBOFace( renderThread, 4 ),
pBVHInstanceRootNode( 0 )
{
}

deoglOcclusionTracing::~deoglOcclusionTracing(){
	pCleanUp();
}



// Management
///////////////

// #define DO_TIMING 1

void deoglOcclusionTracing::Update( deoglRWorld &world, const decDVector &position ){
	#ifdef DO_TIMING
	decTimer timer;
	#endif
	
	pBVHInstances.Clear();
	pOccMeshInstanceCount = 0;
	pOccMeshCount = 0;
	
	pTBOFace.Clear();
	pTBOMatrix.Clear();
	pTBOInstance.Clear();
	pTBOIndex.Clear();
	pTBONodeBox.Clear();
	pBVHInstanceRootNode = 0;
	
	pPosition = pGrid2World( pWorld2Grid( position ) );
	pFindComponents( world );
	pAddOcclusionMeshes();
	
	pFinish();
	
	#ifdef DO_TIMING
	pRenderThread.GetLogger().LogInfoFormat("OcclusionTracing: Elapsed %.1fys", timer.GetElapsedTime() * 1e6f);
	#endif
}



// Private Functions
//////////////////////

void deoglOcclusionTracing::pCleanUp(){
	if( pBVHBuildPrimitives ){
		delete [] pBVHBuildPrimitives;
	}
	if( pOccMeshInstances ){
		delete [] pOccMeshInstances;
	}
	if( pOccMeshes ){
		delete [] pOccMeshes;
	}
}

decPoint3 deoglOcclusionTracing::pWorld2Grid( const decDVector &position ) const{
	const double factor = 1.0 / pProbeSpacing;
	return decPoint3( position * factor + decDVector( 0.5, 0.5, 0.5 ) );
}

decDVector deoglOcclusionTracing::pGrid2World( const decPoint3 &position ) const{
	return decDVector( position ) * pProbeSpacing;
}

void deoglOcclusionTracing::pFindComponents( deoglRWorld &world ){
	const decDVector size( pProbeSpacing * pProbeCount.x, pProbeSpacing * pProbeCount.y, pProbeSpacing * pProbeCount.z );
	deoglDCollisionBox colbox( pPosition, size * 0.5 );
	
	pCollideList.Clear();
	pCollideList.AddComponentsColliding( world.GetOctree(), &colbox );
}

void deoglOcclusionTracing::pAddOcclusionMeshes(){
	const decDMatrix matrix( decDMatrix::CreateTranslation( -pPosition ) );
	const int count = pCollideList.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglRComponent &component = *pCollideList.GetComponentAt( i )->GetComponent();
		if( component.GetOcclusionMesh() && ! component.GetDynamicOcclusionMesh() ){
			pAddOcclusionMesh( ( component.GetMatrix() * matrix ).ToMatrix(), component.GetOcclusionMesh() );
		}
	}
}

void deoglOcclusionTracing::pAddOcclusionMesh( const decMatrix &matrix, deoglROcclusionMesh *occlusionMesh ){
	// find occlusion mesh
	int indexOccMesh;
	for( indexOccMesh=0; indexOccMesh<pOccMeshCount; indexOccMesh++ ){
		if( pOccMeshes[ indexOccMesh ].occlusionMesh == occlusionMesh ){
			break;
		}
	}
	
	// if occlusion mesh does not exist add it
	if( indexOccMesh == pOccMeshCount ){
		// prepare BVH first just in case something goes wrong
		occlusionMesh->PrepareBVH();
		
		if( ! occlusionMesh->GetBVH()->GetRootNode() ){
			return; // empty occlusion mesh
		}
		
		// add occlusion mesh to list of encountered occlusion meshes
		if( pOccMeshCount == pOccMeshSize ){
			const int newSize = pOccMeshCount + 10;
			sOccMesh * const newArray = new sOccMesh[ newSize ];
			if( pOccMeshes ){
				memcpy( newArray, pOccMeshes, sizeof( sOccMesh ) * pOccMeshCount );
				delete [] pOccMeshes;
			}
			pOccMeshes = newArray;
			pOccMeshSize = newSize;
		}
		
		sOccMesh &occMesh = pOccMeshes[ pOccMeshCount++ ];
		occMesh.occlusionMesh = occlusionMesh;
		occMesh.indexFaces = pTBOFace.GetPixelCount() / 3;
		occMesh.indexNodes = pTBOIndex.GetPixelCount();
		
		// add faces to TBOs using primitive mapping from BVH
		const deoglBVH &bvh = *occlusionMesh->GetBVH();
		const int * const primitives = bvh.GetPrimitives();
		const int primitiveCount = bvh.GetPrimitiveCount();
		
		const deoglROcclusionMesh::sVertex * const vertices = occlusionMesh->GetVertices();
		const int singleSidedFaceCount = occlusionMesh->GetSingleSidedFaceCount();
		const unsigned short * const corners = occlusionMesh->GetCorners();
		int i;
		
		for( i=0; i<primitiveCount; i++ ){
			const int faceIndex = primitives[ i ];
			const unsigned short * const faceCorners = corners + 3 * faceIndex;
			const float doubleSided = faceIndex < singleSidedFaceCount ? 0.0f : 1.0f;
			pTBOFace.AddVec4( vertices[ faceCorners[ 0 ] ].position, doubleSided );
			pTBOFace.AddVec4( vertices[ faceCorners[ 1 ] ].position, doubleSided );
			pTBOFace.AddVec4( vertices[ faceCorners[ 2 ] ].position, doubleSided );
		}
		
		// add BVH to TBOs
		const int nodeCount = bvh.GetNodeCount();
		const deoglBVHNode * const nodes = bvh.GetNodes();
		
		for( i=0; i<nodeCount; i++ ){
			const deoglBVHNode &node = nodes[ i ];
			pTBONodeBox.AddVec4( node.GetMinExtend(), 0.0f );
			pTBONodeBox.AddVec4( node.GetMaxExtend(), 0.0f );
			
			if( node.GetPrimitiveCount() == 0 ){
				pTBOIndex.AddVec2( occMesh.indexNodes + node.GetFirstIndex(), 0 );
				
			}else{
				pTBOIndex.AddVec2( occMesh.indexFaces + node.GetFirstIndex(), node.GetPrimitiveCount() );
			}
		}
	}
	
	// add instance to TBO
	if( pOccMeshInstanceCount == pOccMeshInstanceSize ){
		const int newSize = pOccMeshInstanceCount + 10;
		sOccMeshInstance * const newArray = new sOccMeshInstance[ newSize ];
		if( pOccMeshInstances ){
			memcpy( newArray, pOccMeshInstances, sizeof( sOccMeshInstance ) * pOccMeshInstanceCount );
			delete [] pOccMeshInstances;
		}
		pOccMeshInstances = newArray;
		pOccMeshInstanceSize = newSize;
	}
	
	sOccMeshInstance &occMeshInst = pOccMeshInstances[ pOccMeshInstanceCount++ ];
	occMeshInst.indexMesh = indexOccMesh;
	occMeshInst.indexInstance = 0;
	occMeshInst.indexMatrix = 0;
	occMeshInst.matrix = matrix;
}

#if 0
bool deoglOcclusionTracing::pAddFace( const decVector& v1, const decVector& v2, const decVector& v3, bool doubleSided ){
	const decVector edge1( v2 - v1 );
	const decVector edge2( v3 - v2 );
	const decVector edge3( v1 - v3 );
	const float edge1Len = edge1.Length();
	const float edge2Len = edge2.Length();
	const float edge3Len = edge3.Length();
	if( edge1Len < 0.001f || edge2Len < 0.001f || edge3Len < 0.001f ){
		return false;
	}
	
	const decVector normEdge1( edge1 / edge1Len );
	const decVector normEdge2( edge2 / edge2Len );
	const decVector normEdge3( edge3 / edge3Len );
	
	const decVector faceNormal( normEdge1 % normEdge2 );
	if( faceNormal.IsZero( 0.001f ) ){
		return false;
	}
	
	pTBOPrimitiveData.AddVec3( faceNormal );
	pTBOPrimitiveData.AddFloat( faceNormal * v1 );
	
	const decVector edge1Normal( faceNormal % normEdge1 );
	tbo.AddVec3( edge1Normal );
	tbo.AddFloat( edge1Normal * v1 );
	
	const decVector edge2Normal( faceNormal % normEdge2 );
	tbo.AddVec3( edge2Normal );
	tbo.AddFloat( edge2Normal * v2 );
	
	const decVector edge3Normal( faceNormal % normEdge3 );
	tbo.AddVec3( edge3Normal );
	tbo.AddFloat( edge3Normal * v3 );
	
	return true;
}
#endif

void deoglOcclusionTracing::pFinish(){
	// build BVH from mesh instances. each mesh instance then uses the occlusion mesh bvh
	// with the matrix to do the local tracing. this allows creating occlusion mesh BVH
	// once and then copy it to GPU as required.
	// 
	// NOTE texture formats are restricted so something like RGB16F doesn't even exist.
	//      furthermore 3-component versions exists only with OGL 4.0 onwards
	// 
	// NOTE use depth 4 for instance BVH and depth 12 for mesh BVH
	// 
	// requires these TBO to be build:
	// 
	// - TBONodeBox: RGBA16F (stride 2 pixels)
	//   stores bounding box of all bvh nodes. each node uses two pixels: minExtend(0:RGB)
	//   maxExtend(1:RGB). one component has to be wasted in each pixel due to format
	//   restrictions. contains package nodes of all mesh BVH then nodes of instance BVH.
	//   
	// - TBOIndex: RG32UI (stride 1 pixel)
	//   stores bvh node indiced.
	//   1) all mesh bvh indices. firstIndex(R) primitiveCount(G). if leaf node points to mesh
	//      faces in TBOPrimitiveData (absolute strided index). otherwise points to first child
	//      node in TBONodeBox/TBOIndex (absolute strided index).
	//   2) all instance bvh indices. firstIndex(R) primitiveCount(G). points to next node in
	//      TBONodeBox/TBOIndex (absolute strided index). for child nodes stays in instance BVH.
	//      for leaf nodes switches to mesh BVH traversal. points into TBOInstance and TBOMatrix.
	// 
	// - TBOInstance: R32UI (stride 1 pixel)
	//   stores instance offsets. bvhIndex(R) is the absolute strided index into TBONodeBox
	//   and TBOIndex with the mesh bvh root node.
	//   
	// - TBOMatrix: RGBA16F (stride 3 pixels)
	//   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
	//   
	// - TBOFace: RGBA16F (stride 3 pixels)
	//   stores mesh faces. vertex1(0:RGB) vertex2(1:RGB) vertex3(2:RGB) doubleSided(0:A).
	//   vertices are transformed by "current BVH transformation matrix". face is doubleSided
	//   if doubleSided has value 1 or single sided if value is 0.
	//   
	// requires uniforms:
	// 
	// - uint IndexRootNode: index into TBONodeBox/TBOIndex containing instance BVH root node.
	
	// build instance bvh
	if( pOccMeshInstanceCount > pBVHBuildPrimitiveSize ){
		deoglBVH::sBuildPrimitive * const newArray = new deoglBVH::sBuildPrimitive[ pOccMeshInstanceCount ];
		if( pBVHBuildPrimitives ){
			delete [] pBVHBuildPrimitives;
		}
		pBVHBuildPrimitives = newArray;
		pBVHBuildPrimitiveSize = pOccMeshInstanceCount;
	}
	
	int i;
	for( i=0; i<pOccMeshInstanceCount; i++ ){
		deoglBVH::sBuildPrimitive &primitive = pBVHBuildPrimitives[ i ];
		const sOccMeshInstance &instance = pOccMeshInstances[ i ];
		const sOccMesh &mesh = pOccMeshes[ instance.indexMesh ];
		const deoglBVH &bvh = *mesh.occlusionMesh->GetBVH();
		const deoglBVHNode &rootNode = *bvh.GetRootNode();
		
		const decVector &minExtend = rootNode.GetMinExtend();
		const decVector &maxExtend = rootNode.GetMaxExtend();
		const decVector center( ( minExtend + maxExtend ) * 0.5f );
		const decVector halfSize( ( maxExtend - minExtend ) * 0.5f );
		deoglCollisionBox box( instance.matrix * center,
			instance.matrix.GetScale().Multiply( halfSize ), \
			instance.matrix.Normalized().ToQuaternion() );
		deoglCollisionBox enclosing;
		box.GetEnclosingBox( &enclosing );
		
		primitive.center = enclosing.GetCenter();
		primitive.minExtend = primitive.center - enclosing.GetHalfSize();
		primitive.maxExtend = primitive.center + enclosing.GetHalfSize();
	}
	
	pBVHInstances.Build( pBVHBuildPrimitives, pOccMeshInstanceCount, 4 );
	
	// add to TBOs using primitive mapping from BVH
	const int * const primitives = pBVHInstances.GetPrimitives();
	
	for( i=0; i<pOccMeshInstanceCount; i++ ){
		sOccMeshInstance &instance = pOccMeshInstances[ primitives[ i ] ];
		instance.indexMatrix = pTBOMatrix.GetPixelCount() / 3;
		pTBOMatrix.AddMat3x4( instance.matrix );
		
		instance.indexInstance = pTBOInstance.GetPixelCount();
		pTBOInstance.AddInt( pOccMeshes[ instance.indexMesh ].indexNodes );
	}
	
	// add BVH to TBOs
	const int nodeCount = pBVHInstances.GetNodeCount();
	const deoglBVHNode * const nodes = pBVHInstances.GetNodes();
	
	pBVHInstanceRootNode = pTBOIndex.GetPixelCount();
	
	for( i=0; i<nodeCount; i++ ){
		const deoglBVHNode &node = nodes[ i ];
		pTBONodeBox.AddVec4( node.GetMinExtend(), 0.0f );
		pTBONodeBox.AddVec4( node.GetMaxExtend(), 0.0f );
		
		if( node.GetPrimitiveCount() == 0 ){
			pTBOIndex.AddVec2( pBVHInstanceRootNode + node.GetFirstIndex(), 0 );
			
		}else{
			pTBOIndex.AddVec2( node.GetFirstIndex(), node.GetPrimitiveCount() );
		}
	}
	
	// update TBOs
	pTBONodeBox.Update();
	pTBOIndex.Update();
	pTBOInstance.Update();
	pTBOMatrix.Update();
	pTBOFace.Update();
	
	// DEBUG
	if(false){
		deoglRTLogger &logger = pRenderThread.GetLogger();
		int i;
		logger.LogInfoFormat("OcclusionTracing: %d OcclusionMeshes", pOccMeshCount);
		for(i=0; i<pOccMeshCount; i++){
			logger.LogInfoFormat("- %d: indexNodes=%d indexFaces=%d path=%s", i,
				pOccMeshes[i].indexNodes, pOccMeshes[i].indexFaces,
				pOccMeshes[i].occlusionMesh->GetFilename().GetString());
		}
		logger.LogInfoFormat("OcclusionTracing: %d Instances", pOccMeshInstanceCount);
		for(i=0; i<pOccMeshInstanceCount; i++){
			const decDVector p(pPosition + pOccMeshInstances[i].matrix.GetPosition());
			logger.LogInfoFormat("- %d: indexMatrix=%d indexMesh=%d indexInstance=%d position=(%g,%g,%g)",
				i, pOccMeshInstances[i].indexMatrix, pOccMeshInstances[i].indexMesh,
				pOccMeshInstances[i].indexInstance, p.x, p.y, p.z);
		}
		logger.LogInfoFormat("OcclusionTracing: Root Node %d", pBVHInstanceRootNode);
		logger.LogInfo("OcclusionTracing: TBONodeBox");
		pTBONodeBox.DebugPrint();
		logger.LogInfo("OcclusionTracing: TBOIndex");
		pTBOIndex.DebugPrint();
		logger.LogInfo("OcclusionTracing: TBOInstance");
		pTBOInstance.DebugPrint();
		logger.LogInfo("OcclusionTracing: TBOMatrix");
		pTBOMatrix.DebugPrint();
		logger.LogInfo("OcclusionTracing: TBOFace");
		pTBOFace.DebugPrint();
		
		struct PrintBVH{
			deoglRTLogger &logger;
			const sOccMeshInstance *instances;
			PrintBVH(deoglRTLogger &logger, const sOccMeshInstance *instances) : logger(logger), instances(instances){
			}
			void Print(const decString &prefix, const deoglBVH &bvh, const deoglBVHNode &node) const{
				logger.LogInfoFormat("%sNode: (%g,%g,%g)-(%g,%g,%g)", prefix.GetString(),
					node.GetMinExtend().x, node.GetMinExtend().y, node.GetMinExtend().z,
					node.GetMaxExtend().x, node.GetMaxExtend().y, node.GetMaxExtend().z);
				if(node.GetPrimitiveCount() == 0){
					Print(prefix + "L ", bvh, bvh.GetNodeAt(node.GetFirstIndex()));
					Print(prefix + "R ", bvh, bvh.GetNodeAt(node.GetFirstIndex()+1));
				}else{
					for(int i=0; i<node.GetPrimitiveCount(); i++){
						const int index = bvh.GetPrimitiveAt(node.GetFirstIndex()+i);
						const decVector p(instances[index].matrix.GetPosition());
						const decVector r(instances[index].matrix.Normalized().GetEulerAngles() * RAD2DEG);
						logger.LogInfoFormat("%sP%03d position=(%g,%g,%g) rotation=(%g,%g,%g) mesh=%d",
							prefix.GetString(), i, p.x, p.y, p.z, r.x, r.y, r.z, instances[index].indexMesh);
					}
				}
			}
		};
		logger.LogInfo("OccMesh Instance BVH");
		if(pBVHInstances.GetRootNode()){
			PrintBVH(logger, pOccMeshInstances).Print("", pBVHInstances, *pBVHInstances.GetRootNode());
		}
	}
}
