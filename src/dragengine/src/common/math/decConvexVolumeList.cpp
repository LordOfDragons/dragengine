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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "decConvexVolume.h"
#include "decConvexVolumeFace.h"
#include "decConvexVolumeList.h"
#include "../exceptions.h"



// Definitions
////////////////

#define TEST_NONE			0
#define TEST_SPLIT			1
#define TEST_REMOVE			2

#define SPLIT_FRONT_BACK	0
#define SPLIT_FRONT			1
#define SPLIT_BACK			2
#define SPLIT_NONE			3

#define EQUALITY_THRESHOLD	1e-3f



// Class decConvexVolumeList
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

decConvexVolumeList::decConvexVolumeList(){
	pVolumes = NULL;
	pVolumeCount = 0;
	pVolumeSize = 0;
}

decConvexVolumeList::~decConvexVolumeList(){
	RemoveAllVolumes();
	if( pVolumes ) delete [] pVolumes;
}



// Volume Management
//////////////////////

decConvexVolume *decConvexVolumeList::GetVolumeAt( int index ) const{
	if( index < 0 || index >= pVolumeCount ) DETHROW( deeInvalidParam );
	return pVolumes[ index ];
}

bool decConvexVolumeList::HasVolume( decConvexVolume *volume ) const{
	int i;
	
	if( volume ){
		for( i=0; i<pVolumeCount; i++ ){
			if( pVolumes[ i ] == volume ) return true;
		}
	}
	
	return false;
}

int decConvexVolumeList::IndexOfVolume( decConvexVolume *volume ) const{
	int i;
	
	if( volume ){
		for( i=0; i<pVolumeCount; i++ ){
			if( pVolumes[ i ] == volume ) return i;
		}
	}
	
	return -1;
}

void decConvexVolumeList::AddVolume( decConvexVolume *volume ){
	if( ! volume || HasVolume( volume ) ) DETHROW( deeInvalidParam );
	
	if( pVolumeCount == pVolumeSize ){
		int i, newSize = pVolumeSize * 3 / 2 + 1;
		decConvexVolume **newArray = new decConvexVolume*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		
		if( pVolumes ){
			for( i=0; i<pVolumeSize; i++ ) newArray[ i ] = pVolumes[ i ];
			delete [] pVolumes;
		}
		
		pVolumes = newArray;
		pVolumeSize = newSize;
	}
	
	pVolumes[ pVolumeCount ] = volume;
	pVolumeCount++;
}

void decConvexVolumeList::RemoveVolume( decConvexVolume *volume ){
	ExtractVolume( volume );
	delete volume;
}

void decConvexVolumeList::RemoveVolumeAt( int index ){
	decConvexVolume *volume = GetVolumeAt( index );
	ExtractVolumeAt( index );
	delete volume;
}

void decConvexVolumeList::ExtractVolume( decConvexVolume *volume ){
	int i, index = IndexOfVolume( volume );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	for( i=index+1; i<pVolumeCount; i++ ) pVolumes[ i - 1 ] = pVolumes[ i ];
	pVolumeCount--;
}

void decConvexVolumeList::ExtractVolumeAt( int index ){
	if( index < 0 || index >= pVolumeCount ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=index+1; i<pVolumeCount; i++ ) pVolumes[ i - 1 ] = pVolumes[ i ];
	pVolumeCount--;
}

void decConvexVolumeList::RemoveAllVolumes(){
	while( pVolumeCount > 0 ){
		pVolumeCount--;
		delete pVolumes[ pVolumeCount ];
	}
}



// Management
///////////////

void decConvexVolumeList::SetToCube( const decVector &halfSize ){
	if( halfSize < decVector( 0.0f, 0.0f, 0.0f ) ) DETHROW( deeInvalidParam );
	decConvexVolume *volume = NULL;
	
	// remove all volumes
	RemoveAllVolumes();
	
	// add new volume set to cube
	try{
		volume = CreateVolume( NULL, true );
		if( ! volume ) DETHROW( deeOutOfMemory );
		
		volume->SetToCube( halfSize );
		
		AddVolume( volume );
		
	}catch( const deException & ){
		if( volume ) delete volume;
		throw;
	}
}

