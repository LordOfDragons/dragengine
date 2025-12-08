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

#ifndef _RERIGCONSTRAINT_H_
#define _RERIGCONSTRAINT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/collider/deColliderConstraint.h>

class deDebugDrawer;
class reRig;
class reRigBone;
class deRigConstraint;
class reRigConstraintDof;
class deCollider;
class deColliderVolume;
class igdeWDebugDrawerShape;
class igdeWCoordSysArrows;
class igdeWAngleRange;
class deEngine;



/**
 * \brief Rig constraint.
 */
class reRigConstraint : public deObject{
private:
	deEngine *pEngine;
	
	reRig *pRig;
	reRigBone *pRigBone;
	
	deDebugDrawer::Ref pDebugDrawer;
	igdeWDebugDrawerShape *pDDSConstraint;
	igdeWDebugDrawerShape *pDDSJointError;
	igdeWCoordSysArrows *pDDSCoordSys;
	igdeWDebugDrawerShape *pDDSOffset;
	igdeWDebugDrawerShape *pDDSRangeLinear;
	igdeWAngleRange *pDDSRangeAngularX;
	igdeWAngleRange *pDDSRangeAngularY;
	igdeWAngleRange *pDDSRangeAngularZ;
	deColliderVolume::Ref pCollider;
	deColliderConstraint *pEngConstraint;
	
	decVector pPosition;
	decVector pOrientation;
	decVector pOffset;
	
	reRigConstraintDof *pDof[6];
	
	float pDampingLinear;
	float pDampingAngular;
	float pDampingSpring;
	
	bool pIsRope;
	
	float pBreakingThreshold;
	
	reRigBone::Ref pConstraintBone;
	
	decDMatrix pPoseMatrix1;
	decDMatrix pPoseMatrix2;
	
	bool pSelected;
	bool pActive;
	
	bool pShowJointError;
	
	bool pDirtyPositions;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reRigConstraint> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new rig controller. */
	reRigConstraint(deEngine *engine);
	/** \brief Cleans up the rig controller. */
	virtual ~reRigConstraint();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the parent rig. */
	inline reRig *GetRig() const{ return pRig; }
	/** \brief Sets the parent rig. */
	void SetRig(reRig *rig);
	/** \brief Retrieves the parent rig bone. */
	inline reRigBone *GetRigBone() const{ return pRigBone; }
	/** \brief Sets the parent rig bone. */
	void SetRigBone(reRigBone *rigBone);
	
	/** \brief Retrieves the engine constraint linked to this constraint. */
	inline deColliderConstraint *GetEngineConstraint() const{ return pEngConstraint; }
	/** \brief Sets the engine constraint linked to this constraint. */
	void SetEngineConstraint(deColliderConstraint *constraint);
	
	/** \brief Builds an engine rig constraint from the actual state. */
	deRigConstraint *BuildEngineRigConstraint();
	/** \brief Builds an engine collider constraint from the actual state. */
	deColliderConstraint *BuildEngineColliderConstraint();
	
	/** \brief Retrieves the bone collider. */
	inline deColliderVolume *GetCollider() const{ return pCollider; }
	
	/** \brief Retrieves the constraint position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	/** \brief Sets the constraint position. */
	void SetPosition(const decVector &position);
	/** \brief Retrieves the constraint orientation. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	/** \brief Sets the constraint orientation. */
	void SetOrientation(const decVector &orientation);
	/** \brief Retrieves the constraint offset. */
	inline const decVector &GetOffset() const{ return pOffset; }
	/** \brief Sets the constraint offset. */
	void SetOffset(const decVector &offset);
	
