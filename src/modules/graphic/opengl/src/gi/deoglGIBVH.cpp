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

#include "deoglGIBVH.h"
#include "deoglGIInstance.h"
#include "deoglGIInstances.h"
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideList.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../component/deoglRComponentTexture.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/face/deoglModelFace.h"
#include "../model/texture/deoglModelTexture.h"
#include "../occlusiontest/mesh/deoglDynamicOcclusionMesh.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../rendering/light/deoglRenderLight.h"
#include "../rendering/light/deoglRenderGI.h"
#include "../rendering/task/deoglRenderTask.h"
#include "../rendering/task/deoglRenderTaskTexture.h"
#include "../rendering/task/deoglAddToRenderTaskGIMaterial.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../utils/collision/deoglCollisionBox.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIBVH::deoglGIBVH( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pMaxDetectionRange( 10.0f ),
pModels( NULL ),
pModelCount( 0 ),
pModelSize( 0 ),
pComponents( NULL ),
pComponentCount( 0 ),
pComponentSize( 0 ),
pPrimitives( NULL ),
pPrimitiveSize( 0 ),
pIndexRootNode( 0 ),
pTBONodeBox( renderThread, 4 ),
pTBOIndex( renderThread, 2 ),
pTBOInstance( renderThread, 4 ),
pTBOMatrix( renderThread, 4 ),
pTBOFace( renderThread, 4 ),
pTBOVertex( renderThread, 4 ),
pTBOTexCoord( renderThread, 2 ),
pTBOMaterial( renderThread, 4 ),
pTBOMaterial2( renderThread, 4 ){
}

deoglGIBVH::~deoglGIBVH(){
	if( pPrimitives ){
		delete [] pPrimitives;
	}
	if( pComponents ){
		delete [] pComponents;
	}
	if( pModels ){
		delete [] pModels;
	}
}



// Management
///////////////

void deoglGIBVH::Clear(){
	pBVH.Clear();
	pComponentCount = 0;
	pModelCount = 0;
	pIndexRootNode = -1;
	pTBOVertex.Clear();
	pTBOTexCoord.Clear();
	pTBOFace.Clear();
	pTBOMatrix.Clear();
	pTBOInstance.Clear();
	pTBOIndex.Clear();
	pTBONodeBox.Clear();
	pTBOMaterial.Clear();
	pTBOMaterial2.Clear();
}

void deoglGIBVH::AddComponents( deoglRenderPlan &plan, const decDVector &position, const deoglGIInstances &instances ){
	const decDMatrix matrix( decDMatrix::CreateTranslation( -position ) );
	const int count = instances.GetInstanceCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglGIInstance &instance = instances.GetInstanceAt( i );
		if( instance.GetComponent() ){
			deoglRComponent &component = *instance.GetComponent();
			AddComponent( plan, ( component.GetMatrix() * matrix ).ToMatrix(), component.GetLODAt( -1 ) );
		}
	}
}

void deoglGIBVH::AddComponents( deoglRenderPlan &plan, const decDVector &position,
const deoglCollideList &list ){
	const decDMatrix matrix( decDMatrix::CreateTranslation( -position ) );
	const int count = list.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRComponent &component = *list.GetComponentAt( i )->GetComponent();
		AddComponent( plan, ( component.GetMatrix() * matrix ).ToMatrix(), component.GetLODAt( -1 ) );
	}
}