void decConvexVolumeList::Move( const decVector &direction ){
	int v;
	
	for( v=0; v<pVolumeCount; v++ ){
		pVolumes[ v ]->Move( direction );
	}
}

void decConvexVolumeList::SplitByPlane( const decVector &splitNormal, const decVector &splitPosition, bool deleteBackVolume, decConvexVolumeFace *cutFaceInit ){
	int l, volumeCount, result;
	
	//printf( "split by plane: normal=(%g,%g,%g) position=(%g,%g,%g)\n", splitNormal.x, splitNormal.y, splitNormal.z, splitPosition.x, splitPosition.y, splitPosition.z );
	// determine the split plane parameters
	float splitDot = splitNormal * splitPosition;
	
	// loop over all volumes
	volumeCount = pVolumeCount;
	for( l=0; l<pVolumeCount; l++ ){
		result = pTestByPlane( l, splitNormal, splitDot );
		
		if( deleteBackVolume ){
			if( result == TEST_SPLIT ){
				if( pSplitByPlane( l, splitNormal, splitDot, deleteBackVolume, cutFaceInit ) == SPLIT_NONE ){
					volumeCount--;
					l--;
				}
				
			}else if( result == TEST_REMOVE ){
				RemoveVolumeAt( l );
				volumeCount--;
				l--;
			}
			
		}else{
			if( result == TEST_SPLIT ){
				if( pSplitByPlane( l, splitNormal, splitDot, deleteBackVolume, cutFaceInit ) == SPLIT_NONE ){
					volumeCount--;
					l--;
				}
			}
		}
	}
}

void decConvexVolumeList::SplitByFace( const decConvexVolume &volume, int face ){
	decConvexVolumeFace *splitFace = volume.GetFaceAt( face );
	int l, volumeCount;
	
	// determine the split plane parameters
	const decVector &splitNormal = splitFace->GetNormal();
	const decVector &splitVertex = volume.GetVertexAt( splitFace->GetVertexAt( 0 ) );
	float splitDot = splitNormal * splitVertex;
	
	// loop over all volumes
	volumeCount = pVolumeCount;
	for( l=0; l<pVolumeCount; l++ ){
		if( pTestByFace( l, volume, *splitFace ) ){
			if( pSplitByPlane( l, splitNormal, splitDot, false, splitFace ) == SPLIT_NONE ){
				volumeCount--;
				l--;
			}
		}
	}
}

void decConvexVolumeList::SplitByVolume( const decConvexVolume &volume ){
	int f, faceCount = volume.GetFaceCount();
	decConvexVolumeFace *splitFace;
	int l, volumeCount, result;
	int testVolume;
	float splitDot;
	
	// loop over all volumes
	volumeCount = pVolumeCount;
	for( l=0; l<volumeCount; l++ ){
		// test what to do with the volume
		result = pTestByVolume( l, volume );
		if( result == TEST_SPLIT ){
			// assign the current volume as the test volume
			testVolume = l;
			
			// loop over all split faces
			for( f=0; f<faceCount; f++ ){
				splitFace = volume.GetFaceAt( f );
				
				// determine the split plane parameters
				const decVector &splitNormal = splitFace->GetNormal();
				const decVector &splitVertex = volume.GetVertexAt( splitFace->GetVertexAt( 0 ) );
				splitDot = splitNormal * splitVertex;
				
				// split the volume using the split plane
				result = pSplitByPlane( testVolume, splitNormal, splitDot, false, splitFace );
				if( result == SPLIT_FRONT_BACK ){
					testVolume = pVolumeCount - 1;
					
				}else if( result == SPLIT_FRONT ){
					testVolume = -1;
					break;
					
				}else if( result == SPLIT_NONE ){
					if( testVolume == l ){
						volumeCount--;
						l--;
					}
					testVolume = -1;
					break;
				}
			}
			
			// if a test volume exists it is now completly inside the
			// split volume and has to be removed
			if( testVolume != -1 ){
				RemoveVolumeAt( testVolume );
				if( testVolume == l ){
					volumeCount--;
					l--;
				}
			}
			
		}else if( result == TEST_REMOVE ){
			RemoveVolumeAt( l );
			volumeCount--;
			l--;
		}
	}
}



