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
#include "deoglGIBVHLocal.h"
#include "deoglGIBVHDynamic.h"
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
#include "../rendering/light/deoglRenderLight.h"
#include "../rendering/light/deoglRenderGI.h"
#include "../rendering/task/deoglRenderTask.h"
#include "../rendering/task/deoglRenderTaskTexture.h"
#include "../rendering/task/deoglAddToRenderTaskGIMaterial.h"
#include "../rendering/task/shared/deoglRenderTaskSharedTexture.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../tbo/deoglDynamicTBOUInt16.h"
#include "../tbo/deoglDynamicTBOBlock.h"
#include "../tbo/deoglDynamicTBOShared.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../utils/collision/deoglCollisionBox.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIBVH
/////////////////////

// Constructor, destructor
////////////////////////////

deoglGIBVH::deoglGIBVH( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pComponents( NULL ),
pComponentCount( 0 ),
pComponentSize( 0 ),
pPrimitives( NULL ),
pPrimitiveSize( 0 ),
pRecalcNodes( NULL ),
pRecalcNodeSize( 0 ),
pIndexRootNode( 0 ),
pTBONodeBox( NULL ),
pTBOIndex( NULL ),
pTBOInstance( NULL ),
pTBOMatrix( NULL ),
pTBOFace( NULL ),
pTBOVertex( NULL ),
pTBOTexCoord( NULL ),
pTBOMaterial( NULL ),
pTBOMaterial2( NULL ),
pBVHTBONodeBox( NULL ),
pBVHTBOIndex(  NULL )
{
	try{
		pTBONodeBox = new deoglDynamicTBOFloat32( renderThread, 4 );
		pTBOIndex = new deoglDynamicTBOUInt16( renderThread, 2 );
		pTBOInstance = new deoglDynamicTBOUInt32( renderThread, 4 );
		pTBOMatrix = new deoglDynamicTBOFloat32( renderThread, 4 );
		pTBOFace = new deoglDynamicTBOUInt16( renderThread, 4 );
		pTBOVertex = new deoglDynamicTBOFloat32( renderThread, 4 );
		pTBOTexCoord = new deoglDynamicTBOFloat16( renderThread, 2 );
		pTBOMaterial = new deoglDynamicTBOUInt32( renderThread, 4 );
		pTBOMaterial2 = new deoglDynamicTBOFloat16( renderThread, 4 );
		
		pSharedTBONode = new deoglDynamicTBOShared( pTBOIndex, 1, pTBONodeBox, 2 );
		pSharedTBOFace = new deoglDynamicTBOShared( pTBOFace, 1, pTBOTexCoord, 3 );
		pSharedTBOVertex = new deoglDynamicTBOShared( pTBOVertex, 1 );
		pSharedTBOMaterial = new deoglDynamicTBOShared( pTBOMaterial, 1, pTBOMaterial2, 3 );
		
		pBVHTBONodeBox = new deoglDynamicTBOFloat32( renderThread, 4 );
		pBVHTBOIndex = new deoglDynamicTBOUInt16( renderThread, 2 );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGIBVH::~deoglGIBVH(){
	pCleanUp();
}



// Management
///////////////

void deoglGIBVH::Clear(){
	pDropBlockBVH();
	pBVHTBONodeBox->Clear();
	pBVHTBOIndex->Clear();
	
	pBVH.Clear();
	pComponentCount = 0;
	pIndexRootNode = -1;
	pTBOMatrix->Clear();
	pTBOInstance->Clear();
}

// #define DO_TIMING_TEST 1
#ifdef DO_TIMING_TEST
static int vDebugTimeA = 0, vDebugTimeB = 0, vDebugTimeC = 0, vDebugTUCs = 0;
#endif
void deoglGIBVH::AddComponents( deoglRenderPlan &plan, const decDVector &position,
const deoglGIInstances &instances ){
	const decDMatrix matrix( decDMatrix::CreateTranslation( -position ) );
	const int count = instances.GetInstanceCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = instances.GetInstanceAt( i );
		if( instance.GetComponent() ){
			deoglRComponent &component = *instance.GetComponent();
			AddComponent( plan, ( component.GetMatrix() * matrix ).ToMatrix(), instance );
		}
	}
}

void deoglGIBVH::AddComponents( deoglRenderPlan &plan, const decDVector &position,
const deoglGIInstances &instances, bool dynamic ){
#ifdef DO_TIMING_TEST
	vDebugTimeA=0; vDebugTimeB=0; vDebugTimeC=0; int vDebugCount=0; vDebugTUCs=0;
#endif
	const decDMatrix matrix( decDMatrix::CreateTranslation( -position ) );
	const int count = instances.GetInstanceCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglGIInstance &instance = instances.GetInstanceAt( i );
		if( instance.GetComponent() && instance.GetDynamic() == dynamic ){
			deoglRComponent &component = *instance.GetComponent();
			AddComponent( plan, ( component.GetMatrix() * matrix ).ToMatrix(), instance );
#ifdef DO_TIMING_TEST
			vDebugCount++;
#endif
		}
	}
#ifdef DO_TIMING_TEST
	pRenderThread.GetLogger().LogInfoFormat("> Add Components %d: %d [%d,%d,%d] %d",
		vDebugCount, vDebugTimeA + vDebugTimeB + vDebugTimeC, vDebugTimeA, vDebugTimeB,
		vDebugTimeC, vDebugTUCs);
#endif
}

void deoglGIBVH::AddComponent( deoglRenderPlan &plan, const decMatrix &matrix, deoglGIInstance &instance ){
	if( ! instance.GetHasBVHNodes() || ! instance.GetComponent() ){
		return;
	}
	
#ifdef DO_TIMING_TEST
	decTimer timer1;
#endif
	deoglRComponent &component = *instance.GetComponent();
	deoglRComponentLOD &lod = component.GetLODAt( -1 );
	
	if( instance.GetDynamic() ){
		// update vertices and BVH extends if dirty
		lod.PrepareGIDynamicBVH();
		instance.UpdateBVHExtends();
		
		if( ! component.GetStaticTextures() ){
			instance.SetDirtyTUCs( true );
		}
	}
#ifdef DO_TIMING_TEST
	//const int debugElapsedA = (int)(timer1.GetElapsedTime() * 1e6f);
	vDebugTimeA += (int)(timer1.GetElapsedTime() * 1e6f);
#endif
	
	// add materials
	const int textureCount = component.GetTextureCount();
	int i;
	
	if( textureCount != instance.GetTUCCount() ){
		instance.SetDirtyTUCs( true ); // safety check
	}
	
	deoglDynamicTBOBlock *blockMaterial;
	
	if( instance.GetDirtyTUCs() ){
#ifdef DO_TIMING_TEST
	vDebugTUCs++;
#endif
		deoglAddToRenderTaskGIMaterial &addToRenderTask =
			pRenderThread.GetRenderers().GetLight().GetRenderGI().GetAddToRenderTask();
		
		instance.RemoveAllTUCs();
		instance.SetDirtyTUCs( false );
		
		if( instance.GetTBOMaterial()->GetPixelCount() != textureCount ){
			instance.DropBlockMaterial();
			instance.GetTBOMaterial()->SetPixelCount( textureCount );
			instance.GetTBOMaterial2()->SetPixelCount( textureCount * 3 );
		}
		
		for( i=0; i<textureCount; i++ ){
			deoglRenderTaskTexture * const rttexture = addToRenderTask.AddComponentTexture( lod, i );
			deoglTexUnitsConfig * const tuc = rttexture ? rttexture->GetTexture()->GetTUC() : NULL;
			pAddMaterial( instance, i, component.GetTextureAt( i ), tuc );
			instance.AddTUC( tuc );
		}
		
		blockMaterial = instance.GetBlockMaterial();
		blockMaterial->WriteToTBO();
		
	}else{
		blockMaterial = instance.GetBlockMaterial();
	}
	
#ifdef DO_TIMING_TEST
	//const int debugElapsedB = (int)(timer1.GetElapsedTime() * 1e6f);
	vDebugTimeB += (int)(timer1.GetElapsedTime() * 1e6f);
#endif
	
	// add component
	pAddComponent( instance, blockMaterial->GetOffset(), matrix );
#ifdef DO_TIMING_TEST
	vDebugTimeC += (int)(timer1.GetElapsedTime() * 1e6f);
// 				const int debugElapsedC = (int)(timer1.GetElapsedTime() * 1e6f);
// 				pRenderThread.GetLogger().LogInfoFormat("> Add Component %d: %d [%d,%d,%d] (%s)",
// 					lod.GetModelLODRef().GetFaceCount(), debugElapsedA + debugElapsedB + debugElapsedC,
// 					debugElapsedA, debugElapsedB, debugElapsedC, component.GetModel()->GetFilename().GetString());
			// 				pRenderThread.GetLogger().LogInfoFormat("> Add Component %d: %d (%s)",
			// 					lod.GetModelLOD()->GetFaceCount(), (int)(timer1.GetElapsedTime() * 1e6f),
			// 					component.GetModel()->GetFilename().GetString());
#endif
}

void deoglGIBVH::BuildBVH(){
	pDropBlockBVH(); // safety check
	
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
		const deoglGIInstance &instance = *component.instance;
		const decVector &minExtend = instance.GetBVHMinimumExtend();
		const decVector &maxExtend = instance.GetBVHMaximumExtend();
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
		const deoglGIInstance &instance = *component.instance;
		
		component.indexMatrix = pTBOMatrix->GetPixelCount() / 3;
		pTBOMatrix->AddMat3x4( component.matrix.QuickInvert() );
		
		component.indexInstance = pTBOInstance->GetPixelCount();
		pTBOInstance->AddVec4( instance.GetIndexNodes(), component.indexMaterial,
			instance.GetIndexVertices(), instance.GetIndexFaces() );
	}
	
	// add BVH to TBOs
	const int nodeCount = pBVH.GetNodeCount();
	
	if( nodeCount > 0 ){
		const deoglBVHNode * const nodes = pBVH.GetNodes();
		
		for( i=0; i<nodeCount; i++ ){
			const deoglBVHNode &node = nodes[ i ];
			pBVHTBONodeBox->AddVec4( node.GetMinExtend(), 0.0f );
			pBVHTBONodeBox->AddVec4( node.GetMaxExtend(), 0.0f );
			pBVHTBOIndex->AddVec2( node.GetFirstIndex(), node.GetPrimitiveCount() );
		}
		
		pBlockBVH.TakeOver( pSharedTBONode->AddBlock( pBVHTBOIndex, pBVHTBONodeBox ) );
		
		pIndexRootNode = ( ( deoglDynamicTBOBlock* )( deObject* )pBlockBVH )->GetOffset();
	}
	
	// update TBOs
	pSharedTBONode->Prepare();
	pSharedTBOFace->Prepare();
	pSharedTBOVertex->Prepare();
	pSharedTBOMaterial->Prepare();
	
	pTBOInstance->Update();
	pTBOMatrix->Update();
}

