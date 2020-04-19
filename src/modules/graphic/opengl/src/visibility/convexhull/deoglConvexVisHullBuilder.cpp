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
#include <stdlib.h>
#include <string.h>

#include "deoglConvexVisHullBuilder.h"
#include "../../triangles/deoglTriangleSorter.h"
#include "../../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../../utils/collision/deoglDCollisionDetection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/resources/occlusionmesh/deOcclusionMesh.h>



// Class deoglConvexVisHullBuilder
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglConvexVisHullBuilder::deoglConvexVisHullBuilder(){
}

deoglConvexVisHullBuilder::~deoglConvexVisHullBuilder(){
}



// Management
///////////////

void deoglConvexVisHullBuilder::BuildSphere( decConvexVolumeList &volumeList, const decVector &position, float radius ) const{
	decConvexVolume *volume = NULL;
	float offset;
	
	volumeList.RemoveAllVolumes();
	
	try{
		volume = new decConvexVolume;
		
		offset = ( sqrtf( 2.0f ) - 1.0f ) * radius;
		
		volume->AddVertex( position + decVector( -offset, radius, -offset ) );
		volume->AddVertex( position + decVector( offset, radius, -offset ) );
		volume->AddVertex( position + decVector( -offset, radius, offset ) );
		volume->AddVertex( position + decVector( offset, radius, offset ) );
		
		volume->AddVertex( position + decVector( -radius, offset, -offset ) );
		volume->AddVertex( position + decVector( -offset, offset, -radius ) );
		volume->AddVertex( position + decVector( offset, offset, -radius ) );
		volume->AddVertex( position + decVector( radius, offset, -offset ) );
		volume->AddVertex( position + decVector( -radius, offset, offset ) );
		volume->AddVertex( position + decVector( -offset, offset, radius ) );
		volume->AddVertex( position + decVector( offset, offset, radius ) );
		volume->AddVertex( position + decVector( radius, offset, offset ) );
		
		volume->AddVertex( position + decVector( -radius, -offset, -offset ) );
		volume->AddVertex( position + decVector( -offset, -offset, -radius ) );
		volume->AddVertex( position + decVector( offset, -offset, -radius ) );
		volume->AddVertex( position + decVector( radius, -offset, -offset ) );
		volume->AddVertex( position + decVector( -radius, -offset, offset ) );
		volume->AddVertex( position + decVector( -offset, -offset, radius ) );
		volume->AddVertex( position + decVector( offset, -offset, radius ) );
		volume->AddVertex( position + decVector( radius, -offset, offset ) );
		
		volume->AddVertex( position + decVector( -offset, -radius, -offset ) );
		volume->AddVertex( position + decVector( offset, -radius, -offset ) );
		volume->AddVertex( position + decVector( -offset, -radius, offset ) );
		volume->AddVertex( position + decVector( offset, -radius, offset ) );
		
		// add top face
		pAddQuad( volume, 2, 3, 1, 0, decVector( 0.0f, 1.0f, 0.0f ) );
		
		// add faces of the first band
		pAddQuad( volume, 0, 1, 6, 5, decVector( 0.0f, 1.0f, -1.0f ).Normalized() );
		pAddTriangle( volume, 1, 7, 6, decVector( 1.0f, 1.0f, -1.0f ).Normalized() );
		pAddQuad( volume, 1, 3, 11, 7, decVector( 1.0f, 1.0f, 0.0f ).Normalized() );
		pAddTriangle( volume, 3, 10, 11, decVector( 1.0f, 1.0f, 1.0f ).Normalized() );
		pAddQuad( volume, 3, 2, 9, 10, decVector( 0.0f, 1.0f, 1.0f ).Normalized() );
		pAddTriangle( volume, 2, 8, 9, decVector( -1.0f, 1.0f, 1.0f ).Normalized() );
		pAddQuad( volume, 2, 0, 4, 8, decVector( -1.0f, 1.0f, 0.0f ).Normalized() );
		pAddTriangle( volume, 0, 5, 4, decVector( -1.0f, 1.0f, -1.0f ).Normalized() );
		
		// add faces of the second band
		pAddQuad( volume, 5, 6, 14, 13, decVector( 0.0f, 0.0f, -1.0f ) );
		pAddQuad( volume, 6, 7, 15, 14, decVector( 1.0f, 0.0f, -1.0f ).Normalized() );
		pAddQuad( volume, 7, 11, 19, 15, decVector( 1.0f, 0.0f, 0.0f ) );
		pAddQuad( volume, 11, 10, 18, 19, decVector( 1.0f, 0.0f, 1.0f ).Normalized() );
		pAddQuad( volume, 10, 9, 17, 18, decVector( 0.0f, 0.0f, 1.0f ) );
		pAddQuad( volume, 9, 8, 16, 17, decVector( -1.0f, 0.0f, 1.0f ).Normalized() );
		pAddQuad( volume, 8, 4, 12, 16, decVector( -1.0f, 0.0f, 0.0f ) );
		pAddQuad( volume, 4, 5, 13, 12, decVector( -1.0f, 0.0f, -1.0f ).Normalized() );
		
		// add faces of the third band
		pAddQuad( volume, 13, 14, 21, 20, decVector( 0.0f, -1.0f, -1.0f ).Normalized() );
		pAddTriangle( volume, 14, 15, 21, decVector( 1.0f, -1.0f, -1.0f ).Normalized() );
		pAddQuad( volume, 15, 19, 23, 21, decVector( 1.0f, -1.0f, 0.0f ).Normalized() );
		pAddTriangle( volume, 19, 18, 23, decVector( 1.0f, -1.0f, 1.0f ).Normalized() );
		pAddQuad( volume, 18, 17, 22, 23, decVector( 0.0f, -1.0f, 1.0f ).Normalized() );
		pAddTriangle( volume, 17, 16, 22, decVector( -1.0f, -1.0f, 1.0f ).Normalized() );
		pAddQuad( volume, 16, 12, 20, 22, decVector( -1.0f, -1.0f, 0.0f ).Normalized() );
		pAddTriangle( volume, 12, 13, 20, decVector( -1.0f, -1.0f, -1.0f ).Normalized() );
		
		// add bottom face
		pAddQuad( volume, 20, 21, 23, 22, decVector( 0.0f, -1.0f, 0.0f ) );
		
		// add volume to list
		volumeList.AddVolume( volume );
		
	}catch( const deException & ){
		if( volume ){
			delete volume;
		}
		throw;
	}
}