// Protected Functions
////////////////////////

decConvexVolume *decConvexVolumeList::CreateVolume( decConvexVolume *volume, bool front ){
	decConvexVolume *newVolume = new decConvexVolume();
	if( ! newVolume ) DETHROW( deeOutOfMemory );
	
	return newVolume;
}

decConvexVolumeFace *decConvexVolumeList::CreateVolumeFace( decConvexVolumeFace *face ){
	decConvexVolumeFace *newFace = new decConvexVolumeFace();
	if( ! newFace ) DETHROW( deeOutOfMemory );
	
	if( face ){
		newFace->SetMarker( face->GetMarker() );
	}
	
	return newFace;
}



// Private Functions
//////////////////////

int decConvexVolumeList::pTestByVolume( int volume, const decConvexVolume &splitVolume ) const{
	int vertexCount = pVolumes[ volume ]->GetVertexCount();
	int sv, svCount = splitVolume.GetVertexCount();
	int sf, sfCount = splitVolume.GetFaceCount();
	decConvexVolumeFace *testFace, *splitFace;
	int resultSide, curSide, testSide;
	int v, f, faceCount;
	float dot, planeDot;
	
	// loop over all split faces
	resultSide = TEST_REMOVE;
	for( sf=0; sf<sfCount; sf++ ){
		splitFace = splitVolume.GetFaceAt( sf );
		
		// determine the split plane parameters
		const decVector &splitNormal = splitFace->GetNormal();
		const decVector &splitVertex = splitVolume.GetVertexAt( splitFace->GetVertexAt( 0 ) );
		float splitDot = splitNormal * splitVertex;
		
		// if the volume is completly on either side there is no split
		curSide = TEST_SPLIT;
		for( v=0; v<vertexCount; v++ ){
			dot = splitNormal * pVolumes[ volume ]->GetVertexAt( v ) - splitDot;
			if( dot > EQUALITY_THRESHOLD ){
				testSide = TEST_NONE;
			}else if( dot < -EQUALITY_THRESHOLD ){
				testSide = TEST_REMOVE;
			}else{
				testSide = TEST_SPLIT;
			}
			if( curSide == TEST_SPLIT ){
				curSide = testSide;
			}else if( testSide != TEST_SPLIT && testSide != curSide ){
				curSide = TEST_SPLIT;
				break;
			}
		}
		if( curSide == TEST_NONE ) return TEST_NONE;
		if( curSide == TEST_SPLIT ) resultSide = TEST_SPLIT;
	}
	
	// if any of the volume face normals separates there is no split
	faceCount = pVolumes[ volume ]->GetFaceCount();
	for( f=0; f<faceCount; f++ ){
		testFace = pVolumes[ volume ]->GetFaceAt( f );
		const decVector &normal = testFace->GetNormal();
		planeDot = normal * pVolumes[ volume ]->GetVertexAt( testFace->GetVertexAt( 0 ) );
		
		for( sv=0; sv<svCount; sv++ ){
			if( normal * splitVolume.GetVertexAt( sv ) - planeDot < -EQUALITY_THRESHOLD ) break;
		}
		if( sv == svCount ) return TEST_NONE;
	}
	
	// return the result
	return resultSide;
}