void deoglGIBVH::AddComponent( deoglRenderPlan &plan, const decMatrix &matrix, deoglRComponentLOD &lod ){
				decTimer timer1;
	deoglModelLOD &modelLOD = *lod.GetModelLOD();
	deoglRComponent &component = lod.GetComponent();
	int indexModel;
	
	// update renderables in case this component is not visible
	component.UpdateRenderables( plan );
	
	//if( component.GetRenderMode() == deoglRComponent::ermDynamic ){ // not catching all
	if( modelLOD.GetWeightsCount() > 0 ){
				decTimer timer2;
		// prepare BVH
		lod.PrepareBVH();
		if( ! lod.GetBVH()->GetRootNode() ){
			return; // empty model
		}
				pRenderThread.GetLogger().LogInfoFormat("> > Dynamic Prepare BVH: %d (%s)",
					(int)(timer2.GetElapsedTime() * 1e6f), component.GetModel()->GetFilename().GetString());
		
		// add model to list of encountered models
		indexModel = pModelCount;
		sModel &bvhModel = pAddModel();
		bvhModel.component = &lod;
		
		// add vertices to TBO in mesh order
				modelLOD.PrepareBVH();
		const oglVector * const positions = lod.GetPositions();
		const int positionPoint = modelLOD.GetPositionCount();
		int i;
		
		for( i=0; i<positionPoint; i++ ){
			const oglVector &position = positions[ i ];
			pTBOVertex.AddVec4( position.x, position.y, position.z, 0.0f );
		}
				pRenderThread.GetLogger().LogInfoFormat("> > Dynamic Add Vertices: %d (%s)",
					(int)(timer2.GetElapsedTime() * 1e6f), modelLOD.GetModel().GetFilename().GetString());
		
		// add faces to TBOs using primitive mapping from BVH
		const deoglBVH &bvh = *lod.GetBVH();
		const decVector2 * const texCoords = modelLOD.GetTextureCoordinates();
		const oglModelVertex * const vertices = modelLOD.GetVertices();
		const deoglModelFace * const faces = modelLOD.GetFaces();
		const int * const primitives = bvh.GetPrimitives();
		const int primitiveCount = bvh.GetPrimitiveCount();
		
		for( i=0; i<primitiveCount; i++ ){
			const deoglModelFace &face = faces[ primitives[ i ] ];
			const oglModelVertex &v1 = vertices[ face.GetVertex1() ];
			const oglModelVertex &v2 = vertices[ face.GetVertex2() ];
			const oglModelVertex &v3 = vertices[ face.GetVertex3() ];
			
			pTBOFace.AddVec4( v1.position, v2.position, v3.position, face.GetTexture() );
			
			pTBOTexCoord.AddVec2( texCoords[ v1.texcoord ] );
			pTBOTexCoord.AddVec2( texCoords[ v2.texcoord ] );
			pTBOTexCoord.AddVec2( texCoords[ v3.texcoord ] );
		}
				pRenderThread.GetLogger().LogInfoFormat("> > Dynamic Add Faces: %d (%s)",
					(int)(timer2.GetElapsedTime() * 1e6f), modelLOD.GetModel().GetFilename().GetString());
		
		pAddBVH( bvh );
				pRenderThread.GetLogger().LogInfoFormat("> > Dynamic Add Nodes: %d (%s)",
					(int)(timer2.GetElapsedTime() * 1e6f), modelLOD.GetModel().GetFilename().GetString());
		
	}else{
				decTimer timer2;
		// find model
		for( indexModel=0; indexModel<pModelCount; indexModel++ ){
			//if( pModels[ indexModel ].component->GetComponent().GetRenderMode() == deoglRComponent::ermStatic
			if( pModels[ indexModel ].component->GetModelLOD() == &modelLOD && modelLOD.GetWeightsCount() == 0 ){
				break;
			}
		}
				pRenderThread.GetLogger().LogInfoFormat("> > Static Find Model: %d (%s)",
					(int)(timer2.GetElapsedTime() * 1e6f), modelLOD.GetModel().GetFilename().GetString());
		
		// if model does not exist add it
		if( indexModel == pModelCount ){
			// prepare BVH first just in case something goes wrong
			modelLOD.PrepareBVH();
			if( ! modelLOD.GetBVH()->GetRootNode() ){
				return; // empty model
			}
			
			// add model to list of encountered models
			sModel &bvhModel = pAddModel();
			bvhModel.component = &lod;
			
			// add vertices to TBO in mesh order
			const oglModelPosition * const positions = modelLOD.GetPositions();
			const int positionCount = modelLOD.GetPositionCount();
			int i;
			
			for( i=0; i<positionCount; i++ ){
				pTBOVertex.AddVec4( positions[ i ].position, 0.0f );
			}
				pRenderThread.GetLogger().LogInfoFormat("> > Static Add Vertices: %d (%s)",
					(int)(timer2.GetElapsedTime() * 1e6f), modelLOD.GetModel().GetFilename().GetString());
						
// 						static float ttv[15000*3*4];
// 						timer2.Reset();
// 						memcpy(&ttv, pTBOVertex.pDataFloat + bvhModel.indexVertices*4, positionCount*16);
// 						pRenderThread.GetLogger().LogInfoFormat("> > > Copy Vertices Test: %d (%s)",
// 							(int)(timer2.GetElapsedTime() * 1e6f), modelLOD.GetModel().GetFilename().GetString());
// 						timer2.Reset();
			
			// add faces to TBOs using primitive mapping from BVH
			const deoglBVH &bvh = *modelLOD.GetBVH();
			const decVector2 * const texCoords = modelLOD.GetTextureCoordinates();
			const oglModelVertex * const vertices = modelLOD.GetVertices();
			const deoglModelFace * const faces = modelLOD.GetFaces();
			const int * const primitives = bvh.GetPrimitives();
			const int primitiveCount = bvh.GetPrimitiveCount();
			
			for( i=0; i<primitiveCount; i++ ){
				const deoglModelFace &face = faces[ primitives[ i ] ];
				const oglModelVertex &v1 = vertices[ face.GetVertex1() ];
				const oglModelVertex &v2 = vertices[ face.GetVertex2() ];
				const oglModelVertex &v3 = vertices[ face.GetVertex3() ];
				
				pTBOFace.AddVec4( v1.position, v2.position, v3.position, face.GetTexture() );
				
				pTBOTexCoord.AddVec2( texCoords[ v1.texcoord ] );
				pTBOTexCoord.AddVec2( texCoords[ v2.texcoord ] );
				pTBOTexCoord.AddVec2( texCoords[ v3.texcoord ] );
			}
				pRenderThread.GetLogger().LogInfoFormat("> > Static Add Faces: %d (%s)",
					(int)(timer2.GetElapsedTime() * 1e6f), modelLOD.GetModel().GetFilename().GetString());
						
// 						static uint32_t ttf[15000*4];
// 						static HALF_FLOAT tttc[15000*6];
// 						timer2.Reset();
// 						memcpy(&ttf, pTBOFace.pDataUInt + bvhModel.indexFaces*4, primitiveCount*16);
// 						memcpy(&tttc, pTBOTexCoord.pDataFloat + bvhModel.indexFaces*6, primitiveCount*12);
// 						pRenderThread.GetLogger().LogInfoFormat("> > > Copy Faces Test: %d (%s)",
// 							(int)(timer2.GetElapsedTime() * 1e6f), modelLOD.GetModel().GetFilename().GetString());
// 						timer2.Reset();
			
			pAddBVH( bvh );
				pRenderThread.GetLogger().LogInfoFormat("> > Static Add Nodes: %d (%s)",
					(int)(timer2.GetElapsedTime() * 1e6f), modelLOD.GetModel().GetFilename().GetString());
		}
	}
	
	// add materials
// 				decTimer timer1;
	deoglAddToRenderTaskGIMaterial &addToRenderTask =
		pRenderThread.GetRenderers().GetLight().GetRenderGI().GetAddToRenderTask();
	const int textureCount = component.GetTextureCount();
	const int indexMaterial = pTBOMaterial.GetPixelCount();
	int i;
	
	for( i=0; i<textureCount; i++ ){
		const deoglRComponentTexture &texture = component.GetTextureAt( i );
		pAddMaterial( texture, addToRenderTask.AddComponentTexture( lod, i ) );
	}
	
	// add component
	pAddComponent( indexModel, indexMaterial, matrix );
				pRenderThread.GetLogger().LogInfoFormat("> Add Component %d: %d (%s)",
					lod.GetModelLOD()->GetFaceCount(), (int)(timer1.GetElapsedTime() * 1e6f),
					component.GetModel()->GetFilename().GetString());
}

