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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglLPTProbe.h"
#include "deoglLightProbeTexture.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglLightProbeTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightProbeTexture::deoglLightProbeTexture( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ){
	pProbes = NULL;
	pProbeCount = 0;
	pProbeSize = 0;
	
	pTexturePosition = NULL;
	pTextureDirection = NULL;
	pTextureProbes = NULL;
	pProbesPerLine = 0;
	pMaxProbeCount = 0;
	pUsedWidth = 0;
	pUsedHeight = 0;
	
	pVBO = 0;
	pVAO = 0;
	pVBOData = NULL;
	pVBOPointCount = 0;
	
	try{
		// create textures for storing 160 probes
		/*
		pTexturePosition = new deoglTexture( ogl );
		if( ! pTexturePosition ) DETHROW( deeOutOfMemory );
		pTexturePosition->SetCompressed( false );
		pTexturePosition->SetMipMapped( false );
		pTexturePosition->SetSize( 32, 32 );
		pTexturePosition->SetFBOFormat( 8, 3, true );
		pTexturePosition->CreateTexture();
		
		pTextureDirection = new deoglTexture( ogl );
		if( ! pTextureDirection ) DETHROW( deeOutOfMemory );
		pTextureDirection->SetCompressed( false );
		pTextureDirection->SetMipMapped( false );
		pTextureDirection->SetSize( 32, 32 );
		pTextureDirection->SetFBOFormat( 8, 3, true );
		pTextureDirection->CreateTexture();
		*/
		pTextureProbes = new deoglTexture( renderThread );
		pTextureProbes->SetSize( 32, 32 );
		pTextureProbes->SetFBOFormat( 3, true );
		pTextureProbes->SetMipMapped( false );
		pTextureProbes->CreateTexture();
		
		pProbesPerLine = 5; // 6 * 5 = 30 (<=32)
		pMaxProbeCount = 160; // probesPerLine * 32
		
	}catch( const deException & ){
		if( pTextureProbes ) delete pTextureProbes;
		if( pTextureDirection ) delete pTextureDirection;
		if( pTexturePosition ) delete pTexturePosition;
		throw;
	}
}

deoglLightProbeTexture::~deoglLightProbeTexture(){
	if( pVAO ) pglDeleteVertexArrays( 1, &pVAO );
	if( pVBO ) pglDeleteBuffers( 1, &pVBO );
	if( pVBOData ) delete [] pVBOData;
	if( pProbes ) delete [] pProbes;
	if( pTextureProbes ) delete pTextureProbes;
	if( pTextureDirection ) delete pTextureDirection;
	if( pTexturePosition ) delete pTexturePosition;
}



// Management
///////////////

const deoglLPTProbe& deoglLightProbeTexture::GetProbeAt( int index ){
	if( index < 0 || index >= pProbeCount ) DETHROW( deeInvalidParam );
	
	return pProbes[ index ];
}

void deoglLightProbeTexture::AddProbe( const decVector &position ){
	if( pProbeCount == pProbeSize ){
		int newSize = pProbeSize + 20;
		deoglLPTProbe *newArray = new deoglLPTProbe[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pProbes ){
			memcpy( newArray, pProbes, sizeof( deoglLPTProbe ) * pProbeSize );
			delete [] pProbes;
		}
		pProbes = newArray;
		pProbeSize = newSize;
	}
	
	if( pProbeCount == pMaxProbeCount ){
		int width = pTextureProbes->GetWidth();
		int height = pTextureProbes->GetHeight();
		
		if( width == height ){
			width *= 2;
			
		}else{ // width > height
			height = width;
		}
		
		//pTexturePosition->SetSize( width, height );
		//pTexturePosition->CreateTexture();
		//pTextureDirection->SetSize( width, height );
		//pTextureDirection->CreateTexture();
		pTextureProbes->SetSize( width, height );
		pTextureProbes->CreateTexture();
		
		pProbesPerLine = width / 6;
		pMaxProbeCount = pProbesPerLine * height;
	}
	
	pProbes[ pProbeCount++ ].SetPosition( position );
}

void deoglLightProbeTexture::RemoveAllProbes(){
	pProbeCount = 0;
	pUsedWidth = 0;
	pUsedHeight = 0;
}