void deoglConvexVisHullBuilder::BuildCone( decConvexVolumeList &volumeList, const decMatrix &matrix,
float distance, float angle, int resolution ) const{
	if( resolution < 3 ){
		DETHROW( deeInvalidParam );
	}
	
	// calculate the circle radius required for the cone. due to the tessellation the cone would penetrate
	// into the visibility hull between points. to counter this the radius is enlarged by the difference
	// halfway between two circle points. r is d times tan(a). the enlarged radius r' and r are related by
	// r = r' * cos(a/2) in the middle between points where the deviation is the largest. this gives
	// r' = r / cos(a/2) and thus r' = d * tan(a) / cos(a/2)
	const float circleAngleStep = PI * 2.0f / ( float )resolution;
	const float circleRadius = distance * tanf( angle ) / cosf( circleAngleStep * 0.5f );
	
	decConvexVolumeFace *face = NULL;
	decConvexVolume *volume = NULL;
	decVector circlePoint;
	float circleAngle;
	int i;
	
	volumeList.RemoveAllVolumes();
	
	try{
		volume = new decConvexVolume;
		
		// add vertices. cone tip and base circle
		volume->AddVertex( matrix.GetPosition() );
		
		circlePoint.z = distance;
		for( i=0; i<resolution; i++ ){
			circleAngle = circleAngleStep * ( float )i;
			circlePoint.x = sinf( circleAngle ) * circleRadius;
			circlePoint.y = cosf( circleAngle ) * circleRadius;
			volume->AddVertex( matrix * circlePoint );
		}
		
		// add faces. mantle and base
		for( i=0; i<resolution; i++ ){
			pAddTriangle( volume, 0, 1 + i, 1 + ( i + 1 ) % resolution );
		}
		
		face = new decConvexVolumeFace;
		face->SetNormal( matrix.TransformView() );
		for( i=0; i<resolution; i++ ){
			face->AddVertex( 1 + ( resolution - 1 - i ) );
		}
		volume->AddFace( face );
		face = NULL;
		
		volumeList.AddVolume( volume );
		
	}catch( const deException & ){
		if( face ){
			delete face;
		}
		if( volume ){
			delete volume;
		}
		throw;
	}
}

