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

#ifndef _RERIGBONE_H_
#define _RERIGBONE_H_

#include "../shape/reRigShape.h"
#include "../constraint/reRigConstraint.h"

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/collider/deColliderVolume.h>
#include <dragengine/resources/debug/deDebugDrawer.h>

class reRig;
class reRigShape;
class reRigConstraint;
class deCollider;
class igdeWCoordSysArrows;
class deEngine;



/**
 * \brief Rig bone.
 */
class reRigBone : public deObject{
public:
	typedef deTObjectReference<reRigBone> Ref;
	typedef decTObjectOrderedSet<reRigBone> List;
	typedef decTOrderedSet<deTObjectReference<reRigConstraint>, reRigConstraint*> ConstraintList;
	
	
private:
	deEngine *pEngine;
	
	reRig *pRig;
	int pOrder;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape::Ref pDDSBone;
	igdeWDebugDrawerShape::Ref pDDSCmp;
	igdeWCoordSysArrows *pDDSCoordSys;
	deColliderVolume::Ref pCollider;
	
	decString pName;
	reRigBone *pParentBone;
	decVector pPosition;
	decVector pOrientation;
	
	decMatrix pMatrix;
	decMatrix pInverseMatrix;
	decDMatrix pPoseMatrix;
	decDMatrix pInversePoseMatrix;
	
	decVector pCMP;
	float pMass;
	
	decVector pIKLimitsLower;
	decVector pIKLimitsUpper;
	decVector pIKResistance;
	bool pIKLocked[3];
	
	reRigShape::List pShapes;
	ConstraintList pConstraints;
	
	bool pSelected;
	bool pActive;
	
	bool pDynamic;
	bool pDirtyMatrix;
	bool pVisited;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new rig controller. */
	reRigBone(deEngine *engine);
	/** \brief Cleans up the rig controller. */
protected:
	virtual ~reRigBone();
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the parent rig. */
	inline reRig *GetRig() const{ return pRig; }
	/** \brief Sets the parent rig. */
	void SetRig(reRig *rig);
	
	/** \brief Sets the order of the bone in the engine rig. */
	inline int GetOrder() const{ return pOrder; }
	/** \brief Sets the order of the bone in the engine rig. */
	void SetOrder(int order);
	
	/** \brief Retrieves the bone collider. */
	inline const deColliderVolume::Ref &GetCollider() const{ return pCollider; }
	
	/** \brief Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Sets the world filename. */
	void SetName(const char *filename);
	
	/** \brief Retrieves parent rig bone or nullptr if a top level bone. */
	inline reRigBone *GetParentBone() const{ return pParentBone; }
	/** \brief Sets the parent bone or nullptr if a top level bone. */
	void SetParentBone(reRigBone *bone);
	/** \brief Retrieves the position. */
	inline decVector GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition(const decVector &position);
	/** \brief Retrieves the orientation. */
	inline decVector GetOrientation() const{ return pOrientation; }
	/** \brief Sets the orientation. */
	void SetOrientation(const decVector &orientation);
	
	/** \brief Update matrices if dirty. */
	void UpdateMatrices();
	/** \brief Determines if the matrix is dirty. */
	inline bool GetDirtyMatrix() const{ return pDirtyMatrix; }
	/** \brief Sets if the matrix is dirty. */
	void SetDirtyMatrix(bool dirty);
	/** \brief Retrieves the matrix transforming from local bone space to model space in reference pose. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	/** \brief Retrieves the matrix transforming from model space to local bone space in reference pose. */
	inline const decMatrix &GetInverseMatrix() const{ return pInverseMatrix; }
	/** \brief Sets the matrices. */
	void SetMatrices(const decMatrix &matrix);
	/** \brief Updates the pose matrix from the rest pose. */
	void SetPoseFromRest();
	/** \brief Retrieves the current pose matrix. */
	inline const decDMatrix &GetPoseMatrix() const{ return pPoseMatrix; }
	/** \brief Retrieves the current inverse pose matrix. */
	inline const decDMatrix &GetInversePoseMatrix() const{ return pInversePoseMatrix; }
	
	/** \brief Retrieves the position of the central mass point. */
	inline const decVector &GetCentralMassPoint() const{ return pCMP; }
	/** \brief Sets the position of the central mass point. */
	void SetCentralMassPoint(const decVector &cmp);
	/** \brief Determines if the bone is dynamic or not. */
	inline bool GetDynamic() const{ return pDynamic; }
	/** \brief Sets of the bone is dynamic. */
	void SetDynamic(bool dynamic);
	/** \brief Retrieves the mass. */
	inline float GetMass() const{ return pMass; }
	/** \brief Sets the mass factor. */
	void SetMass(float mass);
	