	/** \brief Retrieves a degree of freedom. */
	reRigConstraintDof &GetDof(deColliderConstraint::eDegreesOfFreedom dof) const;
	/** \brief Retrieves the x-axis linear degree of freedom. */
	inline reRigConstraintDof &GetDofLinearX() const{ return *pDof[deColliderConstraint::edofLinearX]; }
	/** \brief Retrieves the y-axis linear degree of freedom. */
	inline reRigConstraintDof &GetDofLinearY() const{ return *pDof[deColliderConstraint::edofLinearY]; }
	/** \brief Retrieves the z-axis linear degree of freedom. */
	inline reRigConstraintDof &GetDofLinearZ() const{ return *pDof[deColliderConstraint::edofLinearZ]; }
	/** \brief Retrieves the x-axis angular degree of freedom. */
	inline reRigConstraintDof &GetDofAngularX() const{ return *pDof[deColliderConstraint::edofAngularX]; }
	/** \brief Retrieves the y-axis angular degree of freedom. */
	inline reRigConstraintDof &GetDofAngularY() const{ return *pDof[deColliderConstraint::edofAngularY]; }
	/** \brief Retrieves the z-axis angular degree of freedom. */
	inline reRigConstraintDof &GetDofAngularZ() const{ return *pDof[deColliderConstraint::edofAngularZ]; }
	
	/** \brief Retrieves the linear damping. */
	inline float GetLinearDamping() const{ return pDampingLinear; }
	/** \brief Sets the linear damping. */
	void SetLinearDamping(float damping);
	/** \brief Retrieves the angular damping. */
	inline float GetAngularDamping() const{ return pDampingAngular; }
	/** \brief Sets the angular damping. */
	void SetAngularDamping(float damping);
	/** \brief Retrieves the spring damping. */
	inline float GetSpringDamping() const{ return pDampingSpring; }
	/** \brief Sets the spring damping. */
	void SetSpringDamping(float damping);
	
	/** \brief Determines if this constraint is a rope. */
	inline bool GetIsRope() const{ return pIsRope; }
	/** \brief Sets if this constraint is a rope. */
	void SetIsRope(bool isRope);
	
	/** \brief Retrieves the breaking impulse threshold or 0 if disabled. */
	inline float GetBreakingThreshold() const{ return pBreakingThreshold; }
	/** \brief Sets the breaking impulse threshold or 0 to disable. */
	void SetBreakingThreshold(float impulseThreshold);
	
	/** \brief Retrieves the constraint rig bone or NULL. */
	inline reRigBone *GetConstraintBone() const{ return pConstraintBone; }
	/** \brief Sets the constraint rig bone or NULL. */
	void SetConstraintBone(reRigBone::Ref bone);
	
	/** \brief Retrieves the first pose matrix. */
	inline const decDMatrix &GetPoseMatrix1() const{ return pPoseMatrix1; }
	/** \brief Retrieves the second pose matrix. */
	inline const decDMatrix &GetPoseMatrix2() const{ return pPoseMatrix2; }
	
	/** \brief Determines if the constraint is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the constraint is selected. */
	void SetSelected(bool selected);
	/** \brief Determines if the constraint is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the constraint is the active one. */
	void SetActive(bool active);
	
	/** \brief Determines if joint error is shown. */
	inline bool GetShowJointError() const{ return pShowJointError; }
	/** \brief Sets if joint error is shown. */
	void SetShowJointError(bool showJointError);
	
	/** \brief Show state changed. */
	void ShowStateChanged();
	/** \brief Updates from the current pose. */
	void Update();
	
	/** \brief Invalidates the world positions of the constraint. */
	void InvalidatePositions();
	
	/** \brief Determines if this constraint is visible in the current state. */
	bool IsVisible() const;
	
	/** \brief Creates a copy of this constraint. */
	reRigConstraint *Duplicate() const;
	/** \brief Uniformly scale constraint. */
	void Scale(float scale);
	
	/** \brief Notifies all that the constraint changed. */
	void NotifyAllConstraintChanged();
	/** \brief Notify engine about change to the constraint. */
	void NotifyEngineConstraintChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateDDConstraint();
	void pUpdateDDConstraintGeometry();
	void pUpdateDDJointError();
	void pUpdateColliderShape();
	
	void pUpdateConstraintPosition(deColliderConstraint &engConstraint);
};

#endif