void deoglConvexVisHullBuilder::BuildFrustum( decConvexVolumeList &volumeList, const decMatrix &matrix, float farDistance, float angleX, float angleY ) const{
	decConvexVolume *volume = NULL;
	float sinX = sinf( angleX );
	float cosX = cosf( angleX );
	float sinY = sinf( angleY );
	float cosY = cosf( angleY );
	float zf = farDistance;
	float xn = 0.0f;
	float yn = 0.0f;
	float xf = xn + tanf( angleX ) * zf;
	float yf = yn + tanf( angleY ) * zf;
	
	volumeList.RemoveAllVolumes();
	
	try{
		volume = new decConvexVolume;
		
		volume->AddVertex( matrix * decVector( -xn, yn, 0.0f ) );
		volume->AddVertex( matrix * decVector( xn, yn, 0.0f ) );
		volume->AddVertex( matrix * decVector( xn, -yn, 0.0f ) );
		volume->AddVertex( matrix * decVector( -xn, -yn, 0.0f ) );
		volume->AddVertex( matrix * decVector( -xf, yf, zf ) );
		volume->AddVertex( matrix * decVector( xf, yf, zf ) );
		volume->AddVertex( matrix * decVector( xf, -yf, zf ) );
		volume->AddVertex( matrix * decVector( -xf, -yf, zf ) );
		
		pAddQuad( volume, 1, 5, 6, 2, matrix.TransformNormal( decVector( cosX, 0.0f, -sinX ) ) ); // right
		pAddQuad( volume, 4, 0, 3, 7, matrix.TransformNormal( decVector( -cosX, 0.0f, -sinX ) ) ); // left
		pAddQuad( volume, 4, 5, 1, 0, matrix.TransformNormal( decVector( 0.0f, cosY, -sinY ) ) ); // top
		pAddQuad( volume, 3, 2, 6, 7, matrix.TransformNormal( decVector( 0.0f, -cosY, -sinY ) ) ); // bottom
		pAddQuad( volume, 5, 4, 7, 6, matrix.TransformView() ); // back
		pAddQuad( volume, 0, 1, 2, 3, -matrix.TransformView() ); // front
		
		volumeList.AddVolume( volume );
		
	}catch( const deException & ){
		if( volume ){
			delete volume;
		}
		throw;
	}
}