void deoglGIBVH::AddOcclusionMeshes( deoglRenderPlan &plan, const decDVector &position, const deoglGIInstances &instances ){
	const decDMatrix matrix( decDMatrix::CreateTranslation( -position ) );
	const int count = instances.GetInstanceCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglGIInstance &instance = instances.GetInstanceAt( i );
		if( instance.GetOcclusionMesh() ){
			deoglRComponent &component = *instance.GetOcclusionMesh();
			AddOcclusionMesh( plan, ( component.GetMatrix() * matrix ).ToMatrix(), component );
		}
	}
}

void deoglGIBVH::AddOcclusionMeshes( deoglRenderPlan &plan, const decDVector &position, const deoglCollideList &list ){
	const decDMatrix matrix( decDMatrix::CreateTranslation( -position ) );
	const int count = list.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRComponent &component = *list.GetComponentAt( i )->GetComponent();
		AddOcclusionMesh( plan, ( component.GetMatrix() * matrix ).ToMatrix(), component );
	}
}

void deoglGIBVH::AddOcclusionMesh( deoglRenderPlan &plan, const decMatrix &matrix, deoglRComponent &component ){
	deoglDynamicOcclusionMesh * const dynOccMesh = component.GetDynamicOcclusionMesh();
	int indexModel;
	
	if( dynOccMesh ){
		// prepare BVH
		dynOccMesh->PrepareBVH();
		if( ! dynOccMesh->GetBVH()->GetRootNode() ){
			return; // empty model
		}
		
		// add occlusion mesh to list of encountered models
		indexModel = pModelCount;
		sModel &bvhModel = pAddModel();
		bvhModel.occlusionMesh = &component;
		
		// add vertices to TBO in mesh order
		const decVector * const vertices = dynOccMesh->GetVertices();
		deoglROcclusionMesh &occMesh = *component.GetOcclusionMesh();
		const int vertexCount = occMesh.GetVertexCount();
		int i;
		
		for( i=0; i<vertexCount; i++ ){
			pTBOVertex.AddVec4( vertices[ i ], 0.0f );
		}
		
		// add faces to TBOs using primitive mapping from BVH
		const int singleSidedFaceCount = occMesh.GetSingleSidedFaceCount();
		const unsigned short * const corners = occMesh.GetCorners();
		const deoglBVH &bvh = *dynOccMesh->GetBVH();
		const int * const primitives = bvh.GetPrimitives();
		const int primitiveCount = bvh.GetPrimitiveCount();
		
		for( i=0; i<primitiveCount; i++ ){
			const int face = primitives[ i ];
			const unsigned short * const fc = corners + face * 3;
			
			pTBOFace.AddVec4( fc[ 0 ], fc[ 1 ], fc[ 2 ], face < singleSidedFaceCount ? 0.0 : 1.0 );
		}
		
		pAddBVH( bvh );
		
	}else{
		// find model
		for( indexModel=0; indexModel<pModelCount; indexModel++ ){
			if( pModels[ indexModel ].occlusionMesh == &component
			&& ! pModels[ indexModel ].occlusionMesh->GetDynamicOcclusionMesh() ){
				break;
			}
		}
		
		// if model does not exist add it
		if( indexModel == pModelCount ){
			deoglROcclusionMesh &occMesh = *component.GetOcclusionMesh();
			
			// prepare BVH first just in case something goes wrong
			occMesh.PrepareBVH();
			if( ! occMesh.GetBVH()->GetRootNode() ){
				return; // empty model
			}
			
			// add occlusion mesh to list of encountered models
			sModel &bvhModel = pAddModel();
			bvhModel.occlusionMesh = &component;
			
			// add vertices to TBO in mesh order
			const deoglROcclusionMesh::sVertex * const vertices = occMesh.GetVertices();
			const int vertexCount = occMesh.GetVertexCount();
			int i;
			
			for( i=0; i<vertexCount; i++ ){
				pTBOVertex.AddVec4( vertices[ i ].position, 0.0f );
			}
			
			// add faces to TBOs using primitive mapping from BVH
			const unsigned short * const corners = occMesh.GetCorners();
			const int singleSidedFaceCount = occMesh.GetSingleSidedFaceCount();
			const deoglBVH &bvh = *occMesh.GetBVH();
			const int * const primitives = bvh.GetPrimitives();
			const int primitiveCount = bvh.GetPrimitiveCount();
			
			for( i=0; i<primitiveCount; i++ ){
				const int face = primitives[ i ];
				const unsigned short * const fc = corners + face * 3;
				
				pTBOFace.AddVec4( fc[ 0 ], fc[ 1 ], fc[ 2 ], face < singleSidedFaceCount ? 0.0 : 1.0 );
			}
			
			pAddBVH( bvh );
		}
	}
	
	// add component
	pAddComponent( indexModel, 0, matrix );
}

