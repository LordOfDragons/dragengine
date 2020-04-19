/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#include "deoglRTBufferObject.h"
#include "deoglRTLogger.h"
#include "deoglRTChoices.h"
#include "deoglRenderThread.h"
#include "../capabilities/deoglCapabilities.h"
#include "../extensions/deoglExtensions.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../shaders/paramblock/deoglSPBlockMemory.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListUBO.h"
#include "../shaders/paramblock/shared/deoglSharedSPBListSSBO.h"
#include "../shapes/deoglShapeManager.h"
#include "../shapes/deoglShapeSphere.h"
#include "../shapes/deoglShapeBox.h"
#include "../shapes/deoglShapeCylinder.h"
#include "../shapes/deoglShapeCapsule.h"
#include "../vbo/deoglSharedVBOListList.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglVBOAttribute.h"

#include <dragengine/common/exceptions.h>



// Class deoglRTBufferObject
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRTBufferObject::deoglRTBufferObject( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pShapeManager( NULL ),
pSharedVBOListList( NULL ),

pLayoutSkinInstanceUBO( NULL ),
pInstanceArraySizeUBO( 0 ),

pLayoutSkinInstanceSSBO( NULL ),
pInstanceArraySizeSSBO( 0 ),

pLayoutOccMeshInstanceUBO( NULL ),
pOccMeshInstanceArraySizeUBO( 0 ),

pLayoutOccMeshInstanceSSBO( NULL ),
pOccMeshInstanceArraySizeSSBO( 0 ),

pBillboardSPBListUBO( NULL ),

pTemporaryVBOData( NULL ),
pTemporaryVBODataSize( 0 )
{
	memset( pSharedVBOListByType, '\0', sizeof( pSharedVBOListByType ) );
	memset( pSharedSPBList, '\0', sizeof( pSharedSPBList ) );
}

deoglRTBufferObject::~deoglRTBufferObject(){
	pCleanUp();
}



// Management
///////////////

void deoglRTBufferObject::Init(){
	pSharedVBOListList = new deoglSharedVBOListList( pRenderThread, 4194304 ); // 8388608
	pCreateSharedVBOLists();
	
	pCreateShapes();
	
	pCreateLayoutSkinInstance();
	pCreateLayoutOccMeshInstance();
	pCreateLayoutInstanceIndex();
	
	pCreateSharedSPBLists(); // depends on pCreateLayout*()
	
	pBillboardSPBListUBO = new deoglSharedSPBListUBO( pRenderThread, GetLayoutSkinInstanceUBO() );
}



deoglSharedVBOList &deoglRTBufferObject::GetSharedVBOListForType( eSharedVBOLists type ) const{
	return *pSharedVBOListByType[ type ];
}

deoglSharedSPBList &deoglRTBufferObject::GetSharedSPBList( eSharedSPBLists type ) const{
	return *pSharedSPBList[ type ];
}

char *deoglRTBufferObject::GetTemporaryVBOData( int size ){
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( size > pTemporaryVBODataSize ){
		if( pTemporaryVBOData ){
			delete [] pTemporaryVBOData;
			pTemporaryVBOData = NULL;
			pTemporaryVBODataSize = 0;
		}
		
		pTemporaryVBOData = new char[ size ];
		pTemporaryVBODataSize = size;
	}
	
	return pTemporaryVBOData;
}



// Private Functions
//////////////////////

void deoglRTBufferObject::pCleanUp(){
	int i;
	
	pCleanUpReports();
	
	if( pShapeManager ){
		delete pShapeManager;
	}
	
	if( pBillboardSPBListUBO ){
		delete pBillboardSPBListUBO;
	}
	
	if( pLayoutOccMeshInstanceUBO ){
		pLayoutOccMeshInstanceUBO->FreeReference();
	}
	if( pLayoutOccMeshInstanceSSBO ){
		pLayoutOccMeshInstanceSSBO->FreeReference();
	}
	
	if( pLayoutSkinInstanceUBO ){
		pLayoutSkinInstanceUBO->FreeReference();
	}
	if( pLayoutSkinInstanceSSBO ){
		pLayoutSkinInstanceSSBO->FreeReference();
	}
	
	// pSharedVBOListByType are not deleted since they are shared from pSharedVBOListList
	
	for( i=esspblSkinInstanceUBO; i<=esspblOccMeshInstanceSSBO; i++ ){
		if( pSharedSPBList[ i ] ){
			delete pSharedSPBList[ i ];
		}
	}
	
	if( pSharedVBOListList ){
		delete pSharedVBOListList;
	}
	
	if( pTemporaryVBOData ){
		delete [] pTemporaryVBOData;
	}
}

void deoglRTBufferObject::pCleanUpReports(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	
	if( pSharedSPBList[ esspblSkinInstanceUBO ] ){
		const deoglSharedSPBList &list = *pSharedSPBList[ esspblSkinInstanceUBO ];
		logger.LogInfoFormat( "pSharedSPBList(SkinInstanceUBO): count=%d size=%.2fMB",
			list.GetCount(), 1e-6f * list.GetLayout().GetBufferSize() * list.GetCount() );
	}
	if( pSharedSPBList[ esspblSkinInstanceSSBO ] ){
		const deoglSharedSPBList &list = *pSharedSPBList[ esspblSkinInstanceSSBO ];
		logger.LogInfoFormat( "pSharedSPBList(SkinInstanceSSBO): count=%d size=%.2fMB",
			list.GetCount(), 1e-6f * list.GetLayout().GetBufferSize() * list.GetCount() );
	}
	
	if( pSharedSPBList[ esspblOccMeshInstanceUBO ] ){
		const deoglSharedSPBList &list = *pSharedSPBList[ esspblOccMeshInstanceUBO ];
		logger.LogInfoFormat( "pSharedSPBList(OccMeshInstanceUBO): count=%d size=%.2fMB",
			list.GetCount(), 1e-6f * list.GetLayout().GetBufferSize() * list.GetCount() );
	}
	if( pSharedSPBList[ esspblOccMeshInstanceSSBO ] ){
		const deoglSharedSPBList &list = *pSharedSPBList[ esspblOccMeshInstanceSSBO ];
		logger.LogInfoFormat( "pSharedSPBList(OccMeshInstanceSSBO): count=%d size=%.2fMB",
			list.GetCount(), 1e-6f * list.GetLayout().GetBufferSize() * list.GetCount() );
	}
}



void deoglRTBufferObject::pCreateLayoutSkinInstance(){
	const int uboMaxSize = pRenderThread.GetCapabilities().GetUBOMaxSize();
	const int maxUBOIndexCount = pRenderThread.GetConfiguration().GetMaxSPBIndexCount();
	const int maxSSBOIndexCount = pRenderThread.GetConfiguration().GetMaxSPBIndexCount();
	
	pLayoutSkinInstanceUBO = deoglSkinShader::CreateLayoutSkinInstanceUBO( pRenderThread );
	pLayoutSkinInstanceUBO->SetElementCount( decMath::min( maxUBOIndexCount,
		uboMaxSize / pLayoutSkinInstanceUBO->GetElementStride() ) );
	pLayoutSkinInstanceUBO->SetBindingPoint( deoglSkinShader::eubInstanceParameters );
// 	pLayoutSkinInstanceUBO->DebugPrintConfig( "pLayoutSkinInstanceUBO" );
	
	pRenderThread.GetLogger().LogInfoFormat(
		"pLayoutSkinInstanceUBO: uboMaxSize=%d elementStride=%d maxCount=%d",
		uboMaxSize, pLayoutSkinInstanceUBO->GetElementStride(),
		pLayoutSkinInstanceUBO->GetElementCount() );
	
	// shared spb layout using SSBO if supported
	if( pRenderThread.GetExtensions().GetHasExtension(
	deoglExtensions::ext_ARB_shader_storage_buffer_object ) ){
		const int ssboMaxSize = pRenderThread.GetCapabilities().GetSSBOMaxSize();
		
		pLayoutSkinInstanceSSBO = deoglSkinShader::CreateLayoutSkinInstanceSSBO( pRenderThread );
		pLayoutSkinInstanceSSBO->SetElementCount( decMath::min( maxSSBOIndexCount,
			ssboMaxSize / pLayoutSkinInstanceSSBO->GetElementStride() ) );
		pLayoutSkinInstanceSSBO->SetBindingPoint( deoglSkinShader::essboInstanceParameters );
		
		pRenderThread.GetLogger().LogInfoFormat(
			"pLayoutSkinInstanceSSBO: uboMaxSize=%d elementStride=%d maxCount=%d",
			ssboMaxSize, pLayoutSkinInstanceSSBO->GetElementStride(),
			pLayoutSkinInstanceSSBO->GetElementCount() );
	}
}

void deoglRTBufferObject::pCreateLayoutOccMeshInstance(){
	// ubo layout
	const bool rowMajor = pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	const int uboMaxSize = pRenderThread.GetCapabilities().GetUBOMaxSize();
	const int maxUBOIndexCount = pRenderThread.GetConfiguration().GetMaxSPBIndexCount();
	const int maxSSBOIndexCount = pRenderThread.GetConfiguration().GetMaxSPBIndexCount();
	
	pLayoutOccMeshInstanceUBO = new deoglSPBlockUBO( pRenderThread );
	pLayoutOccMeshInstanceUBO->SetRowMajor( rowMajor );
	pLayoutOccMeshInstanceUBO->SetParameterCount( 1 );
	pLayoutOccMeshInstanceUBO->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3 pMatrixModel
	
	pLayoutOccMeshInstanceUBO->MapToStd140();
	pLayoutOccMeshInstanceUBO->CalculateOffsetPadding();
	pLayoutOccMeshInstanceUBO->SetElementCount( decMath::min( maxUBOIndexCount,
		uboMaxSize / pLayoutOccMeshInstanceUBO->GetElementStride() ) );
	pLayoutOccMeshInstanceUBO->SetBindingPoint( deoglSkinShader::eubInstanceParameters );
	
	pRenderThread.GetLogger().LogInfoFormat(
		"pLayoutOccMeshInstanceUBO: uboMaxSize=%d elementStride=%d maxCount=%d",
		uboMaxSize, pLayoutOccMeshInstanceUBO->GetElementStride(),
		pLayoutOccMeshInstanceUBO->GetElementCount() );
	
	// ssbo layout
	if( pRenderThread.GetExtensions().GetHasExtension(
	deoglExtensions::ext_ARB_shader_storage_buffer_object ) ){
		const int ssboMaxSize = pRenderThread.GetCapabilities().GetSSBOMaxSize();
		
		pLayoutOccMeshInstanceSSBO = new deoglSPBlockSSBO( pRenderThread );
		pLayoutOccMeshInstanceSSBO->SetRowMajor( rowMajor );
		pLayoutOccMeshInstanceSSBO->SetParameterCount( 1 );
		pLayoutOccMeshInstanceSSBO->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 ); // mat4x3 pMatrixModel
		
		pLayoutOccMeshInstanceSSBO->MapToStd140();
		pLayoutOccMeshInstanceSSBO->CalculateOffsetPadding();
		pLayoutOccMeshInstanceSSBO->SetBindingPoint( deoglSkinShader::essboInstanceParameters );
		
		pLayoutOccMeshInstanceSSBO->SetElementCount( decMath::min( maxSSBOIndexCount,
			ssboMaxSize / pLayoutOccMeshInstanceSSBO->GetElementStride() ) );
		
		pRenderThread.GetLogger().LogInfoFormat(
			"pLayoutOccMeshInstanceSSBO: uboMaxSize=%d elementStride=%d maxCount=%d",
			uboMaxSize, pLayoutOccMeshInstanceSSBO->GetElementStride(),
			pLayoutOccMeshInstanceSSBO->GetElementCount() );
	}
}