#if 0
void deoglConvexVisHullBuilder::SetCropVolumeBox( decConvexVolume &volume, const decDMatrix &matrix,
const decDVector &minExtend, const decDVector &maxExtend ) const{
	// the crop volume vox is like a conventional box volume just with the face normals flipped
	const decVector normalRight = matrix.TransformRight().ToVector();
	const decVector normalFront = matrix.TransformView().ToVector();
	const decVector normalUp = matrix.TransformUp().ToVector();
	decConvexVolumeFace *face = NULL;
	
	volume.SetEmpty();
	
	volume.AddVertex( matrix.Transform( minExtend.x, maxExtend.y, minExtend.z ).ToVector() ); // -x,  y, -z
	volume.AddVertex( matrix.Transform( maxExtend.x, maxExtend.y, minExtend.z ).ToVector() ); //  x,  y, -z
	volume.AddVertex( matrix.Transform( maxExtend.x, minExtend.y, minExtend.z ).ToVector() ); //  x, -y, -z
	volume.AddVertex( matrix.Transform( minExtend.x, minExtend.y, minExtend.z ).ToVector() ); // -x, -y, -z
	volume.AddVertex( matrix.Transform( minExtend.x, maxExtend.y, maxExtend.z ).ToVector() ); // -x,  y,  z
	volume.AddVertex( matrix.Transform( maxExtend.x, maxExtend.y, maxExtend.z ).ToVector() ); //  x,  y,  z
	volume.AddVertex( matrix.Transform( maxExtend.x, minExtend.y, maxExtend.z ).ToVector() ); //  x, -y,  z
	volume.AddVertex( matrix.Transform( minExtend.x, minExtend.y, maxExtend.z ).ToVector() ); // -x, -y,  z
	
	try{
		face = new decConvexVolumeFace;
		face->SetNormal( normalFront );
		face->AddVertex( 3 );
		face->AddVertex( 2 );
		face->AddVertex( 1 );
		face->AddVertex( 0 );
		volume.AddFace( face );
		face = NULL;
		
		face = new decConvexVolumeFace;
		face->SetNormal( -normalRight );
		face->AddVertex( 2 );
		face->AddVertex( 6 );
		face->AddVertex( 5 );
		face->AddVertex( 1 );
		volume.AddFace( face );
		face = NULL;
		
		face = new decConvexVolumeFace;
		face->SetNormal( -normalFront );
		face->AddVertex( 6 );
		face->AddVertex( 7 );
		face->AddVertex( 4 );
		face->AddVertex( 5 );
		volume.AddFace( face );
		face = NULL;
		
		face = new decConvexVolumeFace;
		face->SetNormal( normalRight );
		face->AddVertex( 7 );
		face->AddVertex( 3 );
		face->AddVertex( 0 );
		face->AddVertex( 4 );
		volume.AddFace( face );
		face = NULL;
		
		face = new decConvexVolumeFace;
		face->SetNormal( -normalUp );
		face->AddVertex( 0 );
		face->AddVertex( 1 );
		face->AddVertex( 5 );
		face->AddVertex( 4 );
		volume.AddFace( face );
		face = NULL;
		
		face = new decConvexVolumeFace;
		face->SetNormal( normalUp );
		face->AddVertex( 7 );
		face->AddVertex( 6 );
		face->AddVertex( 2 );
		face->AddVertex( 3 );
		volume.AddFace( face );
		
	}catch( const deException & ){
		if( face ){
			delete face;
		}
		throw;
	}
}
#endif


void deoglConvexVisHullBuilder::GetVolumeExtends( const decConvexVolumeList &volumeList, decVector &minExtend, decVector &maxExtend ) const{
	int v, volumeCount = volumeList.GetVolumeCount();
	const decConvexVolume *volume;
	bool firstVertex = true;
	int x, vertexCount;
	
	for( v=0; v<volumeCount; v++ ){
		volume = volumeList.GetVolumeAt( v );
		vertexCount = volume->GetVertexCount();
		
		for( x=0; x<vertexCount; x++ ){
			const decVector &vertex = volume->GetVertexAt( x );
			
			if( firstVertex ){
				minExtend = vertex;
				maxExtend = vertex;
				firstVertex = false;
				
			}else{
				if( vertex.x < minExtend.x ){
					minExtend.x = vertex.x;
					
				}else if( vertex.x > maxExtend.x ){
					maxExtend.x = vertex.x;
				}
				
				if( vertex.y < minExtend.y ){
					minExtend.y = vertex.y;
					
				}else if( vertex.y > maxExtend.y ){
					maxExtend.y = vertex.y;
				}
				
				if( vertex.z < minExtend.z ){
					minExtend.z = vertex.z;
					
				}else if( vertex.z > maxExtend.z ){
					maxExtend.z = vertex.z;
				}
			}
		}
	}
}

void deoglConvexVisHullBuilder::GetTransformedVolumeExtends( const decConvexVolumeList &volumeList,
const decDMatrix &matrix, decDVector &minExtend, decDVector &maxExtend ) const{
	const int volumeCount = volumeList.GetVolumeCount();
	
	if( volumeCount > 0 ){
		bool first = true;
		int i, j;
		
		for( i=0; i<volumeCount; i++ ){
			const decConvexVolume &convexVolume = *volumeList.GetVolumeAt( i );
			const int vertexCount = convexVolume.GetVertexCount();
			
			for( j=0; j<vertexCount; j++ ){
				const decDVector position = matrix * decDVector( convexVolume.GetVertexAt( j ) );
				
				if( first ){
					minExtend = position;
					maxExtend = position;
					first = false;
					
				}else{
					if( position.x < minExtend.x ){
						minExtend.x = position.x;
						
					}else if( position.x > maxExtend.x ){
						maxExtend.x = position.x;
					}
					
					if( position.y < minExtend.y ){
						minExtend.y = position.y;
						
					}else if( position.y > maxExtend.y ){
						maxExtend.y = position.y;
					}
					
					if( position.z < minExtend.z ){
						minExtend.z = position.z;
						
					}else if( position.z > maxExtend.z ){
						maxExtend.z = position.z;
					}
				}
			}
		}
	}
}



