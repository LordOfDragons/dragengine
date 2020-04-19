/* 
 * Drag[en]gine Android Launcher
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

// includes
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decBSPTree.h"
#include "decBSPTreeFace.h"
#include "../exceptions.h"



// Class decBSPTree
/////////////////////

// Constructors and Destructors
/////////////////////////////////

decBSPTree::decBSPTree( const decVector &normal, float distance ){
	pNodeFront = NULL;
	pNodeBack = NULL;
	pNormal = normal;
	pDistance = distance;
	pParent = NULL;
}

decBSPTree::~decBSPTree(){
	if( pNodeBack ) delete pNodeBack;
	if( pNodeFront ) delete pNodeFront;
}



// Management
///////////////

void decBSPTree::SetParent( decBSPTree *parent ){
	pParent = parent;
}

void decBSPTree::SetFrontNode( decBSPTree *node ){
	if( pNodeFront != node ){
		if( pNodeFront ) delete pNodeFront;
		pNodeFront = node;
	}
}

void decBSPTree::SetBackNode( decBSPTree *node ){
	if( pNodeBack != node ){
		if( pNodeBack ) delete pNodeBack;
		pNodeBack = node;
	}
}

void decBSPTree::ClearTree( bool clearNodes ){
	ClearNodeContent();
	
	if( pNodeFront ){
		pNodeFront->ClearTree( clearNodes );
		if( clearNodes ){
			delete pNodeFront;
			pNodeFront = NULL;
		}
	}
	
	if( pNodeBack ){
		pNodeBack->ClearTree( clearNodes );
		if( clearNodes ){
			delete pNodeBack;
			pNodeBack = NULL;
		}
	}
}

decBSPTree *decBSPTree::CreateBSPTree( const decVector &normal, float distance ) const{
	decBSPTree *bspTree = new decBSPTree( normal, distance );
	if( ! bspTree ) DETHROW( deeOutOfMemory );
	
	return bspTree;
}

void decBSPTree::ClearNodeContent(){
}

void decBSPTree::AddCoplanarFace( const decBSPTreeFace &face ){
}

int decBSPTree::TestFaceAgainstNode( const decBSPTreeFace &face ) const{
	int v, vertexCount = face.GetVertexCount();
	int side = esCoplanar;
	float dot;
	
	for( v=0; v<vertexCount; v++ ){
		dot = face.GetVertexAt( v ) * pNormal - pDistance;
		
		if( dot > 1e-5f ){
			if( side == esBack ) return esOverlaping;
			side = esFront;
			
		}else if( dot < -1e-5f ){
			if( side == esFront ) return esOverlaping;
			side = esBack;
		}
	}
	
	return side;
}

bool decBSPTree::IsFaceInsideTree( const decBSPTreeFace &face ) const{
	int side = TestFaceAgainstNode( face );
	
	if( side == esCoplanar ){
		return true;
		
	}else if( side == esFront ){
		if( pNodeFront ){
			return pNodeFront->IsFaceInsideTree( face );
			
		}else{
			return true;
		}
		
	}else if( side == esBack ){
		if( pNodeBack ){
			return pNodeBack->IsFaceInsideTree( face );
			
		}else{
			return false;
		}
		
	}else{
		if( pNodeFront || pNodeBack ){
			decBSPTreeFace front, back;
			
			SplitFace( face, front, back );
			
			if( pNodeFront ){
				if( pNodeFront->IsFaceInsideTree( front ) ) return true;
			}
			
			if( pNodeBack ){
				if( pNodeBack->IsFaceInsideTree( back ) ) return true;
			}
			
			return false;
			
		}else{
			return true;
		}
	}
}

void decBSPTree::SplitFace( const decBSPTreeFace &face, decBSPTreeFace &front, decBSPTreeFace &back ) const{
	int v, vertexCount = face.GetVertexCount();
	decVector cut, edge;
	int side1, side2;
	float dot;
	
	front.RemoveAllVertices();
	back.RemoveAllVertices();
	
	for( v=0; v<vertexCount; v++ ){
		const decVector &v1 = face.GetVertexAt( v );
		const decVector &v2 = face.GetVertexAt( ( v + 1 ) % vertexCount );
		
		dot = v1 * pNormal - pDistance;
		if( dot > 1e-5f ){
			side1 = 1;
		}else if( dot < -1e-5f ){
			side1 = -1;
		}else{
			side1 = 0;
		}
		
		dot = v2 * pNormal - pDistance;
		if( dot > 1e-5f ){
			side2 = 1;
		}else if( dot < -1e-5f ){
			side2 = -1;
		}else{
			side2 = 0;
		}
		
		if( side1 >= 0 ){
			front.AddVertex( v1 );
		}
		if( side1 <= 0 ){
			back.AddVertex( v1 );
		}
		if( side1 != side2 && side1 != 0 && side2 != 0 ){
			edge = v2 - v1;
			cut = v1 + edge * ( ( pDistance - pNormal * v1 ) / ( pNormal * edge ) );
			front.AddVertex( cut );
			back.AddVertex( cut );
		}
	}
}

void decBSPTree::InsertFaceIntoTree( const decBSPTreeFace &face ){
	int side = TestFaceAgainstNode( face );
	float faceDot;
	
	if( side == esCoplanar ){
		AddCoplanarFace( face );
		
	}else{
		const decVector &v1 = face.GetVertexAt( 0 );
		const decVector &v2 = face.GetVertexAt( 1 );
		const decVector &v3 = face.GetVertexAt( 2 );
		
		decVector faceNormal = ( v2 - v1 ) % ( v3 - v2 );
		faceNormal.Normalize();
		
		faceDot = faceNormal * v2;
		
		if( side == esFront ){
			if( pNodeFront ){
				pNodeFront->InsertFaceIntoTree( face );
				
			}else{
				pNodeFront = CreateBSPTree( faceNormal, faceDot );
				if( ! pNodeFront ) DETHROW( deeOutOfMemory );
				
				pNodeFront->AddCoplanarFace( face );
			}
			
		}else if( side == esBack ){
			if( pNodeBack ){
				pNodeBack->InsertFaceIntoTree( face );
				
			}else{
				pNodeBack = CreateBSPTree( faceNormal, faceDot );
				if( ! pNodeBack ) DETHROW( deeOutOfMemory );
				
				pNodeBack->AddCoplanarFace( face );
			}
			
		}else{
			decBSPTreeFace front, back;
			
			SplitFace( face, front, back );
			
			if( front.GetVertexCount() > 0 ){
				if( pNodeFront ){
					pNodeFront->InsertFaceIntoTree( front );
					
				}else{
					pNodeFront = CreateBSPTree( faceNormal, faceDot );
					if( ! pNodeFront ) DETHROW( deeOutOfMemory );
					
					pNodeFront->AddCoplanarFace( front );
				}
			}
			
			if( back.GetVertexCount() > 0 ){
				if( pNodeBack ){
					pNodeBack->InsertFaceIntoTree( back );
					
				}else{
					pNodeBack = CreateBSPTree( faceNormal, faceDot );
					if( ! pNodeBack ) DETHROW( deeOutOfMemory );
					
					pNodeBack->AddCoplanarFace( back );
				}
			}
		}
	}
}