void deoglGIBVH::DebugPrint( const decDVector &position ){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	int i;
	logger.LogInfoFormat("GIBVH: %d Components", pComponentCount);
	for(i=0; i<pComponentCount; i++){
		const decDVector p(position + pComponents[i].matrix.GetPosition());
		logger.LogInfoFormat("- %d: indexMatrix=%d instance=%p indexInstance=%d"
			" indexMaterial=%d position=(%g,%g,%g)", i, pComponents[i].indexMatrix,
			pComponents[i].instance, pComponents[i].indexInstance,
			pComponents[i].indexMaterial, p.x, p.y, p.z);
	}
	logger.LogInfoFormat("GIBVH: Root Node %d", pIndexRootNode);
	logger.LogInfo("GIBVH: TBONodeBox");
	pTBONodeBox->DebugPrint();
	logger.LogInfo("GIBVH: TBOIndex");
	pTBOIndex->DebugPrint();
	logger.LogInfo("GIBVH: TBOInstance");
	pTBOInstance->DebugPrint();
	logger.LogInfo("GIBVH: TBOMatrix");
	pTBOMatrix->DebugPrint();
	logger.LogInfo("GIBVH: TBOFace");
	pTBOFace->DebugPrint();
	logger.LogInfo("GIBVH: pTBOVertex");
	pTBOVertex->DebugPrint();
	logger.LogInfo("GIBVH: pTBOTexCoord");
	pTBOTexCoord->DebugPrint();
	logger.LogInfo("GIBVH: pTBOMaterial");
	pTBOMaterial->DebugPrint();
	logger.LogInfo("GIBVH: pTBOMaterial2");
	pTBOMaterial2->DebugPrint();
	
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
					logger.LogInfoFormat("%sP%03d position=(%g,%g,%g) rotation=(%g,%g,%g) instance=%p",
						prefix.GetString(), i, p.x, p.y, p.z, r.x, r.y, r.z, instances[index].instance);
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

void deoglGIBVH::pCleanUp(){
	if( pSharedTBONode ){
		pSharedTBONode->FreeReference();
	}
	if( pSharedTBOFace ){
		pSharedTBOFace->FreeReference();
	}
	if( pSharedTBOVertex ){
		pSharedTBOVertex->FreeReference();
	}
	if( pSharedTBOMaterial ){
		pSharedTBOMaterial->FreeReference();
	}
	
	if( pBVHTBONodeBox ){
		pBVHTBONodeBox->FreeReference();
	}
	if( pBVHTBOIndex ){
		pBVHTBOIndex->FreeReference();
	}
	
	if( pTBONodeBox ){
		pTBONodeBox->FreeReference();
	}
	if( pTBOIndex ){
		pTBOIndex->FreeReference();
	}
	if( pTBOInstance ){
		pTBOInstance->FreeReference();
	}
	if( pTBOMatrix ){
		pTBOMatrix->FreeReference();
	}
	if( pTBOFace ){
		pTBOFace->FreeReference();
	}
	if( pTBOVertex ){
		pTBOVertex->FreeReference();
	}
	if( pTBOTexCoord ){
		pTBOTexCoord->FreeReference();
	}
	if( pTBOMaterial ){
		pTBOMaterial->FreeReference();
	}
	if( pTBOMaterial2 ){
		pTBOMaterial2->FreeReference();
	}
	
	if( pRecalcNodes ){
		delete [] pRecalcNodes;
	}
	if( pPrimitives ){
		delete [] pPrimitives;
	}
	if( pComponents ){
		delete [] pComponents;
	}
}

void deoglGIBVH::pDropBlockBVH(){
	if( pBlockBVH ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockBVH )->Drop();
		pBlockBVH = NULL;
	}
}

