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
