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
#include "../component/deoglRComponentTexture.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/face/deoglModelFace.h"
#include "../model/texture/deoglModelTexture.h"
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
pTBOInstance( renderThread, 2 ),
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
	
	deoglModelLOD &modelLOD = *lod.GetModelLOD();
	
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
		const int positionPoint = modelLOD.GetPositionCount();
		int i;
		
		for( i=0; i<positionPoint; i++ ){
			pTBOVertex.AddVec4( positions[ i ].position, 0.0f );
		}
		
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
			
			pTBOFace.AddVec4( bvhModel.indexVertices + v1.position, bvhModel.indexVertices + v2.position,
				bvhModel.indexVertices + v3.position, face.GetTexture() );
			
			pTBOTexCoord.AddVec2( texCoords[ v1.texcoord ] );
			pTBOTexCoord.AddVec2( texCoords[ v2.texcoord ] );
			pTBOTexCoord.AddVec2( texCoords[ v3.texcoord ] );
		}
		
		pAddBVH( bvh, bvhModel.indexNodes, bvhModel.indexFaces );
	}
	
	// add materials
	deoglAddToRenderTaskGIMaterial &addToRenderTask =
		pRenderThread.GetRenderers().GetLight().GetRenderGI().GetAddToRenderTask();
	deoglRComponent &component = lod.GetComponent();
	const int textureCount = component.GetTextureCount();
	const int indexMaterial = pTBOMaterial.GetPixelCount();
	int i;
	
	for( i=0; i<textureCount; i++ ){
		const deoglRComponentTexture &texture = component.GetTextureAt( i );
		pAddMaterial( texture, addToRenderTask.AddComponentTexture( lod, i ) );
	}
	
	// add component
	pAddComponent( indexModel, indexMaterial, matrix );
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
			rootNode = model.component->GetModelLOD()->GetBVH()->GetRootNode();
			
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
		pTBOInstance.AddVec2( pModels[ component.indexModel ].indexNodes, component.indexMaterial );
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
	
	const int materialIndex = renderTaskTexture ? decMath::min( renderTaskTexture->GetMaterialIndex(), 16383 ) : 0;
	
	const bool ignoreMaterial = skinTexture.GetHasTransparency(); // || skinTexture.GetHasSolidity();
	const bool texCoordClamp = skinTexture.GetTexCoordClamp();
	
	// pack into values and add them
	#define BITS_MASK(bits) ((1<<bits)-1)
	#define PACK_I(value, bits, shift) (uint32_t)(decMath::clamp(value, 0, BITS_MASK(bits)) << shift)
	#define PACK(value, bits, shift) PACK_I((int)((value)*BITS_MASK(bits) + 0.5f), bits, shift)
	#define PACK_M(value, lower, upper, bits, shift) PACK((value-lower)/(upper-lower), bits, shift)
	#define PACK_G(value, bits, shift) PACK_M(value, 0.4, 2.2, bits, shift)
	#define PACK_B(value, bit) (uint32_t)((value) ? (1<<bit) : 0)
	
	pTBOMaterial.AddVec4(
		PACK( colorTint.r, 8, 24 ) | PACK( roughnessRemapLower, 8, 16 )
			| PACK_B( ignoreMaterial, 15 ) | PACK_B( texCoordClamp, 14 )
			| PACK_I( materialIndex, 14, 0 ),
		PACK( colorTint.g, 8, 24 ) | PACK( roughnessRemapUpper, 8, 16 ),
		PACK( colorTint.b, 8, 24 ) | PACK_G( roughnessGamma, 8, 16 ),
		PACK_G( colorGamma, 8, 24 ) | PACK( reflectivityMultiplier, 8, 16 ) );
			//| PACK_B( variationU, 15 ) | PACK_B( variationV, 14 ) | PACK_I( materialIndex, 14, 0 ) );
	
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