deoglGIBVH::sComponent &deoglGIBVH::pAddComponent( const deoglGIInstance &instance,
int indexMaterial, const decMatrix &matrix ){
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
	component.instance = &instance;
	component.indexMaterial = indexMaterial;
	component.indexInstance = 0;
	component.indexMatrix = 0;
	component.matrix = matrix;
	return component;
}

void deoglGIBVH::pAddMaterial( deoglGIInstance &instance, int index,
const deoglRComponentTexture &texture, deoglTexUnitsConfig *tuc ){
	deoglSkinTexture * const skinTexture = texture.GetUseSkinTexture();
	if( skinTexture ){
		pAddMaterial( instance, index, *skinTexture, texture.GetUseSkinState(),
			texture.GetUseDynamicSkin(), tuc, texture.CalcTexCoordMatrix() );
		
	}else{
		// TODO we have to add the texture even if not containing maps. in this case
		//      the materialIndex has to be 0 (aka not set)
		instance.GetTBOMaterial()->SetVec4At( index * 4, 0, 0, 0, 0 );
		
		deoglDynamicTBOFloat16 &tbo = *instance.GetTBOMaterial2();
		int index2 = index * 12; // 4 components, 3 pixels
		tbo.SetVec4At( index2, 0.0f, 0.0f, 0.0f, 0.0f );
		tbo.SetVec4At( index2 + 4, 0.0f, 0.0f, 0.0f, 0.0f );
		tbo.SetVec4At( index2 + 8, 0.0f, 0.0f, 0.0f, 0.0f );
	}
}