void deoglGIBVH::BuildBVH(){
	// build instance bvh
	if( pComponentCount > pPrimitiveSize ){
		deoglBVH::sBuildPrimitive * const newArray = new deoglBVH::sBuildPrimitive[ pComponentCount ];
		if( pPrimitives ){
			delete [] pPrimitives;
		}
		pPrimitives = newArray;
		pPrimitiveSize = pComponentCount;
	}
	
	int i;
	for( i=0; i<pComponentCount; i++ ){
		deoglBVH::sBuildPrimitive &primitive = pPrimitives[ i ];
		const sComponent &component = pComponents[ i ];
		const sModel &model = pModels[ component.indexModel ];
		const deoglBVHNode *rootNode = NULL;
		
		if( model.component != NULL ){
			//if( model.component->GetComponent().GetRenderMode() == deoglRComponent::ermStatic ){
			if( model.component->GetModelLOD()->GetWeightsCount() == 0 ){
				rootNode = model.component->GetModelLOD()->GetBVH()->GetRootNode();
				
			}else{
				rootNode = model.component->GetBVH()->GetRootNode();
			}
			
		}else if( model.occlusionMesh != NULL ){
			if( model.occlusionMesh->GetDynamicOcclusionMesh() ){
				rootNode = model.occlusionMesh->GetDynamicOcclusionMesh()->GetBVH()->GetRootNode();
				
			}else{
				rootNode = model.occlusionMesh->GetOcclusionMesh()->GetBVH()->GetRootNode();
			}
			
		}else{
			DETHROW( deeInvalidParam );
		}
		
		const decVector &minExtend = rootNode->GetMinExtend();
		const decVector &maxExtend = rootNode->GetMaxExtend();
		const decVector center( ( minExtend + maxExtend ) * 0.5f );
		const decVector halfSize( ( maxExtend - minExtend ) * 0.5f );
		deoglCollisionBox box( component.matrix * center,
			component.matrix.GetScale().Multiply( halfSize ), \
			component.matrix.Normalized().ToQuaternion() );
		deoglCollisionBox enclosing;
		box.GetEnclosingBox( &enclosing );
		
		primitive.center = enclosing.GetCenter();
		primitive.minExtend = primitive.center - enclosing.GetHalfSize();
		primitive.maxExtend = primitive.center + enclosing.GetHalfSize();
	}
	
	pBVH.Build( pPrimitives, pComponentCount, 12 );
	
	// add to TBOs using primitive mapping from BVH
	const int * const primitives = pBVH.GetPrimitives();
	
	for( i=0; i<pComponentCount; i++ ){
		sComponent &component = pComponents[ primitives[ i ] ];
		const sModel &model = pModels[ component.indexModel ];
		
		component.indexMatrix = pTBOMatrix.GetPixelCount() / 3;
		pTBOMatrix.AddMat3x4( component.matrix.QuickInvert() );
		
		component.indexInstance = pTBOInstance.GetPixelCount();
		pTBOInstance.AddVec4( model.indexNodes, component.indexMaterial,
			model.indexVertices, model.indexFaces );
	}
	
	// add BVH to TBOs
	const int nodeCount = pBVH.GetNodeCount();
	
	if( nodeCount > 0 ){
		const deoglBVHNode * const nodes = pBVH.GetNodes();
		
		pIndexRootNode = pTBOIndex.GetPixelCount();
		
		for( i=0; i<nodeCount; i++ ){
			const deoglBVHNode &node = nodes[ i ];
			pTBONodeBox.AddVec4( node.GetMinExtend(), 0.0f );
			pTBONodeBox.AddVec4( node.GetMaxExtend(), 0.0f );
			
			if( node.GetPrimitiveCount() == 0 ){
				pTBOIndex.AddVec2( pIndexRootNode + node.GetFirstIndex(), 0 );
				
			}else{
				pTBOIndex.AddVec2( node.GetFirstIndex(), node.GetPrimitiveCount() );
			}
		}
	}
	
	// update TBOs
	pTBONodeBox.Update();
	pTBOIndex.Update();
	pTBOInstance.Update();
	pTBOMatrix.Update();
	pTBOFace.Update();
	pTBOVertex.Update();
	pTBOTexCoord.Update();
	pTBOMaterial.Update();
	pTBOMaterial2.Update();
}

