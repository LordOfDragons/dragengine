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
#include "../rendering/deoglRenderOcclusion.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../utils/collision/deoglCollisionBox.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionTracing::deoglOcclusionTracing( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pProbeSpacing( 1.0f, 1.0f, 1.0f ),
pProbeCount( 32, 4, 32 ),
pProbeOrigin( pProbeSpacing.Multiply( decVector( pProbeCount - decPoint3( 1, 1, 1 ) ) * -0.5f ) ),
pStrideProbeCount( pProbeCount.x * pProbeCount.z ),
pRaysPerProbe( 64 ),
pMaxUpdateProbeCount( 256 ), // keep at power of two
pProbesPerLine( 4 ), // 16 for 4k probes
pSampleImageSize( 0, 0 ),
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
pTBOVertex( renderThread, 4 ),
pBVHInstanceRootNode( 0 ),
pElapsedUpdateProbe( 0.0f ),
pUpdateProbeInterval( 0.1f ),
pUpdateProbes( NULL ),
pUpdateProbeCount( 0 ),
pUBOTracing( NULL ),
pTexRayOrigin( renderThread ),
pTexRayDirection( renderThread ),
pFBORay( renderThread, false ),
pTexProbeOcclusion( renderThread ),
pTexProbeDistance( renderThread ),
pFBOProbe( renderThread, false )
{
	pUpdateProbes = new int[ pMaxUpdateProbeCount ];
	pUBOTracing = new deoglSPBlockUBO( renderThread );
}

deoglOcclusionTracing::~deoglOcclusionTracing(){
	pCleanUp();
}



// Management
///////////////

// #define DO_TIMING 1

decDVector deoglOcclusionTracing::WorldClosestGrid(const decDVector &position ) const{
	decDVector result( ( double )pProbeSpacing.x * floor( position.x / ( double )pProbeSpacing.x ),
		( double )pProbeSpacing.y * floor( position.y / ( double )pProbeSpacing.y ),
		( double )pProbeSpacing.z * floor( position.z / ( double )pProbeSpacing.z ) );
	
	const decDVector halfGrid( decDVector( pProbeSpacing ) * 0.5 );
	if( position.x - result.x >= halfGrid.x ){
		result.x += ( double )pProbeSpacing.x;
	}
	if( position.y - result.y >= halfGrid.y ){
		result.y += ( double )pProbeSpacing.y;
	}
	if( position.z - result.z >= halfGrid.z ){
		result.z += ( double )pProbeSpacing.z;
	}
	
	return result;
}

decPoint3 deoglOcclusionTracing::ProbeIndex2GridCoord( int index ) const{
	decPoint3 coord;
	coord.y = index / pStrideProbeCount;
	index -= pStrideProbeCount * coord.y;
	coord.z = index / pProbeCount.x;
	coord.x = index - pProbeCount.x * coord.z;
	return coord;
}

int deoglOcclusionTracing::GridCoord2ProbeIndex( const decPoint3 &coord ) const{
	return pStrideProbeCount * coord.y + pProbeCount.x * coord.z + coord.x;
}

decVector deoglOcclusionTracing::Grid2Local( const decPoint3 &coord ) const{
	return decVector( pProbeSpacing.x * coord.x, pProbeSpacing.y * coord.y, pProbeSpacing.z * coord.z ) + pProbeOrigin;
}

/*
decPoint3 deoglOcclusionTracing::World2Grid( const decDVector &p ) const{
	return decPoint3( p.x / pProbeSpacing.x + 0.5, p.y / pProbeSpacing.y + 0.5, p.z / pProbeSpacing.z + 0.5 );
}

decDVector deoglOcclusionTracing::Grid2World( const decPoint3 &p ) const{
	return decDVector( pProbeSpacing.x * p.x, pProbeSpacing.y * p.y, pProbeSpacing.z * p.z );
}
*/



void deoglOcclusionTracing::Update( deoglRWorld &world, const decDVector &position ){
	#ifdef DO_TIMING
	decTimer timer;
	#endif
	
	pBVHInstances.Clear();
	pOccMeshInstanceCount = 0;
	pOccMeshCount = 0;
	
	pTBOVertex.Clear();
	pTBOFace.Clear();
	pTBOMatrix.Clear();
	pTBOInstance.Clear();
	pTBOIndex.Clear();
	pTBONodeBox.Clear();
	pBVHInstanceRootNode = 0;
	
	pUpdateProbeCount = 0;
	
	pUpdatePosition( position );
	pFindComponents( world );
	pAddOcclusionMeshes();
	pWriteTBOs();
	pTraceProbes();
	
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
	if( pUpdateProbes ){
		delete [] pUpdateProbes;
	}
	if( pUBOTracing ){
		pUBOTracing->FreeReference();
	}
}