void deoglConvexVisHullBuilder::CropByTriangles( decConvexVolumeList &volumeList, const deoglTriangleSorter &triangles,
const decVector &origin, float distance ) const{
	const int triangleCount = triangles.GetTriangleCount();
	decVector pf1, pf2, pf3, normal;
	decConvexVolume splitter;
	float dot, scale;
	int i;
	
	for( i=0; i<triangleCount; i++ ){
		const decVector &p1 = triangles.GetTriangleVertex1( i );
		const decVector &p2 = triangles.GetTriangleVertex2( i );
		const decVector &p3 = triangles.GetTriangleVertex3( i );
		
		normal = ( ( p2 - p1 ) % ( p3 - p2 ) ).Normalized();
		dot = ( origin - p1 ) * normal;
		
		if( dot < 1e-5f ){
			dot = 1e-5f;
		}
		scale = distance / dot;
		
		pf1 = origin + ( p1 - origin ) * scale;
		pf2 = origin + ( p2 - origin ) * scale;
		pf3 = origin + ( p3 - origin ) * scale;
		
		splitter.SetEmpty();
		
		splitter.AddVertex( p1 );
		splitter.AddVertex( p2 );
		splitter.AddVertex( p3 );
		splitter.AddVertex( pf1 );
		splitter.AddVertex( pf2 );
		splitter.AddVertex( pf3 );
		
		pAddTriangle( &splitter, 0, 1, 2, normal ); // front
		pAddTriangle( &splitter, 5, 4, 3, -normal ); // back
		pAddQuad( &splitter, 0, 3, 4, 1, ( ( pf1 - p1 ) % ( p2 - p1 ) ).Normalized() ); // side
		pAddQuad( &splitter, 1, 4, 5, 2, ( ( pf2 - p2 ) % ( p3 - p2 ) ).Normalized() ); // side
		pAddQuad( &splitter, 2, 5, 3, 0, ( ( pf3 - p3 ) % ( p1 - p3 ) ).Normalized() ); // side
		
		volumeList.SplitByVolume( splitter );
	}
}

void deoglConvexVisHullBuilder::CropByOcclusionMesh( decConvexVolumeList &volumeList,
const deoglROcclusionMesh &occlusionMesh, const decMatrix &matrix,
const decVector &origin, float distance ) const{
	const int singleSidedFaceCount = occlusionMesh.GetSingleSidedFaceCount();
	const int doubleSidedFaceCount = occlusionMesh.GetDoubleSidedFaceCount();
	const deoglROcclusionMesh::sVertex * const vertices = occlusionMesh.GetVertices();
	const int faceCount = singleSidedFaceCount + doubleSidedFaceCount;
	unsigned short * const corners = occlusionMesh.GetCorners();
	deoglTriangleSorter triangleList;
	decVector tv1, tv2, tv3;
	int i, pointIndex = 0;
	
	for( i=0; i<faceCount; i++ ){
		const deoglROcclusionMesh::sVertex &v3 = vertices[ corners[ pointIndex++ ] ];
		const deoglROcclusionMesh::sVertex &v2 = vertices[ corners[ pointIndex++ ] ];
		const deoglROcclusionMesh::sVertex &v1 = vertices[ corners[ pointIndex++ ] ];
		
		tv1 = matrix * v1.position;
		tv2 = matrix * v2.position;
		tv3 = matrix * v3.position;
		
		if( ( origin - tv1 ) * ( ( tv2 - tv1 ) % ( tv3 - tv2 ) ) > 0.0f ){ // front facing
			triangleList.AddTriangle( tv1, tv2, tv3 );
			
		}else{ // back facing
			if( i < singleSidedFaceCount ){
				continue;
			}
			triangleList.AddTriangle( tv3, tv2, tv1 );
		}
	}
	
	if( triangleList.GetTriangleCount() > 0 ){
		triangleList.SortRadial( origin );
		CropByTriangles( volumeList, triangleList, origin, distance );
	}
}