void deoglGIBVH::DebugPrint( const decDVector &position ){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	int i;
	logger.LogInfoFormat("GIBVH: %d Models", pModelCount);
	for(i=0; i<pModelCount; i++){
		const char * filename = "-";
		if( pModels[i].component != NULL ){
			filename = pModels[i].component->GetComponent().GetModel()->GetFilename();
		}else if( pModels[i].occlusionMesh != NULL ){
			filename = pModels[i].occlusionMesh->GetOcclusionMesh()->GetFilename();
		}
		logger.LogInfoFormat("- %d: indexNodes=%d indexFaces=%d path=%s", i,
			pModels[i].indexNodes, pModels[i].indexFaces,filename );
	}
	logger.LogInfoFormat("GIBVH: %d Components", pComponentCount);
	for(i=0; i<pComponentCount; i++){
		const decDVector p(position + pComponents[i].matrix.GetPosition());
		logger.LogInfoFormat("- %d: indexMatrix=%d indexModel=%d indexInstance=%d"
			" indexMaterial=%d position=(%g,%g,%g)", i, pComponents[i].indexMatrix,
			pComponents[i].indexModel, pComponents[i].indexInstance,
			pComponents[i].indexMaterial, p.x, p.y, p.z);
	}
	logger.LogInfoFormat("GIBVH: Root Node %d", pIndexRootNode);
	logger.LogInfo("GIBVH: TBONodeBox");
	pTBONodeBox.DebugPrint();
	logger.LogInfo("GIBVH: TBOIndex");
	pTBOIndex.DebugPrint();
	logger.LogInfo("GIBVH: TBOInstance");
	pTBOInstance.DebugPrint();
	logger.LogInfo("GIBVH: TBOMatrix");
	pTBOMatrix.DebugPrint();
	logger.LogInfo("GIBVH: TBOFace");
	pTBOFace.DebugPrint();
	logger.LogInfo("GIBVH: pTBOVertex");
	pTBOVertex.DebugPrint();
	logger.LogInfo("GIBVH: pTBOTexCoord");
	pTBOTexCoord.DebugPrint();
	logger.LogInfo("GIBVH: pTBOMaterial");
	pTBOMaterial.DebugPrint();
	logger.LogInfo("GIBVH: pTBOMaterial2");
	pTBOMaterial2.DebugPrint();
	
	struct PrintBVH{
		deoglRTLogger &logger;
		const sComponent *instances;
		PrintBVH(deoglRTLogger &logger, const sComponent *instances) : logger(logger), instances(instances){
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
						prefix.GetString(), i, p.x, p.y, p.z, r.x, r.y, r.z, instances[index].indexModel);
				}
			}
		}
	};
	logger.LogInfo("Component BVH");
	if(pBVH.GetRootNode()){
		PrintBVH(logger, pComponents).Print("", pBVH, *pBVH.GetRootNode());
	}
}



