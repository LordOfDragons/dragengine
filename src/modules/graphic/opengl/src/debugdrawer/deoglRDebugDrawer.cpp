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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRDebugDrawer.h"
#include "deoglDebugDrawerShape.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>



// Class deoglRDebugDrawer
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDebugDrawer::deoglRDebugDrawer( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pParentWorld( NULL ),

pVisible( false ),
pXRay( false ),

pHasShapes( false ),
pHasFaces( false ),

pShapes( NULL ),
pShapeCount( 0 ),

pVBOData( NULL ),
pVBOPointCount( 0 ),
pVBO( 0 ),
pVAO( NULL ),
pDirtyVBO( true ),

pWorldMarkedRemove( false ){
	LEAK_CHECK_CREATE( renderThread, DebugDrawer );
}

deoglRDebugDrawer::~deoglRDebugDrawer(){
	LEAK_CHECK_FREE( pRenderThread, DebugDrawer );
	if( pShapes ){
		delete [] pShapes;
	}
	if( pVBOData ){
		delete [] pVBOData;
	}
	if( pVAO ){
		delete pVAO;
	}
	
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLBuffer( pVBO );
}



// Management
///////////////

void deoglRDebugDrawer::SetParentWorld( deoglRWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	pParentWorld = parentWorld;
	
	//if( pOctreeNode ){
	//	pOctreeNode->GetBillboardList().Remove( this );
	//}
}



void deoglRDebugDrawer::SetMatrix( const decDMatrix &matrix ){
	pMatrix = matrix;
}

void deoglRDebugDrawer::SetVisible( bool visible ){
	pVisible = visible;
}

void deoglRDebugDrawer::SetXRay( bool xray ){
	pXRay = xray;
}



void deoglRDebugDrawer::UpdateShapes( const deDebugDrawer &debugDrawer ){
	const int shapeCount = debugDrawer.GetShapeCount();
	int i;
	
	if( shapeCount != pShapeCount ){
		if( pShapes ){
			delete [] pShapes;
			pShapes = NULL;
			pShapeCount = 0;
		}
		
		if( shapeCount > 0 ){
			pShapes = new deoglDebugDrawerShape[ shapeCount ];
			pShapeCount = shapeCount;
		}
	}
	
	for( i=0; i<shapeCount; i++ ){
		const deDebugDrawerShape &ddshape = *debugDrawer.GetShapeAt( i );
		
		pShapes[ i ].SetMatrix( decMatrix::CreateWorld( ddshape.GetPosition(), ddshape.GetOrientation(), ddshape.GetScale() ) );
		pShapes[ i ].SetEdgeColor( ddshape.GetEdgeColor() );
		pShapes[ i ].SetFillColor( ddshape.GetFillColor() );
		pShapes[ i ].SetShapeList( ddshape.GetShapeList() );
	}
	
	pWriteVBOData( debugDrawer );
	pUpdateHasFlags( debugDrawer );
	
	pDirtyVBO = true;
}



deoglDebugDrawerShape &deoglRDebugDrawer::GetShapeAt( int index ) const{
	if( index < 0 || index >= pShapeCount ){
		DETHROW( deeInvalidParam );
	}
	return pShapes[ index ];
}



void deoglRDebugDrawer::UpdateVBO(){
	if( ! pDirtyVBO ){
		return;
	}
	
	if( pVBOPointCount == 0 ){
		pDirtyVBO = false;
		return;
	}
	
	// create vbo if not existing
	if( ! pVBO ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	// write vbo data to vbo
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( oglVector3 ) * pVBOPointCount, NULL, GL_STATIC_DRAW ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, sizeof( oglVector3 ) * pVBOPointCount, pVBOData, GL_STATIC_DRAW ) );
	
	// create vao if not existing
	if( ! pVAO ){
		deoglVBOLayout vboLayout;
		vboLayout.SetAttributeCount( 1 );
		vboLayout.SetStride( 12 );
		vboLayout.SetSize( 12 * pVBOPointCount );
		vboLayout.SetIndexType( deoglVBOLayout::eitNone );
		
		deoglVBOAttribute &attrPos = vboLayout.GetAttributeAt( 0 );
		attrPos.SetComponentCount( 3 );
		attrPos.SetDataType( deoglVBOAttribute::edtFloat );
		attrPos.SetOffset( 0 );
		
		pVAO = new deoglVAO( pRenderThread );
		pVAO->SetIndexType( deoglVBOLayout::eitNone );
		OGL_CHECK( pRenderThread, pglBindVertexArray( pVAO->GetVAO() ) );
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
		vboLayout.SetVAOAttributeAt( pRenderThread, 0, 0 );
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
	}
	
	pDirtyVBO = false;
}



void deoglRDebugDrawer::PrepareQuickDispose(){
	pParentWorld = NULL;
	//pOctreeNode = NULL;
}



// Render world usage
///////////////////////

void deoglRDebugDrawer::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}



// Private Functions
//////////////////////

void deoglRDebugDrawer::pUpdateHasFlags( const deDebugDrawer &debugDrawer ){
	const int shapeCount = debugDrawer.GetShapeCount();
	int i;
	
	pHasShapes = false;
	//pHasFaces = false;
	
	for( i=0; i<shapeCount; i++ ){
		const deDebugDrawerShape &ddshape = *debugDrawer.GetShapeAt( i );
		
		if( ddshape.GetShapeList().GetCount() > 0 ){
			pHasShapes = true;
		}
		//if( ddshape.GetFaceCount() > 0 ){
		//	pHasFaces = true;
		//}
	}
	
	pHasFaces = pVBOPointCount > 0; // this covers also convex hull shapes
}

void deoglRDebugDrawer::pWriteVBOData( const deDebugDrawer &debugDrawer ){
	int pointCount = 0;
	int i;
	
	for( i=0; i<pShapeCount; i++ ){
		pointCount += pShapes[ i ].CalcRequiredPoints( *debugDrawer.GetShapeAt( i ), pointCount );
	}
	
	if( pointCount != pVBOPointCount ){
		if( pVBOData ){
			delete [] pVBOData;
			pVBOData = NULL;
			pVBOPointCount = 0;
		}
		
		if( pointCount > 0 ){
			pVBOData = new oglVector3[ pointCount ];
			pVBOPointCount = pointCount;
		}
	}
	
	if( pVBOData ){
		for( i=0; i<pShapeCount; i++ ){
			pShapes[ i ].WriteVBOData( *debugDrawer.GetShapeAt( i ), pVBOData );
		}
	}
}