void deoglRTBufferObject::pCreateLayoutInstanceIndex(){
	const int maxUBOIndexCount = pRenderThread.GetConfiguration().GetMaxSPBIndexCount();
	const int maxSSBOIndexCount = pRenderThread.GetConfiguration().GetMaxSPBIndexCount();
	
	// spb instance constant using int32 indices and ivec4 packing.
	pInstanceArraySizeUBO = decMath::min( maxUBOIndexCount,
		pRenderThread.GetCapabilities().GetUBOMaxSize() / 16 );
	pRenderThread.GetLogger().LogInfoFormat( "pInstanceArraySizeUBO %d", pInstanceArraySizeUBO );
	
	// spb instance constant using int32 indices and ivec4 packing.
	if( pRenderThread.GetExtensions().GetHasExtension(
	deoglExtensions::ext_ARB_shader_storage_buffer_object ) ){
		pInstanceArraySizeSSBO = decMath::min( maxSSBOIndexCount,
			pRenderThread.GetCapabilities().GetSSBOMaxSize() / 16 );
		pRenderThread.GetLogger().LogInfoFormat( "pInstanceArraySizeSSBO %d", pInstanceArraySizeSSBO );
	}
}

void deoglRTBufferObject::pCreateSharedSPBLists(){
	pSharedSPBList[ esspblSkinInstanceUBO ] = new deoglSharedSPBListUBO(
		pRenderThread, pLayoutSkinInstanceUBO );
	
	pSharedSPBList[ esspblOccMeshInstanceUBO ] = new deoglSharedSPBListUBO(
		pRenderThread, pLayoutOccMeshInstanceUBO );
	
	if( pRenderThread.GetExtensions().GetHasExtension(
	deoglExtensions::ext_ARB_shader_storage_buffer_object ) ){
		pSharedSPBList[ esspblSkinInstanceSSBO ] = new deoglSharedSPBListSSBO(
			pRenderThread, pLayoutSkinInstanceSSBO );
		
		pSharedSPBList[ esspblOccMeshInstanceSSBO ] = new deoglSharedSPBListSSBO(
			pRenderThread, pLayoutOccMeshInstanceSSBO );
	}
}



