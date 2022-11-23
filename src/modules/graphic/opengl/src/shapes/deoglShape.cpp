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
#include <string.h>
#include <stdlib.h>

#include "deoglShape.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTChoices.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglSharedVBOListList.h"

#include <dragengine/common/exceptions.h>



// Class deoglShape
/////////////////////

// Struct sVBOData
////////////////////

void deoglShape::sVBOData::Set( float nx, float ny, float nz, bool nselector ){
	x = ( GLfloat )nx;
	y = ( GLfloat )ny;
	z = ( GLfloat )nz;
	
	if( nselector ){
		selector = ( GLfloat )1.0f;
		
	}else{
		selector = ( GLfloat )0.0f;
	}
}

void deoglShape::sVBOData::SetSelTrue( float nx, float ny, float nz ){
	x = ( GLfloat )nx;
	y = ( GLfloat )ny;
	z = ( GLfloat )nz;
	selector = ( GLfloat )1.0f;
}

void deoglShape::sVBOData::SetSelFalse( float nx, float ny, float nz ){
	x = ( GLfloat )nx;
	y = ( GLfloat )ny;
	z = ( GLfloat )nz;
	selector = ( GLfloat )0.0f;
}

void deoglShape::sVBOData::SetFrom( const sVBOData &data ){
	x = data.x;
	y = data.y;
	z = data.z;
	selector = data.selector;
}

void deoglShape::sVBOData::SetFromSelTrue( const sVBOData &data ){
	x = data.x;
	y = data.y;
	z = data.z;
	selector = ( GLfloat )1.0f;
}

void deoglShape::sVBOData::SetFromSelFalse( const sVBOData &data ){
	x = data.x;
	y = data.y;
	z = data.z;
	selector = ( GLfloat )0.0f;
}



// Constructor, destructor
////////////////////////////

deoglShape::deoglShape( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pVBOBlock( NULL ),
pPointOffsetFaces( 0 ),
pPointCountFaces( 0 ),
pPointOffsetLines( 0 ),
pPointCountLines( 0 ){
}

deoglShape::~deoglShape(){
	if( pVBOBlock ){
		pVBOBlock->GetVBO()->RemoveBlock( pVBOBlock );
		pVBOBlock->FreeReference();
	}
}



// Management
///////////////

deoglSharedVBOBlock *deoglShape::GetVBOBlock(){
	if( ! pVBOBlock ){
		const int pointCount = pPointCountFaces + pPointCountLines;
		//printf( "pointCount %i\n", pointCount );
		try{
			pVBOBlock = pRenderThread.GetBufferObject().GetSharedVBOListForType(
				deoglRTBufferObject::esvbolMathShapes ).AddData( sizeof( sVBOData ) * pointCount );
			
			AddVBOLines( ( sVBOData* )( pVBOBlock->GetData() ) + pPointOffsetLines );
			AddVBOFaces( ( sVBOData* )( pVBOBlock->GetData() ) + pPointOffsetFaces );
			
		}catch( const deException & ){
			if( pVBOBlock ){
				pVBOBlock->GetVBO()->RemoveBlock( pVBOBlock );
				pVBOBlock->FreeReference();
				pVBOBlock = NULL;
			}
			throw;
		}
	}
	
	return pVBOBlock;
}

void deoglShape::SetPointOffsetFaces( int pointOffset ){
	if( pointOffset < 0 ){
		DETHROW( deeInvalidParam );
	}
	pPointOffsetFaces = pointOffset;
}

void deoglShape::SetPointCountFaces( int pointCount ){
	if( pointCount < 0 || ( pointCount % 4 ) > 0 ){
		DETHROW( deeInvalidParam );
	}
	pPointCountFaces = pointCount;
}

void deoglShape::SetPointOffsetLines( int pointLines ){
	if( pointLines < 0 ){
		DETHROW( deeInvalidParam );
	}
	pPointOffsetLines = pointLines;
}

void deoglShape::SetPointCountLines( int pointCount ){
	if( pointCount < 0 || ( pointCount % 2 ) > 0 ){
		DETHROW( deeInvalidParam );
	}
	pPointCountLines = pointCount;
}



deoglVAO *deoglShape::GetVAO(){
	return GetVBOBlock()->GetVBO()->GetVAO();
}

void deoglShape::ActivateVAO(){
	pglBindVertexArray( GetVAO()->GetVAO() );
}

void deoglShape::RenderLines(){
	OGL_CHECK( pRenderThread, glDrawArrays( GL_LINES, GetVBOBlock()->GetOffset() + pPointOffsetLines, pPointCountLines ) );
}

void deoglShape::RenderLines( const deoglRenderPlan &plan ){
	if( plan.GetRenderStereo() && pRenderThread.GetChoices().GetRenderStereoVSLayer() ){
		const GLint first[ 2 ] = {
			GetVBOBlock()->GetOffset() + pPointOffsetLines,
			GetVBOBlock()->GetOffset() + pPointOffsetLines };
		
		const GLsizei count[ 2 ] = { pPointCountLines, pPointCountLines };
		
		OGL_CHECK( pRenderThread, pglMultiDrawArrays( GL_LINES, first, count, 2 ) );
		
	}else{
		OGL_CHECK( pRenderThread, glDrawArrays( GL_LINES, GetVBOBlock()->GetOffset() + pPointOffsetLines, pPointCountLines ) );
	}
}

void deoglShape::RenderFaces(){
	OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLES, GetVBOBlock()->GetOffset() + pPointOffsetFaces, pPointCountFaces ) );
}

void deoglShape::RenderFaces( const deoglRenderPlan &plan ){
	if( plan.GetRenderStereo() && pRenderThread.GetChoices().GetRenderStereoVSLayer() ){
		const GLint first[ 2 ] = {
			GetVBOBlock()->GetOffset() + pPointOffsetFaces,
			GetVBOBlock()->GetOffset() + pPointOffsetFaces };
		
		const GLsizei count[ 2 ] = { pPointCountFaces, pPointCountFaces };
		
		OGL_CHECK( pRenderThread, pglMultiDrawArrays( GL_TRIANGLES, first, count, 2 ) );
		
	}else{
		OGL_CHECK( pRenderThread, glDrawArrays( GL_TRIANGLES, GetVBOBlock()->GetOffset() + pPointOffsetFaces, pPointCountFaces ) );
	}
}