void deoglLightProbeTexture::Update(){
	int p, x, y, pointCount, dataSize;
	sVBOData *pdata;
	
	// calculate texture parameters
	for( p=0; p<pProbeCount; p++ ){
		y = p / pProbesPerLine;
		x = ( p - y * pProbesPerLine ) * 6;
		pProbes[ p ].SetTextureCoordinates( x, y );
	}
	
	if( pProbeCount == 0 ){
		pUsedWidth = 0;
		pUsedHeight = 0;
		
	}else{
		pUsedHeight = ( ( pProbeCount - 1 ) / pProbesPerLine ) + 1;
		
		if( pUsedHeight > 1 ){
			pUsedWidth = pProbesPerLine * 6;
			
		}else{
			pUsedWidth = pProbeCount * 6;
		}
	}
	
	// update vbo data in CPU memory
	pointCount = pProbeCount * 6;
	dataSize = pointCount * 28;
	
	if( pointCount > pVBOPointCount ){
		sVBOData *newArray = new sVBOData[ pointCount ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pVBOData ) delete [] pVBOData;
		pVBOData = newArray;
		pVBOPointCount = pointCount;
	}
	
	pdata = pVBOData;
	
	for( p=0; p<pProbeCount; p++ ){
		const decPoint &tc = pProbes[ p ].GetTextureCoordinates();
		const decVector &position = pProbes[ p ].GetPosition();
		
		// positive x axis
		pdata->x = ( GLushort )tc.x;
		pdata->y = ( GLushort )tc.y;
		pdata->px = position.x;
		pdata->py = position.y;
		pdata->pz = position.z;
		pdata->dx = 1.0f;
		pdata->dy = 0.0f;
		pdata->dz = 0.0f;
		pdata++;
		
		// negative x axis
		pdata->x = ( GLushort )( tc.x + 1 );
		pdata->y = ( GLushort )tc.y;
		pdata->px = position.x;
		pdata->py = position.y;
		pdata->pz = position.z;
		pdata->dx = -1.0f;
		pdata->dy = 0.0f;
		pdata->dz = 0.0f;
		pdata++;
		
		// positive y axis
		pdata->x = ( GLushort )( tc.x + 2 );
		pdata->y = ( GLushort )tc.y;
		pdata->px = position.x;
		pdata->py = position.y;
		pdata->pz = position.z;
		pdata->dx = 0.0f;
		pdata->dy = 1.0f;
		pdata->dz = 0.0f;
		pdata++;
		
		// negative y axis
		pdata->x = ( GLushort )( tc.x + 3 );
		pdata->y = ( GLushort )tc.y;
		pdata->px = position.x;
		pdata->py = position.y;
		pdata->pz = position.z;
		pdata->dx = 0.0f;
		pdata->dy = -1.0f;
		pdata->dz = 0.0f;
		pdata++;
		
		// positive z axis
		pdata->x = ( GLushort )( tc.x + 4 );
		pdata->y = ( GLushort )tc.y;
		pdata->px = position.x;
		pdata->py = position.y;
		pdata->pz = position.z;
		pdata->dx = 0.0f;
		pdata->dy = 0.0f;
		pdata->dz = 1.0f;
		pdata++;
		
		// negative z axis
		pdata->x = ( GLushort )( tc.x + 5 );
		pdata->y = ( GLushort )tc.y;
		pdata->px = position.x;
		pdata->py = position.y;
		pdata->pz = position.z;
		pdata->dx = 0.0f;
		pdata->dy = 0.0f;
		pdata->dz = -1.0f;
		pdata++;
	}
	
	// write vbo data from CPU memory to vbo
	if( ! pVBO ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBO ) );
		if( ! pVBO ) DETHROW( deeOutOfMemory );
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, dataSize, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, dataSize, pVBOData, GL_STREAM_DRAW ) );
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	
	// update vao if required
	if( ! pVAO && pVBO ){
		OGL_CHECK( pRenderThread, pglGenVertexArrays( 1, &pVAO ) );
		if( ! pVAO ) DETHROW( deeOutOfMemory );
		
		OGL_CHECK( pRenderThread, pglBindVertexArray( pVAO ) );
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBO ) );
		
		OGL_CHECK( pRenderThread, pglVertexAttribPointer( 0, 2, GL_UNSIGNED_SHORT, GL_FALSE, 28, ( const GLvoid * )0 ) ); // point position
		OGL_CHECK( pRenderThread, pglVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 28, ( const GLvoid * )4 ) ); // probe position
		OGL_CHECK( pRenderThread, pglVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, 28, ( const GLvoid * )16 ) ); // probe direction
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
}
}