void deoglRTBufferObject::pCreateSharedVBOLists(){
	deoglVBOLayout layout;
	
	// static model: esvbolStaticModel
	// normal and tanget are stored in a reduced format of size 6 bytes. ATI can't handle this
	// situation as the alignment is no more 4 bytes. for these two values a padding of 2 bytes
	// is added
	// 
	// name       | offset | type   | components
	// -----------+--------+--------+-----------
	// position   |      0 | float  | x, y, z
	// normal     |     12 | short  | x, y, z
	// tangent    |     20 | short  | x, y, z, w
	// tex-coord  |     28 | float  | u, v
	layout.SetAttributeCount( 4 );
	layout.SetStride( 36 ); // 32 is said to be best alignment
	layout.SetIndexType( deoglVBOLayout::eitUnsignedInt );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 1 ).SetOffset( 12 );
	
	layout.GetAttributeAt( 2 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 2 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 2 ).SetOffset( 20 );
	
	layout.GetAttributeAt( 3 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 3 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 3 ).SetOffset( 28 );
	
	pSharedVBOListByType[ esvbolStaticModel ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
	// static model: esvbolStaticModel
	// normal and tanget are stored in a reduced format of size 6 bytes. ATI can't handle this
	// situation as the alignment is no more 4 bytes. for these two values a padding of 2 bytes
	// is added
	// 
	// name       | offset | type   | components
	// -----------+--------+--------+-----------
	// position   |      0 | float  | x, y, z
	// normal     |     12 | short  | x, y, z
	// tangent    |     20 | short  | x, y, z, w
	// tex-coord  |     28 | float  | u, v
	// weight     |     36 | iint   | weight
	layout.SetAttributeCount( 5 );
	layout.SetStride( 40 );
	layout.SetIndexType( deoglVBOLayout::eitUnsignedInt );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 1 ).SetOffset( 12 );
	
	layout.GetAttributeAt( 2 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 2 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 2 ).SetOffset( 20 );
	
	layout.GetAttributeAt( 3 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 3 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 3 ).SetOffset( 28 );
	
	layout.GetAttributeAt( 4 ).SetComponentCount( 1 );
	layout.GetAttributeAt( 4 ).SetDataType( deoglVBOAttribute::edtIInt );
	layout.GetAttributeAt( 4 ).SetOffset( 36 );
	
	pSharedVBOListByType[ esvbolStaticModelWeight ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
	// static model: esvbolStaticModelTCS1
	// normal and tanget are stored in a reduced format of size 6 bytes. ATI can't handle this
	// situation as the alignment is no more 4 bytes. for these two values a padding of 2 bytes
	// is added
	// 
	// name       | offset | type   | components
	// -----------+--------+--------+-----------
	// position   |      0 | float  | x, y, z
	// normal     |     12 | short  | x, y, z
	// tangent    |     20 | short  | x, y, z, w
	// tex-coord  |     28 | float  | u, v
	// tangent2   |     36 | short  | x, y, z, w
	// tex-coord2 |     44 | float  | u, v
	layout.SetAttributeCount( 6 );
	layout.SetStride( 52 );
	layout.SetIndexType( deoglVBOLayout::eitUnsignedInt );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 1 ).SetOffset( 12 );
	
	layout.GetAttributeAt( 2 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 2 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 2 ).SetOffset( 20 );
	
	layout.GetAttributeAt( 3 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 3 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 3 ).SetOffset( 28 );
	
	layout.GetAttributeAt( 4 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 4 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 4 ).SetOffset( 36 );
	
	layout.GetAttributeAt( 5 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 5 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 5 ).SetOffset( 44 );
	
	pSharedVBOListByType[ esvbolStaticModelTCS1 ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
	// static model: esvbolStaticModelTCS2
	// normal and tanget are stored in a reduced format of size 6 bytes. ATI can't handle this
	// situation as the alignment is no more 4 bytes. for these two values a padding of 2 bytes
	// is added
	// 
	// name       | offset | type   | components
	// -----------+--------+--------+-----------
	// position   |      0 | float  | x, y, z
	// normal     |     12 | short  | x, y, z
	// tangent    |     20 | short  | x, y, z, w
	// tex-coord  |     28 | float  | u, v
	// tangent2   |     36 | short  | x, y, z, w
	// tex-coord2 |     44 | float  | u, v
	// tangent3   |     52 | short  | x, y, z, w
	// tex-coord3 |     60 | float  | u, v
	layout.SetAttributeCount( 8 );
	layout.SetStride( 68 );
	layout.SetIndexType( deoglVBOLayout::eitUnsignedInt );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 1 ).SetOffset( 12 );
	
	layout.GetAttributeAt( 2 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 2 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 2 ).SetOffset( 20 );
	
	layout.GetAttributeAt( 3 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 3 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 3 ).SetOffset( 28 );
	
	layout.GetAttributeAt( 4 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 4 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 4 ).SetOffset( 36 );
	
	layout.GetAttributeAt( 5 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 5 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 5 ).SetOffset( 44 );
	
	layout.GetAttributeAt( 6 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 6 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 6 ).SetOffset( 52 );
	
	layout.GetAttributeAt( 7 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 7 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 7 ).SetOffset( 60 );
	
	pSharedVBOListByType[ esvbolStaticModelTCS2 ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
#if 0
	// static model: esvbolStaticModelTCSet
	// tanget is stored in a reduced format of size 6 bytes. ATI can't handle this situation as the
	// alignment is no more 4 bytes. for this value a padding of 2 bytes is added.
	// 
	// name       | offset | type   | components
	// -----------+--------+--------+-----------
	// tangent    |      0 | short  | x, y, z, w
	// tex-coord  |      8 | float  | u, v
	layout.SetAttributeCount( 2 );
	layout.SetStride( 16 ); // 32 is said to be best alignment
	layout.SetIndexType( deoglVBOLayout::eitUnsignedInt );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 1 ).SetOffset( 8 );
	
	pSharedVBOListByType[ esvbolStaticModelTCSet ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
#endif
	
	// simple models with one bone per vertex or a max of 4 weights per vertex: esvbolSimpleModel
	// normal and tanget are stored in a reduced format of size 6 bytes. ATI can't handle this
	// situation as the alignment is no more 4 bytes. for these two values a padding of 2 bytes
	// is added
	// 
	// name       | offset | type    | components
	// -----------+--------+---------+---------------
	// position   |      0 | float   | x, y, z
	// normal     |     12 | short   | x, y, z
	// tangent    |     20 | short   | x, y, z
	// tex-coord  |     28 | float   | u, v
	// bones      |     36 | iushort | b1, b2, b3, b4
	// weights    |     44 | ubyte   | w1, w2, w3, w4
	layout.SetAttributeCount( 6 );
	layout.SetStride( 48 );
	layout.SetIndexType( deoglVBOLayout::eitUnsignedInt );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 1 ).SetOffset( 12 );
	
	layout.GetAttributeAt( 2 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 2 ).SetDataType( deoglVBOAttribute::edtShort );
	layout.GetAttributeAt( 2 ).SetOffset( 20 );
	
	layout.GetAttributeAt( 3 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 3 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 3 ).SetOffset( 28 );
	
	layout.GetAttributeAt( 4 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 4 ).SetDataType( deoglVBOAttribute::edtIUShort );
	layout.GetAttributeAt( 4 ).SetOffset( 36 );
	
	layout.GetAttributeAt( 5 ).SetComponentCount( 4 );
	layout.GetAttributeAt( 5 ).SetDataType( deoglVBOAttribute::edtUByte );
	layout.GetAttributeAt( 5 ).SetOffset( 44 );
	
	pSharedVBOListByType[ esvbolSimpleModel ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
	// model position weight indicies used to transform model positions using transform feedback.
	// indices are stored as non-normalized integer since padding is anways required.
	// 
	// name       | offset | type   | components
	// -----------+--------+--------+---------------
	// position   |      0 | float  | x, y, z
	// weight     |     12 | iint   | weight
	layout.SetAttributeCount( 2 );
	layout.SetStride( 16 );
	layout.SetIndexType( deoglVBOLayout::eitNone );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 1 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtIInt );
	layout.GetAttributeAt( 1 ).SetOffset( 12 );
	
	pSharedVBOListByType[ esvbolModelPositionWeightIndices ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
	// model calculate normals and tangents. contains position, normal and tangent indices for each
	// triangle corner as well as the tangent factors.
	// 
	// name          | offset | type   | components
	// --------------+--------+--------+---------------
	// positionIndex |      0 | iint   | pi1, pi2, pi3
	// normalIndex   |     12 | iint   | ni1, ni2, ni3
	// tangentIndex  |     24 | iint   | ti1, ti2, ti3
	// tangentFactor |     36 | float  | d1, d2
	layout.SetAttributeCount( 4 );
	layout.SetStride( 44 );
	layout.SetIndexType( deoglVBOLayout::eitNone );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtIInt );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtIInt );
	layout.GetAttributeAt( 1 ).SetOffset( 12 );
	
	layout.GetAttributeAt( 2 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 2 ).SetDataType( deoglVBOAttribute::edtIInt );
	layout.GetAttributeAt( 2 ).SetOffset( 24 );
	
	layout.GetAttributeAt( 3 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 3 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 3 ).SetOffset( 36 );
	
	pSharedVBOListByType[ esvbolModelCalcNormalTangent ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
	// model write skinned vbo. contains position, normal and tangent index for each render point.
	// 
	// name          | offset | type   | components
	// --------------+--------+--------+---------------
	// position      |      0 | iint   | posIndex
	// normal        |      4 | iint   | norIndex
	// tangent       |      8 | iint   | tanIndex
	// negateTangent |     12 | float  | flipTan
	layout.SetAttributeCount( 4 );
	layout.SetStride( 16 );
	layout.SetIndexType( deoglVBOLayout::eitNone );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 1 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtIInt );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 1 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtIInt );
	layout.GetAttributeAt( 1 ).SetOffset( 4 );
	
	layout.GetAttributeAt( 2 ).SetComponentCount( 1 );
	layout.GetAttributeAt( 2 ).SetDataType( deoglVBOAttribute::edtIInt );
	layout.GetAttributeAt( 2 ).SetOffset( 8 );
	
	layout.GetAttributeAt( 3 ).SetComponentCount( 1 );
	layout.GetAttributeAt( 3 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 3 ).SetOffset( 12 );
	
	pSharedVBOListByType[ esvbolModelWriteSkinnedVBO ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
	// static occlusion mesh: esvbolStaticOcclusionMesh
	// 
	// name       | offset | type   | components
	// -----------+--------+--------+------------
	// position   |      0 | float  | x, y, z
	layout.SetAttributeCount( 1 );
	layout.SetStride( 12 ); // best performance with multiple of 32/64? (16 would yield 2 vertices in one cache run)
	layout.SetIndexType( pRenderThread.GetChoices().GetSharedVBOUseBaseVertex()
		? deoglVBOLayout::eitUnsignedShort : deoglVBOLayout::eitUnsignedInt );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	pSharedVBOListByType[ esvbolStaticOcclusionMesh ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
	// mathematical shapes
	// 
	// name       | offset | type   | components
	// -----------+--------+--------+------------
	// position   |      0 | float  | x, y, z
	// selector   |     12 | float  | selector
	layout.SetAttributeCount( 2 );
	layout.SetStride( 16 ); // best performance with multiple of 32/64? (16 would yield 2 vertices in one cache run)
	layout.SetIndexType( deoglVBOLayout::eitNone ); // use indices later on
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 3 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	layout.GetAttributeAt( 1 ).SetComponentCount( 1 );
	layout.GetAttributeAt( 1 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 1 ).SetOffset( 12 );
	
	pSharedVBOListByType[ esvbolMathShapes ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
	
	// canvas paint
	// 
	// name       | offset | type   | components
	// -----------+--------+--------+------------
	// position   |      0 | float  | x, y
	layout.SetAttributeCount( 1 );
	layout.SetStride( 8 );
	layout.SetIndexType( deoglVBOLayout::eitNone );
	
	layout.GetAttributeAt( 0 ).SetComponentCount( 2 );
	layout.GetAttributeAt( 0 ).SetDataType( deoglVBOAttribute::edtFloat );
	layout.GetAttributeAt( 0 ).SetOffset( 0 );
	
	pSharedVBOListByType[ esvbolCanvasPaint ] = pSharedVBOListList->GetWith( layout, GL_STATIC_DRAW );
}

void deoglRTBufferObject::pCreateShapes(){
	deoglShape *shape = NULL;
	int i;
	
	pShapeManager = new deoglShapeManager;
	
	try{
		shape = new deoglShapeSphere( pRenderThread );
		pShapeManager->AddShape( shape );
		shape = NULL;
		
		shape = new deoglShapeBox( pRenderThread );
		pShapeManager->AddShape( shape );
		shape = NULL;
		
		shape = new deoglShapeCylinder( pRenderThread );
		pShapeManager->AddShape( shape );
		shape = NULL;
		
		shape = new deoglShapeCapsule( pRenderThread );
		pShapeManager->AddShape( shape );
		shape = NULL;
		
	}catch( const deException & ){
		if( shape ){
			delete shape;
		}
		throw;
	}
	
	for( i=esSphere; i<=esCapsule; i++ ){
		pShapeManager->GetShapeAt( i )->GetVBOBlock();
	}
}