void deoglGIBVH::pAddMaterial( deoglGIInstance &instance, int index,
const deoglSkinTexture &skinTexture, deoglSkinState *skinState, deoglRDynamicSkin *dynamicSkin,
deoglTexUnitsConfig *tuc, const decTexMatrix2 &texCoordMatrix ){
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
	
	decColor envRoomEmissivityTint( skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empEnvironmentRoomEmissivityTint )
		.ResolveColor( skinState, dynamicSkin, skinTexture.GetEnvironmentRoomEmissivityTint() ) );
	envRoomEmissivityTint.r = powf( decMath::max( envRoomEmissivityTint.r, 0.0f ), 2.2f );
	envRoomEmissivityTint.g = powf( decMath::max( envRoomEmissivityTint.g, 0.0f ), 2.2f );
	envRoomEmissivityTint.b = powf( decMath::max( envRoomEmissivityTint.b, 0.0f ), 2.2f );
	
	envRoomEmissivityTint *= decMath::max( 0.0f, skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empEnvironmentRoomEmissivityIntensity )
		.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetEnvironmentRoomEmissivityIntensity() ) );
	
	emissivityIntensity += envRoomEmissivityTint;
	
	const float reflectivityMultiplier = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empReflectivityMultiplier )
		.ResolveAsFloat( skinState, dynamicSkin, skinTexture.GetReflectivityMultiplier() );
	
