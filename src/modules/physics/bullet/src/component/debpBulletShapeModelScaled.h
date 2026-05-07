/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEBPBULLETSHAPEMODELSCALED_H_
#define _DEBPBULLETSHAPEMODELSCALED_H_

#include "debpBulletShapeModel.h"
#include "LinearMath/btVector3.h"

#include <dragengine/deTUniqueReference.h>

class btTriangleIndexVertexArray;
class btBvhTriangleMeshShape;
class btCompoundShape;


/**
 * \brief Wraps a scaled model bullet collision shape.
 * 
 * Creates a private btBvhTriangleMeshShape that shares the raw vertex and face
 * arrays of the wrapped debpBulletShapeModel (no data duplication) but owns its
 * own btTriangleIndexVertexArray and BVH. The BVH is rebuilt with the requested
 * scale baked in via btCompoundShape::setLocalScaling, leaving the shared model
 * shape completely unmodified. This keeps the fast TRIANGLE_MESH_SHAPE_PROXYTYPE
 * code path in btCollisionWorld::objectQuerySingleInternal.
 */
class debpBulletShapeModelScaled : public debpBulletShape{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<debpBulletShapeModelScaled> Ref;
	
	
private:
	const debpBulletShapeModel::Ref pModelShape;
	deTUniqueReference<btTriangleIndexVertexArray> pIndexVertexArray;
	deTUniqueReference<btBvhTriangleMeshShape> pMeshShape;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**
	 * \brief Create scaled bullet shape wrapper.
	 * 
	 * Creates a private btBvhTriangleMeshShape and btCompoundShape applying scale.
	 * The raw vertex/face data is shared with modelShape; no data is duplicated.
	 */
	static Ref New(debpBulletShapeModel *modelShape, const btVector3 &scale);
	
	/** \brief Clean up. Deletes private BVH and IVArray; compound deleted by base class. */
	~debpBulletShapeModelScaled() override;
	/*@}*/
	
	
public:
	/** \warning Internal use only. Do not call directly. */
	debpBulletShapeModelScaled(btCollisionShape *shape, debpBulletShapeModel *modelShape,
		deTUniqueReference<btTriangleIndexVertexArray> &&pIndexVertexArray,
		deTUniqueReference<btBvhTriangleMeshShape> &&pMeshShape);
};

#endif
