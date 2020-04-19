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