void deoglOcclusionTracing::pUpdatePosition( const decDVector &position ){
	pPosition = WorldClosestGrid( position );
}

void deoglOcclusionTracing::pFindComponents( deoglRWorld &world ){
	deoglDCollisionBox colbox( pPosition, decDVector( ( double )pProbeSpacing.x * pProbeCount.x * 0.5,
		( double )pProbeSpacing.y * pProbeCount.y * 0.5, ( double )pProbeSpacing.z * pProbeCount.z * 0.5 ) );
	
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
		occMesh.indexVertices = pTBOVertex.GetPixelCount();
		occMesh.indexFaces = pTBOFace.GetPixelCount();
		occMesh.indexNodes = pTBOIndex.GetPixelCount();
		
		// add vertices to TBO in mesh order
		const deoglROcclusionMesh::sVertex * const vertices = occlusionMesh->GetVertices();
		const int vertexCount = occlusionMesh->GetVertexCount();
		int i;
		
		for( i=0; i<vertexCount; i++ ){
			pTBOVertex.AddVec4( vertices[ i ].position, 0.0f );
		}
		
		// add faces to TBOs using primitive mapping from BVH
		const deoglBVH &bvh = *occlusionMesh->GetBVH();
		const int * const primitives = bvh.GetPrimitives();
		const int primitiveCount = bvh.GetPrimitiveCount();
		
		const int singleSidedFaceCount = occlusionMesh->GetSingleSidedFaceCount();
		const unsigned short * const corners = occlusionMesh->GetCorners();
		
		for( i=0; i<primitiveCount; i++ ){
			const int faceIndex = primitives[ i ];
			const unsigned short * const faceCorners = corners + 3 * faceIndex;
			const uint32_t doubleSided = faceIndex < singleSidedFaceCount ? 0 : 1;
			pTBOFace.AddVec4( occMesh.indexVertices + faceCorners[ 0 ],
				occMesh.indexVertices + faceCorners[ 1 ],
				occMesh.indexVertices + faceCorners[ 2 ], doubleSided );
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

void deoglOcclusionTracing::pWriteTBOs(){
	// build BVH from mesh instances. each mesh instance then uses the occlusion mesh bvh
	// with the matrix to do the local tracing. this allows creating occlusion mesh BVH
	// once and then copy it to GPU as required.
	// 
	// NOTE texture formats are restricted so something like RGB32F doesn't even exist.
	//      furthermore 3-component versions exists only with OGL 4.0 onwards
	// 
	// NOTE 16-bit floating point has too little precision causing problems.
	// 
	// requires these TBO to be build:
	// 
	// - TBONodeBox: RGBA32F (stride 2 pixels)
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
	// - TBOMatrix: RGBA32F (stride 3 pixels)
	//   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
	//   
	// - TBOFace: RGBA32UI (stride 1 pixel)
	//   stores mesh faces. vertex1(R) vertex2(G) vertex3(B) doubleSided(A). indices into
	//   TBOVertex. face is doubleSided if doubleSided has value 1 or single sided if value is 0.
	//   
	// - TBOVertex: RGBA32F (stride 1 pixel)
	//   stores mesh vertices. vertices are transformed by "current BVH transformation matrix"
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
	
	pBVHInstances.Build( pBVHBuildPrimitives, pOccMeshInstanceCount, 12 );
	
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
	pTBOVertex.Update();
	
	// DEBUG
	//if(pPosition.IsEqualTo(decDVector(-4.619,2.139,34.115),1.0)){
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
		logger.LogInfo("OcclusionTracing: pTBOVertex");
		pTBOVertex.DebugPrint();
		
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

void deoglOcclusionTracing::pTraceProbes(){
	pElapsedUpdateProbe += pTimerUpdateProbe.GetElapsedTime();
	if( pElapsedUpdateProbe < pUpdateProbeInterval ){
// 		return; // how to use this properly? we track snapped position already
	}
	
	// keep only the remainder. this avoids updates to pile up if framerate drops or staggers
	pElapsedUpdateProbe = fmodf( pElapsedUpdateProbe, pUpdateProbeInterval );
	
	// TODO update/relocate grid
	
	pFindProbesToUpdate();
	pPrepareRayTexturesAndFBO();
	pPrepareProbeTexturesAndFBO();
	pPrepareUBOState();
	pRenderThread.GetRenderers().GetOcclusion().RenderOcclusionTraceProbes( *this );
}

static decVector deoglOcclusionTracing_SphericalFibonacci( float i, float n ){
	const float PHI = sqrtf( 5.0f ) * 0.5f + 0.5f;
	#define madfrac(A, B) ((A)*(B)-floor((A)*(B)))
	const float phi = TWO_PI * madfrac( i, PHI - 1.0f );
	const float cosTheta = 1.0f - ( 2.0f * i + 1.0f ) * ( 1.0f / n );
	const float sinTheta = sqrtf( decMath::clamp( 1.0f - cosTheta * cosTheta, 0.0f, 1.0f ) );
	return decVector( cosf( phi ) * sinTheta, sinf( phi ) * sinTheta, cosTheta );
	#undef madfrac
}

void deoglOcclusionTracing::pFindProbesToUpdate(){
	// TODO update probe grid. for the time being 8x4x8
	pUpdateProbeCount = 256;
	
	decPoint3 gi;
	int i = 0;
	for( gi.y=0; gi.y<4; gi.y++ ){
		for( gi.z=12; gi.z<20; gi.z++ ){
			for( gi.x=12; gi.x<20; gi.x++ ){
				pUpdateProbes[ i++ ] = GridCoord2ProbeIndex( gi );
			}
		}
	}
	
	// determine the required sample image size
	pSampleImageSize.x = pProbesPerLine * pRaysPerProbe;
	pSampleImageSize.y = ( pMaxUpdateProbeCount - 1 ) / pProbesPerLine + 1;
	//pSampleImageSize.y = ( pProbeCount.x * pProbeCount.y * pProbeCount.z - 1 ) / pProbesPerLine + 1;
}

void deoglOcclusionTracing::pInitSphericalFibonacci(){
	int i;
	for( i=0; i<pRaysPerProbe; i++ ){
		pSphericalFibonacci[ i ] = deoglOcclusionTracing_SphericalFibonacci( i, pRaysPerProbe );
	}
}

void deoglOcclusionTracing::pPrepareUBOState(){
	deoglSPBlockUBO &ubo = *pUBOTracing;
	
	if( ubo.GetParameterCount() == 0 ){
		ubo.SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		ubo.SetParameterCount( eutpRayDirection + 1 );
		ubo.GetParameterAt( eutpSampleImageScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		ubo.GetParameterAt( eutpProbeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpRaysPerProbe ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpProbesPerLine ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpBVHInstanceRootNode ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpGridProbeCount ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
		ubo.GetParameterAt( eutpGridProbeSpacing ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		ubo.GetParameterAt( eutpProbeIndex ).SetAll( deoglSPBParameter::evtInt, 4, 1, 64 ); // ivec4: max-count / 4
		ubo.GetParameterAt( eutpProbePosition ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 256 ); // vec3
		ubo.GetParameterAt( eutpRayDirection ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 64 ); // vec3
		ubo.MapToStd140();
		ubo.SetBindingPoint( 0 );
		
		pInitSphericalFibonacci();
	}
	
	int i, j, count;
	
	ubo.MapBuffer();
	try{
		ubo.SetParameterDataVec2( eutpSampleImageScale, 1.0f / ( float )( pSampleImageSize.x - 1 ),
			1.0f / ( float )( pSampleImageSize.y - 1 ) );
		ubo.SetParameterDataInt( eutpProbeCount, pMaxUpdateProbeCount );
		ubo.SetParameterDataInt( eutpRaysPerProbe, pRaysPerProbe );
		ubo.SetParameterDataInt( eutpProbesPerLine, pProbesPerLine );
		ubo.SetParameterDataInt( eutpBVHInstanceRootNode, pBVHInstanceRootNode );
		ubo.SetParameterDataIVec3( eutpGridProbeCount, pProbeCount );
		ubo.SetParameterDataVec3( eutpGridProbeSpacing, pProbeSpacing );
		
		count = pUpdateProbeCount / 4;
		for( i=0, j=0; i<count; i++, j+=4 ){
			ubo.SetParameterDataArrayIVec4( eutpProbeIndex, i,
				j < pUpdateProbeCount ? pUpdateProbes[ j ] : 0,
				j + 1 < pUpdateProbeCount ? pUpdateProbes[ j + 1 ] : 0,
				j + 2 < pUpdateProbeCount ? pUpdateProbes[ j + 2 ] : 0,
				j + 3 < pUpdateProbeCount ? pUpdateProbes[ j + 3 ] : 0 );
		}
		
		for( i=0; i<pUpdateProbeCount; i++ ){
			ubo.SetParameterDataArrayVec3( eutpProbePosition, i,
				Grid2Local( ProbeIndex2GridCoord( pUpdateProbes[ i ] ) ) );
		}
		
		const decMatrix randomOrientation( decMatrix::CreateRotation( decMath::random( -PI, PI ),
			decMath::random( -PI, PI ), decMath::random( -PI, PI ) ) );
		for( i=0; i<pRaysPerProbe; i++ ){
			ubo.SetParameterDataArrayVec3( eutpRayDirection, i, randomOrientation * pSphericalFibonacci[ i ] );
		}
		
		// DEBUG
		/*{
			for( i=0; i<pRaysPerProbe; i++ ){
				float pc = TWO_PI * i / pRaysPerProbe;
				decVector dir( sinf( pc ), 0.0f, cos( pc ) );
				ubo.SetParameterDataArrayVec3( eutpRayDirection, i, dir );
			}
			for( i=0; i<pUpdateProbeCount; i++ ){
				ubo.SetParameterDataArrayVec3( eutpProbePosition, i, (i%4)*3.0/4.0-1.5, 1, 0 );
			}
		}*/
		// DEBUG
		
	}catch( const deException & ){
		ubo.UnmapBuffer();
		throw;
	}
	ubo.UnmapBuffer();
}

void deoglOcclusionTracing::pPrepareRayTexturesAndFBO(){
	if( pTexRayOrigin.GetTexture() && pTexRayDirection.GetTexture()
	&& pTexRayOrigin.GetWidth() == pSampleImageSize.x && pTexRayOrigin.GetHeight() == pSampleImageSize.y ){
		return;
	}
	
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	pRenderThread.GetFramebuffer().Activate( &pFBORay );
	
	pFBORay.DetachAllImages();
	
	if( ! pTexRayOrigin.GetTexture() ){
		pTexRayOrigin.SetFBOFormatFloat32( 4 );
		pTexRayOrigin.SetSize( pSampleImageSize.x, pSampleImageSize.y );
		pTexRayOrigin.CreateTexture();
	}
	pFBORay.AttachColorTexture( 0, &pTexRayOrigin );
	
	if( ! pTexRayDirection.GetTexture() ){
		pTexRayDirection.SetFBOFormatFloat32( 4 );
		pTexRayDirection.SetSize( pSampleImageSize.x, pSampleImageSize.y );
		pTexRayDirection.CreateTexture();
	}
	pFBORay.AttachColorTexture( 1, &pTexRayDirection );
	
	const GLenum buffers[ 2 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 2, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	pFBORay.Verify();
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}

void deoglOcclusionTracing::pPrepareProbeTexturesAndFBO(){
	if( pTexProbeOcclusion.GetTexture() && pTexProbeDistance.GetTexture() ){
		return;
	}
	
	deoglFramebuffer * const oldfbo = pRenderThread.GetFramebuffer().GetActive();
	pRenderThread.GetFramebuffer().Activate( &pFBOProbe );
	
	pFBOProbe.DetachAllImages();
	
	if( ! pTexProbeOcclusion.GetTexture() ){
		pTexProbeOcclusion.SetFBOFormat( 1, true );
		pTexProbeOcclusion.SetSize( 256, 256 );
		pTexProbeOcclusion.CreateTexture();
	}
	pFBOProbe.AttachColorTexture( 0, &pTexProbeOcclusion );
	
	/*
	if( ! pTexProbeDistance.GetTexture() ){
		pTexProbeDistance.SetFBOFormat( 2, true );
		pTexProbeDistance.SetSize( 255, 255 );
		pTexProbeDistance.CreateTexture();
	}
	pFBOProbe.AttachColorTexture( 1, &pTexProbeDistance );
	*/
	
	const GLenum buffers[ 2 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	OGL_CHECK( pRenderThread, pglDrawBuffers( 1/*2*/, buffers ) );
	OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
	
	pFBOProbe.Verify();
	
	pRenderThread.GetFramebuffer().Activate( oldfbo );
}
