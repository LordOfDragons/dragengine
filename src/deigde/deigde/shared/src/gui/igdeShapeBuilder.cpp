/* 
 * Drag[en]gine IGDE
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

#include "igdeShapeBuilder.h"
#include "wrapper/debugdrawer/igdeWDebugDrawerShape.h"

#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/common/exceptions.h>



// Class igdeShapeBuilder
///////////////////////////

// Constructor, destructor
////////////////////////////

igdeShapeBuilder::igdeShapeBuilder(){
}

igdeShapeBuilder::~igdeShapeBuilder(){
}



// Management
///////////////

void igdeShapeBuilder::CreateSphere( decConvexVolumeList &list, const decVector &center, float radius ) const{
	if( radius < 1e-5f ) DETHROW( deeInvalidParam );
	float offset = ( sqrtf( 2.0f ) - 1.0f ) * radius;
	decConvexVolume *volume = NULL;
	
	list.RemoveAllVolumes();
	
	try{
		// create convex volume
		volume = new decConvexVolume;
		if( ! volume ) DETHROW( deeOutOfMemory );
		
		// add vertices
		volume->AddVertex( center + decVector( -offset, radius, -offset ) );
		volume->AddVertex( center + decVector( offset, radius, -offset ) );
		volume->AddVertex( center + decVector( -offset, radius, offset ) );
		volume->AddVertex( center + decVector( offset, radius, offset ) );
		
		volume->AddVertex( center + decVector( -radius, offset, -offset ) );
		volume->AddVertex( center + decVector( -offset, offset, -radius ) );
		volume->AddVertex( center + decVector( offset, offset, -radius ) );
		volume->AddVertex( center + decVector( radius, offset, -offset ) );
		volume->AddVertex( center + decVector( -radius, offset, offset ) );
		volume->AddVertex( center + decVector( -offset, offset, radius ) );
		volume->AddVertex( center + decVector( offset, offset, radius ) );
		volume->AddVertex( center + decVector( radius, offset, offset ) );
		
		volume->AddVertex( center + decVector( -radius, -offset, -offset ) );
		volume->AddVertex( center + decVector( -offset, -offset, -radius ) );
		volume->AddVertex( center + decVector( offset, -offset, -radius ) );
		volume->AddVertex( center + decVector( radius, -offset, -offset ) );
		volume->AddVertex( center + decVector( -radius, -offset, offset ) );
		volume->AddVertex( center + decVector( -offset, -offset, radius ) );
		volume->AddVertex( center + decVector( offset, -offset, radius ) );
		volume->AddVertex( center + decVector( radius, -offset, offset ) );
		
		volume->AddVertex( center + decVector( -offset, -radius, -offset ) );
		volume->AddVertex( center + decVector( offset, -radius, -offset ) );
		volume->AddVertex( center + decVector( -offset, -radius, offset ) );
		volume->AddVertex( center + decVector( offset, -radius, offset ) );
		
		// add faces
		pVolumeAddFace( volume, 2, 3, 1, 0, decVector( 0.0f, 1.0f, 0.0f ) ); // top
		
		pVolumeAddFace( volume, 0, 1, 6, 5, decVector( 0.0f, 1.0f, -1.0f ) ); // first band
		pVolumeAddFace( volume, 1, 7, 6, decVector( 1.0f, 1.0f, -1.0f ) );
		pVolumeAddFace( volume, 1, 3, 11, 7, decVector( 1.0f, 1.0f, 0.0f ) );
		pVolumeAddFace( volume, 3, 10, 11, decVector( 1.0f, 1.0f, 1.0f ) );
		pVolumeAddFace( volume, 3, 2, 9, 10, decVector( 0.0f, 1.0f, 1.0f ) );
		pVolumeAddFace( volume, 2, 8, 9, decVector( -1.0f, 1.0f, 1.0f ) );
		pVolumeAddFace( volume, 2, 0, 4, 8, decVector( -1.0f, 1.0f, 0.0f ) );
		pVolumeAddFace( volume, 0, 5, 4, decVector( -1.0f, 1.0f, -1.0f ) );
		
		pVolumeAddFace( volume, 5, 6, 14, 13, decVector( 0.0f, 0.0f, -1.0f ) ); // second band
		pVolumeAddFace( volume, 6, 7, 15, 14, decVector( 1.0f, 0.0f, -1.0f ) );
		pVolumeAddFace( volume, 7, 11, 19, 15, decVector( 1.0f, 0.0f, 0.0f ) );
		pVolumeAddFace( volume, 11, 10, 18, 19, decVector( 1.0f, 0.0f, 1.0f ) );
		pVolumeAddFace( volume, 10, 9, 17, 18, decVector( 0.0f, 0.0f, 1.0f ) );
		pVolumeAddFace( volume, 9, 8, 16, 17, decVector( -1.0f, 0.0f, 1.0f ) );
		pVolumeAddFace( volume, 8, 4, 12, 16, decVector( -1.0f, 0.0f, 0.0f ) );
		pVolumeAddFace( volume, 4, 5, 13, 12, decVector( -1.0f, 0.0f, -1.0f ) );
		
		pVolumeAddFace( volume, 13, 14, 21, 20, decVector( 0.0f, -1.0f, -1.0f ) ); // third band
		pVolumeAddFace( volume, 14, 15, 21, decVector( 1.0f, -1.0f, -1.0f ) );
		pVolumeAddFace( volume, 15, 19, 23, 21, decVector( 1.0f, -1.0f, 0.0f ) );
		pVolumeAddFace( volume, 19, 18, 23, decVector( 1.0f, -1.0f, 1.0f ) );
		pVolumeAddFace( volume, 18, 17, 22, 23, decVector( 0.0f, -1.0f, 1.0f ) );
		pVolumeAddFace( volume, 17, 16, 22, decVector( -1.0f, -1.0f, 1.0f ) );
		pVolumeAddFace( volume, 16, 12, 20, 22, decVector( -1.0f, -1.0f, 0.0f ) );
		pVolumeAddFace( volume, 12, 13, 20, decVector( -1.0f, -1.0f, -1.0f ) );
		
		pVolumeAddFace( volume, 20, 21, 23, 22, decVector( 0.0f, -1.0f, 0.0f ) ); // bottom face
		
		// add volume to list
		list.AddVolume( volume );
		
	}catch( const deException & ){
		if( volume ) delete volume;
		throw;
	}
}

void igdeShapeBuilder::CreateFrustum( decConvexVolumeList &list, const decVector &position, const decQuaternion &orientation, float sizeX, float sizeY, float angleX, float angleY, float distance ) const{
	float xn = sizeX * 0.5f;
	float yn = sizeY * 0.5f;
	float xf = xn + tanf( angleX ) * distance;
	float yf = yn + tanf( angleY ) * distance;
	float sinX = sinf( angleX );
	float cosX = cosf( angleX );
	float sinY = sinf( angleY );
	float cosY = cosf( angleY );
	decMatrix matrix = decMatrix::CreateFromQuaternion( orientation )
		* decMatrix::CreateTranslation( position );
	decConvexVolume *volume = NULL;
	
	list.RemoveAllVolumes();
	
	try{
		// create convex volume
		volume = new decConvexVolume;
		if( ! volume ) DETHROW( deeOutOfMemory );
		
		// add vertices
		volume->AddVertex( matrix * decVector( -xn, yn, 0.0f ) );
		volume->AddVertex( matrix * decVector( xn, yn, 0.0f ) );
		volume->AddVertex( matrix * decVector( xn, -yn, 0.0f ) );
		volume->AddVertex( matrix * decVector( -xn, -yn, 0.0f ) );
		volume->AddVertex( matrix * decVector( -xf, yf, distance ) );
		volume->AddVertex( matrix * decVector( xf, yf, distance ) );
		volume->AddVertex( matrix * decVector( xf, -yf, distance ) );
		volume->AddVertex( matrix * decVector( -xf, -yf, distance ) );
		
		// add faces
		pVolumeAddFace( volume, 1, 5, 6, 2, matrix.TransformNormal( decVector( cosX, 0.0f, -sinX ) ) ); // right
		pVolumeAddFace( volume, 4, 0, 3, 7, matrix.TransformNormal( decVector( -cosX, 0.0f, -sinX ) ) ); // left
		pVolumeAddFace( volume, 4, 5, 1, 0, matrix.TransformNormal( decVector( 0.0f, cosY, -sinY ) ) ); // top
		pVolumeAddFace( volume, 3, 2, 6, 7, matrix.TransformNormal( decVector( 0.0f, -cosY, -sinY ) ) ); // bottom
		pVolumeAddFace( volume, 5, 4, 7, 6, matrix.TransformView() ); // back
		pVolumeAddFace( volume, 0, 1, 2, 3, -matrix.TransformView() ); // front
		
		// add volume to list
		list.AddVolume( volume );
		
	}catch( const deException & ){
		if( volume ) delete volume;
		throw;
	}
}

void igdeShapeBuilder::CreateArrow( decConvexVolumeList &list, const decVector &from, const decVector &to, float headRadius, float headLength, float tailRadius ) const{
	if( headRadius <= 0.0f || headLength <= 0.0f || tailRadius < 0.0f ) DETHROW( deeInvalidParam );
	decVector direction, up, right, base;
	decConvexVolume *volume = NULL;
	float length, factor;
	bool hasTail;
	
	// clear the volume
	list.RemoveAllVolumes();
	
	// calculate required parameters
	direction = to - from;
	length = direction.Length();
	if( length < 1e-3f ) return;
	direction /= length;
	
	up.Set( 0.0f, 1.0f, 0.0f );
	if( fabs( up * direction ) > 0.9f ){
		up.Set( 0.0f, 0.0f, 1.0f );
	}
	right = up % direction;
	up = direction % right;
	
	if( length - headLength > 1e-4f ){
		hasTail = true;
	}else{
		hasTail = false;
		factor = length / headLength;
		headLength = length;
		headRadius *= factor;
		tailRadius *= factor;
	}
	
	// create the volume
	try{
		// create convex volume
		volume = new decConvexVolume;
		if( ! volume ) DETHROW( deeOutOfMemory );
		
		// add vertices
		volume->AddVertex( to );
		
		base = to - direction * headLength;
		volume->AddVertex( base + up * headRadius );
		volume->AddVertex( base + right * headRadius );
		volume->AddVertex( base - up * headRadius );
		volume->AddVertex( base - right * headRadius );
		
		if( hasTail ){
			volume->AddVertex( base + up * tailRadius );
			volume->AddVertex( base + right * tailRadius );
			volume->AddVertex( base - up * tailRadius );
			volume->AddVertex( base - right * tailRadius );
			
			volume->AddVertex( from + up * tailRadius );
			volume->AddVertex( from + right * tailRadius );
			volume->AddVertex( from - up * tailRadius );
			volume->AddVertex( from - right * tailRadius );
		}
		
		// add faces
		pVolumeAddFace( volume, 0, 2, 1 ); // head
		pVolumeAddFace( volume, 0, 3, 2 );
		pVolumeAddFace( volume, 0, 4, 3 );
		pVolumeAddFace( volume, 0, 1, 4 );
		if( hasTail ){
			pVolumeAddFace( volume, 1, 2, 6, 5, -direction );
			pVolumeAddFace( volume, 2, 3, 7, 6, -direction );
			pVolumeAddFace( volume, 3, 4, 8, 7, -direction );
			pVolumeAddFace( volume, 4, 1, 5, 8, -direction );
			
			pVolumeAddFace( volume, 5, 6, 10, 9, up + right ); // tail
			pVolumeAddFace( volume, 6, 7, 11, 10, right - up );
			pVolumeAddFace( volume, 7, 8, 12, 11, -( up + right ) );
			pVolumeAddFace( volume, 8, 5, 9, 12, up - right );
			
			pVolumeAddFace( volume, 9, 10, 11, 12, -direction );
			
		}else{
			pVolumeAddFace( volume, 1, 2, 3, 4, -direction );
		}
		
		// add volume to list
		list.AddVolume( volume );
		
	}catch( const deException & ){
		if( volume ) delete volume;
		throw;
	}
}

void igdeShapeBuilder::CreateArrowHead( decConvexVolumeList &list, const decVector &from, const decVector &to, float headRadius ) const{
	if( headRadius <= 0.0f ){
		DETHROW( deeInvalidParam );
	}
	
	decVector direction, up, right, base;
	decConvexVolume *volume = NULL;
	float length;
	
	// clear the volume
	list.RemoveAllVolumes();
	
	// calculate required parameters
	direction = to - from;
	length = direction.Length();
	if( length < 1e-3f ){
		return;
	}
	direction /= length;
	
	up.Set( 0.0f, 1.0f, 0.0f );
	if( fabs( up * direction ) > 0.9f ){
		up.Set( 0.0f, 0.0f, 1.0f );
	}
	right = up % direction;
	up = direction % right;
	
	// create the volume
	try{
		// create convex volume
		volume = new decConvexVolume;
		
		// add vertices
		volume->AddVertex( to );
		
		base = to - direction * length;
		volume->AddVertex( base + up * headRadius );
		volume->AddVertex( base + right * headRadius );
		volume->AddVertex( base - up * headRadius );
		volume->AddVertex( base - right * headRadius );
		
		// add faces
		pVolumeAddFace( volume, 0, 2, 1 ); // head
		pVolumeAddFace( volume, 0, 3, 2 );
		pVolumeAddFace( volume, 0, 4, 3 );
		pVolumeAddFace( volume, 0, 1, 4 );
		pVolumeAddFace( volume, 1, 2, 3, 4, -direction );
		
		// add volume to list
		list.AddVolume( volume );
		
	}catch( const deException & ){
		if( volume ){
			delete volume;
		}
		throw;
	}
}


void igdeShapeBuilder::Convert( decConvexVolumeList &cvList, deDebugDrawerShape &ddVolume ) const{
	deDebugDrawerShapeFace *ddFace = NULL;
	decConvexVolumeFace *cvVolumeFace;
	decConvexVolume *cvVolume;
	int v, volumeCount;
	int x, vertexCount;
	int f, faceCount;
	
	volumeCount = cvList.GetVolumeCount();
	ddVolume.RemoveAllFaces();
	
	try{
		for( v=0; v<volumeCount; v++ ){
			cvVolume = cvList.GetVolumeAt( v );
			
			faceCount = cvVolume->GetFaceCount();
			for( f=0; f<faceCount; f++ ){
				cvVolumeFace = cvVolume->GetFaceAt( f );
				vertexCount = cvVolumeFace->GetVertexCount();
				if( vertexCount < 3 ) continue;
				
				ddFace = new deDebugDrawerShapeFace;
				if( ! ddFace ) DETHROW( deeOutOfMemory );
				
				ddFace->SetNormal( cvVolumeFace->GetNormal() );
				
				for( x=0; x<vertexCount; x++ ){
					ddFace->AddVertex( cvVolume->GetVertexAt( cvVolumeFace->GetVertexAt( x ) ) );
				}
				
				ddVolume.AddFace( ddFace );
				ddFace = NULL;
			}
		}
		
	}catch( const deException & ){
		if( ddFace ) delete ddFace;
		throw;
	}
}

void igdeShapeBuilder::Convert( decConvexVolumeList &cvList, igdeWDebugDrawerShape &ddShape ) const{
	deDebugDrawerShapeFace *ddFace = NULL;
	decConvexVolumeFace *cvVolumeFace;
	decConvexVolume *cvVolume;
	int v, volumeCount;
	int x, vertexCount;
	int f, faceCount;
	
	volumeCount = cvList.GetVolumeCount();
	ddShape.RemoveAllFaces();
	
	try{
		for( v=0; v<volumeCount; v++ ){
			cvVolume = cvList.GetVolumeAt( v );
			
			faceCount = cvVolume->GetFaceCount();
			for( f=0; f<faceCount; f++ ){
				cvVolumeFace = cvVolume->GetFaceAt( f );
				vertexCount = cvVolumeFace->GetVertexCount();
				if( vertexCount < 3 ) continue;
				
				ddFace = new deDebugDrawerShapeFace;
				if( ! ddFace ) DETHROW( deeOutOfMemory );
				
				ddFace->SetNormal( cvVolumeFace->GetNormal() );
				
				for( x=0; x<vertexCount; x++ ){
					ddFace->AddVertex( cvVolume->GetVertexAt( cvVolumeFace->GetVertexAt( x ) ) );
				}
				
				ddShape.AddFace( ddFace );
				ddFace = NULL;
			}
		}
		
	}catch( const deException & ){
		if( ddFace ) delete ddFace;
		throw;
	}
	
	if( ddShape.GetParentDebugDrawer() ){
		ddShape.GetParentDebugDrawer()->NotifyShapeContentChanged();
	}
}

void igdeShapeBuilder::CreateSphere( deDebugDrawerShape &ddVolume, const decVector &center, float radius ) const{
	decConvexVolumeList cvList;
	CreateSphere( cvList, center, radius );
	Convert( cvList, ddVolume );
}

void igdeShapeBuilder::CreateFrustum( deDebugDrawerShape &ddVolume, const decVector &position, const decQuaternion &orientation, float sizeX, float sizeY, float angleX, float angleY, float distance ) const{
	decConvexVolumeList cvList;
	CreateFrustum( cvList, position, orientation, sizeX, sizeY, angleX, angleY, distance );
	Convert( cvList, ddVolume );
}

void igdeShapeBuilder::CreateArrow( deDebugDrawerShape &ddVolume, const decVector &from, const decVector &to, float headRadius, float headLength, float tailRadius ) const{
	decConvexVolumeList cvList;
	CreateArrow( cvList, from, to, headRadius, headLength, tailRadius );
	Convert( cvList, ddVolume );
}

void igdeShapeBuilder::CreateArrowHead( deDebugDrawerShape &ddVolume, const decVector &from, const decVector &to, float headRadius ) const{
	decConvexVolumeList cvList;
	CreateArrowHead( cvList, from, to, headRadius );
	Convert( cvList, ddVolume );
}

void igdeShapeBuilder::CreateSphere( igdeWDebugDrawerShape &ddShape, const decVector &center, float radius ) const{
	decConvexVolumeList cvList;
	CreateSphere( cvList, center, radius );
	Convert( cvList, ddShape );
}

void igdeShapeBuilder::CreateBox( igdeWDebugDrawerShape &ddShape, const decVector &center, const decVector &halfExtends ) const{
	CreateBoxFromExtends( ddShape, center - halfExtends, center + halfExtends );
}

void igdeShapeBuilder::CreateBoxFromExtends( igdeWDebugDrawerShape &ddShape, const decVector &lower, const decVector &upper ) const{
	decVector p1( lower.x, upper.y, lower.z );
	decVector p2( upper.x, upper.y, lower.z );
	decVector p3( upper.x, lower.y, lower.z );
	decVector p4( lower.x, lower.y, lower.z );
	decVector p5( lower.x, upper.y, upper.z );
	decVector p6( upper.x, upper.y, upper.z );
	decVector p7( upper.x, lower.y, upper.z );
	decVector p8( lower.x, lower.y, upper.z );
	
	ddShape.RemoveAllFaces();
	AddQuadToShape( ddShape, p1, p2, p3, p4, decVector( 0.0f, 0.0f, -1.0f ) );
	AddQuadToShape( ddShape, p2, p6, p7, p3, decVector( 1.0f, 0.0f, 0.0f ) );
	AddQuadToShape( ddShape, p6, p5, p8, p7, decVector( 0.0f, 0.0f, 1.0f ) );
	AddQuadToShape( ddShape, p5, p1, p4, p8, decVector( -1.0f, 0.0f, 0.0f ) );
	AddQuadToShape( ddShape, p5, p6, p2, p1, decVector( 0.0f, 1.0f, 0.0f ) );
	AddQuadToShape( ddShape, p4, p3, p7, p8, decVector( 0.0f, -1.0f, 0.0f ) );
	
	if( ddShape.GetParentDebugDrawer() ){
		ddShape.GetParentDebugDrawer()->NotifyShapeContentChanged();
	}
}

void igdeShapeBuilder::CreateFrustum( igdeWDebugDrawerShape &ddShape, const decVector &position, const decQuaternion &orientation, float sizeX, float sizeY, float angleX, float angleY, float distance ) const{
	decConvexVolumeList cvList;
	CreateFrustum( cvList, position, orientation, sizeX, sizeY, angleX, angleY, distance );
	Convert( cvList, ddShape );
}

void igdeShapeBuilder::CreateArrow( igdeWDebugDrawerShape &ddShape, const decVector &from, const decVector &to, float headRadius, float headLength, float tailRadius ) const{
	decConvexVolumeList cvList;
	CreateArrow( cvList, from, to, headRadius, headLength, tailRadius );
	Convert( cvList, ddShape );
}

void igdeShapeBuilder::CreateArrowHead( igdeWDebugDrawerShape &ddShape, const decVector &from, const decVector &to, float headRadius ) const{
	decConvexVolumeList cvList;
	CreateArrowHead( cvList, from, to, headRadius );
	Convert( cvList, ddShape );
}



void igdeShapeBuilder::CreateQuad( igdeWDebugDrawerShape &ddShape, const decVector &p1,
const decVector &p2, const decVector &p3, const decVector &p4, const decVector &normal ) const{
	ddShape.RemoveAllFaces();
	AddQuadToShape( ddShape, p1, p2, p3, p4, normal );
	
	if( ddShape.GetParentDebugDrawer() ){
		ddShape.GetParentDebugDrawer()->NotifyShapeContentChanged();
	}
}



void igdeShapeBuilder::AddTriToShape( igdeWDebugDrawerShape &ddShape, const decVector &p1,
const decVector &p2, const decVector &p3 ) const{
	decVector normal( ( p2 - p1 ) % ( p3 - p2 ) );
	
	normal.Normalize();
	
	AddTriToShape( ddShape, p1, p2, p3, normal );
}

void igdeShapeBuilder::AddTriToShape( igdeWDebugDrawerShape &ddShape, const decVector &p1,
const decVector &p2, const decVector &p3, const decVector &normal ) const{
	deDebugDrawerShapeFace *ddFace = NULL;
	
	try{
		ddFace = new deDebugDrawerShapeFace;
		if( ! ddFace ) DETHROW( deeOutOfMemory );
		
		ddFace->SetNormal( normal );
		
		ddFace->AddVertex( p1 );
		ddFace->AddVertex( p2 );
		ddFace->AddVertex( p3 );
		
		ddShape.AddFace( ddFace );
		ddFace = NULL;
		
	}catch( const deException & ){
		if( ddFace ) delete ddFace;
		throw;
	}
}

void igdeShapeBuilder::AddQuadToShape( igdeWDebugDrawerShape &ddShape, const decVector &p1,
const decVector &p2, const decVector &p3, const decVector &p4 ) const{
	decVector normal( ( ( p2 - p1 ) % ( p3 - p2 ) ) + ( ( p4 - p3 ) % ( p1 - p4 ) ) );
	
	normal.Normalize();
	
	AddQuadToShape( ddShape, p1, p2, p3, p4, normal );
}

void igdeShapeBuilder::AddQuadToShape( igdeWDebugDrawerShape &ddShape, const decVector &p1,
const decVector &p2, const decVector &p3, const decVector &p4, const decVector &normal ) const{
	deDebugDrawerShapeFace *ddFace = NULL;
	
	try{
		ddFace = new deDebugDrawerShapeFace;
		if( ! ddFace ) DETHROW( deeOutOfMemory );
		
		ddFace->SetNormal( normal );
		
		ddFace->AddVertex( p1 );
		ddFace->AddVertex( p2 );
		ddFace->AddVertex( p3 );
		ddFace->AddVertex( p4 );
		
		ddShape.AddFace( ddFace );
		ddFace = NULL;
		
	}catch( const deException & ){
		if( ddFace ) delete ddFace;
		throw;
	}
}



// Private Functions
//////////////////////

void igdeShapeBuilder::pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal ) const{
	decConvexVolumeFace *face = NULL;
	decVector faceNormal;
	
	try{
		face = new decConvexVolumeFace;
		if( ! face ) DETHROW( deeOutOfMemory );
		
		faceNormal = normal;
		faceNormal.Normalize();
		face->SetNormal( faceNormal );
		
		face->AddVertex( p1 );
		face->AddVertex( p2 );
		face->AddVertex( p3 );
		
		volume->AddFace( face );
		
	}catch( const deException & ){
		if( face ) delete face;
		throw;
	}
}

void igdeShapeBuilder::pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal ) const{
	decConvexVolumeFace *face = NULL;
	decVector faceNormal;
	
	try{
		face = new decConvexVolumeFace;
		if( ! face ) DETHROW( deeOutOfMemory );
		
		faceNormal = normal;
		faceNormal.Normalize();
		face->SetNormal( faceNormal );
		
		face->AddVertex( p1 );
		face->AddVertex( p2 );
		face->AddVertex( p3 );
		face->AddVertex( p4 );
		
		volume->AddFace( face );
		
	}catch( const deException & ){
		if( face ) delete face;
		throw;
	}
}

void igdeShapeBuilder::pVolumeAddFace( decConvexVolume *volume, int p1, int p2, int p3 ) const{
	decConvexVolumeFace *face = NULL;
	decVector faceNormal;
	
	try{
		face = new decConvexVolumeFace;
		if( ! face ) DETHROW( deeOutOfMemory );
		
		const decVector &v1 = volume->GetVertexAt( p1 );
		const decVector &v2 = volume->GetVertexAt( p2 );
		const decVector &v3 = volume->GetVertexAt( p3 );
		
		faceNormal = ( v2 - v1 ) % ( v3 - v2 );
		faceNormal.Normalize();
		face->SetNormal( faceNormal );
		
		face->AddVertex( p1 );
		face->AddVertex( p2 );
		face->AddVertex( p3 );
		
		volume->AddFace( face );
		
	}catch( const deException & ){
		if( face ) delete face;
		throw;
	}
}
