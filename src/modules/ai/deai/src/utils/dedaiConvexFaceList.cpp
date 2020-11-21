/* 
 * Drag[en]gine AI Module
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

#include "dedaiConvexFace.h"
#include "dedaiConvexFaceList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>



// Definitions
////////////////

#define TEST_NONE			0
#define TEST_SPLIT			1
#define TEST_REMOVE			2

#define SPLIT_FRONT_BACK	0
#define SPLIT_FRONT			1
#define SPLIT_BACK			2
#define SPLIT_NONE			3

#define THRESHOLD_EQUAL		0.0001f

#define THRESHOLD_ON_PLANE	0.0001f

enum ePlaneResult{
	eprFront,
	eprBack,
	eprTouch
};

#define PLANE_SIDE(dist)	( \
	( ( dist ) > THRESHOLD_ON_PLANE ) ? eprFront : \
		( ( ( dist ) < -THRESHOLD_ON_PLANE ) ? eprBack : eprTouch ) )

#define DBG_PLANE_RESULT(result)	( \
	( ( result ) == eprFront ) ? "front" : \
		( ( ( result ) == eprBack ) ? "back" : "touch" ) )



// Class dedaiConvexFaceList
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiConvexFaceList::dedaiConvexFaceList(){
	pVertices = NULL;
	pVertexCount = 0;
	pVertexSize = 0;
}

dedaiConvexFaceList::dedaiConvexFaceList( const dedaiConvexFaceList &list ){
	const int vertexCount = list.GetVertexCount();
	const int faceCount = list.GetFaceCount();
	dedaiConvexFace *face = NULL;
	int i;
	
	pVertices = NULL;
	pVertexCount = 0;
	pVertexSize = 0;
	
	try{
		for( i=0; i<vertexCount; i++ ){
			AddVertex( list.GetVertexAt( i ) );
		}
		for( i=0; i<faceCount; i++ ){
			face = CreateFace( list.GetFaceAt( i )->GetMarker() );
			*face = *list.GetFaceAt( i ); // CreateFace does not create a copy just a new face
			AddFace( face );
			face = NULL;
		}
		
	}catch( const deException & ){
		RemoveAllFaces();
		if( pVertices ){
			delete [] pVertices;
		}
		throw;
	}
}

dedaiConvexFaceList::~dedaiConvexFaceList(){
	RemoveAllFaces();
	if( pVertices ){
		delete [] pVertices;
	}
}



// Vertices
/////////////

int dedaiConvexFaceList::GetVertexCount() const{
	return pVertexCount;
}

const decVector &dedaiConvexFaceList::GetVertexAt( int index ) const{
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	return pVertices[ index ];
}

bool dedaiConvexFaceList::HasVertex( const decVector &vertex ) const{
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		if( vertex.IsEqualTo( pVertices[ i ], THRESHOLD_EQUAL ) ){
			return true;
		}
	}
	
	return false;
}

int dedaiConvexFaceList::IndexOfVertex( const decVector &vertex ) const{
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		if( vertex.IsEqualTo( pVertices[ i ], THRESHOLD_EQUAL ) ){
			return i;
		}
	}
	
	return -1;
}

void dedaiConvexFaceList::AddVertex( const decVector &vertex ){
// 	if( HasVertex( vertex ) ){
// 		DETHROW( deeInvalidParam );
// 	}
	
	if( pVertexCount == pVertexSize ){
		const int newSize = pVertexSize * 3 / 2 + 1;
		decVector * const newArray = new decVector[ newSize ];
		
		if( pVertices ){
			memcpy( newArray, pVertices, sizeof( decVector ) * pVertexCount );
			delete [] pVertices;
		}
		
		pVertices = newArray;
		pVertexSize = newSize;
	}
	
	pVertices[ pVertexCount ] = vertex;
	pVertexCount++;
}

void dedaiConvexFaceList::RemoveVertex( int index ){
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	
	for( i=index+1; i<pVertexCount; i++ ){
		pVertices[ i - 1 ] = pVertices[ i ];
	}
	pVertexCount--;
}

void dedaiConvexFaceList::RemoveAllVertices(){
	pVertexCount = 0;
}




// Faces
//////////

int dedaiConvexFaceList::GetFaceCount() const{
	return pFaces.GetCount();
}

dedaiConvexFace *dedaiConvexFaceList::GetFaceAt( int index ) const{
	return ( dedaiConvexFace* )pFaces.GetAt( index );
}

bool dedaiConvexFaceList::HasFace( dedaiConvexFace *face ) const{
	return pFaces.Has( face );
}

int dedaiConvexFaceList::IndexOfFace( dedaiConvexFace *face ) const{
	return pFaces.IndexOf( face );
}

void dedaiConvexFaceList::AddFace( dedaiConvexFace *face ){
	// no pFaces.Has(face) check. with larger number of faces this becomes very slow
	if( ! face ){
		DETHROW( deeInvalidParam );
	}
	pFaces.Add( face );
}

void dedaiConvexFaceList::RemoveFaceAt( int index ){
	dedaiConvexFace * const face = ( dedaiConvexFace* )pFaces.GetAt( index );
	pFaces.RemoveFrom( index );
	delete face;
}

void dedaiConvexFaceList::RemoveFace( dedaiConvexFace *face ){
	ExtractFace( face );
	delete face;
}

void dedaiConvexFaceList::ExtractFaceAt( int index ){
	pFaces.RemoveFrom( index );
}

void dedaiConvexFaceList::ExtractFace( dedaiConvexFace *face ){
	const int index = pFaces.IndexOf( face );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	pFaces.RemoveFrom( index );
}

void dedaiConvexFaceList::RemoveAllFaces(){
	const int count = pFaces.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( dedaiConvexFace* )pFaces.GetAt( i );
	}
	pFaces.RemoveAll();
}



// Management
///////////////

void dedaiConvexFaceList::Move( const decVector &direction ){
	int i;
	for( i=0; i<pVertexCount; i++ ){
		pVertices[ i ] += direction;
	}
}

void dedaiConvexFaceList::SplitByFace( const dedaiConvexFaceList &splitterFaceList, int splitterFaceIndex ){
	const dedaiConvexFace &splitterFace = *splitterFaceList.GetFaceAt( splitterFaceIndex );
	const int splitVertexCount = splitterFace.GetVertexCount();
	
	// if the splitter face does not have at least 3 vertices do not split at all
	if( splitVertexCount < 3 ){
		return;
	}
	
	int faceCount = pFaces.GetCount();
	int i, j;
	
	// process each face in the list one by one. this is required since faces are allowed
	// to be located in different planes. the face count is kept constant to avoid testing
	// newly created faces again
	for( i=0; i<faceCount; i++ ){
//		if( splitterFaceIndex > 2 ) continue;
		const dedaiConvexFace &testFace = *( ( dedaiConvexFace* )pFaces.GetAt( i ) );
		
		// if the face does not have at least 3 vertices do not split but remove it
		if( testFace.GetVertexCount() < 3 ){
			RemoveFaceAt( i );
			i--;
			faceCount--;
			continue;
		}
		
		const decVector &planeNormal = testFace.GetNormal();
		const decVector &planePosition = pVertices[ testFace.GetVertexAt( 0 ) ];
		const float planeDist = planeNormal * planePosition;
		
		// determine the split edge. for this find the two edges piercing the face plane
		// if existing. if the face is not convex due to a mistake by the user use the
		// first two found vertices as split edge. a better solution can be done later on
		bool hasEdgeFrom = false;
		bool hasEdgeTo = false;
		decVector edgeFrom;
		decVector edgeTo;
		
		for( j=0; j<splitVertexCount; j++ ){
			const decVector &edgeVertex1 = splitterFaceList.GetVertexAt( splitterFace.GetVertexAt( j ) );
			const decVector &edgeVertex2 = splitterFaceList.GetVertexAt(
				splitterFace.GetVertexAt( ( j + 1 ) % splitVertexCount ) );
			const decVector edgeDirection( edgeVertex2 - edgeVertex1 );
			
			const float numer = planeNormal * edgeVertex1 - planeDist;
			const float denom = planeNormal * edgeDirection;
			
			// if the edge is parallel to plane ignore it. the edges right before and after this
			// edge are adding the edge start and end point already
			if( fabsf( denom ) < THRESHOLD_ON_PLANE ){
				continue;
			}
			
			// if the hit point is on the edge the edge contributes a cut edge point
			const float dist = -numer / denom;
			if( dist >= 0.0f && dist <= 1.0f ){
				const decVector hitPoint( edgeVertex1 + edgeDirection * dist );
				
				if( ! hasEdgeFrom ){
					edgeFrom = hitPoint;
					hasEdgeFrom = true;
					
				}else if( ! hasEdgeTo ){
					edgeTo = hitPoint;
					hasEdgeTo = true;
					
				}else{
					printf( "more than 2 cut points found. ignoring the new cut point." );
				}
			}
		}
		
		// if there is a split edge split the face
		if( hasEdgeFrom && hasEdgeTo ){
			pSplitFaceByEdge( i, edgeFrom, edgeTo );
			
			// if the face becomes degenerated or empty remove it. we can not use testFace
			// reference since pSplitFaceByEdge potentially added faces which potentially
			// caused memory rellocation. the face index though stays correct
			if( ( ( dedaiConvexFace* )pFaces.GetAt( i ) )->GetVertexCount() < 3 ){
				RemoveFaceAt( i );
				i--;
				faceCount--;
			}
		}
	}
}

void dedaiConvexFaceList::SplitByFaceList( const dedaiConvexFaceList &splitterFaceList ){
	const int count = splitterFaceList.GetFaceCount();
	int i;
	
	for( i=0; i<count; i++ ){
		SplitByFace( splitterFaceList, i );
	}
}

void dedaiConvexFaceList::SplitByVolume( const decConvexVolume &volume ){
	// convert the volume first into a convex face list
	const int volumeVertexCount = volume.GetVertexCount();
	const int volumeFaceCount = volume.GetFaceCount();
	dedaiConvexFaceList list;
	int i, j;
	
	for( i=0; i<volumeVertexCount; i++ ){
		list.AddVertex( volume.GetVertexAt( i ) );
	}
	
	dedaiConvexFace *newFace = NULL;
	try{
		for( i=0; i<volumeFaceCount; i++ ){
			const decConvexVolumeFace &volumeFace = *volume.GetFaceAt( i );
			const int volumeFaceVertexCount = volumeFace.GetVertexCount();
			
			newFace = new dedaiConvexFace;
			newFace->SetNormal( volumeFace.GetNormal() );
			for( j=0; j<volumeFaceVertexCount; j++ ){
				newFace->AddVertex( volumeFace.GetVertexAt( j ) );
			}
			list.AddFace( newFace );
			newFace = NULL;
		}
		
	}catch( const deException & ){
		if( newFace ){
			delete newFace;
		}
		throw;
	}
	
	// split by the generated convex face list
	SplitByFaceList( list );
	
	// remove all faces with centers located in the split volume
	for( i=pFaces.GetCount()-1; i>=0; i-- ){
		const dedaiConvexFace &face = *( ( dedaiConvexFace* )pFaces.GetAt( i ) );
		const decVector faceCenter( face.CalculateCenter( *this ) );
		
		for( j=0; j<volumeFaceCount; j++ ){
			const decConvexVolumeFace &volumeFace = *volume.GetFaceAt( j );
			const decVector &volumeFaceNormal = volumeFace.GetNormal();
			const float volumeFaceDist = volumeFaceNormal * volume.GetVertexAt( volumeFace.GetVertexAt( 0 ) );
			const float faceDist = volumeFaceNormal * faceCenter;
			
			if( faceDist > volumeFaceDist ){
				break;
			}
		}
		
		if( j == volumeFaceCount ){
			RemoveFaceAt( i );
		}
	}
}



// Subclassing
////////////////

dedaiConvexFace *dedaiConvexFaceList::CreateFace( int marker ){
	dedaiConvexFace * const newFace = new dedaiConvexFace;
	newFace->SetMarker( marker );
	return newFace;
}



// Private Functions
//////////////////////

// #define DEBUG_ON 1
#ifdef DEBUG_ON
#define DBG(c) c
#else
#define DBG(c)
#endif

void dedaiConvexFaceList::pSplitFaceByEdge( int faceIndex, const decVector &cutEdgeFrom, const decVector &cutEdgeTo ){
	DBG( printf( "\nsplit face %i by edge (%f,%f,%f)-(%f,%f,%f)\n", faceIndex, cutEdgeFrom.x,
		cutEdgeFrom.y, cutEdgeFrom.z, cutEdgeTo.x, cutEdgeTo.y, cutEdgeTo.z ) );
	
	// cut edges and determine at the same time if cut edge points are inside
	dedaiConvexFace &face = *( ( dedaiConvexFace* )pFaces.GetAt( faceIndex ) );
	const decVector cutEdgeDir( cutEdgeTo - cutEdgeFrom );
	const decVector &faceNormal = face.GetNormal();
	bool insideEdgeFrom = true;
	bool insideEdgeTo = true;
	bool hasIntersection = false;
	int i;
	DBG( printf( "faceNormal=(%f,%f,%f) cutEdgeDir=(%f,%f,%f)\n",
		faceNormal.x, faceNormal.y, faceNormal.z, cutEdgeDir.x, cutEdgeDir.y, cutEdgeDir.z ) );
	
	for( i=0; i<face.GetVertexCount(); i++ ){
		const decVector &v1 = pVertices[ face.GetVertexAt( i ) ];
		const decVector &v2 = pVertices[ face.GetVertexAt( ( i + 1 ) % face.GetVertexCount() ) ];
		const decVector edge( v2 - v1 );
		const float edgeLen = edge.Length();
		const decVector edgeNorm( edge / edgeLen );
		const decVector planeNormal( faceNormal % edgeNorm );
		const float planeDist = planeNormal * v1;
		
		// test if the edge points are not inside
		const float fromDist = planeNormal * cutEdgeFrom - planeDist;
		const float toDist = planeNormal * cutEdgeTo - planeDist;
		const ePlaneResult fromSide = PLANE_SIDE( fromDist );
		const ePlaneResult toSide = PLANE_SIDE( toDist );
		insideEdgeFrom &= fromSide != eprBack; // touching counts as inside
		insideEdgeTo &= toSide != eprBack; // touching counts as inside
		
		// cut edge if intersected by the split edge. parallel does not cut since other edges do this
		const float numer = fromDist;
		const float denom = planeNormal * cutEdgeDir;
		
		if( fabsf( denom ) < THRESHOLD_ON_PLANE ){
			// if the cut edge is located on the face edge do not even try to cut
			if( fabsf( numer ) < THRESHOLD_ON_PLANE ){
				return;
			}
			
			// otherwise just ignore this edge
			continue;
		}
		
		const float dist = -numer / denom;
		if( dist < 0.0f || dist > 1.0f ){
			continue;
		}
		
		const decVector hitPoint( cutEdgeFrom + cutEdgeDir * dist );
		const float hitDist = edgeNorm * ( hitPoint - v1 );
		if( hitDist < 0.0f || hitDist > edgeLen ){
			continue;
		}
		
		DBG( printf( "split edge %i in face %i cut (%f,%f,%f)\n",
			i, faceIndex, hitPoint.x, hitPoint.y, hitPoint.z ) );
		int cutIndex = IndexOfVertex( hitPoint );
		
		if( cutIndex == -1 ){
			cutIndex = pVertexCount;
			AddVertex( hitPoint );
		}
		
		if( ! face.HasVertex( cutIndex ) ){
			face.InsertVertex( i + 1, cutIndex );
			i++;
			
			// it is possible the intersection point is the same as a face corner.
			// in this case the face is also split but the cut point is not added
		}
		
		// if the cut edge is touching the split point with one end it is possible the other end
		// is outside the face and then no split has to be done
		if( hitPoint.IsEqualTo( cutEdgeFrom, THRESHOLD_EQUAL ) ){
			if( toSide != eprFront ){
				continue;
			}
			
		}else if( hitPoint.IsEqualTo( cutEdgeTo, THRESHOLD_EQUAL ) ){
			if( fromSide != eprFront ){
				continue;
			}
		}
		
		hasIntersection = true;
	}
	
	DBG( printf( "inside test face %d from=%d to=%d hasSplitEdge=%d\n",
		faceIndex, insideEdgeFrom, insideEdgeTo, hasSplitEdge ) );
	
	// add the inside edge vertices and keep track of their indices
	int indexCutEdgeFrom = -1;
	int indexCutEdgeTo = -1;
	
	if( insideEdgeFrom ){
		indexCutEdgeFrom = IndexOfVertex( cutEdgeFrom );
		
		if( indexCutEdgeFrom == -1 ){
			indexCutEdgeFrom = pVertexCount;
			AddVertex( cutEdgeFrom );
			
		}else if( face.HasVertex( indexCutEdgeFrom ) ){
			indexCutEdgeFrom = -1;
		}
	}
	
	if( insideEdgeTo ){
		indexCutEdgeTo = IndexOfVertex( cutEdgeTo );
		
		if( indexCutEdgeTo == -1 ){
			indexCutEdgeTo = pVertexCount;
			AddVertex( cutEdgeTo );
			
		}else if( face.HasVertex( indexCutEdgeTo ) ){
			indexCutEdgeTo = -1;
		}
	}
	
	// split up the face if at least one edge vertex is inside. it doesn't matter which
	// edge vertex is used for the splitting as long as it is inside. the chosen vertex
	// acts as pivot point and the edge as pivot axis. 
	if( ! ( hasIntersection || ( insideEdgeFrom && insideEdgeTo ) ) ){
		return;
	}
	
	const decVector refAxis( faceNormal % cutEdgeDir.Normalized() );
	const float refDist = refAxis * cutEdgeFrom;
	
	// splitting the face by an edge can result in up to four faces which each can be
	// empty. the front face contains all points with a side of >= 0. the back face
	// contains all points with a side of <= 0. the two filler faces contain each the
	// two consequtive vertices crossing an edge and the respective split edge end
	// point. crossing the edge means one vertex is < 0 and the other > 0. if one of
	// the vertices is 0 or the respective cut edge end is not inside the respective
	// filler face does not exist.
	// 
	// front side is inside of cut volume. face to be cut is back face
	dedaiConvexFace *newFaceFront = NULL;
	dedaiConvexFace *newFaceFiller = NULL;
	
	int lastVertex = face.GetVertexAt( face.GetVertexCount() - 1 );
	ePlaneResult lastSide = PLANE_SIDE( refAxis * pVertices[ lastVertex ] - refDist );
	DBG( for(i=0; i<face.GetVertexCount(); i++ ){
		printf( "state: face %i vertex %i(%i) (%f,%f,%f)\n", faceIndex, i, face.GetVertexAt(i),
			pVertices[face.GetVertexAt(i)].x, pVertices[face.GetVertexAt(i)].y,
			pVertices[face.GetVertexAt(i)].z );
		} )
	
	// AddFace() causes potential memory rellocation. it is thus not possible to use
	// the face reference safely
	const int newFaceMarker = face.GetMarker();
	
	try{
		for( i=0; i<( ( dedaiConvexFace* )pFaces.GetAt( faceIndex ) )->GetVertexCount(); i++ ){
			const int curVertex = ( ( dedaiConvexFace* )pFaces.GetAt( faceIndex ) )->GetVertexAt( i );
			const ePlaneResult curSide = PLANE_SIDE( refAxis * pVertices[ curVertex ] - refDist );
			DBG( printf( "corner=%i last=(%i,%s) cur=(%i,%s) indexCutEdgeFrom=%i indexCutEdgeTo=%i\n",
				i, lastVertex, DBG_PLANE_RESULT(lastSide), curVertex,
				DBG_PLANE_RESULT(curSide), indexCutEdgeFrom, indexCutEdgeTo ) );
			
			// if the last vertex is back and the cur vertex is front the cut edge has not
			// crossed nor touched the edge. in this configuration the end point of the cut
			// edge is always closer to the face edge than the begin end
			if( lastSide == eprBack && curSide == eprFront && indexCutEdgeTo != -1 ){
				DBG( printf( "add filler face at cutEdgeTo with vertices %i, %i and %i\n",
					indexCutEdgeTo, lastVertex, curVertex ) );
				
				// add filler face if the cut edge end point is inside the face
				newFaceFiller = CreateFace( newFaceMarker );
				newFaceFiller->SetNormal( faceNormal );
				newFaceFiller->AddVertex( indexCutEdgeTo );
				newFaceFiller->AddVertex( lastVertex );
				newFaceFiller->AddVertex( curVertex );
				AddFace( newFaceFiller );
				newFaceFiller = NULL;
				
				// add cutEdgeTo to the front face
				if( ! newFaceFront ){
					DBG( printf( "crossTo: create front face\n" ) );
					newFaceFront = CreateFace( newFaceMarker );
					newFaceFront->SetNormal( faceNormal );
				}
				DBG( printf( "crossTo: add indexCutEdgeTo %i to front face\n", indexCutEdgeTo ) );
				newFaceFront->AddVertex( indexCutEdgeTo );
				
				// add cutEdgeTo to the back face
				DBG( printf( "crossTo: insert indexCutEdgeTo %i into back face at %i\n", indexCutEdgeTo, i ) );
				( ( dedaiConvexFace* )pFaces.GetAt( faceIndex ) )->InsertVertex( i, indexCutEdgeTo );
				i++;
			}
			
			// if the last vertex is front and the cur vertex is back the cut edge has not
			// crossed nor touched the edge. in this configuration the start point of the cut
			// edge is always closer to the face edge than the end end
			if( lastSide == eprFront && curSide == eprBack && indexCutEdgeFrom != -1 ){
				DBG( printf( "add filler face at cutEdgeFrom with vertices %i, %i and %i\n",
					indexCutEdgeFrom, lastVertex, curVertex ) );
				
				// add filler face if the last vertex, cur vertex and cutEdgeFrom are not co-linear.
				// this is the case if insideEdgeFrom is true
				newFaceFiller = CreateFace( newFaceMarker );
				newFaceFiller->SetNormal( faceNormal );
				newFaceFiller->AddVertex( indexCutEdgeFrom );
				newFaceFiller->AddVertex( lastVertex );
				newFaceFiller->AddVertex( curVertex );
				AddFace( newFaceFiller );
				newFaceFiller = NULL;
				
				// add cutEdgeFrom to the front face
				if( ! newFaceFront ){
					DBG( printf( "crossFrom: create back face\n" ) );
					newFaceFront = CreateFace( newFaceMarker );
					newFaceFront->SetNormal( faceNormal );
				}
				DBG( printf( "crossFrom: add indexCutEdgeFrom %i to front face\n", indexCutEdgeFrom ) );
				newFaceFront->AddVertex( indexCutEdgeFrom );
				
				// add cutEdgeFrom to the back face
				DBG( printf( "crossFrom: insert indexCutEdgeFrom %i into back face at %i\n", indexCutEdgeFrom, i ) );
				( ( dedaiConvexFace* )pFaces.GetAt( faceIndex ) )->InsertVertex( i, indexCutEdgeFrom );
				i++;
			}
			
			// if the current side is not back add the vertex to front face. this has to
			// come after the crossing checks above to keep the ordering correct
			if( curSide != eprBack ){
				if( ! newFaceFront ){
					DBG( printf( "curSide != back: create front face\n" ) );
					newFaceFront = CreateFace( newFaceMarker );
					newFaceFront->SetNormal( faceNormal );
				}
				DBG( printf( "curSide != back: add vertex %i to front face\n", curVertex ) );
				newFaceFront->AddVertex( curVertex );
			}
			
			// if the side is front remove the vertex from the back face since it belongs
			// now only to the front face
			if( curSide == eprFront ){
				DBG( printf( "curSide == front: remove vertex %i from back face at %i\n", curVertex, i ) );
				( ( dedaiConvexFace* )pFaces.GetAt( faceIndex ) )->RemoveVertexFrom( i );
				i--;
			}
			
			// keep track of the last vertex
			lastVertex = curVertex;
			lastSide = curSide;
		}
		
		// add front face if enough corners are present
		if( newFaceFront ){
			if( newFaceFront->GetVertexCount() > 2 ){
				AddFace( newFaceFront );
				
			}else{
				delete newFaceFront;
			}
			newFaceFront = NULL;
		}
		
	}catch( const deException & ){
		if( newFaceFiller ){
			delete newFaceFiller;
		}
		if( newFaceFront ){
			delete newFaceFront;
		}
		throw;
	}
}
