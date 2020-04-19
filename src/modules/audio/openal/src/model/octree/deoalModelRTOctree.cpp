/* 
 * Drag[en]gine OpenAL Audio Module
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

#include "deoalModelRTOctree.h"
#include "deoalModelOctree.h"
#include "deoalModelOctreeVisitor.h"
#include "../deoalModelFace.h"

#include <dragengine/common/exceptions.h>



// Visitors
/////////////

class deoalModelRTOctreeCalcCounts : public deoalModelOctreeVisitor {
public:
	int pNodeCount;
	int pFaceCount;
	
	deoalModelRTOctreeCalcCounts() :
	pNodeCount( 0 ),
	pFaceCount( 0 ){
	}
	
	inline int GetNodeCount() const{ return pNodeCount; }
	inline int GetFaceCount() const{ return pFaceCount; }
	inline bool IsEmpty() const{ return pNodeCount == 0 || pFaceCount == 0; }
	
	virtual void VisitNode( deoalOctree *node, int /*intersection*/ ){
		pNodeCount++;
		pFaceCount += ( ( deoalModelOctree* )node )->GetFaceCount();
	}
};


class deoalModelRTOctreeBuild{
private:
	deoalModelRTOctree::sFace *pFaces;
	deoalModelRTOctree::sNode *pNodes;
	int pIndexNode;
	int pIndexChild;
	int pIndexFace;
	
public:
	deoalModelRTOctreeBuild( deoalModelRTOctree::sFace *faces, deoalModelRTOctree::sNode *nodes ) :
	pFaces( faces ),
	pNodes( nodes ),
	pIndexNode( 0 ),
	pIndexChild( 1 ),
	pIndexFace( 0 ){
	}
	
	void VisitNode( deoalModelOctree &node ){
		deoalModelRTOctree::sNode &curNode = pNodes[ pIndexNode ];
		int i;
		
		curNode.center = node.GetCenter();
		curNode.halfSize = node.GetHalfSize();
		
		// pFaces
		const int faceCount = node.GetFaceCount();
		curNode.firstFace = pIndexFace;
		curNode.faceCount = faceCount;
		
		for( i=0; i<faceCount; i++ ){
			const deoalModelFace &modelFace = *node.GetFaceAt( i );
			deoalModelRTOctree::sFace &face = pFaces[ pIndexFace++ ];
			
			face.normal = modelFace.GetNormal();
			face.baseVertex = modelFace.GetVertex1();
			face.edgeNormal[ 0 ] = modelFace.GetEdge1Normal();
			face.edgeNormal[ 1 ] = modelFace.GetEdge2Normal();
			face.edgeNormal[ 2 ] = modelFace.GetEdge3Normal();
			face.edgeDistance[ 0 ] = modelFace.GetEdge1DistanceSafe();
			face.edgeDistance[ 1 ] = modelFace.GetEdge2DistanceSafe();
			face.edgeDistance[ 2 ] = modelFace.GetEdge3DistanceSafe();
			face.indexFace = modelFace.GetIndex();
			face.indexTexture = modelFace.GetTexture();
		}
		
		// child pNodes
		curNode.firstNode = pIndexChild;
		curNode.nodeCount = 0;
		for( i=0; i<8; i++ ){
			if( node.GetNodeAt( i ) ){
				curNode.nodeCount++;
			}
		}
		pIndexChild += curNode.nodeCount;
		
		int iterIndex = curNode.firstNode;
		for( i=0; i<8; i++ ){
			deoalOctree * const child = node.GetNodeAt( i );
			if( child ){
				pIndexNode = iterIndex++;
				VisitNode( *( ( deoalModelOctree* )child ) );
			}
		}
	}
};



// Class deoalModelRTOctree
/////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalModelRTOctree::deoalModelRTOctree( deoalModelOctree &octree ) :
pFaces( NULL ),
pFaceCount( 0 ),
pNodes( NULL ),
pNodeCount( 0 )
{
	try{
		deoalModelRTOctreeCalcCounts visitorCounts;
		octree.VisitNodes( &visitorCounts );
		if( visitorCounts.IsEmpty() ){
			return;
		}
		
		pFaceCount = visitorCounts.GetFaceCount();
		pFaces = new sFace[ pFaceCount ];
		
		pNodeCount = visitorCounts.GetNodeCount();
		pNodes = new sNode[ pNodeCount ];
		
		deoalModelRTOctreeBuild( pFaces, pNodes ).VisitNode( octree );
		
	}catch( const deException &e ){
		e.PrintError();
		pCleanUp();
		throw;
	}
}

deoalModelRTOctree::~deoalModelRTOctree(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoalModelRTOctree::pCleanUp(){
	if( pNodes ){
		delete [] pNodes;
	}
	if( pFaces ){
		delete [] pFaces;
	}
}