void deoglConvexVisHullBuilder::CropByBoundingBox( decConvexVolumeList &volumeList, const decDMatrix &matrix,
const decDVector &minExtend, const decDVector &maxExtend ) const{
	const decVector normalXAxis = matrix.TransformRight().ToVector();
	const decVector normalZAxis = matrix.TransformView().ToVector();
	const decVector normalYAxis = matrix.TransformUp().ToVector();
	const decVector localMinExtend = ( matrix * minExtend ).ToVector();
	const decVector localMaxExtend = ( matrix * maxExtend ).ToVector();
	
	volumeList.SplitByPlane( normalXAxis, localMinExtend, true, NULL ); // crop left face
	volumeList.SplitByPlane( -normalXAxis, localMaxExtend, true, NULL ); // crop right face
	volumeList.SplitByPlane( normalYAxis, localMinExtend, true, NULL ); // crop bottom face
	volumeList.SplitByPlane( -normalYAxis, localMaxExtend, true, NULL ); // crop top face
	volumeList.SplitByPlane( normalZAxis, localMinExtend, true, NULL ); // crop back face
	volumeList.SplitByPlane( -normalZAxis, localMaxExtend, true, NULL ); // crop front face
}



// Private Functions
//////////////////////

void deoglConvexVisHullBuilder::pAddTriangle( decConvexVolume *volume, int p1, int p2, int p3 ) const{
	const decVector &vertex1 = volume->GetVertexAt( p1 );
	const decVector &vertex2 = volume->GetVertexAt( p2 );
	const decVector &vertex3 = volume->GetVertexAt( p3 );
	decConvexVolumeFace *face = NULL;
	decVector normal;
	float length;
	
	normal = ( vertex2 - vertex1 ) % ( vertex3 - vertex2 );
	length = normal.Length();
	if( length < 1e-6f ){
		return; // we add no ill shaped triangles
	}
	
	normal /= length;
	
	try{
		face = new decConvexVolumeFace;
		face->SetNormal( normal );
		face->AddVertex( p1 );
		face->AddVertex( p2 );
		face->AddVertex( p3 );
		
		volume->AddFace( face );
		
	}catch( const deException & ){
		if( face ){
			delete face;
		}
		throw;
	}
}

void deoglConvexVisHullBuilder::pAddTriangle( decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal ) const{
	decConvexVolumeFace *face = NULL;
	
	try{
		face = new decConvexVolumeFace;
		face->SetNormal( normal );
		face->AddVertex( p1 );
		face->AddVertex( p2 );
		face->AddVertex( p3 );
		
		volume->AddFace( face );
		
	}catch( const deException & ){
		if( face ){
			delete face;
		}
		throw;
	}
}

void deoglConvexVisHullBuilder::pAddQuad( decConvexVolume *volume, int p1, int p2, int p3, int p4 ) const{
	const decVector &vertex1 = volume->GetVertexAt( p1 );
	const decVector &vertex2 = volume->GetVertexAt( p2 );
	const decVector &vertex3 = volume->GetVertexAt( p3 );
	decConvexVolumeFace *face = NULL;
	decVector normal;
	float length;
	
	normal = ( vertex2 - vertex1 ) % ( vertex3 - vertex2 );
	length = normal.Length();
	if( length < 1e-6f ){
		return; // we add no ill shaped quads
	}
	
	normal /= length;
	
	try{
		face = new decConvexVolumeFace;
		face->SetNormal( normal );
		face->AddVertex( p1 );
		face->AddVertex( p2 );
		face->AddVertex( p3 );
		face->AddVertex( p4 );
		
		volume->AddFace( face );
		
	}catch( const deException & ){
		if( face ){
			delete face;
		}
		throw;
	}
}

void deoglConvexVisHullBuilder::pAddQuad( decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal ) const{
	decConvexVolumeFace *face = NULL;
	
	try{
		face = new decConvexVolumeFace;
		face->SetNormal( normal );
		face->AddVertex( p1 );
		face->AddVertex( p2 );
		face->AddVertex( p3 );
		face->AddVertex( p4 );
		
		volume->AddFace( face );
		
	}catch( const deException & ){
		if( face ){
			delete face;
		}
		throw;
	}
}
