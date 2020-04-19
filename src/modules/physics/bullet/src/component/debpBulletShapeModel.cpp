/* 
 * Drag[en]gine Bullet Physics Module
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

#include "debpBulletShapeModel.h"

#include "BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"

#include <dragengine/common/exceptions.h>



// Class debpBulletShapeModel
///////////////////////////////

// Constructor, destructor
////////////////////////////

debpBulletShapeModel::debpBulletShapeModel( btTriangleMeshShape *meshShape,
btTriangleIndexVertexArray *indexVertexArray, btScalar *vertices,
int *faces, int vertexCount, int faceCount ) :
debpBulletShape( meshShape ),
pMeshShape( meshShape ),
pIndexVertexArray( indexVertexArray ),
pVertices( vertices ),
pFaces( faces ),
pVertexCount( vertexCount ),
pFaceCount( faceCount )
{
	if( ! indexVertexArray || vertexCount < 0 || faceCount < 0 ){
		DETHROW( deeInvalidParam );
	}
}

debpBulletShapeModel::~debpBulletShapeModel(){
	if( pIndexVertexArray ){
		delete pIndexVertexArray;
	}
	if( pFaces ){
		delete [] pFaces;
	}
	if( pVertices ){
		delete [] pVertices;
	}
}



// Management
///////////////

