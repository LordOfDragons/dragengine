/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