// Private Functions
//////////////////////

deoglGIBVH::sModel &deoglGIBVH::pAddModel(){
	if( pModelCount == pModelSize ){
		const int newSize = pModelCount * 3 / 2 + 1;
		sModel * const newArray = new sModel[ newSize ];
		if( pModels ){
			memcpy( newArray, pModels, sizeof( sModel ) * pModelCount );
			delete [] pModels;
		}
		pModels = newArray;
		pModelSize = newSize;
	}
	
	sModel &model = pModels[ pModelCount++ ];
	model.component = NULL;
	model.occlusionMesh = NULL;
	model.indexVertices = pTBOVertex.GetPixelCount();
	model.indexFaces = pTBOFace.GetPixelCount();
	model.indexNodes = pTBOIndex.GetPixelCount();
	return model;
}

deoglGIBVH::sComponent &deoglGIBVH::pAddComponent( int indexModel, int indexMaterial,
const decMatrix &matrix ){
	if( pComponentCount == pComponentSize ){
		const int newSize = pComponentCount + 10;
		sComponent * const newArray = new sComponent[ newSize ];
		if( pComponents ){
			memcpy( newArray, pComponents, sizeof( sComponent ) * pComponentCount );
			delete [] pComponents;
		}
		pComponents = newArray;
		pComponentSize = newSize;
	}
	
	sComponent &component = pComponents[ pComponentCount++ ];
	component.indexModel = indexModel;
	component.indexMaterial = indexMaterial;
	component.indexInstance = 0;
	component.indexMatrix = 0;
	component.matrix = matrix;
	return component;
}