bool decConvexVolumeList::pTestByFace( int volume, const decConvexVolume &splitVolume, const decConvexVolumeFace &splitFace ) const{
	int vertexCount = pVolumes[ volume ]->GetVertexCount();
	decConvexVolumeFace *testFace;
	int curSide, testSide;
	int v, f, faceCount;
	float dot, planeDot;
	
	// determine the split plane parameters
	const decVector &splitNormal = splitFace.GetNormal();
	const decVector &splitVertex = splitVolume.GetVertexAt( splitFace.GetVertexAt( 0 ) );
	float splitDot = splitNormal * splitVertex;
	
	// if the volume is completly on either side there is no split
	curSide = TEST_SPLIT;
	for( v=0; v<vertexCount; v++ ){
		dot = splitNormal * pVolumes[ volume ]->GetVertexAt( v ) - splitDot;
		if( dot > EQUALITY_THRESHOLD ){
			testSide = TEST_NONE;
		}else if( dot < -EQUALITY_THRESHOLD ){
			testSide = TEST_REMOVE;
		}else{
			testSide = TEST_SPLIT;
		}
		if( curSide == TEST_SPLIT ){
			curSide = testSide;
		}else if( testSide != TEST_SPLIT && testSide != curSide ){
			curSide = TEST_SPLIT;
			break;
		}
	}
	if( curSide != TEST_SPLIT ) return false;
	
	// if any of the volume face normals separates there is no split
	faceCount = pVolumes[ volume ]->GetFaceCount();
	vertexCount = splitFace.GetVertexCount();
	
	for( f=0; f<faceCount; f++ ){
		testFace = pVolumes[ volume ]->GetFaceAt( f );
		const decVector &normal = testFace->GetNormal();
		planeDot = normal * pVolumes[ volume ]->GetVertexAt( testFace->GetVertexAt( 0 ) );
		
		for( v=0; v<vertexCount; v++ ){
			if( normal * splitVolume.GetVertexAt( v ) - planeDot < -EQUALITY_THRESHOLD ) break;
		}
		if( v == vertexCount ) return false;
	}
	
	// the volume has to be split
	return true;
}

int decConvexVolumeList::pTestByPlane( int volume, const decVector &splitNormal, float splitDot ) const{
	int vertexCount = pVolumes[ volume ]->GetVertexCount();
	int curSide, testSide, v;
	float dot;
	
	// if the volume is completly on either side there is no split
	curSide = TEST_SPLIT;
	for( v=0; v<vertexCount; v++ ){
		dot = splitNormal * pVolumes[ volume ]->GetVertexAt( v ) - splitDot;
		if( dot > EQUALITY_THRESHOLD ){
			testSide = TEST_NONE;
		}else if( dot < -EQUALITY_THRESHOLD ){
			testSide = TEST_REMOVE;
		}else{
			testSide = TEST_SPLIT;
		}
		if( curSide == TEST_SPLIT ){
			curSide = testSide;
		}else if( testSide != TEST_SPLIT && testSide != curSide ){
			curSide = TEST_SPLIT;
			break;
		}
	}
	if( curSide != TEST_SPLIT ) return curSide;
	
	// the volume has to be split
	return TEST_SPLIT;
}

