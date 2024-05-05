/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "deoglRComponent.h"
#include "deoglRComponentLOD.h"
#include "deoglRComponentTexture.h"
#include "deoglRComponentWCElement.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/texture/deoglModelTexture.h"
#include "../occlusiontest/mesh/deoglDynamicOcclusionMesh.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../world/deoglWorldCompute.h"

#include <dragengine/common/exceptions.h>


// Class deoglRComponentWCElement
///////////////////////////////////

// Constructor, Destructor
////////////////////////////

deoglRComponentWCElement::deoglRComponentWCElement( deoglRComponent &component ) :
deoglWorldComputeElement( eetComponent, &component ),
pComponent( component ){
}

deoglRComponentWCElement::~deoglRComponentWCElement(){
}



// Management
///////////////

void deoglRComponentWCElement::UpdateData( sDataElement &data ) const{
	const int shadowCombineMask = ertfRender | ertfSolid | ertfShadowNone | ertfHoles | ertfDecal | ertfDoubleSided;
	const int lodCount = decMath::min( pComponent.GetLODCount(), 15 );
	
	const decDVector &refpos = GetReferencePosition();
	data.SetExtends( pComponent.GetMinimumExtend() - refpos, pComponent.GetMaximumExtend() - refpos );
	data.SetLayerMask( pComponent.GetLayerMask() );
	
	if( pComponent.GetModel() ){
		data.highestLod = lodCount - 1;
		
		const int textureCount = pComponent.GetTextureCount();
		const deoglRModel &model = *pComponent.GetModel();
		int i, j;
		
		const int modelLodCount = decMath::min( model.GetLODCount(), 5 );
		if( modelLodCount > 1 ){
			const float errorScaling = pComponent.GetLODErrorScaling();
			
			for( i=1; i<modelLodCount; i++ ){
				data.lodFactors[ i - 1 ] = model.GetLODAt( i ).GetMaxError() * errorScaling;
			}
			
			data.lodFactors[ 1 ] = decMath::max( data.lodFactors[ 1 ], data.lodFactors[ 0 ] + FLOAT_SAFE_EPSILON );
			data.lodFactors[ 2 ] = decMath::max( data.lodFactors[ 2 ], data.lodFactors[ 1 ] + FLOAT_SAFE_EPSILON );
			data.lodFactors[ 3 ] = decMath::max( data.lodFactors[ 3 ], data.lodFactors[ 2 ] + FLOAT_SAFE_EPSILON );
		}
		
		for( i=0; i<lodCount; i++ ){
			const deoglModelLOD &modelLod = model.GetLODAt( i );
			if( modelLod.GetFaceCount() == 0 ){
				continue;
			}
			
			int shadowCombineFilter = 0;
			int shadowCombineCount = 0;
			
			for( j=0; j<textureCount; j++ ){
				if( modelLod.GetTextureAt( j ).GetFaceCount() == 0 ){
					continue;
				}
				
				const deoglSkinTexture * const texture = pComponent.GetTextureAt( j ).GetUseSkinTexture();
				if( ! texture ){
					if( shadowCombineCount > 1 ){
						data.geometryCount++;
					}
					shadowCombineCount = 0;
					continue;
				}
				
				data.geometryCount++;
				
				if( texture->GetHasOutline() ){
					data.geometryCount++;
				}
				
				// this one is a bit annoying. at the time of updating element data the RenderTaskConfig
				// of component lods have not been updated yet. we can use it only during update of element
				// geometries. we can only reduce the potential set of geometries by applying similar logic
				// here as in deoglRComponentTexture::PrepareParamBlocks()
				// 
				// rules for combining shadow textures:
				// - (renderTaskFilters & (ertfRender|ertfShadowNone|ertfDecal)) == ertfRender
				// - combineCount >= 2
				//   counts follow up textures all matching:
				//   special: if modelLod.texture[i].faceCount == 0 this counts for all as fulfilled
				//   - solid: (renderTaskFilters & ertfSolid) == ertfSolid
				//   - no holes: (renderTaskFilters & ertfHoles) == 0
				//   - useSkinTexture != nullptr
				//   - not part of a previous combine group
				//   with mask = ertfRender|ertfSolid|ertfShadowNone|ertfHoles|ertfDecal|ertfDoubleSided
				//   - (texture.renderTaskFilters & mask) == (renderTaskFilters & mask)
				if( shadowCombineCount == 0 ){
					shadowCombineFilter = texture->GetRenderTaskFilters() & shadowCombineMask;
					if( ( shadowCombineFilter & ( ertfRender | ertfShadowNone | ertfDecal | ertfSolid | ertfHoles ) ) == ( ertfRender | ertfSolid ) ){
						shadowCombineCount = 1;
					}
					
				}else if( ( texture->GetRenderTaskFilters() & shadowCombineMask ) == shadowCombineFilter ){
					shadowCombineCount++;
					
				}else{
					if( shadowCombineCount > 1 ){
						data.geometryCount++;
					}
					shadowCombineCount = 0;
				}
			}
			
			if( shadowCombineCount > 1 ){
				data.geometryCount++;
			}
		}
	}
	
	const deoglROcclusionMesh * const occmesh = pComponent.GetOcclusionMesh();
	if( occmesh ){
		if( occmesh->GetDoubleSidedFaceCount() > 0 ){
			data.geometryCount++;
		}
		if( occmesh->GetSingleSidedFaceCount() > 0 ){
			data.geometryCount++;
		}
	}
	
	data.flags = ( uint32_t )deoglWorldCompute::eefComponent;
	data.flags |= ( uint32_t )( pComponent.GetRenderStatic()
		? deoglWorldCompute::eefStatic : deoglWorldCompute::eefDynamic );
	data.flags |= ( uint32_t )( pComponent.IsGIStatic()
		? deoglWorldCompute::eefGIStatic : deoglWorldCompute::eefGIDynamic );
}

