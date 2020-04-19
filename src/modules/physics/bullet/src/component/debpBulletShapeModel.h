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

#ifndef _DEBPBULLETSHAPEMODEL_H_
#define _DEBPBULLETSHAPEMODEL_H_

#include "../debpBulletShape.h"
#include "LinearMath/btScalar.h"

class btTriangleIndexVertexArray;
class btTriangleMeshShape;



/**
 * \brief Wraps a model bullet collision shape.
 * 
 * Models use triangle bullet shapes. These shapes required external memory
 * that has to be lifetime managed together with the shape itself. This class
 * stores in addition these data arrays to keep the shape working after the
 * parent model is released from memory.
 */
class debpBulletShapeModel : public debpBulletShape {
private:
	btTriangleMeshShape *pMeshShape;
	btTriangleIndexVertexArray *pIndexVertexArray;
	btScalar *pVertices;
	int *pFaces;
	int pVertexCount;
	int pFaceCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create bullet shape wrapper taking ownership of bullet shape. */
	debpBulletShapeModel( btTriangleMeshShape *meshShape, btTriangleIndexVertexArray *indexVertexArray,
	btScalar *vertices, int *faces, int vertexCount, int faceCount );
	
	/** \brief Clean up bullet shape wrapper deleting wrapped bullet shape. */
	virtual ~debpBulletShapeModel();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Triangle mesh shape. */
	inline btTriangleMeshShape *GetMeshShape() const{ return pMeshShape; }
	
	/** \brief Index vertex array. */
	inline btTriangleIndexVertexArray *GetIndexVertexArray() const{ return pIndexVertexArray; }
	
	/** \brief Vertices. */
	inline btScalar *GetVertices() const{ return pVertices; }
	
	/** \brief Faces. */
	inline int *GetFaces() const{ return pFaces; }
	
	/** \brief Vertex count. */
	inline int GetVertexCount() const{ return pVertexCount; }
	
	/** \brief Face count. */
	inline int GetFaceCount() const{ return pFaceCount; }
	/*@}*/
};

#endif