int decConvexVolumeList::pSplitByPlane( int volume, const decVector &splitNormal, float splitDot, bool deleteBackVolume, decConvexVolumeFace *face ){
	int f, faceCount = pVolumes[ volume ]->GetFaceCount();
	decConvexVolume *volumeFront = NULL;
	decConvexVolume *volumeBack = NULL;
	decConvexVolumeFace *faceFront = NULL;
	decConvexVolumeFace *faceBack = NULL;
	decConvexVolumeFace *faceCutFront = NULL;
	decConvexVolumeFace *faceCutBack = NULL;
	decConvexVolumeFace *testFace;
	decVector edgeDirection, cutVertexPosition;
	bool isFrontFirst, isFrontSecond;
	float dotFirst, dotSecond;
	int vertexFirst, vertexSecond;
	int vertexIndex, v, vertexCount;
	int result = SPLIT_FRONT;
	float lambda;
	
	// things can go wrong so we protect this place
	try{
		// loop over all faces
		for( f=0; f<faceCount; f++ ){
			testFace = pVolumes[ volume ]->GetFaceAt( f );
			vertexCount = testFace->GetVertexCount();
			
			// for every face loop over all vertices
			for( v=0; v<vertexCount; v++ ){
				// get the first and second vertex of this edge
				vertexFirst = testFace->GetVertexAt( v );
				vertexSecond = testFace->GetVertexAt( ( v + 1 ) % vertexCount );
				
				const decVector &vertexFirstPosition = pVolumes[ volume ]->GetVertexAt( vertexFirst );
				const decVector &vertexSecondPosition = pVolumes[ volume ]->GetVertexAt( vertexSecond );
				
				// determine if the vertices are in front of the split plane
				dotFirst = splitNormal * vertexFirstPosition - splitDot;
				dotSecond = splitNormal * vertexSecondPosition - splitDot;
				isFrontFirst = ( dotFirst > EQUALITY_THRESHOLD );
				isFrontSecond = ( dotSecond > EQUALITY_THRESHOLD );
				
				// add vertex to the front volume if front side
				if( isFrontFirst ){
					// if the front volume does not exist we create it
					if( ! volumeFront ){
						volumeFront = CreateVolume( pVolumes[ volume ], true );
						if( ! volumeFront ) DETHROW( deeOutOfMemory );
					}
					
					// if the front face does not exist we create it
					if( ! faceFront ){
						faceFront = CreateVolumeFace( testFace );
						if( ! faceFront ) DETHROW( deeOutOfMemory );
						faceFront->SetNormal( testFace->GetNormal() );
					}
					
					// add the vertex if not existing already
					vertexIndex = volumeFront->IndexOfVertex( vertexFirstPosition );
					if( vertexIndex == -1 ){
						vertexIndex = volumeFront->GetVertexCount();
						volumeFront->AddVertex( vertexFirstPosition );
					}
					
					// add to front face
					faceFront->AddVertex( vertexIndex );
					
				// add vertex to the back volume if on back side and not marked for deletion
				}else if( ! deleteBackVolume ){
					// if the back volume does not exist we create it
					if( ! volumeBack ){
						volumeBack = CreateVolume( pVolumes[ volume ], false );
						if( ! volumeBack ) DETHROW( deeOutOfMemory );
					}
					
					// if the back face does not exist we create it
					if( ! faceBack ){
						faceBack = CreateVolumeFace( testFace );
						if( ! faceBack ) DETHROW( deeOutOfMemory );
						faceBack->SetNormal( testFace->GetNormal() );
					}
					
					// add the vertex if not existing already
					vertexIndex = volumeBack->IndexOfVertex( vertexFirstPosition );
					if( vertexIndex == -1 ){
						vertexIndex = volumeBack->GetVertexCount();
						volumeBack->AddVertex( vertexFirstPosition );
					}
					
					// add to back face
					faceBack->AddVertex( vertexIndex );
				}
				
				// if both are different add cut vertex
				if( isFrontFirst != isFrontSecond ){
					// calculate the cut vertex
					edgeDirection = vertexSecondPosition - vertexFirstPosition;
					lambda = ( splitDot - splitNormal * vertexFirstPosition )
						/ ( splitNormal * edgeDirection );
					cutVertexPosition = vertexFirstPosition + edgeDirection * lambda;
					
					// if the front volume does not exist we create it
					if( ! volumeFront ){
						volumeFront = CreateVolume( pVolumes[ volume ], true );
						if( ! volumeFront ) DETHROW( deeOutOfMemory );
					}
					
					// if the front face does not exist we create it
					if( ! faceFront ){
						faceFront = CreateVolumeFace( testFace );
						if( ! faceFront ) DETHROW( deeOutOfMemory );
						faceFront->SetNormal( testFace->GetNormal() );
					}
					
					// if the front cut face does not exist we create it
					if( ! faceCutFront ){
						faceCutFront = CreateVolumeFace( face );
						if( ! faceCutFront ) DETHROW( deeOutOfMemory );
						faceCutFront->SetNormal( -splitNormal );
					}
					
					// add the vertex to front volume if not existing already
					vertexIndex = volumeFront->IndexOfVertex( cutVertexPosition );
					if( vertexIndex == -1 ){
						vertexIndex = volumeFront->GetVertexCount();
						volumeFront->AddVertex( cutVertexPosition );
					}
					
					// add vertex to front face and front cut face
					faceFront->AddVertex( vertexIndex );
					if( ! faceCutFront->HasVertex( vertexIndex ) ){
						faceCutFront->AddVertex( vertexIndex );
					}
					
					// back volume stuff only if not marked for deletion
					if( ! deleteBackVolume ){
						// if the back volume does not exist we create it
						if( ! volumeBack ){
							volumeBack = CreateVolume( pVolumes[ volume ], false );
							if( ! volumeBack ) DETHROW( deeOutOfMemory );
						}
						
						// if the back face does not exist we create it
						if( ! faceBack ){
							faceBack = CreateVolumeFace( testFace );
							if( ! faceBack ) DETHROW( deeOutOfMemory );
							faceBack->SetNormal( testFace->GetNormal() );
						}
						
						// if the back cut face does not exist we create it
						if( ! faceCutBack ){
							faceCutBack = CreateVolumeFace( face );
							if( ! faceCutBack ) DETHROW( deeOutOfMemory );
							faceCutBack->SetNormal( splitNormal );
						}
						
						// add the vertex to back volume if not existing already
						vertexIndex = volumeBack->IndexOfVertex( cutVertexPosition );
						if( vertexIndex == -1 ){
							vertexIndex = volumeBack->GetVertexCount();
							volumeBack->AddVertex( cutVertexPosition );
						}
						
						// add vertex to back face and back cut face
						faceBack->AddVertex( vertexIndex );
						if( ! faceCutBack->HasVertex( vertexIndex ) ){
							faceCutBack->AddVertex( vertexIndex );
						}
					}
				}
			}
			
			// add faces to volumes if exising
			if( faceFront ){
				volumeFront->AddFace( faceFront );
				faceFront = NULL;
			}
			if( faceBack ){
				volumeBack->AddFace( faceBack );
				faceBack = NULL;
			}
		}
		
		// add cut faces if existing
		if( faceCutFront ){
			if( ! faceCutFront->IsTooSmall( *volumeFront ) ){
				faceCutFront->SortVertices( *volumeFront );
				volumeFront->AddFace( faceCutFront );
				
			}else{
				delete faceCutFront;
			}
			faceCutFront = NULL;
		}
		
		if( faceCutBack ){
			if( ! faceCutBack->IsTooSmall( *volumeBack ) ){
				faceCutBack->SortVertices( *volumeBack );
				volumeBack->AddFace( faceCutBack );
				
			}else{
				delete faceCutBack;
			}
			faceCutBack = NULL;
		}
		
		// add volumes if existing
		if( volumeFront ){
			delete pVolumes[ volume ];
			pVolumes[ volume ] = volumeFront;
			volumeFront = NULL;
			
			if( volumeBack ){
				AddVolume( volumeBack );
				volumeBack = NULL;
				result = SPLIT_FRONT_BACK;
				
			}else{
				result = SPLIT_FRONT;
			}
			
		}else if( volumeBack ){
			delete pVolumes[ volume ];
			pVolumes[ volume ] = volumeBack;
			volumeBack = NULL;
			result = SPLIT_BACK;
			
		}else{
			RemoveVolumeAt( volume );
			result = SPLIT_NONE;
		}
		
	}catch( const deException & ){
		if( faceCutFront ) delete faceCutFront;
		if( faceCutBack ) delete faceCutBack;
		if( faceFront ) delete faceFront;
		if( faceBack ) delete faceBack;
		if( volumeFront ) delete volumeFront;
		if( volumeBack ) delete volumeBack;
		throw;
	}
	
	return result;
}

