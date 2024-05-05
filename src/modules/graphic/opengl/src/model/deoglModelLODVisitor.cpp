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

#include "deoglModelLOD.h"
#include "deoglModelLODVisitor.h"
#include "face/deoglModelFace.h"
#include "octree/deoglModelOctree.h"
#include "../utils/collision/deoglCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoglModelLODVisitor
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglModelLODVisitor::deoglModelLODVisitor( deoglModelLOD &modelLOD ) :
pModelLOD( modelLOD ),
pMinLODDistance( 0.0f ){
}

deoglModelLODVisitor::~deoglModelLODVisitor(){
}



// Management
///////////////

void deoglModelLODVisitor::SetBoxExtends( const decVector &minExtend, const decVector &maxExtend ){
	pBoxMinExtend = minExtend;
	pBoxMaxExtend = maxExtend;
}

void deoglModelLODVisitor::SetLODPoint( const decVector &lodPoint ){
	pLODPoint = lodPoint;
}

void deoglModelLODVisitor::SetMinLODDistance( float distance ){
	pMinLODDistance = distance;
}



// Visiting
/////////////

void deoglModelLODVisitor::VisitNode( deoglOctree *node, int intersection ){
	const bool nodeInside = ( intersection == deoglCollisionDetection::eirInside );
	const deoglModelOctree &monode = *( ( deoglModelOctree* )node );
	const oglModelPosition * positions = pModelLOD.GetPositions();
	const oglModelVertex * vertices = pModelLOD.GetVertices();
	const deoglModelFaceList &list = monode.GetFaceList();
	const int count = list.GetCount();
	float distance;
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglModelFace &face = *list.GetAt( i );
		
		if( nodeInside || deoglCollisionDetection::AABoxHitsAABox( face.GetMinExtend(), face.GetMaxExtend(), pBoxMinExtend, pBoxMaxExtend ) ){
			const decVector &vpos1 = positions[ vertices[ face.GetVertex1() ].position ].position;
			const decVector &vpos2 = positions[ vertices[ face.GetVertex2() ].position ].position;
			const decVector &vpos3 = positions[ vertices[ face.GetVertex3() ].position ].position;
			
			distance = ( deoglCollisionDetection::ClosestPointOnTriangle( vpos1, vpos2, vpos3, pLODPoint ) - pLODPoint ).Length();
			if( distance < pMinLODDistance ){
				pMinLODDistance = distance;
			}
		}
	}
}
