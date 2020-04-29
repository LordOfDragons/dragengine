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

#include "deoglRDebugDrawer.h"
#include "deoglDebugDrawerShape.h"
#include "../extensions/deoglExtResult.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../delayedoperation/deoglDelayedDeletion.h"
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

class deoglRDebugDrawerDeletion : public deoglDelayedDeletion{
public:
	GLuint vbo;
	deoglVAO *vao;
	
	deoglRDebugDrawerDeletion() :
	vbo( 0 ),
	vao( NULL ){
	}
	
	virtual ~deoglRDebugDrawerDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread &renderThread ){
		if( vao ){
			delete vao;
		}
		if( vbo ){
			OGL_CHECK( renderThread, pglDeleteBuffers( 1, &vbo ) );
		}
	}
};

deoglRDebugDrawer::~deoglRDebugDrawer(){
	LEAK_CHECK_FREE( pRenderThread, DebugDrawer );
	if( pShapes ){
		delete [] pShapes;
	}
	if( pVBOData ){
		delete [] pVBOData;
	}
	
	// delayed deletion of opengl containing objects
	deoglRDebugDrawerDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRDebugDrawerDeletion;
		delayedDeletion->vao = pVAO;
		delayedDeletion->vbo = pVBO;
		pRenderThread.GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pRenderThread.GetLogger().LogException( e );
		//throw; -> otherwise terminate
	}
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