int decConvexVolumeList::pSplitByVolume( int volume, const decConvexVolume &splitVolume ){
#ifdef NEVER_SET_ME
	int f, faceCount = pVolumes[ volume ]->GetFaceCount();
	decConvexVolume *volumeFront = NULL;
	decConvexVolume *volumeBack = NULL;
	decConvexVolumeFace *faceFront = NULL;
	decConvexVolumeFace *faceBack = NULL;
	decConvexVolumeFace *faceCutFront = NULL;
	decConvexVolumeFace *faceCutBack = NULL;
	decConvexVolumeFace *testFace;
	decVector edgeDirection, cutVertexPosition;
	bool isFrontFirst, isFrontSecond, hasCutVertex;
	float dotFirst, dotSecond;
	int vertexFirst, vertexSecond;
	int vertexIndex, v, vertexCount;
	bool volumeRemoved = false;
	float lambda;
	
	// things can go wrong so we protect this place
	try{
		// loop over all faces
		for( f=0; f<faceCount; f++ ){
			testFace = pVolumes[ volume ]->GetFaceAt( f );
			vertexCount = testFace->GetVertexCount();
			
			// for every face loop over all vertices
			for( v=0; v<vertexCount; v++ ){
				// get the first and second vertex of this edge
				vertexFirst = testFace->GetVertexAt( v );
				vertexSecond = testFace->GetVertexAt( ( v + 1 ) % vertexCount );
				
				const decVector &vertexFirstPosition = pVolumes[ volume ]->GetVertexAt( vertexFirst );
				const decVector &vertexSecondPosition = pVolumes[ volume ]->GetVertexAt( vertexSecond );
				
				// determine if the vertices are in front of the split plane
				dotFirst = splitNormal * vertexFirstPosition - splitDot;
				dotSecond = splitNormal * vertexSecondPosition - splitDot;
				isFrontFirst = ( dotFirst > EQUALITY_THRESHOLD );
				isFrontSecond = ( dotSecond > EQUALITY_THRESHOLD );
				
				// add vertex to the front volume if front side
				if( isFrontFirst ){
					// if the front volume does not exist we create it
					if( ! volumeFront ){
						volumeFront = CreateVolume( pVolumes[ volume ], true );
						if( ! volumeFront ) DETHROW( deeOutOfMemory );
					}
					
					// if the front face does not exist we create it
					if( ! faceFront ){
						faceFront = CreateVolumeFace( testFace );
						if( ! faceFront ) DETHROW( deeOutOfMemory );
						faceFront->SetNormal( testFace->GetNormal() );
					}
					
					// add the vertex if not existing already
					vertexIndex = volumeFront->IndexOfVertex( vertexFirstPosition );
					if( vertexIndex == -1 ){
						vertexIndex = volumeFront->GetVertexCount();
						volumeFront->AddVertex( vertexFirstPosition );
					}
					
					// add to front face
					faceFront->AddVertex( vertexIndex );
					
				// add vertex to the back volume if on back side and not marked for deletion
				}else if( ! deleteBackVolume ){
					// if the back volume does not exist we create it
					if( ! volumeBack ){
						volumeBack = CreateVolume( pVolumes[ volume ], false );
						if( ! volumeBack ) DETHROW( deeOutOfMemory );
					}
					
					// if the back face does not exist we create it
					if( ! faceBack ){
						faceBack = CreateVolumeFace( testFace );
						if( ! faceBack ) DETHROW( deeOutOfMemory );
						faceBack->SetNormal( testFace->GetNormal() );
					}
					
					// add the vertex if not existing already
					vertexIndex = volumeBack->IndexOfVertex( vertexFirstPosition );
					if( vertexIndex == -1 ){
						vertexIndex = volumeBack->GetVertexCount();
						volumeBack->AddVertex( vertexFirstPosition );
					}
					
					// add to back face
					faceBack->AddVertex( vertexIndex );
				}
				
				// if both are different add cut vertex
				if( isFrontFirst != isFrontSecond ){
					// calculate the cut vertex
					edgeDirection = vertexSecondPosition - vertexFirstPosition;
					lambda = ( splitDot - splitNormal * vertexFirstPosition )
						/ ( splitNormal * edgeDirection );
					cutVertexPosition = vertexFirstPosition + edgeDirection * lambda;
					
					// if the front volume does not exist we create it
					if( ! volumeFront ){
						volumeFront = CreateVolume( pVolumes[ volume ], true );
						if( ! volumeFront ) DETHROW( deeOutOfMemory );
					}
					
					// if the front face does not exist we create it
					if( ! faceFront ){
						faceFront = CreateVolumeFace( testFace );
						if( ! faceFront ) DETHROW( deeOutOfMemory );
						faceFront->SetNormal( testFace->GetNormal() );
					}
					
					// if the front cut face does not exist we create it
					if( ! faceCutFront ){
						faceCutFront = CreateVolumeFace();
						if( ! faceCutFront ) DETHROW( deeOutOfMemory );
						faceCutFront->SetNormal( -splitNormal );
					}
					
					// add the vertex to front volume if not existing already
					vertexIndex = volumeFront->IndexOfVertex( cutVertexPosition );
					if( vertexIndex == -1 ){
						vertexIndex = volumeFront->GetVertexCount();
						volumeFront->AddVertex( cutVertexPosition );
					}
					
					// add vertex to front face and front cut face
					faceFront->AddVertex( vertexIndex );
					if( ! faceCutFront->HasVertex( vertexIndex ) ){
						faceCutFront->AddVertex( vertexIndex );
					}
					
					// back volume stuff only if not marked for deletion
					if( ! deleteBackVolume ){
						// if the back volume does not exist we create it
						if( ! volumeBack ){
							volumeBack = CreateVolume( pVolumes[ volume ], false );
							if( ! volumeBack ) DETHROW( deeOutOfMemory );
						}
						
						// if the back face does not exist we create it
						if( ! faceBack ){
							faceBack = CreateVolumeFace( testFace );
							if( ! faceBack ) DETHROW( deeOutOfMemory );
							faceBack->SetNormal( testFace->GetNormal() );
						}
						
						// if the back cut face does not exist we create it
						if( ! faceCutBack ){
							faceCutBack = CreateVolumeFace();
							if( ! faceCutBack ) DETHROW( deeOutOfMemory );
							faceCutBack->SetNormal( splitNormal );
						}
						
						// add the vertex to back volume if not existing already
						vertexIndex = volumeBack->IndexOfVertex( cutVertexPosition );
						if( vertexIndex == -1 ){
							vertexIndex = volumeBack->GetVertexCount();
							volumeBack->AddVertex( cutVertexPosition );
						}
						
						// add vertex to back face and back cut face
						faceBack->AddVertex( vertexIndex );
						if( ! faceCutBack->HasVertex( vertexIndex ) ){
							faceCutBack->AddVertex( vertexIndex );
						}
					}
				}
			}
			
			// add faces to volumes if exising
			if( faceFront ){
				volumeFront->AddFace( faceFront );
				faceFront = NULL;
			}
			if( faceBack ){
				volumeBack->AddFace( faceBack );
				faceBack = NULL;
			}
		}
		
		// add cut faces if existing
		if( faceCutFront ){
			faceCutFront->SortVertices( *volumeFront );
			volumeFront->AddFace( faceCutFront );
			faceCutFront = NULL;
		}
		if( faceCutBack ){
			faceCutBack->SortVertices( *volumeBack );
			volumeBack->AddFace( faceCutBack );
			faceCutBack = NULL;
		}
		
		// add volumes if existing
		if( volumeFront ){
			delete pVolumes[ volume ];
			pVolumes[ volume ] = volumeFront;
			volumeFront = NULL;
			
			if( volumeBack ){
				AddVolume( volumeBack );
				volumeBack = NULL;
			}
			
		}else if( volumeBack ){
			delete pVolumes[ volume ];
			pVolumes[ volume ] = volumeBack;
			volumeBack = NULL;
			
		}else{
			RemoveVolumeAt( volume );
			volumeRemoved = true;
		}
		
	}catch( const deException & ){
		if( faceCutFront ) delete faceCutFront;
		if( faceCutBack ) delete faceCutBack;
		if( faceFront ) delete faceFront;
		if( faceBack ) delete faceBack;
		if( volumeFront ) delete volumeFront;
		if( volumeBack ) delete volumeBack;
		throw;
	}
	
	return volumeRemoved;
#endif
	return SPLIT_FRONT;
}
