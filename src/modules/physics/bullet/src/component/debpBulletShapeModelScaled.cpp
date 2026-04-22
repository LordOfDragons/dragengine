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

#include "debpBulletShapeModelScaled.h"

#include "BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h"

#include <dragengine/common/exceptions.h>


// Class debpBulletShapeModelScaled
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

debpBulletShapeModelScaled::~debpBulletShapeModelScaled() = default;


// Management
///////////////

debpBulletShapeModelScaled::Ref debpBulletShapeModelScaled::New(
debpBulletShapeModel *modelShape, const btVector3 &scale){
	DEASSERT_NOTNULL(modelShape)
	
	// create btTriangleIndexVertexArray pointing at the shared vertex and face arrays owned by
	// modelShape. the array has its own m_scaling member independent of shared model shape.
	// a unique array is required since btBvhTriangleMeshShape modifies btTriangleIndexVertexArray
	// and thus it can not be shared
	auto indexVertexArray = deTUniqueReference<btTriangleIndexVertexArray>::New(
		modelShape->GetFaceCount() / 3, modelShape->GetFaces(), sizeof(int) * 3,
		modelShape->GetVertexCount() / 3, modelShape->GetVertices(), sizeof(btScalar) * 3);
	
	// create btBvhTriangleMeshShape using the unique btTriangleIndexVertexArray. its BVH is
	// rebuilt with the scale baked in below. this does not touch the shared model shape but
	// it does modify the btTriangleIndexVertexArray
	auto meshShape = deTUniqueReference<btBvhTriangleMeshShape>::New(indexVertexArray, true, true);
	meshShape->setUserPointer(nullptr);
	
	// wrap mesh shape in a compound with identity child transform. we have to use the local
	// scaling since btTransform does not support scaling
	btTransform transform;
	transform.setIdentity();
	
	btCompoundShape *compound = new btCompoundShape(true);
	compound->setUserPointer(nullptr);
	compound->addChildShape(transform, meshShape);
	
	// set scaling. has to come last. calls meshShape setLocalScaling which rebuilds the BVH
	// with the scale baked into its node AABBs and stored in
	// indexVertexArray::m_scaling for vertex reads
	compound->setLocalScaling(scale);
	
	return Ref::New(compound, modelShape, std::move(indexVertexArray), std::move(meshShape));
}


// Private Functions
//////////////////////

debpBulletShapeModelScaled::debpBulletShapeModelScaled(
	btCollisionShape *shape, debpBulletShapeModel *modelShape,
	deTUniqueReference<btTriangleIndexVertexArray> &&pIndexVertexArray,
	deTUniqueReference<btBvhTriangleMeshShape> &&pMeshShape) :
debpBulletShape(shape),
pModelShape(modelShape),
pIndexVertexArray(std::move(pIndexVertexArray)),
pMeshShape(std::move(pMeshShape)){
}
