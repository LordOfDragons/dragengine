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
#include <inttypes.h>

#include "fbxObjectMap.h"
#include "fbxNode.h"

#include <dragengine/common/exceptions.h>


// Class fbxObjectMap
///////////////////////

// Constructor, destructor
////////////////////////////

fbxObjectMap::fbxObjectMap( int expectedCount ) :
pBuckets( NULL ),
pBucketCount( 0 )
{
	if( expectedCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = decMath::max( ( int )( ( float )expectedCount * 0.75f ), 1 );
	pBuckets = new sBucket[ count ];
	pBucketCount = count;
}

fbxObjectMap::~fbxObjectMap(){
	if( pBuckets ){
		delete [] pBuckets;
	}
}



// Management
///////////////

void fbxObjectMap::Add( fbxNode *node ){
	if( ! node || node->GetID() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pBuckets[ node->GetID() % pBucketCount ].objects.Add( node );
}

fbxNode *fbxObjectMap::GetAt( int64_t id ) const{
	const decPointerList &objects = pBuckets[ id % pBucketCount ].objects;
	const int count = objects.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		fbxNode * const node = ( fbxNode* )objects.GetAt( i );
		if( node->GetID() == id ){
			return node;
		}
	}
	
	return NULL;
}
