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
#include "../capabilities/deoglCapabilities.h"
#include "../collidelist/deoglCollideListComponent.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/face/deoglModelFace.h"
#include "../model/texture/deoglModelTexture.h"
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
pTBOInstance( renderThread, 1 ),
pTBOMatrix( renderThread, 4 ),
pTBOFace( renderThread, 4 ),
pTBOVertex( renderThread, 4 ){
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
	pTBOFace.Clear();
	pTBOMatrix.Clear();
	pTBOInstance.Clear();
	pTBOIndex.Clear();
	pTBONodeBox.Clear();
}

void deoglGIBVH::FindComponents( deoglRWorld &world, const decDVector &position,
const decVector &detectionBox ){
	deoglDCollisionBox colbox( position, detectionBox );
	pCollideList.Clear();
	pCollideList.AddComponentsColliding( world.GetOctree(), &colbox );
}

void deoglGIBVH::AddStaticComponents( const decDVector &position ){
	const decDMatrix matrix( decDMatrix::CreateTranslation( -position ) );
	const int count = pCollideList.GetComponentCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglRComponent &component = *pCollideList.GetComponentAt( i )->GetComponent();
		if( ! component.GetModel() || component.GetLODCount() == 0 ){
			continue;
		}
		if( component.GetRenderMode() != deoglRComponent::ermStatic ){
			continue;
		}
		
		const deoglRModel::sExtends &extends = component.GetModel()->GetWeightlessExtends();
		if( ( extends.maximum - extends.minimum ) < decVector( 0.5f, 0.5f, 0.5f ) ){
			continue; // skip small models to improve performance
		}
		
		deoglRComponentLOD &lod = component.GetLODAt( component.GetLODCount() - 1 );
		//component.PrepareRayTraceField();
		AddStaticComponent( ( component.GetMatrix() * matrix ).ToMatrix(), lod );
	}
}

void deoglGIBVH::AddStaticComponent( const decMatrix &matrix, deoglRComponentLOD &lod ){
	// find model
	int indexModel;
	for( indexModel=0; indexModel<pModelCount; indexModel++ ){
		if( pModels[ indexModel ].component == &lod ){
			break;
		}
	}
	
	// if model does not exist add it
	if( indexModel == pModelCount ){
		// prepare BVH first just in case something goes wrong
		deoglModelLOD &modelLOD = lod.GetModelLOD();
		modelLOD.PrepareBVH();
		
		if( ! modelLOD.GetBVH()->GetRootNode() ){
			return; // empty model
		}
		
		// add model to list of encountered models
		sModel &bvhModel = pAddModel();
		bvhModel.component = &lod;
		
		// add vertices to TBO in mesh order
		const oglModelPosition * const positions = modelLOD.GetPositions();
		const int positionPoint = modelLOD.GetPositionCount();
		int i;
		
		for( i=0; i<positionPoint; i++ ){
			pTBOVertex.AddVec4( positions[ i ].position, 0.0f );
		}
		
		// add faces to TBOs using primitive mapping from BVH
		const deoglBVH &bvh = *modelLOD.GetBVH();
		const oglModelVertex * const vertices = modelLOD.GetVertices();
		const deoglModelFace * const faces = modelLOD.GetFaces();
		const int * const primitives = bvh.GetPrimitives();
		const int primitiveCount = bvh.GetPrimitiveCount();
		
		for( i=0; i<primitiveCount; i++ ){
			const deoglModelFace &face = faces[ primitives[ i ] ];
			//const uint32_t doubleSided = modelLOD.GetTextureAt( face.GetTexture() ).GetDoubleSided() ? 0 : 1;
			const uint32_t doubleSided = 1; // always double sided to allow disable probe
			pTBOFace.AddVec4( bvhModel.indexVertices + vertices[ face.GetVertex1() ].position,
				bvhModel.indexVertices + vertices[ face.GetVertex2() ].position,
				bvhModel.indexVertices + vertices[ face.GetVertex3() ].position, doubleSided );
		}
		
		pAddBVH( bvh, bvhModel.indexNodes, bvhModel.indexFaces );
	}
	
	pAddComponent( indexModel, matrix );
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
			rootNode = model.component->GetModelLOD().GetBVH()->GetRootNode();
			
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
		
		component.indexMatrix = pTBOMatrix.GetPixelCount() / 3;
		pTBOMatrix.AddMat3x4( component.matrix.QuickInvert() );
		
		component.indexInstance = pTBOInstance.GetPixelCount();
		pTBOInstance.AddInt( pModels[ component.indexModel ].indexNodes );
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
}

void deoglGIBVH::DebugPrint( const decDVector &position ){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	int i;
	logger.LogInfoFormat("GIBVH: %d Models", pModelCount);
	for(i=0; i<pModelCount; i++){
		const char * filename = "-";
		if( pModels[i].component != NULL ){
			filename = pModels[i].component->GetComponent().GetModel()->GetFilename();
		}
		logger.LogInfoFormat("- %d: indexNodes=%d indexFaces=%d path=%s", i,
			pModels[i].indexNodes, pModels[i].indexFaces,filename );
	}
	logger.LogInfoFormat("GIBVH: %d Components", pComponentCount);
	for(i=0; i<pComponentCount; i++){
		const decDVector p(position + pComponents[i].matrix.GetPosition());
		logger.LogInfoFormat("- %d: indexMatrix=%d indexModel=%d indexInstance=%d position=(%g,%g,%g)",
			i, pComponents[i].indexMatrix, pComponents[i].indexModel,
			pComponents[i].indexInstance, p.x, p.y, p.z);
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
		const int newSize = pModelCount + 10;
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
	model.indexVertices = pTBOVertex.GetPixelCount();
	model.indexFaces = pTBOFace.GetPixelCount();
	model.indexNodes = pTBOIndex.GetPixelCount();
	return model;
}

deoglGIBVH::sComponent &deoglGIBVH::pAddComponent( int indexModel, const decMatrix &matrix ){
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
	component.indexInstance = 0;
	component.indexMatrix = 0;
	component.matrix = matrix;
	return component;
}

void deoglGIBVH::pAddBVH( const deoglBVH &bvh, int rootIndexNodes, int rootIndexFaces ){
	const int nodeCount = bvh.GetNodeCount();
	const deoglBVHNode * const nodes = bvh.GetNodes();
	int i;
	
	for( i=0; i<nodeCount; i++ ){
		const deoglBVHNode &node = nodes[ i ];
		pTBONodeBox.AddVec4( node.GetMinExtend(), 0.0f );
		pTBONodeBox.AddVec4( node.GetMaxExtend(), 0.0f );
		
		if( node.GetPrimitiveCount() == 0 ){
			pTBOIndex.AddVec2( rootIndexNodes + node.GetFirstIndex(), 0 );
			
		}else{
			pTBOIndex.AddVec2( rootIndexFaces + node.GetFirstIndex(), node.GetPrimitiveCount() );
		}
	}
}
