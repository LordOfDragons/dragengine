/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _RERIGBONE_H_
#define _RERIGBONE_H_

#include <dragengine/common/string/decString.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>
#include "../shape/reRigShapeList.h"
#include "../constraint/reRigConstraintList.h"

class reRig;
class reRigShape;
class reRigConstraint;
class deCollider;
class deColliderVolume;
class igdeWDebugDrawerShape;
class igdeWCoordSysArrows;
class deEngine;
class deDebugDrawer;



/**
 * \brief Rig bone.
 */
class reRigBone : public deObject{
private:
	deEngine *pEngine;
	
	reRig *pRig;
	int pOrder;
	
	deDebugDrawer *pDebugDrawer;
	igdeWDebugDrawerShape *pDDSBone;
	igdeWDebugDrawerShape *pDDSCmp;
	igdeWCoordSysArrows *pDDSCoordSys;
	deColliderVolume *pCollider;
	
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
	
	reRigShapeList pShapes;
	reRigConstraintList pConstraints;
	
	bool pSelected;
	bool pActive;
	
	bool pDynamic;
	bool pDirtyMatrix;
	bool pVisited;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new rig controller. */
	reRigBone( deEngine *engine );
	/** \brief Cleans up the rig controller. */
	virtual ~reRigBone();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the parent rig. */
	inline reRig *GetRig() const{ return pRig; }
	/** \brief Sets the parent rig. */
	void SetRig( reRig *rig );
	
	/** \brief Sets the order of the bone in the engine rig. */
	inline int GetOrder() const{ return pOrder; }
	/** \brief Sets the order of the bone in the engine rig. */
	void SetOrder( int order );
	
	/** \brief Retrieves the bone collider. */
	inline deColliderVolume *GetCollider() const{ return pCollider; }
	
	/** \brief Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** \brief Sets the world filename. */
	void SetName( const char *filename );
	
	/** \brief Retrieves parent rig bone or NULL if a top level bone. */
	inline reRigBone *GetParentBone() const{ return pParentBone; }
	/** \brief Sets the parent bone or NULL if a top level bone. */
	void SetParentBone( reRigBone *bone );
	/** \brief Retrieves the position. */
	inline decVector GetPosition() const{ return pPosition; }
	/** \brief Sets the position. */
	void SetPosition( const decVector &position );
	/** \brief Retrieves the orientation. */
	inline decVector GetOrientation() const{ return pOrientation; }
	/** \brief Sets the orientation. */
	void SetOrientation( const decVector &orientation );
	
	/** \brief Update matrices if dirty. */
	void UpdateMatrices();
	/** \brief Determines if the matrix is dirty. */
	inline bool GetDirtyMatrix() const{ return pDirtyMatrix; }
	/** \brief Sets if the matrix is dirty. */
	void SetDirtyMatrix( bool dirty );
	/** \brief Retrieves the matrix transforming from local bone space to model space in reference pose. */
	inline const decMatrix &GetMatrix() const{ return pMatrix; }
	/** \brief Retrieves the matrix transforming from model space to local bone space in reference pose. */
	inline const decMatrix &GetInverseMatrix() const{ return pInverseMatrix; }
	/** \brief Sets the matrices. */
	void SetMatrices( const decMatrix &matrix );
	/** \brief Updates the pose matrix from the rest pose. */
	void SetPoseFromRest();
	/** \brief Retrieves the current pose matrix. */
	inline const decDMatrix &GetPoseMatrix() const{ return pPoseMatrix; }
	/** \brief Retrieves the current inverse pose matrix. */
	inline const decDMatrix &GetInversePoseMatrix() const{ return pInversePoseMatrix; }
	
	/** \brief Retrieves the position of the central mass point. */
	inline const decVector &GetCentralMassPoint() const{ return pCMP; }
	/** \brief Sets the position of the central mass point. */
	void SetCentralMassPoint( const decVector &cmp );
	/** \brief Determines if the bone is dynamic or not. */
	inline bool GetDynamic() const{ return pDynamic; }
	/** \brief Sets of the bone is dynamic. */
	void SetDynamic( bool dynamic );
	/** \brief Retrieves the mass. */
	inline float GetMass() const{ return pMass; }
	/** \brief Sets the mass factor. */
	void SetMass( float mass );
	
	/** \brief Determines if the shape is selected. */
	inline bool GetSelected() const{ return pSelected; }
	/** \brief Sets if the shape is selected. */
	void SetSelected( bool selected );
	/** \brief Determines if the shape is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** \brief Sets if the shape is the active one. */
	void SetActive( bool active );
	
	/** \brief Determines if this bone has been visited. */
	inline bool GetVisited() const{ return pVisited; }
	/** \brief Sets if this bone has been visited. */
	void SetVisited( bool visited );
	
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
	bool CanHaveParent( reRigBone *bone );
	/*@}*/
	
	/** \name Shapes */
	/*@{*/
	/** \brief Retrieves the number of shapes. */
	int GetShapeCount() const;
	/** \brief Retrieves the shape at the given index. */
	reRigShape *GetShapeAt( int index ) const;
	/** \brief Retrieves the shape with the given collider or NULL if not found. */
	reRigShape *GetShapeWith( deColliderVolume *collider ) const;
	/** \brief Retrieves the index of the shape or -1 if not found. */
	int IndexOfShape( reRigShape *shape ) const;
	/** \brief Determines if the shape exists. */
	bool HasShape( reRigShape *shape ) const;
	/** \brief Adds a new shape. */
	void AddShape( reRigShape *shape );
	/** \brief Removes the given shape. */
	void RemoveShape( reRigShape *shape );
	/** \brief Removes all shapes. */
	void RemoveAllShapes();
	/*@}*/
	
	/** \name Constraints */
	/*@{*/
	/** \brief Retrieves the number of constraints. */
	int GetConstraintCount() const;
	/** \brief Retrieves the constraint at the given index. */
	reRigConstraint *GetConstraintAt( int index ) const;
	/** \brief Retrieves the constraint with the given collider or NULL if not found. */
	reRigConstraint *GetConstraintWith( deColliderVolume *collider ) const;
	/** \brief Retrieves the index of the constraint or -1 if not found. */
	int IndexOfConstraint( reRigConstraint *constraint ) const;
	/** \brief Determines if the constraint exists. */
	bool HasConstraint( reRigConstraint *constraint ) const;
	/** \brief Adds a new constraint. */
	void AddConstraint( reRigConstraint *constraint );
	/** \brief Removes the given constraint. */
	void RemoveConstraint( reRigConstraint *constraint );
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
