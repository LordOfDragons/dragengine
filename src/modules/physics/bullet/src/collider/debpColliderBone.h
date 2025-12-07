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

#ifndef _DEBPCOLLIDERBONE_H_
#define _DEBPCOLLIDERBONE_H_

#include "../shape/debpShapeList.h"
#include "../debpCollisionObject.h"

#include <dragengine/common/collection/decPointerOrderedSet.h>
#include <dragengine/common/math/decMath.h>

class debpColliderConstraint;
class debpBulletShape;
class debpPhysicsBody;

class btGhostObject;
class btCollisionObject;

class deDebugDrawer;
class deDebugDrawerShape;
class deRigBone;



/**
 * Bullet Collider Bone.
 * 
 * Stores information about a bone in a collider. Besides the physics object a couple of
 * matrices are stored which are required for proper tracking component bones with the collider
 * bones.The following matrices are stored:
 * 
 * ^ Name ^ Description ^
 * | {Inv}BoneMatrix |Transforms from Rig Space into Rig Bone Space|
 * | {Inv}ColMatrix |Transforms from Rig Space into Rig Collider Space|
 * | {Inv}LocalMatrix |Local matrix of bone relative to its parent bone or the rig space|
 * | {Inv}RealMatrix |Transforms Rig Bone from rest position into current position|
 * 
 * All matrices except the real matrix are calculated from a rig upon creation time and do never
 * change. The real Matrix is recalculated each time the collider is moved around and is used to
 * calculate the new position of the tracked rig bone. The Dirty flag is used to signal if the
 * real matrix is valid. This is as parent bones have to be calculated before calculating the
 * children bones.
 */
class debpColliderBone{
private:
	debpPhysicsBody *pPhyBody;
	decDMatrix pBoneMatrix;
	decDMatrix pInvBoneMatrix;
	decDMatrix pColMatrix;
	decDMatrix pInvColMatrix;
	decDMatrix pLocalMatrix;
	decDMatrix pInvLocalMatrix;
	decDMatrix pRealMatrix;
	decDMatrix pInvRealMatrix;
	decDVector pOffset;
	bool pDirty;
	bool pColBoneDynamic;
	int pIndex;
	int pParent;
	
	decPointerOrderedSet pConstraints;
	
	debpShapeList pShapes;
	
	btGhostObject *pStaticCollisionTest;
	debpBulletShape::Ref pStaticCollisionTestShape;
	debpCollisionObject pStaticCollisionTestObject;
	bool pDirtyStaticTest;
	
	deDebugDrawer::Ref pDebugDrawer;
	deDebugDrawerShape *pDDSShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a collider bone. */
	debpColliderBone(debpCollider *collider, int index);
	
	/** Clean up collider bone. */
	~debpColliderBone();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Physics body. */
	inline debpPhysicsBody *GetPhysicsBody() const{ return pPhyBody; }
	
	/** Matrices. */
	inline const decDMatrix &GetBoneMatrix() const{ return pBoneMatrix; }
	inline const decDMatrix &GetInverseBoneMatrix() const{ return pInvBoneMatrix; }
	inline const decDMatrix &GetColliderMatrix() const{ return pColMatrix; }
	inline const decDMatrix &GetInverseColliderMatrix() const{ return pInvColMatrix; }
	inline const decDMatrix &GetLocalMatrix() const{ return pLocalMatrix; }
	inline const decDMatrix &GetInverseLocalMatrix() const{ return pInvLocalMatrix; }
	inline const decDMatrix &GetRealMatrix() const{ return pRealMatrix; }
	inline const decDMatrix &GetInverseRealMatrix() const{ return pInvRealMatrix; }
	
	/** Index of bone. */
	inline int GetIndex() const{ return pIndex; }
	
	/** Matching collider bone is set dynamic. */
	inline bool GetColBoneDynamic() const{ return pColBoneDynamic; }
	
	/** Set bone matrices. */
	void SetBoneMatrices(const decDMatrix &matrix);
	
	/** Set collider matrices. */
	void SetColliderMatrices(const decDMatrix &matrix);
	
	/** Set local matrix. */
	void SetLocalMatrix(const decDMatrix &matrix);
	
	/** Set real matrix. */
	void SetRealMatrix(const decDMatrix &matrix);
	
	/** Dirty. */
	inline bool GetDirty() const{ return pDirty; }
	void SetDirty(bool dirty);
	
	/** Set matching collider bone is set dynamic. */
	void SetColBoneDynamic(bool dynamic);
	
	/** Parent bone index with -1 meaning not parent. */
	inline int GetParent() const{ return pParent; }
	
	/** Central mass point offset. */
	inline const decDVector &GetOffset() const{ return pOffset; }
	
	/** Shapes. */
	inline debpShapeList &GetShapes(){ return pShapes; }
	inline const debpShapeList &GetShapes() const{ return pShapes; }
	
	/** Set parameters from rig bone. */
	void SetFromRigBone(const deRigBone &bone);
	
	
	
	/** Number of constraints. */
	int GetConstraintCount() const;
	
	/** Constraint at index. */
	debpColliderConstraint *GetConstraintAt(int index) const;
	
	/** Index of constraint or -1 if absent. */
	int IndexOfConstraint(debpColliderConstraint *constraint) const;
	
	/** Constraint is present. */
	bool HasConstraint(debpColliderConstraint *constraint) const;
	
	/** Add constraint. */
	void AddConstraint(debpColliderConstraint *constraint);
	
	/** Remove constraint. */
	void RemoveConstraint(debpColliderConstraint *constraint);
	
	/** Remove all constraints. */
	void RemoveAllConstraints();
	
	
	
	/** Static collision test updating it if dirty or null if empty shape. */
	btCollisionObject *GetStaticCollisionTest();
	
	/**
	 * \brief Prepared static collision or null if empty shape.
	 * 
	 * Same as GetStaticCollisionTest() with calling setWorldTransform().
	 */
	btCollisionObject *GetStaticCollisionTestPrepare();
	
	/** Requires bone auto-dirty. */
	bool RequiresAutoDirty() const;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** Debug drawer or \em NULL if not activated .*/
	inline const deDebugDrawer::Ref &GetDebugDrawer() const{ return pDebugDrawer; }
	
	/** Set debug drawer or \em NULL if not activated. */
	void SetDebugDrawer(deDebugDrawer *debugDrawer);
	
	/** Debug drawer shape or \em NULL if not ativated. */
	inline deDebugDrawerShape *GetDDSShape() const{ return pDDSShape; }
	
	/** Set debug drawer shape or \em NULL if not activated. */
	void SetDDSShape(deDebugDrawerShape *shape);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateStaticCollisionTest();
	debpBulletShape *pCreateBPShape();
};

#endif