void deoglGIBVH::pAddMaterial( const deoglRComponentTexture &texture, deoglRenderTaskTexture *renderTaskTexture ){
	deoglSkinTexture * const skinTexture = texture.GetUseSkinTexture();
	if( skinTexture ){
		pAddMaterial( *skinTexture, texture.GetUseSkinState(), texture.GetUseDynamicSkin(),
			renderTaskTexture, texture.CalcTexCoordMatrix() );
		
	}else{
		// TODO we have to add the texture even if not containing maps. in this case
		//      the materialIndex has to be 0 (aka not set)
		pTBOMaterial.AddVec4( 0, 0, 0, 0 );
		pTBOMaterial2.AddVec4( 0.0f, 0.0f, 0.0f, 0.0f );
		pTBOMaterial2.AddVec4( 0.0f, 0.0f, 0.0f, 0.0f );
		pTBOMaterial2.AddVec4( 0.0f, 0.0f, 0.0f, 0.0f );
	}
}

void deoglGIBVH::pAddMaterial( const deoglSkinTexture &skinTexture, deoglSkinState *skinState,
deoglRDynamicSkin *dynamicSkin, deoglRenderTaskTexture *renderTaskTexture,
const decTexMatrix2 &texCoordMatrix ){
	// collect values
	decColor colorTint( skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empColorTint )
		.ResolveColor( skinState, dynamicSkin, skinTexture.GetColorTint() ) );
	colorTint.r = powf( decMath::max( colorTint.r, 0.0f ), 2.2f );
	colorTint.g = powf( decMath::max( colorTint.g, 0.0f ), 2.2f );
	colorTint.b = powf( decMath::max( colorTint.b, 0.0f ), 2.2f );
	
	const float colorGamma = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empColorGamma )
		.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetColorGamma() );
	
	const float roughnessRemapLower = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empRoughnessRemapLower )
		.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetRoughnessRemapLower() );
	const float roughnessRemapUpper = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empRoughnessRemapUpper )
		.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetRoughnessRemapUpper() );
	const float roughnessGamma = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empRoughnessGamma )
		.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetRoughnessGamma() );
	
	decColor emissivityIntensity( skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empEmissivityTint )
		.ResolveColor( skinState, dynamicSkin, skinTexture.GetEmissivityTint() ) );
	emissivityIntensity.r = powf( decMath::max( emissivityIntensity.r, 0.0f ), 2.2f );
	emissivityIntensity.g = powf( decMath::max( emissivityIntensity.g, 0.0f ), 2.2f );
	emissivityIntensity.b = powf( decMath::max( emissivityIntensity.b, 0.0f ), 2.2f );
	
	emissivityIntensity *= decMath::max( 0.0f, skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empEmissivityIntensity )
		.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetEmissivityIntensity() ) );
	
	/*
	decColor envRoomEmissivityTint( skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empEnvironmentRoomEmissivityTint )
		.ResolveColor( skinState, dynamicSkin, skinTexture.GetEnvironmentRoomEmissivityTint() ) );
	envRoomEmissivityTint.r = powf( decMath::max( envRoomEmissivityTint.r, 0.0f ), 2.2f );
	envRoomEmissivityTint.g = powf( decMath::max( envRoomEmissivityTint.g, 0.0f ), 2.2f );
	envRoomEmissivityTint.b = powf( decMath::max( envRoomEmissivityTint.b, 0.0f ), 2.2f );
	
	const float envRoomEmissivityIntensity = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empEnvironmentRoomEmissivityIntensity )
		.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetEnvironmentRoomEmissivityIntensity() );
	
	decColor envRoomEmissivity( envRoomEmissivityTint * decMath::max( envRoomEmissivityIntensity, 0.0f ) );
	*/
	
	const float reflectivityMultiplier = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empReflectivityMultiplier )
		.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetReflectivityMultiplier() );
	
