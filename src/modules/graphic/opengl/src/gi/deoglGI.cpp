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

#include "deoglGI.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../utils/bvh/deoglBVH.h"
#include "../utils/bvh/deoglBVHNode.h"

#include <dragengine/common/exceptions.h>


// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglGI::deoglGI( deoglRenderThread &renderThread ) :
pRenderThread( renderThread  ),
pRaysPerProbe( 64 ),
pUBOTracing( NULL ),
pTBONodeBox( renderThread, 4 ),
pTBOIndex( renderThread, 2 ),
pTBOFace( renderThread, 4 ),
pTBOVertex( renderThread, 4 )
{
	try{
		pUBOTracing = new deoglSPBlockUBO( renderThread );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGI::~deoglGI(){
	pCleanUp();
}



// Management
///////////////

deoglSPBlockUBO &deoglGI::GetUBOTracing(){
	deoglSPBlockUBO &ubo = *pUBOTracing;
	
	if( ubo.GetParameterCount() == 0 ){
		ubo.SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		ubo.SetParameterCount( eutpOrigin + 1 );
		ubo.GetParameterAt( eutpProbeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpProbesPerLine ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpProbeSize ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 ); // int
		ubo.GetParameterAt( eutpResolution ).SetAll( deoglSPBParameter::evtInt, 3, 1, 1 ); // ivec3
		ubo.GetParameterAt( eutpSpacing ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		ubo.GetParameterAt( eutpOrigin ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 ); // vec3
		ubo.MapToStd140();
		ubo.SetBindingPoint( 0 );
	}
	
	return ubo;
}

void deoglGI::PrepareRayTracing( deoglROcclusionMesh &occlusionMesh ){
	pTBOVertex.Clear();
	pTBOFace.Clear();
	pTBOIndex.Clear();
	pTBONodeBox.Clear();
	
	// prepare BVH first just in case something goes wrong
	occlusionMesh.PrepareBVH();
	if( ! occlusionMesh.GetBVH()->GetRootNode() ){
		return; // empty occlusion mesh
	}
	
	// add vertices to TBO in mesh order
	const deoglROcclusionMesh::sVertex * const vertices = occlusionMesh.GetVertices();
	const int vertexCount = occlusionMesh.GetVertexCount();
	int i;
	
	for( i=0; i<vertexCount; i++ ){
		pTBOVertex.AddVec4( vertices[ i ].position, 0.0f );
	}
	
	// add faces to TBOs using primitive mapping from BVH
	const deoglBVH &bvh = *occlusionMesh.GetBVH();
	const int * const primitives = bvh.GetPrimitives();
	const int primitiveCount = bvh.GetPrimitiveCount();
	
	const int singleSidedFaceCount = occlusionMesh.GetSingleSidedFaceCount();
	const unsigned short * const corners = occlusionMesh.GetCorners();
	
	for( i=0; i<primitiveCount; i++ ){
		const int faceIndex = primitives[ i ];
		const unsigned short * const faceCorners = corners + 3 * faceIndex;
		const uint32_t doubleSided = faceIndex < singleSidedFaceCount ? 0 : 1;
		pTBOFace.AddVec4( faceCorners[ 0 ], faceCorners[ 1 ], faceCorners[ 2 ], doubleSided );
	}
	
	// add BVH to TBOs
	const int nodeCount = bvh.GetNodeCount();
	const deoglBVHNode * const nodes = bvh.GetNodes();
	
	for( i=0; i<nodeCount; i++ ){
		const deoglBVHNode &node = nodes[ i ];
		pTBONodeBox.AddVec4( node.GetMinExtend(), 0.0f );
		pTBONodeBox.AddVec4( node.GetMaxExtend(), 0.0f );
		
		if( node.GetPrimitiveCount() == 0 ){
			pTBOIndex.AddVec2( node.GetFirstIndex(), 0 );
			
		}else{
			pTBOIndex.AddVec2( node.GetFirstIndex(), node.GetPrimitiveCount() );
		}
	}
	
	// write TBOs
	pTBONodeBox.Update();
	pTBOIndex.Update();
	pTBOFace.Update();
	pTBOVertex.Update();
}



// Private Functions
//////////////////////

void deoglGI::pCleanUp(){
	if( pUBOTracing ){
		pUBOTracing->FreeReference();
	}
}

/*
- TBONodeBox: RGBA32F (stride 2 pixels)
  stores bounding box of bvh. each node uses two pixels: minExtend(0:RGB)
  maxExtend(1:RGB). one component has to be wasted in each pixel due to format
  restrictions. contains packed nodes of BVH.
  
- TBOIndex: RG32UI (stride 1 pixel)
  stores bvh node indiced. firstIndex(R) primitiveCount(G). if leaf node points to mesh
  faces in TBOPrimitiveData (absolute strided index). otherwise points to first child
  node in TBONodeBox/TBOIndex (absolute strided index).
  
- TBOFace: RGBA32UI (stride 1 pixel)
  stores mesh faces. vertex1(R) vertex2(G) vertex3(B) doubleSided(A). indices into
  TBOVertex. face is doubleSided if doubleSided has value 1 or single sided if value is 0.
  
- TBOVertex: RGBA32F (stride 1 pixel)
  stores mesh vertices. vertices are transformed by "current BVH transformation matrix"
*/
