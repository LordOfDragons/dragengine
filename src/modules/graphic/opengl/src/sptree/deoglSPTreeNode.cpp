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
#include "deoglSPTreeNode.h"
#include "dragengine/common/exceptions.h"



// Class deoglSPTreeNode
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglSPTreeNode::deoglSPTreeNode( const decVector &center, const decVector &halfSize ){
	pCenter = center;
	pHalfSize = halfSize;
	pSplitState = essNone;
	pChildLeft = NULL;
	pChildRight = NULL;
}

deoglSPTreeNode::~deoglSPTreeNode(){
	if( pChildRight ) delete pChildRight;
	if( pChildLeft ) delete pChildLeft;
}



// Management
///////////////

void deoglSPTreeNode::SetSplitState( int splitState ){
	if( splitState < essNone || splitState > essZAxis ) DETHROW( deeInvalidParam );
	
	if( splitState != pSplitState ){
		pSplitState = splitState;
		
		if( pChildRight ){
			delete pChildRight;
			pChildRight = NULL;
		}
		if( pChildLeft ){
			delete pChildLeft;
			pChildLeft = NULL;
		}
		
		if( splitState == essXAxis ){
			float halfSize = pHalfSize.x * 0.5f;
			
			pChildLeft = new deoglSPTreeNode( decVector( pCenter.x - halfSize, pCenter.y, pCenter.z ),
				decVector( halfSize, pHalfSize.y, pHalfSize.z ) );
			if( ! pChildLeft ) DETHROW( deeOutOfMemory );
			
			pChildRight = new deoglSPTreeNode( decVector( pCenter.x + halfSize, pCenter.y, pCenter.z ),
				decVector( halfSize, pHalfSize.y, pHalfSize.z ) );
			if( ! pChildRight ) DETHROW( deeOutOfMemory );
			
		}else if( splitState == essYAxis ){
			float halfSize = pHalfSize.y * 0.5f;
			
			pChildLeft = new deoglSPTreeNode( decVector( pCenter.x, pCenter.y - halfSize, pCenter.z ),
				decVector( pHalfSize.x, halfSize, pHalfSize.z ) );
			if( ! pChildLeft ) DETHROW( deeOutOfMemory );
			
			pChildRight = new deoglSPTreeNode( decVector( pCenter.x, pCenter.y + halfSize, pCenter.z ),
				decVector( pHalfSize.x, halfSize, pHalfSize.z ) );
			if( ! pChildRight ) DETHROW( deeOutOfMemory );
			
		}else if( splitState == essZAxis ){
			float halfSize = pHalfSize.z * 0.5f;
			
			pChildLeft = new deoglSPTreeNode( decVector( pCenter.x, pCenter.y, pCenter.z - halfSize ),
				decVector( pHalfSize.x, pHalfSize.y, halfSize ) );
			if( ! pChildLeft ) DETHROW( deeOutOfMemory );
			
			pChildRight = new deoglSPTreeNode( decVector( pCenter.x, pCenter.y, pCenter.z + halfSize ),
				decVector( pHalfSize.x, pHalfSize.y, halfSize ) );
			if( ! pChildRight ) DETHROW( deeOutOfMemory );
		}
	}
}