// 	const bool variationU = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empVariationU )
// 		.ResolveAsBool( skinState, dynamicSkin, skinTexture.GetVariationU() );
// 	const bool variationV = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empVariationV )
// 		.ResolveAsBool( skinState, dynamicSkin, skinTexture.GetVariationV() );
	
	const int materialIndex = renderTaskTexture ? decMath::clamp( renderTaskTexture->GetTUC()->GetMaterialIndex(), 0, 16383 ) : 0;
	
	const bool ignoreMaterial = skinTexture.GetHasTransparency();
	const bool texCoordClamp = skinTexture.GetTexCoordClamp();
	const bool hasSolidity = skinTexture.GetHasSolidity();
	const bool shadowNone = skinTexture.GetShadowNone();
	
	// pack into values and add them
	#define BITS_MASK(bits) ((1<<bits)-1)
	#define PACK_I(value, bits, shift) (uint32_t)(decMath::clamp(value, 0, BITS_MASK(bits)) << shift)
	#define PACK(value, bits, shift) PACK_I((int)((value)*BITS_MASK(bits) + 0.5f), bits, shift)
	#define PACK_M(value, lower, upper, bits, shift) PACK((value-lower)/(upper-lower), bits, shift)
	#define PACK_G(value, bits, shift) PACK_M(value, 0.4, 2.2, bits, shift)
	#define PACK_B(value, bit) (uint32_t)((value) ? (1<<bit) : 0)
	
	const uint32_t red = PACK_I( materialIndex, 16, 16 )
		| PACK_B( ignoreMaterial, 0 )
		| PACK_B( texCoordClamp, 1 )
		| PACK_B( hasSolidity, 2 )
		| PACK_B( shadowNone, 3 );
	
	const uint32_t green = PACK( colorTint.r, 8, 24 )
		| PACK( colorTint.g, 8, 16 )
		| PACK( colorTint.b, 8, 8 )
		| PACK_G( colorGamma, 8, 0 );
	
	const uint32_t blue = PACK( roughnessRemapLower, 8, 24 )
		| PACK( roughnessRemapUpper, 8, 16 )
		| PACK_G( roughnessGamma, 8, 8 )
		| PACK( reflectivityMultiplier, 8, 0 );
	
	const uint32_t alpha = 0;
	//| PACK_B( variationU, 15 ) | PACK_B( variationV, 14 ) | PACK_I( materialIndex, 14, 0 ) );
	
	pTBOMaterial.AddVec4( red, green, blue, alpha );
	
	pTBOMaterial2.AddVec4( texCoordMatrix.a11, texCoordMatrix.a12, texCoordMatrix.a13, 0.0f );
	pTBOMaterial2.AddVec4( texCoordMatrix.a21, texCoordMatrix.a22, texCoordMatrix.a23, 0.0f );
	pTBOMaterial2.AddVec4( emissivityIntensity.r, emissivityIntensity.g, emissivityIntensity.b, 0.0f );
	
	#undef PACK_B
	#undef PACK_G
	#undef PACK_M
	#undef PACK
	#undef PACK_I
	#undef BITS_MASK
}

void deoglGIBVH::pAddBVH( const deoglBVH &bvh ){
	const int nodeCount = bvh.GetNodeCount();
	const deoglBVHNode * const nodes = bvh.GetNodes();
	int i;
	
	for( i=0; i<nodeCount; i++ ){
		const deoglBVHNode &node = nodes[ i ];
		pTBONodeBox.AddVec4( node.GetMinExtend(), 0.0f );
		pTBONodeBox.AddVec4( node.GetMaxExtend(), 0.0f );
		pTBOIndex.AddVec2( node.GetFirstIndex(), node.GetPrimitiveCount() );
	}
}