void deoglRComponentWCElement::UpdateDataGeometries( sDataElementGeometry *data ) const{
	const int lodCount = decMath::min( pComponent.GetLODCount(), 15 );
	const int textureCount = pComponent.GetTextureCount();
	int i, j;
	
	sDataElementGeometry *lastData = data;
	if( GetSPBGeometries() ){
		lastData += GetSPBGeometries()->GetCount();
	}
	
	for( i=0; i<lodCount; i++ ){
		const deoglRComponentLOD &lod = pComponent.GetLODAt( i );
		const deoglModelLOD &modelLod = lod.GetModelLODRef();
		if( modelLod.GetFaceCount() == 0 ){
			continue;
		}
		
		const deoglVAO * const vao = lod.GetUseVAO();
		if( ! vao || ! vao->GetRTSVAO() ){
			continue;
		}
		
		int shadowCombineCount = 0;
		
		for( j=0; j<textureCount; j++ ){
			if( shadowCombineCount > 0 ){
				shadowCombineCount--;
			}
			
			if( modelLod.GetTextureAt( j ).GetFaceCount() == 0 ){
				continue;
			}
			
			const deoglRComponentTexture &texture = pComponent.GetTextureAt( j );
			deoglSkinTexture * const skinTexture = texture.GetUseSkinTexture();
			if( ! skinTexture ){
				continue;
			}
			
			const deoglRenderTaskSharedInstance * const rtsi = texture.GetSharedSPBRTIGroup( i ).GetRTSInstance();
			const int spbi = texture.GetSharedSPBElement()->GetIndex();
			int filter = texture.GetRenderTaskFilters() & ~RenderFilterOutline;
			
			if( shadowCombineCount == 0 ){
				const deoglSharedSPBRTIGroup * const shadowCombineGroup = texture.GetSharedSPBRTIGroupShadow( i );
				if( shadowCombineGroup ){
					SetDataGeometry( *data, i, filter | ertfShadow | ertfCompactShadow,
						deoglSkinTexturePipelinesList::eptComponent, 0,
						skinTexture, vao, shadowCombineGroup->GetRTSInstance(), spbi );
					
					data++;
					shadowCombineCount = shadowCombineGroup->GetTextureCount();
				}
			}
			
			if( shadowCombineCount == 0 ){
				filter |= ertfShadow;
			}
			
			SetDataGeometry( *data, i, filter, texture.GetUseDecal() ? deoglSkinTexturePipelinesList::eptDecal
					: deoglSkinTexturePipelinesList::eptComponent,
				texture.GetUseDoubleSided() ? deoglSkinTexturePipelines::emDoubleSided : 0,
				skinTexture, vao, rtsi, spbi );
			
			sInfoTUC info;
			info.geometry = texture.GetTUCGeometry();
			info.depth = texture.GetTUCDepth();
			info.counter = texture.GetTUCCounter();
			info.shadow = texture.GetTUCShadow();
			info.shadowCube = texture.GetTUCShadowCube();
			info.envMap = texture.GetTUCEnvMap();
			info.luminance = texture.GetTUCLuminance();
			info.giMaterial = texture.GetTUCGIMaterial();
			SetDataGeometryTUCs( *data, info );
			data++;
			
			if( skinTexture->GetHasOutline() ){
				SetDataGeometry( *data, i, texture.GetRenderTaskFilters(),
					deoglSkinTexturePipelinesList::eptOutline, 0, skinTexture, vao, rtsi, spbi );
				
				sInfoTUC info2;
				info2.geometry = texture.GetTUCOutlineGeometry();
				info2.depth = texture.GetTUCOutlineDepth();
				info2.counter = texture.GetTUCOutlineCounter();
				SetDataGeometryTUCs( *data, info2 );
				data++;
			}
		}
	}
	
	if( pComponent.GetOcclusionMesh() ){
		const deoglROcclusionMesh &occmesh = *pComponent.GetOcclusionMesh();
		const int rtsi = pComponent.GetOccMeshSharedSPBElement()->GetIndex();
		const deoglDynamicOcclusionMesh * const dynoccmesh = pComponent.GetDynamicOcclusionMesh();
		const deoglVAO * const vao = dynoccmesh ? dynoccmesh->GetVAO()
			: pComponent.GetOcclusionMesh()->GetVBOBlock()->GetVBO()->GetVAO();
		
		if( occmesh.GetDoubleSidedFaceCount() > 0 ){
			SetDataGeometry( *data, ertfRender | ertfOcclusion | ertfDoubleSided, vao,
				pComponent.GetOccMeshSharedSPBRTIGroup( true ).GetRTSInstance(), rtsi );
			data++;
		}
		
		if( occmesh.GetSingleSidedFaceCount() > 0 ){
			SetDataGeometry( *data, ertfRender | ertfOcclusion, vao,
				pComponent.GetOccMeshSharedSPBRTIGroup( false ).GetRTSInstance(), rtsi );
			data++;
		}
	}
	
	DEASSERT_TRUE( data <= lastData )
}