// 	const bool variationU = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empVariationU )
// 		.ResolveAsBool( skinState, dynamicSkin, skinTexture.GetVariationU() );
// 	const bool variationV = skinTexture.GetMaterialPropertyAt( deoglSkinTexture::empVariationV )
// 		.ResolveAsBool( skinState, dynamicSkin, skinTexture.GetVariationV() );
	
	const int materialIndex = tuc ? decMath::clamp( tuc->GetMaterialIndex(), 0, 16383 ) : 0;
	
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
	
	instance.GetTBOMaterial()->SetVec4At( index * 4, red, green, blue, alpha );
	
	deoglDynamicTBOFloat16 &tbo = *instance.GetTBOMaterial2();
	int index2 = index * 12; // 4 components, 3 pixels
	tbo.SetVec4At( index2, texCoordMatrix.a11, texCoordMatrix.a12, texCoordMatrix.a13, 0.0f );
	tbo.SetVec4At( index2 + 4, texCoordMatrix.a21, texCoordMatrix.a22, texCoordMatrix.a23, 0.0f );
	tbo.SetVec4At( index2 + 8, emissivityIntensity.r, emissivityIntensity.g, emissivityIntensity.b, 0.0f );
	
	#undef PACK_B
	#undef PACK_G
	#undef PACK_M
	#undef PACK
	#undef PACK_I
	#undef BITS_MASK
}

/*
void deoglGIBVH::pUpdateLocalBVHNodeExtends( const deoglGIBVHLocal &localBVH,
const oglVector *positions, const deoglBVHNode &node, deoglBVHNode &target ){
	const int primitiveCount = node.GetPrimitiveCount();
	const int firstIndex = node.GetFirstIndex();
	const deoglBVH &bvh = localBVH.GetBVH();
	
	if( primitiveCount == 0 ){
		deoglBVHNode &targetLeft = pRecalcNodes[ firstIndex ];
		deoglBVHNode &targetRight = pRecalcNodes[ firstIndex + 1 ];
		
		pUpdateLocalBVHNodeExtends( localBVH, positions, bvh.GetNodes()[ firstIndex ], targetLeft );
		pUpdateLocalBVHNodeExtends( localBVH, positions, bvh.GetNodes()[ firstIndex + 1 ], targetRight );
		
		target.SetExtends( targetLeft.GetMinExtend().Smallest( targetRight.GetMinExtend() ),
			targetLeft.GetMaxExtend().Largest( targetRight.GetMaxExtend() ) );
		
	}else{
		const int * const primitives = bvh.GetPrimitives();
		const uint16_t * const faces = localBVH.GetTBOFace()->GetDataUInt();
		decVector minExtend, maxExtend;
		int i;
		
		for( i=0; i<primitiveCount; i++ ){
			const uint16_t * const face = faces + primitives[ i ] * 4;
			const oglVector &v1 = positions[ face[ 0 ] ];
			const decVector p1( v1.x, v1.y, v1.z );
			if( i > 0 ){
				minExtend.SetSmallest( p1 );
				maxExtend.SetLargest( p1 );
				
			}else{
				minExtend = maxExtend = p1;
			}
			
			const oglVector &v2 = positions[ face[ 1 ] ];
			const decVector p2( v2.x, v2.y, v2.z );
			minExtend.SetSmallest( p2 );
			maxExtend.SetLargest( p2 );
			
			const oglVector &v3 = positions[ face[ 2 ] ];
			const decVector p3( v3.x, v3.y, v3.z );
			minExtend.SetSmallest( p3 );
			maxExtend.SetLargest( p3 );
		}
		
		target.SetExtends( minExtend, maxExtend );
	}
}
*/

void deoglGIBVH::pEnsureRecalcNodeSize( int size ){
	if( size <= pRecalcNodeSize ){
		return;
	}
	
	deoglBVHNode * const newArray = new deoglBVHNode[ size ];
	if( pRecalcNodes ){
		delete [] pRecalcNodes;
	}
	pRecalcNodes = newArray;
	pRecalcNodeSize = size;
}