	/** \brief Determines if the shape is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the shape is selected. */
	void SetSelected(bool selected);
	/** \brief Determines if the shape is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the shape is the active one. */
	void SetActive(bool active);
	
	/** \brief Determines if this bone has been visited. */
	inline bool GetVisited() const{ return pVisited; }
	/** \brief Sets if this bone has been visited. */
	void SetVisited(bool visited);
	
	/** \brief Show state changed. */
	void ShowStateChanged();
	/** \brief Updates from the current pose. */
	void UpdateFromPose();
	/** \brief Updates shapes and constraints. */
	void UpdateShapesAndConstraints();
	
	/** \brief Notifies the shapes and constraints that the position pose changed. */
	void NotifyPoseChanged();
	
	/**
	 * \brief Determines if a given bone can be a parent of this bone.
	 * \details A bone can not be the parent bone if it is the same bone as this
	 *          bone or if while walking back the bone chain at least one bone of
	 *          the chain is walked a second time producing a loop.
	 */
	bool CanHaveParent(reRigBone *bone);
	/*@}*/
	
	/** \name Shapes */
	/*@{*/
	/** \brief Shapes. */
	inline const reRigShape::List &GetShapes() const{ return pShapes; }
	
	/** \brief Retrieves the shape with the given collider or nullptr if not found. */
	reRigShape *GetShapeWith(deColliderVolume *collider) const;
	
	/** \brief Adds a new shape. */
	void AddShape(reRigShape *shape);
	
	/** \brief Removes the given shape. */
	void RemoveShape(reRigShape *shape);
	
	/** \brief Removes all shapes. */
	void RemoveAllShapes();
	/*@}*/
	
	
	
	/** \name Inverse Kinematics */
	/*@{*/
	/** \brief Lower ik limits. */
	inline const decVector &GetIKLimitsLower() const{ return pIKLimitsLower; }
	
	/** \brief Set lower ik limits. */
	void SetIKLimitsLower(const decVector &lower);
	
	/** \brief Upper ik limits. */
	inline const decVector &GetIKLimitsUpper() const{ return pIKLimitsUpper; }
	
	/** \brief Set upper ik limits. */
	void SetIKLimitsUpper(const decVector &upper);
	
	/** \brief IK resistance. */
	inline const decVector &GetIKResistance() const{ return pIKResistance; }
	
	/** \brief Set ik resistance. */
	void SetIKResistance(const decVector &resistance);
	
	/** \brief X rotation is locked under IK. */
	inline bool GetIKLockedX() const{ return pIKLocked[0]; }
	
	/** \brief Set if X rotation is locked under IK. */
	void SetIKLockedX(bool locked);
	
	/** \brief Y rotation is locked under IK. */
	inline bool GetIKLockedY() const{ return pIKLocked[1]; }
	
	/** \brief Set if y rotation is locked under IK. */
	void SetIKLockedY(bool locked);
	
	/** \brief Z rotation is locked under IK. */
	inline bool GetIKLockedZ() const{ return pIKLocked[2]; }
	
	/** \brief Set z rotation is locked under IK. */
	void SetIKLockedZ(bool locked);
	
	/** \brief Rotation is locked under IK. */
	inline bool GetIKLocked(int axis) const{ return pIKLocked[axis]; }
	
	/** \brief Set if rotation is locked under IK. */
	void SetIKLocked(int axis, bool locked);
	/*@}*/
	
	/** \name Constraints */
	/*@{*/
	/** \brief Constraints. */
	inline const ConstraintList &GetConstraints() const{ return pConstraints; }
	
	/** \brief Retrieves the constraint with the given collider or nullptr if not found. */
	reRigConstraint *GetConstraintWith(deColliderVolume *collider) const;
	
	/** \brief Adds a new constraint. */
	void AddConstraint(reRigConstraint *constraint);
	
	/** \brief Removes the given constraint. */
	void RemoveConstraint(reRigConstraint *constraint);
	
	/** \brief Removes all constraints. */
	void RemoveAllConstraints();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pRepositionDDShapes();
	void pUpdateDDSBoneShape();
	void pUpdateDDSBoneColor();
	void pUpdateDDSCmpShape();
	void pUpdateDDSCmpColor();
	void pUpdateColliderShape();
};

#endif
