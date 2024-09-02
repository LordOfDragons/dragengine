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

#include "deoglWorldCompute.h"
#include "deoglWorldComputeElement.h"
#include "../skin/deoglSkinTexture.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../rendering/task/shared/deoglRenderTaskSharedTexture.h"
#include "../rendering/task/shared/deoglRenderTaskSharedVAO.h"
#include "../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../vao/deoglVAO.h"
#include "../world/deoglRWorld.h"

#include <dragengine/common/utils/decLayerMask.h>



// deoglWorldComputeElement::sDataElement
///////////////////////////////////////////

void deoglWorldComputeElement::sDataElement::SetExtends( const decDVector &a, const decDVector &b ){
	minExtendX = ( float )a.x;
	minExtendY = ( float )a.y;
	minExtendZ = ( float )a.z;
	maxExtendX = ( float )b.x;
	maxExtendY = ( float )b.y;
	maxExtendZ = ( float )b.z;
}

void deoglWorldComputeElement::sDataElement::SetLayerMask( const decLayerMask &a ){
	layerMask[ 0 ] = ( uint32_t )( a.GetMask() >> 32 );
	layerMask[ 1 ] = ( uint32_t )a.GetMask();
}

void deoglWorldComputeElement::sDataElement::SetEmptyLayerMask(){
	layerMask[ 0 ] = 0;
	layerMask[ 1 ] = 0;
}



// deoglWorldComputeElement::sInfoTUC
///////////////////////////////////////

deoglWorldComputeElement::sInfoTUC::sInfoTUC() :
geometry( nullptr ),
depth( nullptr ),
counter( nullptr ),
shadow( nullptr ),
shadowCube( nullptr ),
envMap( nullptr ),
luminance( nullptr ),
giMaterial( nullptr ){
}



// class deoglWorldComputeElement
///////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglWorldComputeElement::deoglWorldComputeElement( eElementTypes type, const void *owner ) :
pType( type ),
pOwner( owner ),
pWorldCompute( nullptr ),
pIndex( -1 ),
pUpdateRequired( false ),
pUpdateGeometriesRequired( false )
{
	DEASSERT_NOTNULL( owner );
}

deoglWorldComputeElement::~deoglWorldComputeElement(){
}



// Management
///////////////

deoglWorldCompute &deoglWorldComputeElement::GetWorldComputeRef() const{
	DEASSERT_NOTNULL( pWorldCompute )
	return *pWorldCompute;
}

const decDVector &deoglWorldComputeElement::GetReferencePosition() const{
	return pWorldCompute->GetWorld().GetReferencePosition();
}

void deoglWorldComputeElement::ComputeUpdateElement(){
	if( pWorldCompute ){
		pWorldCompute->UpdateElement( this );
	}
}

void deoglWorldComputeElement::ComputeUpdateElementGeometries(){
	if( pWorldCompute ){
		pWorldCompute->UpdateElementGeometries( this );
	}
}

void deoglWorldComputeElement::ComputeUpdateElementAndGeometries(){
	if( pWorldCompute ){
		pWorldCompute->UpdateElement( this );
		pWorldCompute->UpdateElementGeometries( this );
	}
}

void deoglWorldComputeElement::RemoveFromCompute(){
	if( pWorldCompute ){
		pWorldCompute->RemoveElement( this );
	}
}



void deoglWorldComputeElement::UpdateDataGeometries( sDataElementGeometry* ) const{
}



void deoglWorldComputeElement::SetDataGeometry( sDataElementGeometry &data, int lod, int renderFilter,
deoglSkinTexturePipelinesList::ePipelineTypes pipelinesType, int pipelineModifier,
const deoglSkinTexture *skinTexture, const deoglVAO *vao,
const deoglRenderTaskSharedInstance *instance, int spbInstance ) const {
	data.element = ( uint32_t )pIndex;
	data.lod = ( uint32_t )lod;
	
	if( ! skinTexture || skinTexture->GetRTSIndex() == -1 || ! vao || ! vao->GetRTSVAO()
	|| ! instance || spbInstance < -1 ){
		return;
	}
	
	data.renderFilter = ( uint32_t )renderFilter;
	data.skinTexture = ( uint32_t )skinTexture->GetRTSIndex();
	data.pipelineBase = ( uint32_t )( pipelinesType | ( pipelineModifier << 8 ) );
	data.vao = ( uint32_t )vao->GetRTSVAO()->GetIndex();
	data.instance = ( uint32_t )instance->GetIndex();
	data.spbInstance = ( uint32_t )( spbInstance + 1 );
}

void deoglWorldComputeElement::SetDataGeometryTUCs( sDataElementGeometry &data, const sInfoTUC &info ) const{
	const deoglTexUnitsConfig ** const tucs = ( const deoglTexUnitsConfig** )&info;
	int i;
	
	for( i=0; i<8; i++ ){
		if( ! tucs[ i ] ){
			continue;
		}
		
		const deoglRenderTaskSharedTexture * const rtsTexture = tucs[ i ]->GetRTSTexture();
		if( ! rtsTexture ){
			continue;
		}
		
		data.tucs[ i / 2 ] |= ( uint32_t )rtsTexture->GetIndex() << ( ( i % 2 ) ? 16 : 0 );
	}
}

void deoglWorldComputeElement::SetDataGeometry( sDataElementGeometry &data, int renderFilter,
const deoglVAO *vao, const deoglRenderTaskSharedInstance *instance, int spbInstance ) const{
	data.element = ( uint32_t )pIndex;
	data.lod = 0;
	data.skinTexture = 0;
	data.tucs[ 0 ] = data.tucs[ 1 ] = data.tucs[ 2 ] = data.tucs[ 3 ] = 0;
	
	if( ! vao || ! vao->GetRTSVAO() || ! instance || spbInstance < -1 ){
		return;
	}
	
	data.renderFilter = ( uint32_t )renderFilter;
	data.pipelineBase = ( uint32_t )deoglSkinTexturePipelinesList::PipelineTypesCount;
	data.vao = ( uint32_t )vao->GetRTSVAO()->GetIndex();
	data.instance = ( uint32_t )instance->GetIndex();
	data.spbInstance = ( uint32_t )( spbInstance + 1 );
}



void deoglWorldComputeElement::SetWorldCompute( deoglWorldCompute *worldCompute ){
	pWorldCompute = worldCompute;
}

void deoglWorldComputeElement::SetIndex( int index ){
	pIndex = index;
}

void deoglWorldComputeElement::SetUpdateRequired( bool updateRequired ){
	pUpdateRequired = updateRequired;
}

void deoglWorldComputeElement::SetUpdateGeometriesRequired( bool updateRequired ){
	pUpdateGeometriesRequired = updateRequired;
}
